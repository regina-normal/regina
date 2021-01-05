
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "Python.h"
#include "regina-config.h"
#include "file/globaldirs.h"
#include "packet/packet.h"

// Put this before any Qt/KDE stuff so Python 2.3 "slots" doesn't clash.
#include "../python/gui/pythoninterpreter.h"

#include "pythonmanager.h"
#include "reginafilter.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "commandedit.h"
#include "pythonconsole.h"

#include <fstream>
#include <iostream>

#include <QApplication>
#include <QClipboard>
#include <QCompleter>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QRegularExpression>
#include <QTextCodec>
#include <QTextDocument>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWhatsThis>

class GUICompleter : public regina::python::PrefixCompleter {
    public:
        QStringList completions_;

        GUICompleter() = default;

        const QStringList& completions() const {
            return completions_;
        }

        size_t count() const {
            return completions_.size();
        }

        bool addCompletion(const std::string& s) override {
            // Skip anything that contains __
            if (s.find("__") != std::string::npos)
                return true;

            QString use(s.c_str());
            if (completions_.contains(use)) {
                // We found a duplicate; ignore it.
                return true;
            }

            completions_.append(use);
            PrefixCompleter::addCompletion(s);
            return true; // Never stop asking for completions.
        }
};

PythonConsole::PythonConsole(QWidget* parent, PythonManager* useManager) :
        QMainWindow(parent), manager(useManager) {
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Python Console"));

    // Set up the main widgets.
    QWidget* box = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout;
    session = new QTextEdit();
    session->setReadOnly(true);
    session->setAutoFormatting(QTextEdit::AutoNone);
    session->setFocusPolicy(Qt::NoFocus);
    session->setWhatsThis( tr("This area stores a history of the entire "
        "Python session, including commands that have been typed and the "
        "output they have produced."));
    layout->addWidget(session, 1);

    QHBoxLayout *inputAreaLayout = new QHBoxLayout;
    
    QString inputMsg = tr("Type your Python commands into this box.");
    prompt = new QLabel();
    prompt->setWhatsThis(inputMsg);
    inputAreaLayout->addWidget(prompt);

    input = new CommandEdit(this);
    input->setWhatsThis(inputMsg);
    input->setFocus();

    completer = nullptr;
    connect(input, SIGNAL(completionRequested(int, int)),
        this, SLOT(processCompletion(int, int)),Qt::QueuedConnection);

    inputAreaLayout->addWidget(input, 1);
    layout->addLayout(inputAreaLayout);

    updatePreferences(); // Set fonts, indents, etc.

    setCentralWidget(box);
    box->setLayout(layout);
    box->show();

    // Set up the actions.
    // Don't use XML files since we don't know whether we're in the shell or
    // the part.
    QMenu* menuConsole = new QMenu(this);
    QMenu* menuEdit = new QMenu(this);
    QMenu* menuHelp = new QMenu(this);

    QAction* act = new QAction(this);
    act->setText(tr("&Save Session"));
    act->setIcon(ReginaSupport::themeIcon("document-save"));
    act->setShortcuts(QKeySequence::Save);
    act->setToolTip(tr("Save session history"));
    act->setWhatsThis(tr("Save the entire history of this Python session "
        "into a text file."));
    connect(act, SIGNAL(triggered()), this, SLOT(saveLog()));
    menuConsole->addAction(act);

    menuConsole->addSeparator();

    act = new QAction(this);
    act->setText(tr("&Close"));
    act->setIcon(ReginaSupport::themeIcon("window-close"));
    act->setShortcuts(QKeySequence::Close);
    act->setToolTip(tr("Close Python console"));
    act->setWhatsThis(tr("Close this Python console."));
    connect(act, SIGNAL(triggered()), this, SLOT(close()));
    menuConsole->addAction(act);

    actCut = new QAction(this);
    actCut->setText(tr("Cut"));
    actCut->setIcon(ReginaSupport::themeIcon("edit-cut"));
    actCut->setShortcuts(QKeySequence::Cut);
    actCut->setToolTip(tr(
        "Cut selected text from the input area to the clipboard"));
    actCut->setWhatsThis(tr(
        "Cut selected text from the input area to the clipboard.  "
        "The input area is the small box at the bottom of the window "
        "where you can type your next command."));
    connect(actCut, SIGNAL(triggered()), this, SLOT(cut()));
    actCut->setEnabled(false);
    menuEdit->addAction(actCut);

    actCopy = new QAction(this);
    actCopy->setText(tr("Copy"));
    actCopy->setIcon(ReginaSupport::themeIcon("edit-copy"));
    actCopy->setShortcuts(QKeySequence::Copy);
    actCopy->setToolTip(tr(
        "Copy selected text to the clipboard."));
    actCopy->setWhatsThis(tr(
        "Copy selected text to the clipboard.  "
        "You may select text in either the session log (the main "
        "part of the window) or the input area (the small box at the "
        "bottom of the window)."));
    connect(actCopy, SIGNAL(triggered()), this, SLOT(copy()));
    actCopy->setEnabled(false);
    menuEdit->addAction(actCopy);

    connect(input, SIGNAL(selectionChanged()), this,
        SLOT(inputSelectionChanged()));
    connect(session, SIGNAL(selectionChanged()), this,
        SLOT(sessionSelectionChanged()));

    actPaste = new QAction(this);
    actPaste->setText(tr("&Paste"));
    actPaste->setIcon(ReginaSupport::themeIcon("edit-paste"));
    actPaste->setShortcuts(QKeySequence::Paste);
    actPaste->setToolTip(tr(
        "Paste text from the clipboard into the input area"));
    actPaste->setWhatsThis(tr(
        "Paste text from the clipboard into the input area.  "
        "The input area is the small box at the bottom of the window "
        "where you can type your next command."));
    connect(actPaste, SIGNAL(triggered()), this, SLOT(paste()));
    actPaste->setEnabled(false);
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this,
        SLOT(clipboardChanged()));
    menuEdit->addAction(actPaste);

    act = new QAction(this);
    act->setText(tr("Select &All"));
    act->setIcon(ReginaSupport::themeIcon("edit-select-all"));
    act->setShortcuts(QKeySequence::SelectAll);
    act->setToolTip(tr("Select all text in the session log"));
    act->setWhatsThis(tr(
        "Select all text in the session log.  "
        "The session log is the main part of the window, where you see "
        "the full history of commands and their results."));
    connect(act, SIGNAL(triggered()), this, SLOT(selectAll()));
    menuEdit->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Scripting Overview"));
    act->setIcon(ReginaSupport::themeIcon("help-contents"));
    act->setShortcuts(QKeySequence::HelpContents);
    act->setToolTip(tr("Read Python scripting overview"));
    act->setWhatsThis(tr("Open the <i>Python Scripting</i> section of the "
        "users' handbook."));
    connect(act, SIGNAL(triggered()), this, SLOT(scriptingOverview()) );
    menuHelp->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Python API Reference"));
    act->setIcon(ReginaSupport::themeIcon("utilities-terminal"));
    act->setToolTip(tr("Read detailed Python scripting reference"));
    act->setWhatsThis(tr("Open the detailed reference of classes, methods "
        "and routines that Regina makes available to Python scripts."));
    connect(act, SIGNAL(triggered()), this, SLOT(pythonReference()));
    menuHelp->addAction(act);

    menuHelp->addSeparator();

    act = new QAction(this);
    act->setText(tr("What's &This?"));
    act->setIcon(ReginaSupport::themeIcon("help-contextual"));
    connect(act, SIGNAL(triggered()), this, SLOT(contextHelpActivated()));
    menuHelp->addAction(act);
    
    menuConsole->setTitle(tr("&Console"));
    menuEdit->setTitle(tr("&Edit"));
    menuHelp->setTitle(tr("&Help"));
    menuBar()->addMenu(menuConsole);
    menuBar()->addMenu(menuEdit);
    menuBar()->addMenu(menuHelp);

    inputSelectionChanged();
    sessionSelectionChanged();
    clipboardChanged();

    // Prepare the console for use.
    if (manager)
        manager->registerConsole(this);

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));

    output = new PythonConsole::OutputStream(this);
    error = new PythonConsole::ErrorStream(this);
    interpreter = new regina::python::PythonInterpreter(*output, *error);

    blockInput();
}

