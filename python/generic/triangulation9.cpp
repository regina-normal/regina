
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

void addTriangulations9(pybind11::module_& m, pybind11::module_& internal) {
    addSimplex<9>(m, "Simplex9");
    addComponent<9>(m, internal, "Component9");
    addBoundaryComponent<9>(m, internal, "BoundaryComponent9");
    addTriangulation<9>(m, internal, "Triangulation9");

    addIsoSigClassic<9>(m, "IsoSigClassic9");
    addIsoSigEdgeDegrees<9>(m, "IsoSigEdgeDegrees9");
    addIsoSigRidgeDegrees<9>(m, "IsoSigRidgeDegrees9");
    addIsoSigData<1, 9>(m, "IsoSigData1_9");
    addIsoSigData<2, 9>(m, "IsoSigData2_9");

    m.attr("Face9_9") = m.attr("Simplex9");
}

// Instantiate templates for isomorphism signature encodings:
template void regina::python::add_isosig_encoding_functions<1, 9>(
    pybind11::class_<regina::IsoSigPrintable>&);
template void regina::python::add_isosig_encoding_functions<2, 9>(
    pybind11::class_<regina::IsoSigPrintable>&);
template void regina::python::add_isosig_encoding_functions<1, 9>(
    pybind11::class_<regina::IsoSigPrintableLockFree>&);
template void regina::python::add_isosig_encoding_functions<2, 9>(
    pybind11::class_<regina::IsoSigPrintableLockFree>&);
template void regina::python::add_isosig_encoding_functions<2, 9>(
    pybind11::class_<regina::IsoSigBinary>&);
