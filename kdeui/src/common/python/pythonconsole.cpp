
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "regina-config.h"
#include "packet/npacket.h"

#include "../pythonmanager.h"
#include "../reginafilter.h"
#include "pythonconsole.h"
#include "pythoninterpreter.h"

#include <fstream>
#include <iostream>
#include <kaction.h>
#include <kapplication.h>
#include <kfiledialog.h>
#include <kglobalsettings.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kstatusbar.h>
#include <ktextedit.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qvbox.h>

// TODO: tab, history

PythonConsole::PythonConsole(QWidget* parent, PythonManager* useManager,
        regina::NPacket* tree, regina::NPacket* selectedPacket) :
        KMainWindow(parent, "PythonConsole#"), manager(useManager) {
    // Resize ourselves nicely.
    if (! initialGeometrySet())
        resize(500, 400);

    // Set up the main widgets.
    QVBox* box = new QVBox(this);

    session = new KTextEdit(box);
    session->setTextFormat(Qt::LogText);
    session->setWordWrap(QTextEdit::NoWrap);
    session->setAutoFormatting(QTextEdit::AutoNone);
    session->setFont(KGlobalSettings::fixedFont());
    session->setFocusPolicy(QWidget::NoFocus);
    box->setStretchFactor(session, 1);

    QHBox* inputArea = new QHBox(box);
    prompt = new QLabel(inputArea);
    prompt->setFont(KGlobalSettings::fixedFont());

    input = new KLineEdit(inputArea);
    input->setFont(KGlobalSettings::fixedFont());
    input->setFocus();
    connect(input, SIGNAL(returnPressed()), this, SLOT(processCommand()));

    setCentralWidget(box);
    box->show();

    // Set up the actions.
    // Don't use XML files since we don't know whether we're in the shell or
    // the part.
    KPopupMenu* menuConsole = new KPopupMenu(this);
    KPopupMenu* menuEdit = new KPopupMenu(this);

    (new KAction(i18n("&Save Log"), "filesave", CTRL+Key_S, this,
        SLOT(saveLog()), actionCollection(), "console_save"))->
        plug(menuConsole);
    menuConsole->insertSeparator();
    (new KAction(i18n("&Close"), "fileclose", CTRL+Key_D, this, SLOT(close()),
        actionCollection(), "console_close"))->plug(menuConsole);

    KAction* actCopy = KStdAction::copy(session, SLOT(copy()),
        actionCollection());
    actCopy->setEnabled(false);
    connect(session, SIGNAL(copyAvailable(bool)), actCopy,
        SLOT(setEnabled(bool)));
    actCopy->plug(menuEdit);
    KStdAction::selectAll(session, SLOT(selectAll()), actionCollection())->
        plug(menuEdit);

    menuBar()->insertItem(i18n("&Console"), menuConsole);
    menuBar()->insertItem(i18n("&Edit"), menuEdit);

    // Prepare the console for use.
    if (manager)
        manager->registerConsole(this);

    output = new PythonConsole::OutputStream(this);
    error = new PythonConsole::ErrorStream(this);
    interpreter = new PythonInterpreter(output, error);

    init(tree, selectedPacket);
}

PythonConsole::~PythonConsole() {
    delete interpreter;
    delete output;
    delete error;
    if (manager)
        manager->deregisterConsole(this);
}

void PythonConsole::processCommand() {
    input->setEnabled(false);

    QString cmd = input->text();
    lastIndent = initialIndent(cmd);

    // Log the input line.
    // Include the prompt but ignore the initial space.
    addInput(prompt->text().mid(1) + cmd);
    input->setText(i18n("Processing..."));
    setPromptMode(PROCESSING);

    // Do the actual processing (which could take some time).
    KApplication::kApplication()->processEvents();
    bool done = interpreter->executeLine(cmd.ascii());

    // Finish the output.
    output->flush();
    error->flush();

    // Prepare for a new command.
    setPromptMode(done ? PRIMARY : SECONDARY);
    if (prefs.pythonAutoIndent) {
        input->setText(lastIndent);
        input->end(false);
    }
    input->setEnabled(true);
    input->setFocus();
}

void PythonConsole::saveLog() {
    QString file = KFileDialog::getSaveFileName(QString::null,
        i18n(FILTER_ALL), this, i18n("Save Session Transcript"));
    if (! file.isEmpty()) {
        std::ofstream out(file.ascii());
        if (out)
            out << session->text(); // TODO: This doesn't work (tags).
        else
            KMessageBox::error(this, i18n("An error occurred whilst "
                "attempting to write to the file %1.").arg(file));
    }
}

void PythonConsole::init(regina::NPacket* tree,
        regina::NPacket* selectedPacket) {
    // Import the regina module.
    if (! interpreter->importRegina()) {
        KMessageBox::error(this, i18n("<qt>The Python module <i>regina</i> "
            "could not be loaded.  None of Regina's functions will "
            "be available during this Python session.<p>"
            "The module should be installed as the file "
            "<tt>%1/regina.so</tt>.  Please write to %2 if you require "
            "further assistance.</qt>")
            .arg(REGINA_PYLIBDIR).arg(PACKAGE_BUGREPORT));
        addError(i18n("Unable to load module \"regina\"."));
    } else {
        interpreter->executeLine("print regina.welcome() + '\\n'");

        // Set base variables.
        if (tree) {
            if (interpreter->setVar("root", tree))
                addOutput(i18n("The root of the packet tree is in the "
                    "variable [root]."));
            else {
                KMessageBox::error(this, i18n("<qt>An error occurred "
                    "whilst attempting to place the root of the packet "
                    "tree in the variable <i>root</i>.</qt>"));
                addError(i18n("The variable \"root\" has not been set."));
            }
        }
        if (selectedPacket) {
            if (interpreter->setVar("selected", tree))
                addOutput(i18n("The selected packet (%1) is in the "
                    "variable [selected].")
                    .arg(selectedPacket->getPacketLabel().c_str()));
            else {
                KMessageBox::error(this, i18n("<qt>An error occurred "
                    "whilst attempting to place the selected packet (%1) "
                    "in the variable <i>selected</i>.</qt>")
                    .arg(selectedPacket->getPacketLabel().c_str()));
                addError(i18n("The variable \"selected\" has not been set."));
            }
        }
    }

    // TODO: Run library scripts.

    setPromptMode(PRIMARY);
}

void PythonConsole::setPromptMode(PromptMode mode) {
    switch (mode) {
        case PRIMARY:
            prompt->setText(" >>> ");
            break;
        case SECONDARY:
            prompt->setText(" ... ");
            break;
        case PROCESSING:
            prompt->setText("     ");
            break;
    }
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
