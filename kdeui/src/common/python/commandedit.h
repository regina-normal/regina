
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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

/*! \file commandedit.h
 *  \brief Provides a line edit designed for entering python commands.
 */

#ifndef __COMMANDEDIT_H
#define __COMMANDEDIT_H

#include <klineedit.h>
#include <qstringlist.h>

/**
 * A line edit extension designed for entering python commands.
 * Tabs are converted into spaces, and command-line history is
 * available.
 */
class CommandEdit : public KLineEdit {
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

    public:
        /**
         * Constructor.
         */
        CommandEdit(QWidget* parent = 0, const char* name = 0);

        /**
         * Configuration.
         */
        unsigned getSpacesPerTab();
        void setSpacesPerTab(unsigned spacesPerTab);

    protected:
        /**
         * QLineEdit/KLineEdit overrides.
         */
        void keyPressEvent(QKeyEvent* event);
};

inline unsigned CommandEdit::getSpacesPerTab() {
    return tabReplacement.length();
}

inline void CommandEdit::setSpacesPerTab(unsigned spacesPerTab) {
    tabReplacement.fill(' ', spacesPerTab);
}

#endif
