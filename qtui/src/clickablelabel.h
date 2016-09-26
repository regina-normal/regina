
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file clickablelabel.h
 *  \brief Provides a label that emits a clicked() signal.
 */

#ifndef __CLICKABLELABEL_H
#define __CLICKABLELABEL_H

#include <QLabel>

/**
 * A label that responds to mouse presses.
 */
class ClickableLabel : public QLabel {
    Q_OBJECT

    public:
        /**
         * Constructors.
         */
        ClickableLabel(const QString& text, QWidget* parent = 0);
        ClickableLabel(const QPixmap& pixmap, QWidget* parent = 0);
        ClickableLabel(const QIcon& icon, QWidget* parent = 0);

        /**
         * The default icon size.
         */
        static const int defaultIconSize = 22;


    signals:
        void clicked();

    protected:
        void mouseReleaseEvent(QMouseEvent*);
};

inline ClickableLabel::ClickableLabel(const QString& text, QWidget* parent) :
        QLabel(text, parent) {
}

inline ClickableLabel::ClickableLabel(const QPixmap& pixmap, QWidget* parent) :
        QLabel(parent) {
    setPixmap(pixmap);
}

inline void ClickableLabel::mouseReleaseEvent(QMouseEvent*) {
    emit clicked();
}

#endif
