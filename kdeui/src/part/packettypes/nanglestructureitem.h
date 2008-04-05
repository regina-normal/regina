
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

/*! \file nanglestructureitem.h
 *  \brief Provides a list view item describing a single angle structure.
 */

#ifndef __NANGLESTRUCTUREITEM_H
#define __NANGLESTRUCTUREITEM_H

#include "utilities/nrational.h"

#include "../gridlistview.h"

namespace regina {
    class NAngleStructure;
    class NTriangulation;
};

/**
 * A list view item describing a single angle structure.
 */
class NAngleStructureItem : public GridListViewItem {
    private:
        /**
         * The underlying angle structure.
         */
        const regina::NAngleStructure* structure;
        int coordCols;

    public:
        /**
         * Constructor.
         */
        NAngleStructureItem(QListView* parent,
            const regina::NAngleStructure* newStructure,
            const regina::NTriangulation* fromTri);

        /**
         * QListItem overrides.
         */
        QString text(int column) const;

    private:
        /**
         * String conversions.
         *
         * This routine returns QString::null if the given angle is 0.
         */
        static QString angleToString(regina::NRational angle);
};

#endif
