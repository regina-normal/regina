
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

/*! \file coordinatechooser.h
 *  \brief Provides a widget for selecting a normal surface coordinate
 *  system.
 */

#ifndef __COORDINATECHOOSER_H
#define __COORDINATECHOOSER_H

#include <QComboBox>
#include <vector>
#include "hypersurface/hypercoords.h"
#include "surfaces/normalcoords.h"

namespace regina {
    class NormalHypersurfaces;
    class NormalSurfaces;
}

/**
 * A widget through which the user can select a normal surface
 * coordinate system in a 3-manifold triangulation.
 */
class CoordinateChooser : public QComboBox {
    Q_OBJECT

    private:
        std::vector<regina::NormalCoords> systems;
            /**< A list of the coordinate systems corresponding to the
                 available entries in the combo box. */

    public:
        /**
         * Constructor that creates an empty combo box.
         */
        CoordinateChooser();

        /**
         * Used to fill the combo box with coordinate systems.
         */
        void insertSystem(regina::NormalCoords coordSystem);
        void insertAllCreators();
        void insertAllViewers(regina::NormalSurfaces* surfaces);

        /**
         * Get and set the currently selected coordinate system.
         */
        void setCurrentSystem(regina::NormalCoords newSystem);
        regina::NormalCoords getCurrentSystem();
};

/**
 * A widget through which the user can select a normal hypersurface
 * coordinate system in a 4-manifold triangulation.
 */
class HyperCoordinateChooser : public QComboBox {
    Q_OBJECT

    private:
        std::vector<regina::HyperCoords> systems;
            /**< A list of the coordinate systems corresponding to the
                 available entries in the combo box. */

    public:
        /**
         * Constructor that creates an empty combo box.
         */
        HyperCoordinateChooser();

        /**
         * Used to fill the combo box with coordinate systems.
         */
        void insertSystem(regina::HyperCoords coordSystem);
        void insertAllCreators();
        void insertAllViewers(regina::NormalHypersurfaces* surfaces);

        /**
         * Get and set the currently selected coordinate system.
         */
        void setCurrentSystem(regina::HyperCoords newSystem);
        regina::HyperCoords getCurrentSystem();
};

inline CoordinateChooser::CoordinateChooser() : QComboBox() {
}

inline regina::NormalCoords CoordinateChooser::getCurrentSystem() {
    return systems[currentIndex()];
}

inline HyperCoordinateChooser::HyperCoordinateChooser() : QComboBox() {
}

inline regina::HyperCoords HyperCoordinateChooser::getCurrentSystem() {
    return systems[currentIndex()];
}

#endif
