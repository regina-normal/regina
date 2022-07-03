
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

using pybind11::overload_cast;

using regina::Integer;
using regina::LPInitialTableaux;
using regina::NormalEncoding;
using regina::Triangulation;

using regina::BanConstraintBase;
using regina::BanNone;
using regina::BanBoundary;
using regina::BanEdge;
using regina::BanTorusBoundary;

using regina::LPConstraintNone;
using regina::LPConstraintEulerPositive;
using regina::LPConstraintEulerZero;
using regina::LPConstraintNonSpun;

template <class LPConstraint>
void addLPConstraint(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<LPConstraint>(m, name)
        .def_readonly_static("nConstraints", &LPConstraint::nConstraints)
        .def_readonly_static("octAdjustment", &LPConstraint::octAdjustment)
        .def_static("addRows", [](const LPInitialTableaux<LPConstraint>& t) {
            // The C++ version of this function is likely to change
            // significantly, once we get rid of LPCol (or at least make
            // it private to LPInitialTableaux).  So, for now, we just
            // do a quick hack: we create a set of "fake" columns, fill
            // them with addRows(), and then return the coefficients
            // that came from these linear constraints.
            std::array<std::vector<typename LPConstraint::Coefficient>,
                LPConstraint::nConstraints> ans;

            auto* col = new regina::LPCol<LPConstraint>[t.columns()];
            LPConstraint::addRows(col, t);
            for (int i = 0; i < LPConstraint::nConstraints; ++i) {
                ans[i].reserve(t.columns());
                for (size_t j = 0; j < t.columns(); ++j)
                    ans[i].push_back(col[j].extra[i]);
            }
            delete[] col;

            return ans;
        })
        .def_static("constrain", &LPConstraint::template constrain<Integer>)
        .def_static("verify", overload_cast<const regina::NormalSurface&>(
            &LPConstraint::verify))
        .def_static("verify", overload_cast<const regina::AngleStructure&>(
            &LPConstraint::verify))
        .def_static("supported", &LPConstraint::supported)
        ;
    regina::python::no_eq_operators(c);
}

template <class BanConstraint, typename... BanArgs>
void addBanConstraint(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<BanConstraint>(m, name)
        .def(pybind11::init<
            const LPInitialTableaux<LPConstraintNone>&, BanArgs...>())
        .def(pybind11::init<
            const LPInitialTableaux<LPConstraintEulerPositive>&, BanArgs...>())
        .def(pybind11::init<
            const LPInitialTableaux<LPConstraintEulerZero>&, BanArgs...>())
        .def(pybind11::init<
            const LPInitialTableaux<LPConstraintNonSpun>&, BanArgs...>())
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
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);
}

void addTreeConstraint(pybind11::module_& m) {
    addLPConstraint<LPConstraintNone>(m, "LPConstraintNone");
    addLPConstraint<LPConstraintEulerPositive>(m, "LPConstraintEulerPositive");
    addLPConstraint<LPConstraintEulerZero>(m, "LPConstraintEulerZero");
    addLPConstraint<LPConstraintNonSpun>(m, "LPConstraintNonSpun");

    addBanConstraint<BanNone>(m, "BanNone");
    addBanConstraint<BanBoundary>(m, "BanBoundary");
    addBanConstraint<BanEdge, regina::Edge<3>*>(m, "BanEdge");
    addBanConstraint<BanTorusBoundary>(m, "BanTorusBoundary");
}

