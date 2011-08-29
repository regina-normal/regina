
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "../pythonmanager.h"
#include "../reginafilter.h"
#include "../reginaprefset.h"
#include "commandedit.h"
#include "pythonconsole.h"

#include <fstream>
#include <iostream>
#include <kaction.h>
#include <kapplication.h>
#include <kencodingfiledialog.h>
#include <kglobalsettings.h>
#include <khelpmenu.h>
#include <klocale.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <kstatusbar.h>
#include <ktextedit.h>
#include <qfile.h>
#include <qlabel.h>
#include <qtextcodec.h>
#include <qtextstream.h>
#include <qwhatsthis.h>

#include <KActionCollection>
#include <KStandardAction>
#include <KToolInvocation>
#include <QHBoxLayout>
#include <QMenu>
#include <QVBoxLayout>

PythonConsole::PythonConsole(QWidget* parent, PythonManager* useManager,
        const ReginaPrefSet* initialPrefs) :
        //KMainWindow(parent, "PythonConsole#"), manager(useManager) {
        KXmlGuiWindow(parent), manager(useManager) {
    // Initialise preferences.
    if (initialPrefs)
        prefs = *initialPrefs;

    // Resize ourselves nicely.
    if (! initialGeometrySet())
        resize(600, 500);

    // Set up the main widgets.
    QWidget* box = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout;
    session = new KTextEdit();
    session->setReadOnly(true);
    session->setWordWrapMode(prefs.pythonWordWrap ? QTextOption::WordWrap :
        QTextOption::NoWrap);
    session->setAutoFormatting(QTextEdit::AutoNone);
    session->setFont(KGlobalSettings::fixedFont());
    session->setFocusPolicy(Qt::NoFocus);
    session->setWhatsThis( i18n("This area stores a history of the entire "
        "Python session, including commands that have been typed and the "
        "output they have produced."));
    layout->addWidget(session, 1);

    QWidget* inputArea = new QWidget(box);
    QHBoxLayout *inputAreaLayout = new QHBoxLayout;
    inputAreaLayout->setContentsMargins(0, 0, 0, 0);
    
    inputArea->setWhatsThis( i18n("Type your Python commands into "
        "this box."));
    prompt = new QLabel();
    prompt->setFont(KGlobalSettings::fixedFont());
    inputAreaLayout->addWidget(prompt);

    input = new CommandEdit();
    input->setFont(KGlobalSettings::fixedFont());
    input->setSpacesPerTab(prefs.pythonSpacesPerTab);
    input->setFocus();
    connect(input, SIGNAL(returnPressed()), this, SLOT(processCommand()));
    inputAreaLayout->addWidget(input, 1);
    inputArea->setLayout(inputAreaLayout);
    layout->addWidget(inputArea);

    setCentralWidget(box);
    box->setLayout(layout);
    box->show();

    // Set up the actions.
    // Don't use XML files since we don't know whether we're in the shell or
    // the part.
    QMenu* menuConsole = new QMenu(this);
    QMenu* menuEdit = new QMenu(this);
    QMenu* menuHelp = new QMenu(this);

    KAction* act = actionCollection()->addAction("console_save");
    act->setText(i18n("&Save Session"));
    act->setIcon(KIcon("document-save"));
    act->setShortcut(tr("Ctrl+s"));
    act->setToolTip(i18n("Save session history"));
    act->setWhatsThis(i18n("Save the entire history of this Python session "
        "into a text file."));
    connect(act, SIGNAL(triggered()), this, SLOT(saveLog()));
    menuConsole->addAction(act);


    act = actionCollection()->addAction("console_close");
    act->setText(i18n("&Close"));
    act->setIcon(KIcon("window-close"));
    act->setShortcut(tr("Ctrl+d"));
    act->setToolTip(i18n("Close Python console"));
    connect(act, SIGNAL(triggered()), this, SLOT(close()));
    menuConsole->insertSeparator(act);
    menuConsole->addAction(act);

    KAction* actCopy = actionCollection()->addAction(
        KStandardAction::Copy,
        session,
        SLOT(copy()) );
    actCopy->setEnabled(false);
    connect(session, SIGNAL(copyAvailable(bool)), actCopy,
        SLOT(setEnabled(bool)));
    menuEdit->addAction(actCopy);

    act = actionCollection()->addAction(
        KStandardAction::SelectAll,
        session,
        SLOT(selectAll()) );

    menuEdit->addAction(act);

    act = actionCollection()->addAction("help_scripting");
    act->setText(i18n("&Scripting Overview"));
    act->setIcon(KIcon("help-contents"));
    act->setShortcut(tr("F1"));
    act->setToolTip(i18n("Read Python scripting overview"));
    act->setWhatsThis(i18n("Open the <i>Python Scripting</i> section of the "
        "users' handbook."));
    connect(act, SIGNAL(triggered()), this, SLOT(scriptingOverview()) );
    menuHelp->addAction(act);

    act = actionCollection()->addAction("help_engine");
    act->setText(i18n("&Python API Reference"));
    act->setIcon(KIcon("python_console"));
    act->setToolTip(i18n("Read detailed Python scripting reference"));
    act->setWhatsThis(i18n("Open the detailed reference of classes, methods "
        "and routines that Regina makes available to Python scripts."));
    connect(act, SIGNAL(triggered()), this, SLOT(pythonReference()));
    menuHelp->addAction(act);

    act = actionCollection()->addAction(
        KStandardAction::WhatsThis, this, SLOT(contextHelpActivated()) );
    menuHelp->insertSeparator(act);
    menuHelp->addAction(act);
    
    menuConsole->setTitle(i18n("&Console"));
    menuEdit->setTitle(i18n("&Edit"));
    menuHelp->setTitle(i18n("&Help"));
    menuBar()->addMenu(menuConsole);
    menuBar()->addMenu(menuEdit);
    menuBar()->addMenu(menuHelp);

    // Prepare the console for use.
    if (manager)
        manager->registerConsole(this);

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
    KApplication::kApplication()->processEvents();
}

