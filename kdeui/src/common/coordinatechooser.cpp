
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

// Regina core includes:
#include "surfaces/nnormalsurfacelist.h"

// UI includes:
#include "coordinatechooser.h"
#include "coordinates.h"

#include <algorithm>

using regina::NNormalSurfaceList;

void CoordinateChooser::insertSystem(int coordSystem) {
    insertItem(Coordinates::name(coordSystem));
    systems.push_back(coordSystem);
}

void CoordinateChooser::insertAllCreators() {
    insertSystem(NNormalSurfaceList::STANDARD);
    insertSystem(NNormalSurfaceList::AN_STANDARD);
    insertSystem(NNormalSurfaceList::QUAD);
}

void CoordinateChooser::insertAllViewers(regina::NNormalSurfaceList* surfaces) {
    if (surfaces->allowsAlmostNormal()) {
        insertSystem(NNormalSurfaceList::AN_STANDARD);
    } else {
        insertSystem(NNormalSurfaceList::STANDARD);
        insertSystem(NNormalSurfaceList::QUAD);
    }

    insertSystem(NNormalSurfaceList::EDGE_WEIGHT);
    insertSystem(NNormalSurfaceList::FACE_ARCS);
}

void CoordinateChooser::setCurrentSystem(int newSystem) {
    std::vector<int>::const_iterator it =
        std::find(systems.begin(), systems.end(), newSystem);

    if (it != systems.end())
        setCurrentItem(it - systems.begin());
}

#include "coordinatechooser.moc"