PythonConsole::~PythonConsole() {
    if (completer) {
        input->setCompleter(nullptr);
        delete completer;
    }

    ReginaPrefSet::global().windowPythonSize = size();

    delete interpreter;
    delete output;
    delete error;
    if (manager)
        manager->deregisterConsole(this);
}

void PythonConsole::addInput(const QString& input) {
    session->moveCursor(QTextCursor::End);
    session->insertHtml("<b>" + encode(input) + "</b><br>");
    QApplication::instance()->processEvents();
}

void PythonConsole::addOutput(const QString& output) {
    // Since empty output has no tags we need to be explicitly sure that
    // blank lines are still written.
    session->moveCursor(QTextCursor::End);
    if (output.isEmpty())
        session->insertHtml("<br>");
    else
        session->insertHtml(encode(output) + "<br>");
    QApplication::instance()->processEvents();
}

void PythonConsole::addInfo(const QString& info) {
    session->moveCursor(QTextCursor::End);
    session->insertHtml("<font color=\"dark goldenrod\">" + encode(info) +
        "</font><br>");
    QApplication::instance()->processEvents();
}

void PythonConsole::addError(const QString& output) {
    session->moveCursor(QTextCursor::End);
    session->insertHtml("<font color=\"dark red\">" + encode(output) +
        "</font><br>");
    QApplication::instance()->processEvents();
}