void PythonConsole::addOutput(const QString& output) {
    // Since empty output has no tags we need to be explicitly sure that
    // blank lines are still written.
    session->moveCursor(QTextCursor::End);
    if (output.isEmpty())
        session->insertHtml("<br>");
    else
        session->insertHtml(encode(output) + "<br>");
    KApplication::kApplication()->processEvents();
}

void PythonConsole::addError(const QString& output) {
    session->moveCursor(QTextCursor::End);
    session->insertHtml("<font color=\"dark red\">" + encode(output) +
        "</font><br>");
    KApplication::kApplication()->processEvents();
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
        KMessageBox::error(this, i18n("<qt>The Python module <i>regina</i> "
            "could not be loaded.  None of Regina's functions will "
            "be available during this Python session.<p>"
            "The module should be installed as the file "
            "<tt>%1/regina.so</tt>.  Please write to %2 if you require "
            "further assistance.</qt>")
            .arg(QFile::decodeName(regina::NGlobalDirs::pythonModule().c_str()))
            .arg(PACKAGE_BUGREPORT));
        addError(i18n("Unable to load module \"regina\"."));
        return false;
    }
}

void PythonConsole::setRootPacket(regina::NPacket* packet) {
    if (interpreter->setVar("root", packet))
        addOutput(i18n("The root of the packet tree is in the "
            "variable [root]."));
    else {
        KMessageBox::error(this, i18n("<qt>An error occurred "
            "whilst attempting to place the root of the packet "
            "tree in the variable <i>root</i>.</qt>"));
        addError(i18n("The variable \"root\" has not been set."));
    }
}

