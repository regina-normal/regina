
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
#include "../pybind11/functional.h"
#include "../pybind11/stl.h"
#include "enumerate/hilbertdual.h"
#include "../helpers.h"

using regina::HilbertDual;
using regina::VectorInt;

void addHilbertDual(pybind11::module_& m) {
    auto c = pybind11::class_<HilbertDual>(m, "HilbertDual")
        .def_static("enumerate", &HilbertDual::enumerate<VectorInt,
                const std::function<void(VectorInt&&)>&>,
            pybind11::arg(), pybind11::arg(), pybind11::arg(),
            pybind11::arg("tracker") = nullptr,
            pybind11::arg("initialRows") = 0)
        .def_static("enumerate", [](const regina::MatrixInt& s,
                const regina::ValidityConstraints& c,
                regina::ProgressTracker* p, unsigned long r) {
            std::vector<VectorInt> ans;
            HilbertDual::enumerate<VectorInt>([&ans](VectorInt&& v) {
                ans.push_back(std::move(v));
            }, s, c, p, r);
            return ans;
        }, pybind11::arg(), pybind11::arg(),
            pybind11::arg("tracker") = nullptr,
            pybind11::arg("initialRows") = 0)
    ;
    regina::python::no_eq_operators(c);
}

