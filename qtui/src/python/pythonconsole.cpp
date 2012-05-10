
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include "Python.h"
#include "regina-config.h"
#include "file/nglobaldirs.h"
#include "packet/npacket.h"

// Put this before any Qt/KDE stuff so Python 2.3 "slots" doesn't clash.
#include "pythoninterpreter.h"

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
#include <QDesktopWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QTextCodec>
#include <QTextDocument>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWhatsThis>

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

    input = new CommandEdit();
    input->setWhatsThis(inputMsg);
    input->setFocus();
    connect(input, SIGNAL(returnPressed()), this, SLOT(processCommand()));
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

    act = new QAction(this);
    act->setText(tr("&Copy From Session"));
    act->setIcon(ReginaSupport::themeIcon("edit-copy"));
    act->setShortcut(tr("Shift+Ctrl+c"));
    act->setToolTip(tr(
        "Copy selected text from the session log to the clipboard"));
    act->setWhatsThis(tr(
        "Copy selected text from the session log to the clipboard.  "
        "The session log is the main part of the window, where you see "
        "the full history of commands and their results."));
    connect(act, SIGNAL(triggered()), session, SLOT(copy()));
    act->setEnabled(false);
    connect(session, SIGNAL(copyAvailable(bool)), act,
        SLOT(setEnabled(bool)));
    menuEdit->addAction(act);

    act = new QAction(this);
    act->setText(tr("Select &All From Session"));
    act->setIcon(ReginaSupport::themeIcon("edit-select-all"));
    act->setShortcut(tr("Shift+Ctrl+a"));
    act->setToolTip(tr(
        "Select all text in the session log"));
    act->setWhatsThis(tr(
        "Select all text in the session log.  "
        "The session log is the main part of the window, where you see "
        "the full history of commands and their results."));
    connect(act, SIGNAL(triggered()), session, SLOT(selectAll()));
    menuEdit->addAction(act);

    menuEdit->addSeparator();

    actCutInput = new QAction(this);
    actCutInput->setText(tr("Cut From Input"));
    actCutInput->setIcon(ReginaSupport::themeIcon("edit-cut"));
    actCutInput->setShortcuts(QKeySequence::Cut);
    actCutInput->setToolTip(tr(
        "Cut selected text from the input area to the clipboard"));
    actCutInput->setWhatsThis(tr(
        "Cut selected text from the input area to the clipboard.  "
        "The input area is the small box at the bottom of the window "
        "where you can type your next command."));
    connect(actCutInput, SIGNAL(triggered()), input, SLOT(cut()));
    actCutInput->setEnabled(false);
    menuEdit->addAction(actCutInput);

    actCopyInput = new QAction(this);
    actCopyInput->setText(tr("Copy From Input"));
    actCopyInput->setIcon(ReginaSupport::themeIcon("edit-copy"));
    actCopyInput->setShortcuts(QKeySequence::Copy);
    actCopyInput->setToolTip(tr(
        "Copy selected text from the input area to the clipboard"));
    actCopyInput->setWhatsThis(tr(
        "Copy selected text from the input area to the clipboard.  "
        "The input area is the small box at the bottom of the window "
        "where you can type your next command."));
    connect(actCopyInput, SIGNAL(triggered()), input, SLOT(copy()));
    actCopyInput->setEnabled(false);
    menuEdit->addAction(actCopyInput);

    connect(input, SIGNAL(selectionChanged()), this,
        SLOT(updateClipboardActions()));

    actPasteInput = new QAction(this);
    actPasteInput->setText(tr("Paste To Input"));
    actPasteInput->setIcon(ReginaSupport::themeIcon("edit-paste"));
    actPasteInput->setShortcuts(QKeySequence::Paste);
    actPasteInput->setToolTip(tr(
        "Paste text from the clipboard into the input area"));
    actPasteInput->setWhatsThis(tr(
        "Paste text from the clipboard into the input area.  "
        "The input area is the small box at the bottom of the window "
        "where you can type your next command."));
    connect(actPasteInput, SIGNAL(triggered()), input, SLOT(paste()));
    actPasteInput->setEnabled(false);
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this,
        SLOT(updateClipboardActions()));
    menuEdit->addAction(actPasteInput);

    act = new QAction(this);
    act->setText(tr("Select All From Input"));
    act->setIcon(ReginaSupport::themeIcon("edit-select-all"));
    act->setShortcuts(QKeySequence::SelectAll);
    act->setToolTip(tr("Select all text in the input area"));
    act->setWhatsThis(tr("Select all text in the input area.  "
        "The input area is the small box at the bottom of the window "
        "where you can type your next command."));
    connect(act, SIGNAL(triggered()), input, SLOT(selectAll()));
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

    updateClipboardActions();

    // Prepare the console for use.
    if (manager)
        manager->registerConsole(this);

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));

    output = new PythonConsole::OutputStream(this);
    error = new PythonConsole::ErrorStream(this);
    interpreter = new PythonInterpreter(output, error);

    blockInput();
}

