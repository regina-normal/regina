
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
#include "angle/nanglestructure.h"

// UI includes:
#include "nanglestructureitem.h"

#include <klocale.h>
#include <klistview.h>
#include <qpainter.h>
#include <qstyle.h>

using regina::NAngleStructure;

QString NAngleStructureItem::text(int column) const {
    if (column == 0) {
        if (structure->isStrict())
            return i18n("Strict");
        else if (structure->isTaut())
            return i18n("Taut");
        else
            return QString::null;
    }

    return angleToString(structure->getAngle((column - 1) / 3,
        (column - 1) % 3));
}

QString NAngleStructureItem::angleToString(regina::NRational angle) {
    if (angle == 0)
        return QString::null;

    static const QString pi(i18n("Pi"));

    if (angle == 1)
        return pi;
    else if (angle.getDenominator() == 1)
        return QString(angle.getNumerator().stringValue().c_str()) + ' ' + pi;
    else if (angle.getNumerator() == 1)
        return pi + " / " + angle.getDenominator().stringValue().c_str();
    else
        return QString(angle.getNumerator().stringValue().c_str()) + ' ' + pi
            + " / " + angle.getDenominator().stringValue().c_str();
}

void NAngleStructureItem::paintCell(QPainter* p, const QColorGroup& cg,
        int column, int width, int align) {
    // Do the standard painting.
    KListViewItem::paintCell(p, cg, column, width, align);

    // Draw a box around the cell.
    p->setPen((QRgb)listView()->style().styleHint(
        QStyle::SH_Table_GridLineColor, listView()));
    p->drawLine(0, height() - 1, width - 1, height() - 1);
    p->lineTo(width - 1, 0);
}

