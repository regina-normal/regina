
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#include "triangulation/forward.h"
#include "../helpers.h"
#include "../docstrings/triangulation/forward.h"

namespace pybind11 { class module_; }

void addDim2Classes(pybind11::module_& m, pybind11::module_& internal);
void addDim3Classes(pybind11::module_& m, pybind11::module_& internal);
void addDim4Classes(pybind11::module_& m, pybind11::module_& internal);
void addHiDimClasses(pybind11::module_& m, pybind11::module_& internal);

void addCut(pybind11::module_& m);
void addFaceNumbering(pybind11::module_& m);
void addFacetSpec(pybind11::module_& m);
void addIsoSigEncodings(pybind11::module_& m);

void addTriangulationClasses(pybind11::module_& m,
        pybind11::module_& internal) {
    addDim2Classes(m, internal);
    addDim3Classes(m, internal);
    addDim4Classes(m, internal);
    addHiDimClasses(m, internal);

    addFacetSpec(m);
    addCut(m);
    addFaceNumbering(m);
    addIsoSigEncodings(m);
}

