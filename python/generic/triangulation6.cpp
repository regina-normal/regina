
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

void addTriangulations6(pybind11::module_& m, pybind11::module_& internal) {
    addSimplex<6>(m, "Simplex6");
    addComponent<6>(m, internal, "Component6");
    addBoundaryComponent<6>(m, internal, "BoundaryComponent6");
    addTriangulation<6>(m, internal, "Triangulation6");

    addIsoSigClassic<6>(m, "IsoSigClassic6");
    addIsoSigEdgeDegrees<6>(m, "IsoSigEdgeDegrees6");
    addIsoSigRidgeDegrees<6>(m, "IsoSigRidgeDegrees6");
    addIsoSigPrintable<6, true>(m, "IsoSigPrintable6");
    addIsoSigPrintable<6, false>(m, "IsoSigPrintableLockFree6");

    m.attr("Face6_6") = m.attr("Simplex6");
}

