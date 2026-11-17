
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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "enumerate/treeconstraint.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../docstrings/enumerate/treeconstraint.h"

using namespace pybind11::literals;

using pybind11::overload_cast;

using regina::Integer;
using regina::LPInitialTableaux;
using regina::NormalEncoding;
using regina::Triangulation;

using regina::BanNone;
using regina::BanBoundary;
using regina::BanEdge;
using regina::BanTorusBoundary;

using regina::LPConstraintNone;
using regina::LPConstraintEulerPositive;
using regina::LPConstraintEulerZero;
using regina::LPConstraintNonSpun;

template <regina::LPConstraint Constraint, regina::python::ClassDocType Docs>
void addLPConstraint(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<Constraint>(m, name, Docs::__class)
        .def_readonly_static("constraints", &Constraint::constraints)
        .def_static("addRows", [](const Triangulation<3>& tri,
                const std::vector<size_t>& columnPerm) {
            // The C++ version of this function is likely to change
            // significantly, once we get rid of LPCol (or at least make
            // it private to LPInitialTableaux).  So, for now, we just
            // do a quick hack: we create a set of "fake" columns, fill
            // them with addRows(), and then return the coefficients
            // that came from these linear constraints.
            using Coefficient = typename Constraint::Coefficient;
            using Col = regina::detail::LPCol<Constraint::constraints.size(),
                Coefficient>;

            std::array<std::vector<Coefficient>,
                Constraint::constraints.size()> ans;
            regina::FixedArray<Col> col(columnPerm.size());
            Constraint::addRows(col.begin(), tri, columnPerm.data());
            for (size_t i = 0; i < Constraint::constraints.size(); ++i) {
                ans[i].reserve(columnPerm.size());
                for (size_t j = 0; j < columnPerm.size(); ++j)
                    ans[i].push_back(col[j].extra[i]);
            }
            return ans;
        }, "tri"_a, "columnPerm"_a, Docs::addRows)
        .def_static("supported", &Constraint::supported, Docs::supported)
        ;
    if constexpr (regina::LPSurfaceConstraint<Constraint>) {
        c.def_readonly_static("octAdjustment", &Constraint::octAdjustment);
        c.def_static("verify", overload_cast<const regina::NormalSurface&>(
            &Constraint::verify), Docs::verify_surface);
    }
    if constexpr (regina::LPStructureConstraint<Constraint>) {
        c.def_static("verify", overload_cast<const regina::AngleStructure&>(
            &Constraint::verify), Docs::verify_structure);
    }
    regina::python::no_eq_static(c);
}

template <typename Ban, regina::python::ClassDocType Docs, typename... BanArgs>
requires regina::BanConstraint<Ban, Integer, LPConstraintNone>
void addBanConstraint(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<Ban>(m, name, Docs::__class)
        .def(pybind11::init<
            const LPInitialTableaux<LPConstraintNone>&, BanArgs...>(),
            Docs::__init)
        .def(pybind11::init<
            const LPInitialTableaux<LPConstraintEulerPositive>&, BanArgs...>(),
            Docs::__init)
        .def(pybind11::init<
            const LPInitialTableaux<LPConstraintEulerZero>&, BanArgs...>(),
            Docs::__init)
        .def(pybind11::init<
            const LPInitialTableaux<LPConstraintNonSpun>&, BanArgs...>(),
            Docs::__init)
        .def("enforceBans",
            &Ban::template enforceBans<LPConstraintNone, Integer>,
            Docs::enforceBans)
        .def("enforceBans",
            &Ban::template enforceBans<LPConstraintEulerPositive, Integer>,
            Docs::enforceBans)
        .def("enforceBans",
            &Ban::template enforceBans<LPConstraintEulerZero, Integer>,
            Docs::enforceBans)
        .def("enforceBans",
            &Ban::template enforceBans<LPConstraintNonSpun, Integer>,
            Docs::enforceBans)
        .def("marked", &Ban::marked, Docs::marked)
        .def_static("supported", &Ban::supported, Docs::supported)
        ;
    regina::python::add_output_rich(c);
}

void addTreeConstraint(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    addLPConstraint<LPConstraintNone, rdoc::LPConstraintNone>(
        m, "LPConstraintNone");
    addLPConstraint<LPConstraintEulerPositive, rdoc::LPConstraintEulerPositive>(
        m, "LPConstraintEulerPositive");
    addLPConstraint<LPConstraintEulerZero, rdoc::LPConstraintEulerZero>(
        m, "LPConstraintEulerZero");
    addLPConstraint<LPConstraintNonSpun, rdoc::LPConstraintNonSpun>(
        m, "LPConstraintNonSpun");

    addBanConstraint<BanNone, rdoc::BanNone>(m, "BanNone");
    addBanConstraint<BanBoundary, rdoc::BanBoundary>(m, "BanBoundary");
    addBanConstraint<BanEdge, rdoc::BanEdge, regina::Edge<3>*>(m, "BanEdge");
    addBanConstraint<BanTorusBoundary, rdoc::BanTorusBoundary>(
        m, "BanTorusBoundary");

    RDOC_SCOPE_END
}