void PythonConsole::blockInput(const QString& msg) {
    input->setEnabled(false);
    prompt->setText("     ");
    if (msg.isEmpty())
        input->clear();
    else
        input->setText(msg);
}

void PythonConsole::allowInput(bool primaryPrompt,
        const QString& suggestedInput) {
    prompt->setText(primaryPrompt ? " >>> " : " ... ");
    if (suggestedInput.isEmpty())
        input->clear();
    else {
        input->setText(suggestedInput);
        input->end(false);
    }
    input->setEnabled(true);
    input->setFocus();
}

bool PythonConsole::importRegina() {
    if (interpreter->importRegina()) {
        executeLine("from regina import *; "
            "regina.reginaSetup(banner=True, readline=False, "
            "namespace=globals())");
        return true;
    } else {
        QString installationMsg;
        std::string regModuleDir = regina::GlobalDirs::pythonModule().c_str();
        if (! regModuleDir.empty())
            installationMsg = tr("The module 'regina' should be installed "
                "beneath the directory <tt>%1/</tt>.  ")
                .arg(QFile::decodeName(regModuleDir.c_str()).toHtmlEscaped());
        else
            installationMsg = tr("The module 'regina' should be installed "
                "in Python's standard site-packages directory.  ");
        ReginaSupport::warn(this,
            tr("Regina's Python module could not be loaded."),
            tr("<qt>%1Please mail the authors if you require "
            "further assistance.<p>"
            "None of Regina's functions will "
            "be available during this Python session.</qt>")
            .arg(installationMsg));
        addError(tr("Unable to load module \"regina\"."));
        return false;
    }
}

void PythonConsole::setRootPacket(regina::Packet* packet) {
    if (interpreter->setVar("root", packet)) {
        if (packet)
            addInfo(tr("The (invisible) root of the packet tree is in the "
                "variable [root]."));
    } else {
        ReginaSupport::warn(this,
            tr("<qt>I could not set the <i>root</i> variable.</qt>"),
            tr("The (invisible) root of the packet tree will not be available "
            "in this Python session.  Please report this error to the authors."));
        addError(tr("The variable \"root\" has not been set."));
    }
}

void PythonConsole::setSelectedPacket(regina::Packet* packet) {
    // Set the variable.
    if (interpreter->setVar("item", packet)) {
        if (packet)
            addInfo(tr("The selected packet (%1) is in the "
                "variable [item].").arg(packet->humanLabel().c_str()));
        // Set "selected" for backward compatibility with Regina <= 4.92.
        // Ignore any errors.
        interpreter->setVar("selected", packet);
    } else {
        ReginaSupport::warn(this,
            tr("<qt>I could not set the <i>item</i> variable.</qt>"),
            tr("The currently selected packet will not be available in "
            "this Python session.  Please report this error to the authors."));
        addError(tr("The variable \"item\" has not been set."));
    }
}

void PythonConsole::setVar(const QString& name, regina::Packet* value) {
    if (! interpreter->setVar(name.toUtf8(), value)) {
        QString pktName;
        if (value)
            pktName = value->humanLabel().c_str();
        else
            pktName = tr("None");

        addError(tr("Could not set variable %1 to %2.").arg(name)
            .arg(pktName));
    }
}

