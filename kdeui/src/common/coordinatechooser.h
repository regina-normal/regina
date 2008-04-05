
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

/*! \file coordinatechooser.h
 *  \brief Provides a widget for selecting a normal surface coordinate
 *  system.
 */

#ifndef __COORDINATECHOOSER_H
#define __COORDINATECHOOSER_H

#include <kcombobox.h>
#include <vector>

namespace regina {
    class NNormalSurfaceList;
}

/**
 * A widget through which a normal surface coordinate system can be
 * selected.  Coordinate systems are described by the integer constants
 * declared in regina::NNormalSurfaceList.
 */
class CoordinateChooser : public KComboBox {
    Q_OBJECT

    private:
        std::vector<int> systems;
            /**< A list of the coordinate systems corresponding to the
                 available entries in the combo box. */

    public:
        /**
         * Constructor that creates an empty combo box.
         */
        CoordinateChooser(QWidget* parent, const char* name = 0);

        /**
         * Used to fill the combo box with coordinate systems.
         */
        void insertSystem(int coordSystem);
        void insertAllCreators();
        void insertAllViewers(regina::NNormalSurfaceList* surfaces);

        /**
         * Get and set the currently selected coordinate system.
         */
        void setCurrentSystem(int newSystem);
        int getCurrentSystem();
};

inline CoordinateChooser::CoordinateChooser(QWidget* parent,
        const char* name) : KComboBox(parent, name) {
}

inline int CoordinateChooser::getCurrentSystem() {
    return systems[currentItem()];
}

#endif
