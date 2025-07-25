
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

#include <pybind11/pybind11.h>
#include "enumerate/ordering.h"
#include "maths/matrix.h"
#include "../helpers.h"
#include "../docstrings/enumerate/ordering.h"

using regina::MatrixInt;
using regina::PosOrder;

void addOrdering(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(PosOrder)

    auto c = pybind11::class_<PosOrder>(m, "PosOrder", rdoc_scope)
        .def(pybind11::init<const MatrixInt&>(), rdoc::__init)
        .def(pybind11::init<const PosOrder&>(), rdoc::__copy)
        .def("__call__", [](const PosOrder& p, long i, long j) {
            return p(i, j);
        }, rdoc::__call)
    ;
    regina::python::add_output_custom(c, [](const PosOrder& o,
            std::ostream& s) {
        // A PosOrder object is basically like a lambda that has captured
        // a matrix by reference.  The main reason this class even exists
        // is because it was written long before lambdas were a part of C++.
        //
        // It doesn't make a lot of sense to output the internal data for
        // what is essentially a function object.
        s << "Compare row indices using position vectors";
    });
    // It doesn't really make sense to compare these objects either.
    regina::python::disable_eq_operators(c);

    RDOC_SCOPE_END
}

