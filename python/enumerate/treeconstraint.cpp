
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
#include "../pybind11/stl.h"
#include "enumerate/treeconstraint.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::Integer;
using regina::LPInitialTableaux;
using regina::NormalEncoding;
using regina::Triangulation;

using regina::BanConstraintBase;
using regina::BanNone;
using regina::BanBoundary;
using regina::BanTorusBoundary;

using regina::LPConstraintNone;
using regina::LPConstraintEulerPositive;
using regina::LPConstraintEulerZero;
using regina::LPConstraintNonSpun;

template <class BanConstraint>
void addBanConstraint(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<BanConstraint>(m, name)
        .def(pybind11::init<const LPInitialTableaux<LPConstraintNone>&>())
        .def(pybind11::init<
            const LPInitialTableaux<LPConstraintEulerPositive>&>())
        .def(pybind11::init<const LPInitialTableaux<LPConstraintEulerZero>&>())
        .def(pybind11::init<const LPInitialTableaux<LPConstraintNonSpun>&>())
        .def("enforceBans",
            &BanConstraint::template enforceBans<LPConstraintNone, Integer>)
        .def("enforceBans",
            &BanConstraint::template enforceBans<LPConstraintEulerPositive,
                Integer>)
        .def("enforceBans",
            &BanConstraint::template enforceBans<LPConstraintEulerZero,
                Integer>)
        .def("enforceBans",
            &BanConstraint::template enforceBans<LPConstraintNonSpun, Integer>)
        .def("marked", &BanConstraint::marked)
        .def_static("supported", &BanConstraint::supported)
        ;
    regina::python::add_eq_operators(c);
}

void addTreeConstraint(pybind11::module_& m) {
    addBanConstraint<BanNone>(m, "BanNone");
    addBanConstraint<BanBoundary>(m, "BanBoundary");
    addBanConstraint<BanTorusBoundary>(m, "BanTorusBoundary");
}

