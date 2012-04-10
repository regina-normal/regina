
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

// UI includes:
#include "codecchooser.h"

#include <QtAlgorithms>
#include <QTextCodec>

CodecChooser::CodecChooser() : QComboBox() {
    QList<QByteArray> all = QTextCodec::availableCodecs();
    qSort(all);

    int defaultIndex = -1;
    for (int i = 0; i < all.size(); ++i) {
        addItem(all[i] /* constructor uses fromAscii() */);
        if (defaultIndex < 0 && all[i] == "UTF-8")
            defaultIndex = i;
    }

    if (defaultIndex >= 0)
        setCurrentIndex(defaultIndex);
}

QByteArray CodecChooser::selectedCodecName() {
    return currentText().toAscii();
}

void CodecChooser::setCodecName(const QByteArray& codec) {
    int pos = findText(QString::fromAscii(codec));
    if (pos >= 0)
        setCurrentIndex(pos);
}

