
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

#include "regina-config.h" // for REGINA_HIGHDIM

namespace pybind11 { class module_; }

void addCut(pybind11::module_& m);
void addExample5(pybind11::module_& m);
void addExample6(pybind11::module_& m);
void addExample7(pybind11::module_& m);
void addExample8(pybind11::module_& m);
void addExample9(pybind11::module_& m);
void addExample10(pybind11::module_& m);
void addExample11(pybind11::module_& m);
void addExample12(pybind11::module_& m);
void addExample13(pybind11::module_& m);
void addExample14(pybind11::module_& m);
void addExample15(pybind11::module_& m);
void addFaceNumbering(pybind11::module_& m);
void addFacetPairing2(pybind11::module_& m);
void addFacetPairing4(pybind11::module_& m);
void addFacetPairing5(pybind11::module_& m);
void addFacetPairing6(pybind11::module_& m);
void addFacetPairing7(pybind11::module_& m);
void addFacetPairing8(pybind11::module_& m);
void addFacetPairing9(pybind11::module_& m);
void addFacetPairing10(pybind11::module_& m);
void addFacetPairing11(pybind11::module_& m);
void addFacetPairing12(pybind11::module_& m);
void addFacetPairing13(pybind11::module_& m);
void addFacetPairing14(pybind11::module_& m);
void addFacetPairing15(pybind11::module_& m);
void addFacetSpec(pybind11::module_& m);
void addIsomorphism5(pybind11::module_& m);
void addIsomorphism6(pybind11::module_& m);
void addIsomorphism7(pybind11::module_& m);
void addIsomorphism8(pybind11::module_& m);
void addIsomorphism9(pybind11::module_& m);
void addIsomorphism10(pybind11::module_& m);
void addIsomorphism11(pybind11::module_& m);
void addIsomorphism12(pybind11::module_& m);
void addIsomorphism13(pybind11::module_& m);
void addIsomorphism14(pybind11::module_& m);
void addIsomorphism15(pybind11::module_& m);
void addFace5(pybind11::module_& m);
void addFace6(pybind11::module_& m);
void addFace7(pybind11::module_& m);
void addFace8(pybind11::module_& m);
void addFace9(pybind11::module_& m);
void addFace10(pybind11::module_& m);
void addFace11(pybind11::module_& m);
void addFace12(pybind11::module_& m);
void addFace13(pybind11::module_& m);
void addFace14(pybind11::module_& m);
void addFace15(pybind11::module_& m);
void addTriangulations5(pybind11::module_& m);
void addTriangulations6(pybind11::module_& m);
void addTriangulations7(pybind11::module_& m);
void addTriangulations8(pybind11::module_& m);
void addTriangulations9(pybind11::module_& m);
void addTriangulations10(pybind11::module_& m);
void addTriangulations11(pybind11::module_& m);
void addTriangulations12(pybind11::module_& m);
void addTriangulations13(pybind11::module_& m);
void addTriangulations14(pybind11::module_& m);
void addTriangulations15(pybind11::module_& m);

void addGenericClasses(pybind11::module_& m) {
    addFacetSpec(m);
    addFacetPairing2(m);
    addFacetPairing4(m);
    addFacetPairing5(m);
    addFacetPairing6(m);
    addFacetPairing7(m);
    addFacetPairing8(m);
    addFace5(m);
    addFace6(m);
    addFace7(m);
    addFace8(m);
    addIsomorphism5(m);
    addIsomorphism6(m);
    addIsomorphism7(m);
    addIsomorphism8(m);
    addTriangulations5(m);
    addTriangulations6(m);
    addTriangulations7(m);
    addTriangulations8(m);
    addExample5(m);
    addExample6(m);
    addExample7(m);
    addExample8(m);
    addCut(m);
    addFaceNumbering(m);
#ifdef REGINA_HIGHDIM
    addFacetPairing9(m);
    addFacetPairing10(m);
    addFacetPairing11(m);
    addFacetPairing12(m);
    addFacetPairing13(m);
    addFacetPairing14(m);
    addFacetPairing15(m);
    addFace9(m);
    addFace10(m);
    addFace11(m);
    addFace12(m);
    addFace13(m);
    addFace14(m);
    addFace15(m);
    addIsomorphism9(m);
    addIsomorphism10(m);
    addIsomorphism11(m);
    addIsomorphism12(m);
    addIsomorphism13(m);
    addIsomorphism14(m);
    addIsomorphism15(m);
    addTriangulations9(m);
    addTriangulations10(m);
    addTriangulations11(m);
    addTriangulations12(m);
    addTriangulations13(m);
    addTriangulations14(m);
    addTriangulations15(m);
    addExample9(m);
    addExample10(m);
    addExample11(m);
    addExample12(m);
    addExample13(m);
    addExample14(m);
    addExample15(m);
#endif /* REGINA_HIGHDIM */
}

