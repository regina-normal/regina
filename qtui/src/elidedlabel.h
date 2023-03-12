
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file elidedlabel.h
 *  \brief Provides a label that supports text elision.
 */

#ifndef __ELIDEDLABEL_H
#define __ELIDEDLABEL_H

#include "reginaqt.h"
#include <QFontMetrics>
#include <QWidget>

/**
 * A label whose text is elided on the right (truncated with an ellipsis)
 * if it does not fit within the widget's current size.
 *
 * Currently this class is very basic:
 *
 * - the text is always elided to the right;
 * - the text is always centred vertically, and left-aligned horizontally;
 * - the size hint is always precisely the size of the full text.
 *
 * More functionality will be added if/when this becomes necessary.
 */
class ElidedLabel : public QWidget {
    Q_OBJECT

    private:
        QString fullText_;

    public:
        ElidedLabel(QWidget* parent = nullptr);
        ElidedLabel(const QString& text, QWidget* parent = nullptr);

        void setText(const QString& text);

        QSize sizeHint() const override;

    protected:
        void paintEvent(QPaintEvent* event) override;
};

inline ElidedLabel::ElidedLabel(QWidget* parent) : QWidget(parent) {
}

inline ElidedLabel::ElidedLabel(const QString& text, QWidget* parent) :
        QWidget(parent), fullText_(text) {
}

inline void ElidedLabel::setText(const QString& text) {
    fullText_ = text;
    update();
}

#endif
