
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
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "enumerate/hilbertcd.h"
#include "../helpers.h"
#include "../docstrings/enumerate/hilbertcd.h"

using regina::HilbertCD;
using regina::VectorInt;

void addHilbertCD(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(HilbertCD)

    auto c = pybind11::class_<HilbertCD>(m, "HilbertCD", rdoc_scope)
        .def_static("enumerate", &HilbertCD::enumerate<VectorInt,
            const std::function<void(VectorInt&&)>&>,
            pybind11::arg("action"), pybind11::arg("subspace"),
            pybind11::arg("constraints"),
            rdoc::enumerate)
        .def_static("enumerate", [](const regina::MatrixInt& s,
                const regina::ValidityConstraints& c) {
            std::vector<VectorInt> ans;
            HilbertCD::enumerate<VectorInt>([&ans](VectorInt&& v) {
                ans.push_back(std::move(v));
            }, s, c);
            return ans;
        }, pybind11::arg("subspace"), pybind11::arg("constraints"),
            rdoc::enumerate)
    ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_END
}

