
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

#include "gridlistview.h"

#include <qglobal.h>
#include <qpainter.h>
#include <qstyle.h>

int GridListViewItem::width(const QFontMetrics& fm, const QListView* lv, int c)
        const {
    /**
     * Add a bit of space so items aren't pressed right against the
     * grid.
     */
    return KListViewItem::width(fm, lv, c) + 2;
}

void GridListViewItem::paintCell(QPainter* p, const QColorGroup& cg,
        int column, int width, int align) {
    // Do the standard painting.
    KListViewItem::paintCell(p, cg, column, width, align);

    // Draw a box around the cell.
#if QT_VERSION < 0x030200
    // Style hint SH_Table_GridLineColor is not supported in Qt 3.1 or
    // earlier (I believe).
    // Use its default implementation directly.
    p->setPen((QRgb)listView()->colorGroup().mid().rgb());
#else
    p->setPen((QRgb)listView()->style().styleHint(
        QStyle::SH_Table_GridLineColor, listView()));
#endif
    p->drawLine(0, height() - 1, width - 1, height() - 1);
    p->lineTo(width - 1, 0);
}

// (int)(w ? w->colorGroup().mid().rgb() : 0)

