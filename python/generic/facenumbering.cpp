
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

#include <pybind11/pybind11.h>
#include "triangulation/facenumbering.h"
#include "utilities/typeutils.h"
#include "facehelper.h"
#include "../helpers.h"
#include "../docstrings/triangulation/facenumbering.h"

void addFaceNumbering(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    m.def("faceOppositeEdge", [](int dim, int i, int j) -> int {
        // Although select_constexpr() will throw an exception if dim is
        // out of range, the message will be difficult for users to understand.
        // Check again here so we can give a more meaningful error.
        if (dim < 2 || dim > regina::maxDim())
            regina::python::invalidFaceDimension("faceOppositeEdge", 2,
                regina::maxDim());
        return regina::select_constexpr<2, regina::maxDim() + 1, int>(dim,
                [=](auto d) {
            return regina::faceOppositeEdge<d>(i, j);
        });
    }, pybind11::arg("dim"), pybind11::arg("i"), pybind11::arg("j"),
        rdoc::faceOppositeEdge);

    RDOC_SCOPE_END
}

