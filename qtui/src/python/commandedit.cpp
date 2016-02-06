
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "commandedit.h"
#include "pythonconsole.h"

#include <iostream>
#include <QAbstractItemView>
#include <QApplication>
#include <QCompleter>
#include <QKeyEvent>
#include <QKeySequence>
#include <QPalette>

#define COMMAND_EDIT_DEFAULT_SPACES_PER_TAB 4

CommandEdit::CommandEdit(PythonConsole* parent) :
        QLineEdit(parent), console(parent) { 
    setSpacesPerTab(COMMAND_EDIT_DEFAULT_SPACES_PER_TAB);
    historyPos = history.end();
    oldColor = 0;
}

bool CommandEdit::event(QEvent* event) {
    if (event->type() != QEvent::KeyPress) {
        return QLineEdit::event(event);
    } else {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        bool completerUp = false;
        if (completer() && completer()->popup() &&
            completer()->popup()->hasFocus()) {
            completerUp = true;
        }
        setCompleter(0); // Started typing again, disable completer
        if (keyEvent->key() == Qt::Key_Tab) {
            // If we have some text, and it doesn't end in whitespace, we
            // attempt tab completion. Else just insert tabs as normal.
            if ((text().length() > 0) && (!text().right(1).contains(" "))) {
                emit completionRequested();
                return true;
            } else {
                insert(tabReplacement);
                return true;
            }
        } else {
            // Not pressing tab. Set foreground colour to normal.
            clearErrorInInput();
            if (keyEvent->key() == Qt::Key_Up) {
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
                return true;
            } else if (keyEvent->key() == Qt::Key_Down) {
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
                return true;
            } else if (keyEvent->key() == Qt::Key_Return) {
                if (completerUp) {
                    // Do nothing, completer will be closed. complete(completion);
                } else {
                    // Save the current line in history before we process it.
                    history.push_back(text());
                    historyPos = history.end();
                    console->processCommand();
                }
                return true;
            } else {
                return QLineEdit::event(event);
            }
        }
    }
}

void CommandEdit::keyPressEvent(QKeyEvent* event) {
    // Let the parent console handle clipboard events, which are "shared"
    // between the command area and the session log.
    if (event->matches(QKeySequence::Cut))
        console->cut();
    else if (event->matches(QKeySequence::Copy))
        console->copy();
    else if (event->matches(QKeySequence::Paste))
        console->paste();
    else if (event->matches(QKeySequence::SelectAll))
        console->selectAll();
    else
        QLineEdit::keyPressEvent(event);
}

void CommandEdit::complete(QString completion) {
    setText(lineStart + completion);
}

void CommandEdit::highlightErrorInInput() {
    QPalette pal = palette();
    if (!(oldColor))
        oldColor = new QColor(pal.color(QPalette::Text));
    pal.setColor(QPalette::Text, Qt::red);
    setPalette(pal);
}

void CommandEdit::setCompletionLineStart(QString line) {
    lineStart = line;
}

void CommandEdit::clearErrorInInput() {
    if (oldColor) {
        QPalette pal = palette();
        pal.setColor(QPalette::Text, *oldColor);
        setPalette(pal);
        delete oldColor;
        oldColor = 0;
    }
}

// #include "commandedit.moc"
