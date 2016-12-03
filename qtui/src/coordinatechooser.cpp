
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

// Regina core includes:
#include "surfaces/normalsurfaces.h"

// UI includes:
#include "coordinatechooser.h"
#include "coordinates.h"
#include "reginaprefset.h"

#include <algorithm>

using regina::NormalSurfaces;

void CoordinateChooser::insertSystem(regina::NormalCoords coordSystem) {
    addItem(tr(Coordinates::name(coordSystem)));
    systems.push_back(coordSystem);
}

void CoordinateChooser::insertAllCreators() {
    insertSystem(regina::NS_STANDARD);
    insertSystem(regina::NS_AN_STANDARD);
    insertSystem(regina::NS_QUAD);
    insertSystem(regina::NS_AN_QUAD_OCT);
    if (ReginaPrefSet::global().surfacesSupportOriented) {
        insertSystem(regina::NS_ORIENTED);
        insertSystem(regina::NS_ORIENTED_QUAD);
    }
}

void CoordinateChooser::insertAllViewers(regina::NormalSurfaces* surfaces) {
    if (surfaces->allowsAlmostNormal()) {
        // For lists created with Regina 4.5.1 or earlier, we have
        // already taken out surfaces with multiple octagons.  Make this
        // clear to the user.
        if (surfaces->coords() == regina::NS_AN_LEGACY)
            insertSystem(regina::NS_AN_LEGACY);
        else {
            insertSystem(regina::NS_AN_STANDARD);
            insertSystem(regina::NS_AN_QUAD_OCT);
        }
    } else {
        insertSystem(regina::NS_STANDARD);
        insertSystem(regina::NS_QUAD);

        if (surfaces->allowsOriented()) {
            insertSystem(regina::NS_ORIENTED);
            insertSystem(regina::NS_ORIENTED_QUAD);
        }
    }

    insertSystem(regina::NS_EDGE_WEIGHT);
    insertSystem(regina::NS_TRIANGLE_ARCS);
}

void CoordinateChooser::setCurrentSystem(regina::NormalCoords newSystem) {
    std::vector<regina::NormalCoords>::const_iterator it =
        std::find(systems.begin(), systems.end(), newSystem);

    if (it != systems.end())
        setCurrentIndex(it - systems.begin());
}

void HyperCoordinateChooser::insertSystem(regina::HyperCoords coordSystem) {
    addItem(tr(Coordinates::name(coordSystem)));
    systems.push_back(coordSystem);
}

void HyperCoordinateChooser::insertAllCreators() {
    insertSystem(regina::HS_STANDARD);
}

void HyperCoordinateChooser::insertAllViewers(
        regina::NormalHypersurfaces*) {
    insertSystem(regina::HS_STANDARD);
    insertSystem(regina::HS_PRISM);
    insertSystem(regina::HS_EDGE_WEIGHT);
}

void HyperCoordinateChooser::setCurrentSystem(regina::HyperCoords newSystem) {
    std::vector<regina::HyperCoords>::const_iterator it =
        std::find(systems.begin(), systems.end(), newSystem);

    if (it != systems.end())
        setCurrentIndex(it - systems.begin());
}

