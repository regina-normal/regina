
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

void addTriangulations13(pybind11::module_& m) {
    addSimplex<13>(m, "Simplex13");
    addComponent<13>(m, "Component13");
    addBoundaryComponent<13>(m, "BoundaryComponent13");
    addTriangulation<13>(m, "Triangulation13");

    addIsoSigClassic<13>(m, "IsoSigClassic13");
    addIsoSigEdgeDegrees<13>(m, "IsoSigEdgeDegrees13");
    addIsoSigRidgeDegrees<13>(m, "IsoSigRidgeDegrees13");
    addIsoSigPrintable<13>(m, "IsoSigPrintable13");

    m.attr("Face13_13") = m.attr("Simplex13");
}

