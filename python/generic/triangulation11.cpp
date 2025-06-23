
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "triangulation-bindings.h"
#include "boundarycomponent-bindings.h"
#include "component-bindings.h"
#include "isosig-bindings.h"
#include "simplex-bindings.h"

void addTriangulations11(pybind11::module_& m, pybind11::module_& internal) {
    addSimplex<11>(m, "Simplex11");
    addComponent<11>(m, internal, "Component11");
    addBoundaryComponent<11>(m, internal, "BoundaryComponent11");
    addTriangulation<11>(m, internal, "Triangulation11");

    addIsoSigClassic<11>(m, "IsoSigClassic11");
    addIsoSigEdgeDegrees<11>(m, "IsoSigEdgeDegrees11");
    addIsoSigRidgeDegrees<11>(m, "IsoSigRidgeDegrees11");
    addIsoSigPrintable<11>(m, "IsoSigPrintable11");

    m.attr("Face11_11") = m.attr("Simplex11");
}

