
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "regina-config.h"
#include "file/nglobaldirs.h"
#include "packet/npacket.h"

// Put this before any Qt/KDE stuff so Python 2.3 "slots" doesn't clash.
#include "pythoninterpreter.h"

#include "../pythonmanager.h"
#include "../reginafilter.h"
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
#include <kpopupmenu.h>
#include <kstatusbar.h>
#include <ktextedit.h>
#include <qfile.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qtextcodec.h>
#include <qtextstream.h>
#include <qvbox.h>
#include <qwhatsthis.h>

PythonConsole::PythonConsole(QWidget* parent, PythonManager* useManager,
        const ReginaPrefSet* initialPrefs) :
        KMainWindow(parent, "PythonConsole#"), manager(useManager) {
    // Initialise preferences.
    if (initialPrefs)
        prefs = *initialPrefs;

    // Resize ourselves nicely.
    if (! initialGeometrySet())
        resize(500, 400);

    // Set up the main widgets.
    QVBox* box = new QVBox(this);

    session = new KTextEdit(box);
    session->setTextFormat(Qt::LogText);
    session->setWordWrap(prefs.pythonWordWrap ? QTextEdit::WidgetWidth :
        QTextEdit::NoWrap);
    session->setAutoFormatting(QTextEdit::AutoNone);
    session->setFont(KGlobalSettings::fixedFont());
    session->setFocusPolicy(QWidget::NoFocus);
    QWhatsThis::add(session, i18n("This area stores a history of the entire "
        "Python session, including commands that have been typed and the "
        "output they have produced."));
    box->setStretchFactor(session, 1);

    QHBox* inputArea = new QHBox(box);
    QWhatsThis::add(inputArea, i18n("Type your Python commands into "
        "this box."));
    prompt = new QLabel(inputArea);
    prompt->setFont(KGlobalSettings::fixedFont());

    input = new CommandEdit(inputArea);
    input->setFont(KGlobalSettings::fixedFont());
    input->setSpacesPerTab(prefs.pythonSpacesPerTab);
    input->setFocus();
    connect(input, SIGNAL(returnPressed()), this, SLOT(processCommand()));

    setCentralWidget(box);
    box->show();

    // Set up the actions.
    // Don't use XML files since we don't know whether we're in the shell or
    // the part.
    KPopupMenu* menuConsole = new KPopupMenu(this);
    KPopupMenu* menuEdit = new KPopupMenu(this);
    KPopupMenu* menuHelp = new KPopupMenu(this);

    KAction* act = new KAction(i18n("&Save Session"), "filesave", CTRL+Key_S,
        this, SLOT(saveLog()), actionCollection(), "console_save");
    act->setToolTip(i18n("Save session history"));
    act->setWhatsThis(i18n("Save the entire history of this Python session "
        "into a text file."));
    act->plug(menuConsole);

    menuConsole->insertSeparator();

    act = new KAction(i18n("&Close"), "fileclose", CTRL+Key_D, this,
        SLOT(close()), actionCollection(), "console_close");
    act->setToolTip(i18n("Close Python console"));
    act->plug(menuConsole);

    KAction* actCopy = KStdAction::copy(session, SLOT(copy()),
        actionCollection());
    actCopy->setEnabled(false);
    connect(session, SIGNAL(copyAvailable(bool)), actCopy,
        SLOT(setEnabled(bool)));
    actCopy->plug(menuEdit);

    KStdAction::selectAll(session, SLOT(selectAll()), actionCollection())->
        plug(menuEdit);

    act = new KAction(i18n("&Scripting Overview"), "contents", Key_F1, this,
        SLOT(scriptingOverview()), actionCollection(), "help_scripting");
    act->setToolTip(i18n("Read Python scripting overview"));
    act->setWhatsThis(i18n("Open the <i>Python Scripting</i> section of the "
        "users' handbook."));
    act->plug(menuHelp);

    act = new KAction(i18n("&Python Reference"), "python_console", 0, this,
        SLOT(pythonReference()), actionCollection(), "help_engine");
    act->setToolTip(i18n("Read detailed Python scripting reference"));
    act->setWhatsThis(i18n("Open the detailed reference of classes, methods "
        "and routines that Regina makes available to Python scripts."));
    act->plug(menuHelp);

    menuHelp->insertSeparator();

    KStdAction::whatsThis(this, SLOT(whatsThis()),
        actionCollection())->plug(menuHelp);

    menuBar()->insertItem(i18n("&Console"), menuConsole);
    menuBar()->insertItem(i18n("&Edit"), menuEdit);
    menuBar()->insertItem(i18n("&Help"), menuHelp);

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
    session->append("<b>" + encode(input) + "</b>");
    session->scrollToBottom();
    KApplication::kApplication()->processEvents();
}

