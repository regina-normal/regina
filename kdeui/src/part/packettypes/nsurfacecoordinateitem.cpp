
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

// Regina core includes:
#include "surfaces/nnormalsurface.h"

// UI includes:
#include "coordinates.h"
#include "nsurfacecoordinateitem.h"

#include <klocale.h>
#include <qpainter.h>
#include <qstyle.h>

unsigned NSurfaceCoordinateItem::propertyColCount(bool embeddedOnly) {
    return (embeddedOnly ? 8 : 5);
}

QString NSurfaceCoordinateItem::propertyColName(int whichCol,
        bool embeddedOnly) {
    if (embeddedOnly) {
        switch (whichCol) {
            case 0 : return i18n("Name");
            case 1 : return i18n("Euler");
            case 2 : return i18n("Orient");
            case 3 : return i18n("Sides");
            case 4 : return i18n("Bdry");
            case 5 : return i18n("Link");
            case 6 : return i18n("Crush");
            case 7 : return i18n("Type");
        }
    } else {
        switch (whichCol) {
            case 0 : return i18n("Name");
            case 1 : return i18n("Euler");
            case 2 : return i18n("Bdry");
            case 3 : return i18n("Link");
            case 4 : return i18n("Type");
        }
    }

    return i18n("Unknown");
}

QString NSurfaceCoordinateItem::propertyColDesc(int whichCol,
        bool embeddedOnly) {
    if (embeddedOnly) {
        switch (whichCol) {
            case 0: return i18n("Name (this has no special meaning and "
                "can be edited)");
            case 1: return i18n("Euler characteristic");
            case 2: return i18n("Orientability");
            case 3: return i18n("1-sided or 2-sided");
            case 4: return i18n("Does this surface have boundary?");
            case 5: return i18n("Has this surface been identified as "
                "the link of a particular subcomplex?");
            case 6: return i18n("Is it safe to crush this surface to a point?");
            case 7: return i18n("Other interesting properties");
        }
    } else {
        switch (whichCol) {
            case 0: return i18n("Name (this has no special meaning and "
                "can be edited)");
            case 1: return i18n("Euler characteristic");
            case 2: return i18n("Does this surface have boundary?");
            case 3: return i18n("Has this surface been identified as "
                "the link of a particular subcomplex?");
            case 4: return i18n("Other interesting properties");
        }
    }

    return i18n("Unknown");
}

QString NSurfaceCoordinateItem::text(int column) const {
    // TODO
}

void NSurfaceCoordinateItem::paintCell(QPainter* p, const QColorGroup& cg,
        int column, int width, int align) {
    // Do the standard painting.
    // TODO: Change colour if appropriate.
    KListViewItem::paintCell(p, cg, column, width, align);

    // Draw a box around the cell.
    p->setPen((QRgb)listView()->style().styleHint(
        QStyle::SH_Table_GridLineColor, listView()));
    p->drawLine(0, height() - 1, width - 1, height() - 1);
    p->lineTo(width - 1, 0);
}