void PythonConsole::executeLine(const QString& line) {
    interpreter->executeLine(line.toUtf8().data());
}

void PythonConsole::executeLine(const std::string& line) {
    interpreter->executeLine(line);
}

void PythonConsole::executeLine(const char* line) {
    interpreter->executeLine(line);
}

void PythonConsole::runScript(regina::Script* script) {
    interpreter->runScript(script);
    
    if (interpreter->exitAttempted())
        close();
}

void PythonConsole::saveLog() {
    QString fileName =
        QFileDialog::getSaveFileName(this, tr("Save Session Transcript"),
        QString::null, tr(FILTER_ALL));
    if (! fileName.isEmpty()) {
        QFile f(fileName);
        if (! f.open(QIODevice::WriteOnly))
            ReginaSupport::warn(this,
                tr("I could not save the session transcript."),
                tr("<qt>An error occurred whilst "
                "attempting to write to the file <tt>%1</tt>.</qt>").
                arg(fileName.toHtmlEscaped()));
        else {
            QTextStream out(&f);
            out.setCodec(QTextCodec::codecForName("UTF-8"));
            out << session->toPlainText();
            endl(out);
        }
    }
}

void PythonConsole::scriptingOverview() {
    ReginaPrefSet::openHandbook("python", 0, this);
}

void PythonConsole::pythonReference() {
    PythonManager::openPythonReference(this);
}

void PythonConsole::contextHelpActivated() {
    QWhatsThis::enterWhatsThisMode();
}

QSize PythonConsole::sizeHint() const {
    if (ReginaPrefSet::global().windowPythonSize.isValid())
        return ReginaPrefSet::global().windowPythonSize;

    QRect d = QApplication::desktop()->availableGeometry();
    return QSize(d.width() / 2,
                 d.height() * 2 / 3); // A little taller for its size.
}

void PythonConsole::updatePreferences() {
    session->setWordWrapMode(ReginaPrefSet::global().pythonWordWrap ?
        QTextOption::WordWrap : QTextOption::NoWrap);
    input->setSpacesPerTab(ReginaPrefSet::global().pythonSpacesPerTab);

    QFont font(ReginaPrefSet::fixedWidthFont());
    session->setFont(font);
    prompt->setFont(font);
    input->setFont(font);
}

QString PythonConsole::encode(const QString& plaintext) {
    QString ans(plaintext);
    return ans.replace('&', "&amp;").
        replace('>', "&gt;").
        replace('<', "&lt;").
        replace(' ', "&nbsp;");
}

QString PythonConsole::initialIndent(const QString& line) {
    const char* start = line.toUtf8();
    const char* pos = start;
    while (*pos && isspace(*pos))
        pos++;

    // If the line is entirely whitespace then return no indent.
    if (*pos == 0)
        return "";
    else
        return line.left(pos - start);
}

void PythonConsole::processCommand() {
    // Fetch what we need and block input ASAP.
    QString cmd = input->text();
    QString cmdPrompt = prompt->text();
    blockInput(tr("Processing..."));

    // Log the input line.
    // Include the prompt but ignore the initial space.
    addInput(cmdPrompt.mid(1) + cmd);

    // Do the actual processing (which could take some time).
    QApplication::instance()->processEvents();
    bool done = interpreter->executeLine(cmd.toUtf8().constData());

    // Finish the output.
    interpreter->flush();
    
    // Prepare for a new command.
    if (interpreter->exitAttempted()) {
        close();
    } else if (ReginaPrefSet::global().pythonAutoIndent) {
        // Only use auto-indent if we are waiting on more text.
        if (done)
            allowInput(true);
        else
            allowInput(false, initialIndent(cmd));
    } else
        allowInput(done);
}

