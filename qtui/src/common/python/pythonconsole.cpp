
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
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QTextCodec>
#include <QTextEdit>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWhatsThis>

PythonConsole::PythonConsole(QWidget* parent, PythonManager* useManager) :
        QMainWindow(parent), manager(useManager) {
    resize(600, 500);

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
    connect(act, SIGNAL(triggered()), this, SLOT(close()));
    menuConsole->addAction(act);

    act = new QAction(this);
    act->setText(tr("&Copy From Session"));
    act->setIcon(ReginaSupport::themeIcon("edit-copy"));
    act->setShortcut(tr("Shift+Ctrl+c"));
    act->setToolTip(tr(
        "Copy selected text from the session log to the clipboard "));
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
        "Selected all text in the session log"));
    connect(act, SIGNAL(triggered()), session, SLOT(selectAll()));
    menuEdit->addAction(act);

    /*
    menuEdit->addSeparator();

    // TODO: Enable iff text is selected (current signal is wrong)
    act = actionCollection()->addAction("edit_cut_input");
    act->setText(tr("Cut From Input"));
    act->setIcon(KIcon("edit-cut"));
    act->setShortcuts(QKeySequence::Cut);
    act->setToolTip(tr(
        "Cut selected text from the input area to the clipboard "));
    connect(act, SIGNAL(triggered()), input, SLOT(cut()));
    act->setEnabled(false);
    connect(input, SIGNAL(copyAvailable(bool)), act, SLOT(setEnabled(bool)));
    menuEdit->addAction(act);

    // TODO: Enable iff text is selected (current signal is wrong)
    act = actionCollection()->addAction("edit_copy_input");
    act->setText(tr("Copy From Input"));
    act->setIcon(KIcon("edit-copy"));
    act->setShortcuts(QKeySequence::Copy);
    act->setToolTip(tr(
        "Copy selected text from the input area to the clipboard "));
    connect(act, SIGNAL(triggered()), input, SLOT(copy()));
    act->setEnabled(false);
    connect(input, SIGNAL(copyAvailable(bool)), act, SLOT(setEnabled(bool)));
    menuEdit->addAction(act);

    // TODO: Enable iff clipboard has text (current signal is wrong)
    act = actionCollection()->addAction("edit_paste_input");
    act->setText(tr("Paste To Input"));
    act->setIcon(KIcon("edit-paste"));
    act->setShortcuts(QKeySequence::Paste);
    act->setToolTip(tr(
        "Paste text from the clipboard into the input area"));
    connect(act, SIGNAL(triggered()), input, SLOT(paste()));
    act->setEnabled(false);
    connect(input, SIGNAL(copyAvailable(bool)), act, SLOT(setEnabled(bool)));
    menuEdit->addAction(act);

    act = actionCollection()->addAction("edit_select_all_input");
    act->setText(tr("Select All From Input"));
    act->setIcon(KIcon("edit-select-all"));
    act->setShortcuts(QKeySequence::SelectAll);
    act->setToolTip(tr("Selected all text in the input area"));
    connect(act, SIGNAL(triggered()), input, SLOT(selectAll()));
    menuEdit->addAction(act);
    */

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
    act->setIcon(ReginaSupport::regIcon("python_console"));
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
    if (interpreter->importRegina())
        return true;
    else {
        ReginaSupport::warn(this,
            tr("Regina's Python module could not be loaded."),
            tr("<qt>The module should be installed as the file "
            "<tt>%1/regina.so</tt>.  Please write to %2 if you require "
            "further assistance.<p>"
            "None of Regina's functions will "
            "be available during this Python session.</qt>")
            .arg(QFile::decodeName(regina::NGlobalDirs::pythonModule().c_str()))
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
    for (ReginaFilePrefList::const_iterator it =
            ReginaPrefSet::global().pythonLibraries.begin();
            it != ReginaPrefSet::global().pythonLibraries.end(); it++) {
        if (! (*it).active)
            continue;

        QString shortName = QFileInfo((*it).filename).fileName();
        addOutput(tr("Loading %1...").arg(shortName));
        if (! interpreter->runScript(
                static_cast<const char*>(it->encodeFilename()), shortName.toAscii())) {
            if (! QFileInfo((*it).filename).exists())
                addError(tr("The library %1 does not exist.").
                    arg((*it).filename));
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
                tr("An error occurred whilst "
                "attempting to write to the file %1.").
                arg(fileName));
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

// #include "pythonconsole.moc"
