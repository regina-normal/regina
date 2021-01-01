
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "../pybind11/pybind11.h"
#include "manifold/graphtriple.h"
#include "manifold/sfs.h"
#include "../helpers.h"

using regina::GraphTriple;
using regina::Matrix2;
using regina::SFSpace;

void addGraphTriple(pybind11::module_& m) {
    pybind11::class_<GraphTriple, regina::Manifold>(m, "GraphTriple")
        .def(pybind11::init([](const SFSpace& s1,
                const SFSpace& s2, const SFSpace& s3,
                const Matrix2& m1, const Matrix2& m2) {
            // Clone the given SFSs to avoid claiming ownership of them.
            return new GraphTriple(
                new SFSpace(s1), new SFSpace(s2), new SFSpace(s3), m1, m2);
        }))
        .def(pybind11::init<const GraphTriple&>())
        .def("end", &GraphTriple::end,
            pybind11::return_value_policy::reference_internal)
        .def("centre", &GraphTriple::centre,
            pybind11::return_value_policy::reference_internal)
        .def("matchingReln", &GraphTriple::matchingReln,
            pybind11::return_value_policy::reference_internal)
    ;

    m.attr("NGraphTriple") = m.attr("GraphTriple");
}

