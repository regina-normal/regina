
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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

/*! \file snappeacomponents.h
 *  \brief Provides GUI components for use with SnapPea calculations.
 */

#ifndef __SNAPPEACOMPONENTS_H
#define __SNAPPEACOMPONENTS_H

#include "triangulation/forward.h"
#include <qlabel.h>

/**
 * Displays a piece of text explaining that SnapPea calculations are not
 * available for a particular triangulation and suggesting why this might be
 * the case.
 *
 * The explanation offered depends upon the properties of the individual
 * triangulation.
 */
class NoSnapPea : public QLabel {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Triangulation<3>* tri;

    public:
        /**
         * Constructor and destructor.
         *
         * If \a delayedRefresh is passed as \c true to the constructor, the
         * text contents will not be initialised until refresh() is called.
         * This allows for a delayed analysis of the underlying
         * triangulation (as may be desirable in a tabbed packet UI, for
         * instance).
         */
        NoSnapPea(regina::Triangulation<3>* useTri,
            QWidget* parent = 0, bool delayedRefresh = false);

        /**
         * Updates the explanation in case the triangulation (or the
         * global SnapPea preferences) have changed.
         */
        void refresh();
};

#endif