void PythonConsole::processCompletion(int start, int end) {
    GUICompleter comp;

    // Send a request for a completion to python.
    // We only send the last "word" before the cursor / selected block, where
    // a "word" starts with a character or underscore, and only contains
    // letters, numbers, underscores and/or the dot.
    QRegularExpression re("[\\p{Ll}\\p{Lu}\\p{Lt}\\p{Lo}_][\\p{Ll}\\p{Lu}\\p{Lt}\\p{Lo}\\p{Nd}_.]*$");
    QRegularExpressionMatch m;
    if (start == input->text().length())
        m = re.match(input->text());
    else
        m = re.match(input->text().left(start));
    if (! m.hasMatch()) {
        // Nothing to complete.
        return;
    }

    // The completion (if any) gets inserted between lineStart and lineEnd:
    QString lineStart = input->text().left(m.capturedStart());
    QString lineEnd = input->text().mid(end);
    input->setCompletionSurrounds(lineStart, lineEnd);

    // Ask python for all completions.
    QString lastWord = m.captured();
    if (interpreter->complete(lastWord.toUtf8().constData(), comp) < 0) {
        // Completion failed.
        return;
    }

    if (completer) {
        input->setCompleter(nullptr); // Remove current completer
        delete completer;
        completer = nullptr;
    }
    // Focus can be lost here since the "console" has gotten the most recent
    // signal.
    input->setFocus();

    if (comp.count() >= 1) {
        // Find a substring common to all suggestions.
        // Note: the PrefixCompleter base class computes this substring in
        // terms of unicode (i.e., it compares UTF-8 characters, not raw bytes).
        QString prefix(comp.prefix().c_str());

        // Automatically extend the last word to become this prefix.
        if (prefix.length() >= lastWord.length()) {
            input->insert(prefix.mid(lastWord.length()));
        } else {
            // This should never happen.
            std::cerr << "ERROR: Completion does not include "
                "original matched word" << std::endl;
            return;
        }

        if (comp.count() == 1)
            return;

        // There are multiple completions; show them to the user.
        completer = new QCompleter(comp.completions(), this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setWidget(input);

        // Disconnect activated signals from the new completer.
        // We call our own instead, since the default action is to completely
        // replace the text.
        disconnect(completer, nullptr, input, nullptr);
        connect(completer, SIGNAL(activated(const QString&)),
            input, SLOT(complete(QString)));
        connect(completer, SIGNAL(highlighted(const QString&)),
            input, SLOT(complete(QString)));

        // Tell the completer to give suggestions "now", normal operation is to
        // wait until the user types something, but here the user has already
        // typed.
        completer->complete();
    } else {
        // No completions, an error occured.
        // Note that if a valid word is completed (like "from") the whole word
        // is also returned as the sole completion, so this part of the
        // function only fires if python does not recognise the word at all.
        input->highlightErrorInInput();
    }
}


void PythonConsole::OutputStream::processOutput(const std::string& data) {
    // Strip the final newline (if any) before we process the string.
    if ((! data.empty()) && *(data.rbegin()) == '\n')
        console_->addOutput(data.substr(0, data.length() - 1).c_str());
    else
        console_->addOutput(data.c_str());
}

void PythonConsole::ErrorStream::processOutput(const std::string& data) {
    // Strip the final newline (if any) before we process the string.
    if ((! data.empty()) && *(data.rbegin()) == '\n')
        console_->addError(data.substr(0, data.length() - 1).c_str());
    else
        console_->addError(data.c_str());
}

void PythonConsole::cut() {
    input->cut();
}

void PythonConsole::copy() {
    if (input->hasSelectedText())
        input->copy();
    else
        session->copy();
}

void PythonConsole::paste() {
    input->paste();
}

void PythonConsole::selectAll() {
    session->selectAll();
}

void PythonConsole::inputSelectionChanged() {
    // Affected operations: cut, copy
    if (input->hasSelectedText()) {
        actCut->setEnabled(true);
        actCopy->setEnabled(true);

        // Surely there is a better way than this..?
        QTextCursor c = session->textCursor();
        c.clearSelection();
        session->setTextCursor(c);
    } else {
        actCut->setEnabled(false);
        actCopy->setEnabled(session->textCursor().hasSelection());
    }
}

void PythonConsole::sessionSelectionChanged() {
    // Affected operations: copy
    if (session->textCursor().hasSelection()) {
        actCopy->setEnabled(true);
        input->deselect();
    } else {
        actCopy->setEnabled(input->hasSelectedText());
    }
}

void PythonConsole::clipboardChanged() {
    // Affected operations: paste
    actPaste->setEnabled(
        ! QApplication::clipboard()->text(QClipboard::Clipboard).isNull());
}

// #include "pythonconsole.moc"
