
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

/*! \file bigwidget.h
 *  \brief Provides a widget whose size hint is larger than just the sum
 *  of its parts.
 */

#ifndef __BIGWIDGET_H
#define __BIGWIDGET_H

#include <QWidget>

/**
 * A widget whose size hint is larger than otherwise necessary.
 */
class BigWidget : public QWidget {
    private:
        int desktopNum_;
        int desktopDen_;

    public:
        /**
         * Constructor: the size hint will always be at least the given
         * fraction of the available desktop size (measured as a single
         * screen, not all virtual desktops combined).
         */
        BigWidget(int desktopFractionNumerator,
                  int desktopFractionDenominator,
                  QWidget* parent = 0);

    protected:
        /**
         * Qt overrides.
         */
        virtual QSize sizeHint() const;
};

inline BigWidget::BigWidget(int desktopFractionNumerator,
        int desktopFractionDenominator, QWidget* parent) :
        QWidget(parent),
        desktopNum_(desktopFractionNumerator),
        desktopDen_(desktopFractionDenominator) {
}

#endif
