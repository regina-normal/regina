
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
#include <kglobalsettings.h>
#include <klineedit.h>
#include <klocale.h>
#include <ktextedit.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qvbox.h>

// TODO: ctrl-d, tab, keypresses upstairs, history
// TODO: copy all, save log
// TODO: python integration!!!

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

    // Prepare the console for use.
    if (manager)
        manager->registerConsole(this);
    interpreter = new PythonInterpreter();
    init();
}

PythonConsole::~PythonConsole() {
    delete interpreter;
    if (manager)
        manager->deregisterConsole(this);
}

void PythonConsole::processCommand() {
    QString cmd = input->text();

    // Log the input line.
    // Include the prompt but ignore the initial space.
    addInput(prompt->text().mid(1) + cmd);
    input->clear();
    setPromptMode(PROCESSING);

    // Do the actual processing.
    // TODO: Processing
    interpreter->executeLine(cmd.ascii());
    QString result = "Result from [" + cmd + "].";

    // Log the output.
    addOutput(result);
    setPromptMode(PRIMARY);
}

void PythonConsole::init() {
    // TODO: print regina.welcome()
    addOutput("Welcome!");
    setPromptMode(PRIMARY);
}

void PythonConsole::setPromptMode(PromptMode mode) {
    switch (mode) {
        case PRIMARY:    prompt->setText(" >>> "); break;
        case SECONDARY:  prompt->setText(" ... "); break;
        case PROCESSING: prompt->setText("     "); break;
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

#include "pythonconsole.moc"
