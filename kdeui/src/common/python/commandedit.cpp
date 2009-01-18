
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

#include "commandedit.h"

#include <iostream>
#include <kapplication.h>

#define COMMAND_EDIT_DEFAULT_SPACES_PER_TAB 4

CommandEdit::CommandEdit(QWidget* parent, const char* name) :
        KLineEdit(parent, name) {
    setSpacesPerTab(COMMAND_EDIT_DEFAULT_SPACES_PER_TAB);
    historyPos = history.end();
}

void CommandEdit::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Tab)
        insert(tabReplacement);
    else if (event->key() == Qt::Key_Up) {
        // Browse backwards through history.
        if (historyPos == history.end())
            newLine = text();
        if (historyPos == history.begin())
            QApplication::beep();
        else {
            historyPos--;
            setText(*historyPos);
            end(false);
        }
    } else if (event->key() == Qt::Key_Down) {
        // Browse forwards through history.
        if (historyPos == history.end())
            QApplication::beep();
        else {
            historyPos++;
            if (historyPos == history.end())
                setText(newLine);
            else
                setText(*historyPos);
        }
    } else if (event->key() == Qt::Key_Return) {
        // Save the current line in history before we process it.
        history.push_back(text());
        historyPos = history.end();
        KLineEdit::keyPressEvent(event);
    } else
        KLineEdit::keyPressEvent(event);
}

#include "commandedit.moc"
