
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

// UI includes:
#include "codecchooser.h"

#include <algorithm>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QTextCodec>
#endif

CodecChooser::CodecChooser() : QComboBox() {
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    addItem(QStringConverter::nameForEncoding(QStringConverter::Utf8));
    addItem(QStringConverter::nameForEncoding(QStringConverter::Utf16));
    addItem(QStringConverter::nameForEncoding(QStringConverter::Utf16BE));
    addItem(QStringConverter::nameForEncoding(QStringConverter::Utf16LE));
    addItem(QStringConverter::nameForEncoding(QStringConverter::Utf32));
    addItem(QStringConverter::nameForEncoding(QStringConverter::Utf32BE));
    addItem(QStringConverter::nameForEncoding(QStringConverter::Utf32LE));
    addItem(QStringConverter::nameForEncoding(QStringConverter::Latin1));
    addItem(QStringConverter::nameForEncoding(QStringConverter::System));

    setCurrentIndex(0); // UTF-8
#else
    QList<QByteArray> all = QTextCodec::availableCodecs();
    std::sort(all.begin(), all.end());

    int defaultIndex = -1;
    for (int i = 0; i < all.size(); ++i) {
        addItem(all[i] /* constructor uses fromAscii() */);
        if (defaultIndex < 0 && all[i] == "UTF-8")
            defaultIndex = i;
    }

    if (defaultIndex >= 0)
        setCurrentIndex(defaultIndex);
#endif
}

QByteArray CodecChooser::selectedCodecName() {
    // UTF-8 is ok since codec names are just ASCII, no?
    return currentText().toUtf8();
}

void CodecChooser::setCodecName(const QByteArray& codec) {
    // UTF-8 is ok since codec names are just ASCII, no?
    int pos = findText(QString::fromUtf8(codec));
    if (pos >= 0)
        setCurrentIndex(pos);
}
