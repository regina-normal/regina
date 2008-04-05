
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

/*! \file nsurfacematchingitem.h
 *  \brief Provides a list view item describing a single normal surface
 *  matching equation.
 */

#ifndef __NSURFACEMATCHINGITEM_H
#define __NSURFACEMATCHINGITEM_H

#include "../gridlistview.h"

namespace regina {
    class NMatrixInt;
};

/**
 * A list view item describing a single normal surface matching equation.
 */
class NSurfaceMatchingItem : public GridListViewItem {
    private:
        /**
         * The underlying matching equation.
         */
        const regina::NMatrixInt* eqns;
        unsigned long whichEqn;

    public:
        /**
         * Constructor.
         */
        NSurfaceMatchingItem(QListView* parent,
            const regina::NMatrixInt* newEqns, unsigned long newWhichEqn);

        /**
         * QListItem overrides.
         */
        QString text(int column) const;
};

inline NSurfaceMatchingItem::NSurfaceMatchingItem(QListView* parent,
        const regina::NMatrixInt* newEqns, unsigned long newWhichEqn) :
        GridListViewItem(parent), eqns(newEqns), whichEqn(newWhichEqn) {
}

#endif
