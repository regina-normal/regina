
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "triangulation-bindings.h"
#include "boundarycomponent-bindings.h"
#include "component-bindings.h"
#include "simplex-bindings.h"

void addTriangulations10(pybind11::module_& m, pybind11::module_& internal) {
    addSimplex<10>(m, "Simplex10");
    addComponent<10>(m, internal, "Component10");
    addBoundaryComponent<10>(m, internal, "BoundaryComponent10");
    addTriangulation<10>(m, internal, "Triangulation10");

    addIsoSigClassic<10>(m, "IsoSigClassic10");
    addIsoSigEdgeDegrees<10>(m, "IsoSigEdgeDegrees10");
    addIsoSigRidgeDegrees<10>(m, "IsoSigRidgeDegrees10");
    addIsoSigPrintable<10, true>(m, "IsoSigPrintable10");
    addIsoSigPrintable<10, false>(m, "IsoSigPrintableLockFree10");

    m.attr("Face10_10") = m.attr("Simplex10");
}