PythonConsole::~PythonConsole() {
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
        executeLine("from regina import *");
        return true;
    } else {
        ReginaSupport::warn(this,
            tr("Regina's Python module could not be loaded."),
            tr("<qt>The module should be installed as the file "
            "<tt>%1/regina.so</tt>.  Please write to %2 if you require "
            "further assistance.<p>"
            "None of Regina's functions will "
            "be available during this Python session.</qt>")
            .arg(Qt::escape(QFile::decodeName(
                regina::NGlobalDirs::pythonModule().c_str())))
            .arg(PACKAGE_BUGREPORT));
        addError(tr("Unable to load module \"regina\"."));
        return false;
    }
}

void PythonConsole::setRootPacket(regina::NPacket* packet) {
    if (interpreter->setVar("root", packet))
        addOutput(tr("The root of the packet tree is in the "
            "variable [root]."));
    else {
        ReginaSupport::warn(this,
            tr("<qt>I could not set the <i>root</i> variable.</qt>"),
            tr("The root of the packet tree will not be available in "
            "this Python session.  Please report this error to %2.")
            .arg(PACKAGE_BUGREPORT));
        addError(tr("The variable \"root\" has not been set."));
    }
}

void PythonConsole::setSelectedPacket(regina::NPacket* packet) {
    // Extract the packet name.
    QString pktName;
    if (packet)
        pktName = packet->getPacketLabel().c_str();
    else
        pktName = tr("None");

    // Set the variable.
    if (interpreter->setVar("selected", packet))
        addOutput(tr("The selected packet (%1) is in the "
            "variable [selected].").arg(pktName));
    else {
        ReginaSupport::warn(this,
            tr("<qt>I could not set the <i>selected</i> variable.</qt>"),
            tr("The currently selected packet will not be available in "
            "this Python session.  Please report this error to %2.")
            .arg(PACKAGE_BUGREPORT));
        addError(tr("The variable \"selected\" has not been set."));
    }
}

void PythonConsole::setVar(const QString& name, regina::NPacket* value) {
    if (! interpreter->setVar(name.toAscii(), value)) {
        QString pktName;
        if (value)
            pktName = value->getPacketLabel().c_str();
        else
            pktName = tr("None");

        addError(tr("Could not set variable %1 to %2.").arg(name)
            .arg(pktName));
    }
}

void PythonConsole::loadAllLibraries() {
    foreach (const ReginaFilePref& f, ReginaPrefSet::global().pythonLibraries) {
        if (! f.isActive())
            continue;

        QString shortName = f.shortDisplayName();
        addOutput(tr("Loading %1...").arg(shortName));
        if (! interpreter->runScript(
                static_cast<const char*>(f.encodeFilename()),
                shortName.toAscii())) {
            if (! f.exists())
                addError(tr("The library %1 does not exist.").
                    arg(f.longDisplayName()));
            else
                addError(tr("The library %1 could not be loaded.").
                    arg(shortName));
        }
    }
}

void PythonConsole::executeLine(const QString& line) {
    interpreter->executeLine(line.toAscii().data());
}

void PythonConsole::executeLine(const std::string& line) {
    interpreter->executeLine(line);
}

void PythonConsole::executeLine(const char* line) {
    interpreter->executeLine(line);
}

bool PythonConsole::compileScript(const QString& script) {
    return interpreter->compileScript(script.toAscii());
}

void PythonConsole::executeScript(const QString& script,
        const QString& scriptName) {
    addOutput(scriptName.isEmpty() ? tr("Running %1...").arg(scriptName) :
            tr("Running script..."));
    interpreter->runScript(script.toAscii());
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
                arg(Qt::escape(fileName)));
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
    const char* start = line.toAscii();
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
    bool done = interpreter->executeLine(cmd.toAscii().constData());

    // Finish the output.
    output->flush();
    error->flush();

    // Prepare for a new command.
    if (ReginaPrefSet::global().pythonAutoIndent) {
        // Only use auto-indent if we are waiting on more text.
        if (done)
            allowInput(true);
        else
            allowInput(false, initialIndent(cmd));
    } else
        allowInput(done);
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

void PythonConsole::updateClipboardActions() {
    actCutInput->setEnabled(input->hasSelectedText());
    actCopyInput->setEnabled(input->hasSelectedText());
    actPasteInput->setEnabled(
        ! QApplication::clipboard()->text(QClipboard::Clipboard).isNull());
}

// #include "pythonconsole.moc"