void PythonConsole::setSelectedPacket(regina::NPacket* packet) {
    // Extract the packet name.
    QString pktName;
    if (packet)
        pktName = packet->getPacketLabel().c_str();
    else
        pktName = i18n("None");

    // Set the variable.
    if (interpreter->setVar("selected", packet))
        addOutput(i18n("The selected packet (%1) is in the "
            "variable [selected].").arg(pktName));
    else {
        KMessageBox::error(this, i18n("<qt>An error occurred "
            "whilst attempting to place the selected packet (%1) "
            "in the variable <i>selected</i>.</qt>").arg(pktName));
        addError(i18n("The variable \"selected\" has not been set."));
    }
}

void PythonConsole::setVar(const QString& name, regina::NPacket* value) {
    if (! interpreter->setVar(name.toAscii(), value)) {
        QString pktName;
        if (value)
            pktName = value->getPacketLabel().c_str();
        else
            pktName = i18n("None");

        addError(i18n("Could not set variable %1 to %2.").arg(name)
            .arg(pktName));
    }
}

void PythonConsole::loadAllLibraries() {
    for (ReginaFilePrefList::const_iterator it = prefs.pythonLibraries.begin();
            it != prefs.pythonLibraries.end(); it++) {
        if (! (*it).active)
            continue;

        QString shortName = QFileInfo((*it).filename).fileName();
        addOutput(i18n("Loading %1...").arg(shortName));
        if (! interpreter->runScript(
                static_cast<const char*>(it->encodeFilename()), shortName.toAscii())) {
            if (! QFileInfo((*it).filename).exists())
                addError(i18n("The library %1 does not exist.").
                    arg((*it).filename));
            else
                addError(i18n("The library %1 could not be loaded.").
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
    addOutput(scriptName.isEmpty() ? i18n("Running %1...").arg(scriptName) :
            i18n("Running script..."));
    interpreter->runScript(script.toAscii());
}

void PythonConsole::saveLog() {
    KEncodingFileDialog::Result result =
        KEncodingFileDialog::getSaveFileNameAndEncoding(
        QString::null /* encoding */, QString::null,
        i18n(FILTER_ALL), this, i18n("Save Session Transcript"));
    if ((! result.fileNames.empty()) &&
            (! result.fileNames.front().isEmpty())) {
        QFile f(result.fileNames.front());
        if (! f.open(QIODevice::WriteOnly))
            KMessageBox::error(this, i18n("An error occurred whilst "
                "attempting to write to the file %1.").
                arg(result.fileNames.front()));
        else {
            QTextStream out(&f);

            if (QTextCodec* encoding = QTextCodec::codecForName(
                    result.encoding.toAscii()))
                out.setCodec(encoding);
            else
                out.setCodec(QTextCodec::codecForName("UTF-8"));

            out << session->toPlainText();
            endl(out);
        }
    }
}

void PythonConsole::scriptingOverview() {
    prefs.openHandbook("python", this);
}

void PythonConsole::pythonReference() {
    PythonManager::openPythonReference(this);
}

void PythonConsole::contextHelpActivated() {
    QWhatsThis::enterWhatsThisMode();
}

void PythonConsole::updatePreferences(const ReginaPrefSet& newPrefs) {
    prefs = newPrefs;

    session->setWordWrapMode(prefs.pythonWordWrap ? QTextOption::WordWrap :
        QTextOption::NoWrap);
    input->setSpacesPerTab(prefs.pythonSpacesPerTab);
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
    blockInput(i18n("Processing..."));

    // Log the input line.
    // Include the prompt but ignore the initial space.
    addInput(cmdPrompt.mid(1) + cmd);

    // Do the actual processing (which could take some time).
    KApplication::kApplication()->processEvents();
    bool done = interpreter->executeLine(cmd.toAscii().constData());

    // Finish the output.
    output->flush();
    error->flush();

    // Prepare for a new command.
    if (prefs.pythonAutoIndent)
        allowInput(done, initialIndent(cmd));
    else
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
