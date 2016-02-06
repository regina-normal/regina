
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

/*! \file commandedit.h
 *  \brief Provides a line edit designed for entering python commands.
 */

#ifndef __COMMANDEDIT_H
#define __COMMANDEDIT_H

#include <QStringList>
#include <QLineEdit>

class PythonConsole;

/**
 * A line edit extension designed for entering python commands.
 * Tabs are converted into spaces, command-line history is available,
 * and cut/copy/paste shortcuts are disabled (so that Regina's actions
 * are called instead).
 */
class CommandEdit : public QLineEdit {
    Q_OBJECT

    private:
        QStringList history;
            /**< The command-line history. */
        QStringList::const_iterator historyPos;
            /**< The current browsing position in command-line history. */
        QString newLine;
            /**< Remembers the new partially-typed command whilst
                 browsing through history. */
        QString tabReplacement;
            /**< The set of spaces to insert for each TAB keypress. */
        QString lineStart;
            /**< The start of the line when a completion is attempted. */
        PythonConsole* console;
            /**< The associated python console. */
        QColor *oldColor;
            /**< The default color of the input text, so we can 
                 change colours if needed. */
       
        void clearErrorInInput();
            /**< Return foreground colour of the input box to the default
                 colour. */

    signals:
        void completionRequested();
            /**< Emitted when tab completion is requested. 
             */

    private slots:
        void complete(QString);
            /**< Override from QT to work completions on words in the
             *   middle of a QLineEdit. */

    public:
        /**
         * Constructor.
         */
        CommandEdit(PythonConsole* parent);

        /**
         * Configuration.
         */
        unsigned getSpacesPerTab();
        void setSpacesPerTab(unsigned spacesPerTab);
        
        /**
         * Set the foreground colour of the input box, to highlight
         * that an error occured.
         */
        void highlightErrorInInput();
       
        /**
         * Stores the part of the line that should not change when
         * a completion is attempted mid-line.
         */
        void setCompletionLineStart(QString start);

    protected:
        /**
         * QLineEdit overrides.
         */
        bool event(QEvent* event);
        void keyPressEvent(QKeyEvent* event);
};

inline unsigned CommandEdit::getSpacesPerTab() {
    return tabReplacement.length();
}

inline void CommandEdit::setSpacesPerTab(unsigned spacesPerTab) {
    tabReplacement.fill(' ', spacesPerTab);
}

#endif
