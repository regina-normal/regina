
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"

// UI includes:
#include "coordinatechooser.h"
#include "coordinates.h"
#include "reginaprefset.h"

#include <algorithm>

// #define SUPPORT_TONS

using regina::HyperCoords;
using regina::NormalCoords;
using regina::NormalHypersurfaces;
using regina::NormalSurfaces;

void CoordinateChooser::insertSystem(NormalCoords coordSystem) {
    addItem(tr(Coordinates::name(coordSystem)));
    systems.push_back(coordSystem);
}

void CoordinateChooser::insertAllCreators() {
    insertSystem(NormalCoords::Standard);
    insertSystem(NormalCoords::AlmostNormal);
    insertSystem(NormalCoords::Quad);
    insertSystem(NormalCoords::QuadOct);
    // TODO: Only insert QuadClosed / QuadOctClosed for the right kind of
    // ideal triangulations.
    insertSystem(NormalCoords::QuadClosed);
    insertSystem(NormalCoords::QuadOctClosed);
#ifdef SUPPORT_TONS
    if (ReginaPrefSet::global().surfacesSupportOriented) {
        insertSystem(NormalCoords::Oriented);
        insertSystem(NormalCoords::OrientedQuad);
    }
#endif
}

void CoordinateChooser::insertAllViewers(NormalSurfaces* surfaces) {
    if (surfaces->allowsAlmostNormal()) {
        // For lists created with Regina 4.5.1 or earlier, we have
        // already taken out surfaces with multiple octagons.  Make this
        // clear to the user.
        if (surfaces->coords() == NormalCoords::LegacyAlmostNormal)
            insertSystem(NormalCoords::LegacyAlmostNormal);
        else {
            insertSystem(NormalCoords::AlmostNormal);
            insertSystem(NormalCoords::QuadOct);
        }
    } else {
        insertSystem(NormalCoords::Standard);
        insertSystem(NormalCoords::Quad);

#ifdef SUPPORT_TONS
        if (surfaces->allowsOriented()) {
            insertSystem(NormalCoords::Oriented);
            insertSystem(NormalCoords::OrientedQuad);
        }
#endif
    }

    insertSystem(NormalCoords::Edge);
    insertSystem(NormalCoords::Arc);
}

void CoordinateChooser::setCurrentSystem(NormalCoords newSystem) {
    auto it = std::find(systems.begin(), systems.end(), newSystem);

    if (it == systems.end()) {
        // Try to find a reasonable fallback.
        if (newSystem == NormalCoords::QuadClosed)
            it = std::find(systems.begin(), systems.end(), NormalCoords::Quad);
#ifdef SUPPORT_TONS
        else if (newSystem == NormalCoords::OrientedQuad)
            it = std::find(systems.begin(), systems.end(), NormalCoords::Quad);
#endif
        else if (newSystem == NormalCoords::QuadOctClosed)
            it = std::find(systems.begin(), systems.end(),
                NormalCoords::QuadOct);
#ifdef SUPPORT_TONS
        else if (newSystem == NormalCoords::Oriented)
            it = std::find(systems.begin(), systems.end(),
                NormalCoords::Standard);
#endif
        else if (newSystem == NormalCoords::LegacyAlmostNormal)
            it = std::find(systems.begin(), systems.end(),
                NormalCoords::AlmostNormal);
    }

    if (it != systems.end())
        setCurrentIndex(static_cast<int>(it - systems.begin()));
}

void HyperCoordinateChooser::insertSystem(HyperCoords coordSystem) {
    addItem(tr(Coordinates::name(coordSystem)));
    systems.push_back(coordSystem);
}

void HyperCoordinateChooser::insertAllCreators() {
    insertSystem(HyperCoords::Standard);
    insertSystem(HyperCoords::Prism);
}

void HyperCoordinateChooser::insertAllViewers(NormalHypersurfaces*) {
    insertSystem(HyperCoords::Standard);
    insertSystem(HyperCoords::Prism);
    insertSystem(HyperCoords::Edge);
}

void HyperCoordinateChooser::setCurrentSystem(HyperCoords newSystem) {
    auto it = std::find(systems.begin(), systems.end(), newSystem);

    if (it != systems.end())
        setCurrentIndex(static_cast<int>(it - systems.begin()));
}

