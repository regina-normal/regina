
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

/*! \file nscriptvaritems.h
 *  \brief Provides various table items for script variables.
 */

#ifndef __NSCRIPTVARITEMS_H
#define __NSCRIPTVARITEMS_H

#include <qtable.h>

/**
 * A table item for script variable names.
 */
class ScriptVarNameItem : public QTableItem {
    public:
        /**
         * Constructor.
         */
        ScriptVarNameItem(QTable* table, const QString& name);

        /**
         * QTableItem overrides.
         */
        virtual QWidget* createEditor() const;
        virtual void setContentFromEditor(QWidget* editor);
};

/**
 * A table item for script variable values.
class ScriptVarValueItem : public QTableItem {
};
 */

#endif
