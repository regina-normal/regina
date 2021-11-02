
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "elidedlabel.h"
#include <QFontMetrics>
#include <QPainter>

void ElidedLabel::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    // TODO: This gets the default widget palette, not the default label
    // palette.  Is this a problem?
    painter.setPen(palette().color(QPalette::WindowText));

    QFontMetrics metrics = fontMetrics();

    painter.drawText(
        QPoint(0, (height() - metrics.height()) / 2 + metrics.ascent()),
        metrics.elidedText(fullText_, Qt::ElideRight, width()));
}

QSize ElidedLabel::sizeHint() const {
    // Add 2 to the width, since on macOS the bounding rectangle is
    // enough to trigger text elision even if there is enough room.
    //
    // (Why +2?  Because we assume some antialiasing effect causes
    // the text to bleed into the adjacent pixels on either side.)

    QSize s = QFontMetrics(font()).boundingRect(fullText_).size();
    return QSize(s.width() + 2, s.height());
}