void PythonConsole::addOutput(const QString& output) {
    // Since empty output has no tags we need to be explicitly sure that
    // blank lines are still written.
    if (output.isEmpty())
        session->append("<br>");
    else
        session->append(encode(output));
    session->scrollToBottom();
    KApplication::kApplication()->processEvents();
}

void PythonConsole::addError(const QString& output) {
    session->append("<font color=\"dark red\">" + encode(output) + "</font>");
    session->scrollToBottom();
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
    if (! interpreter->setVar(name.ascii(), value)) {
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
                static_cast<const char*>(it->encodeFilename()), shortName)) {
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
    interpreter->executeLine(line.ascii());
}

void PythonConsole::executeLine(const std::string& line) {
    interpreter->executeLine(line);
}

void PythonConsole::executeLine(const char* line) {
    interpreter->executeLine(line);
}

bool PythonConsole::compileScript(const QString& script) {
    return interpreter->compileScript(script.ascii());
}

void PythonConsole::executeScript(const QString& script,
        const QString& scriptName) {
    addOutput(scriptName.isEmpty() ? i18n("Running %1...").arg(scriptName) :
            i18n("Running script..."));
    interpreter->runScript(script.ascii());
}

void PythonConsole::saveLog() {
    KEncodingFileDialog::Result result =
        KEncodingFileDialog::getSaveFileNameAndEncoding(
        QString::null /* encoding */, QString::null,
        i18n(FILTER_ALL), this, i18n("Save Session Transcript"));
    if ((! result.fileNames.empty()) &&
            (! result.fileNames.front().isEmpty())) {
        QFile f(result.fileNames.front());
        if (! f.open(IO_WriteOnly))
            KMessageBox::error(this, i18n("An error occurred whilst "
                "attempting to write to the file %1.").
                arg(result.fileNames.front()));
        else {
            QTextStream out(&f);

            if (QTextCodec* encoding = QTextCodec::codecForName(
                    result.encoding))
                out.setCodec(encoding);
            else
                out.setEncoding(QTextStream::UnicodeUTF8);

            // Write the contents to file.
            // We can't just dump text() since this includes HTML tags.
            // We also can't just remove tags since tags and text that
            // looks like tags are indistinguishable (i.e., plaintext is
            // not encoded but is still surrounded by tags - ugh).

            // For the moment we'll do it through selections.  I'm sure
            // there's a better way.
            session->selectAll(true);
            out << session->selectedText();
            endl(out);
            session->selectAll(false);
        }
    }
}

void PythonConsole::scriptingOverview() {
    KApplication::kApplication()->invokeHelp("python", "regina");
}

void PythonConsole::pythonReference() {
    PythonManager::openPythonReference(this);
}

void PythonConsole::updatePreferences(const ReginaPrefSet& newPrefs) {
    prefs = newPrefs;

    session->setWordWrap(prefs.pythonWordWrap ? QTextEdit::WidgetWidth :
        QTextEdit::NoWrap);
    input->setSpacesPerTab(prefs.pythonSpacesPerTab);
}

QString PythonConsole::encode(const QString& plaintext) {
    QString ans(plaintext);
    return ans.replace('&', "&amp;").
        replace('>', "&gt;").
        replace('<', "&lt;");
}

QString PythonConsole::initialIndent(const QString& line) {
    const char* start = line.ascii();
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
    bool done = interpreter->executeLine(cmd.ascii());

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

#include "pythonconsole.moc"
