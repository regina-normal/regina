
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

// UI includes:
#include "../pythonmanager.h"
#include "pythonconsole.h"
#include "pythoninterpreter.h"

#include <iostream>
#include <kaction.h>
#include <kglobalsettings.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmenubar.h>
#include <kpopupmenu.h>
#include <kstatusbar.h>
#include <ktextedit.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qvbox.h>

// TODO: tab, history
// TODO: save log ; cut, copy, paste, select all
// TODO: long processing notification and visual updates

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

    KStdAction::cut(input, SLOT(cut()), actionCollection())->plug(menuEdit);
    KStdAction::copy(input, SLOT(copy()), actionCollection())->plug(menuEdit);
    KStdAction::paste(input, SLOT(paste()), actionCollection())->
        plug(menuEdit);
    menuEdit->insertSeparator();
    KStdAction::selectAll(input, SLOT(selectAll()), actionCollection())->
        plug(menuEdit);

    menuBar()->insertItem(i18n("&Console"), menuConsole);
    menuBar()->insertItem(i18n("&Edit"), menuEdit);

    // Prepare the console for use.
    if (manager)
        manager->registerConsole(this);
    interpreter = new PythonInterpreter(this, this);
    init();
}

PythonConsole::~PythonConsole() {
    delete interpreter;
    if (manager)
        manager->deregisterConsole(this);
}

void PythonConsole::processCommand() {
    QString cmd = input->text();
    lastIndent = initialIndent(cmd);

    // Log the input line.
    // Include the prompt but ignore the initial space.
    addInput(prompt->text().mid(1) + cmd);
    input->clear();
    setPromptMode(PROCESSING);

    // Do the actual processing.
    bool done = interpreter->executeLine(cmd.ascii());

    // Log the output.
    PythonOutputStream::flush();

    // Prepare for a new command.
    setPromptMode(done ? PRIMARY : SECONDARY);
    if (prefs.pythonAutoIndent) {
        input->setText(lastIndent);
        input->end(false);
    }
}

void PythonConsole::saveLog() {
    // TODO
}

void PythonConsole::processOutput(const std::string& data) {
    // Strip the final newline (if any) before we process the string.
    if ((! data.empty()) && *(data.rbegin()) == '\n')
        addOutput(data.substr(0, data.length() - 1).c_str());
    else
        addOutput(data.c_str());
}

void PythonConsole::init() {
    interpreter->importRegina();
    interpreter->executeLine("print regina.welcome()");

    // TODO: More startup tasks.

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
}

void PythonConsole::addOutput(const QString& output) {
    session->append(encode(output));
    session->scrollToBottom();
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

#include "pythonconsole.moc"
