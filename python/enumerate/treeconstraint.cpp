
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

using regina::BanConstraintBase;
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
    RDOC_SCOPE_BEGIN(LPConstraintAPI)

    auto c = pybind11::class_<Constraint>(m, name, Docs::__class)
        .def_readonly_static("constraints", &Constraint::constraints)
        .def_readonly_static("octAdjustment", &Constraint::octAdjustment)
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
        }, "tri"_a, "columnPerm"_a, rdoc::addRows)
        .def_static("verify", overload_cast<const regina::NormalSurface&>(
            &Constraint::verify), rdoc::verify)
        .def_static("verify", overload_cast<const regina::AngleStructure&>(
            &Constraint::verify), rdoc::verify_2)
        .def_static("supported", &Constraint::supported, rdoc::supported)
        ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_END
}

template <typename BanConstraint, typename... BanArgs>
void addBanConstraint(pybind11::module_& m, const char* name,
        const char* classDoc, const char* initDoc) {
    RDOC_SCOPE_BEGIN(BanConstraintBase)

    auto c = pybind11::class_<BanConstraint>(m, name, classDoc)
        .def(pybind11::init<
            const LPInitialTableaux<LPConstraintNone>&, BanArgs...>(),
            initDoc)
        .def(pybind11::init<
            const LPInitialTableaux<LPConstraintEulerPositive>&, BanArgs...>(),
            initDoc)
        .def(pybind11::init<
            const LPInitialTableaux<LPConstraintEulerZero>&, BanArgs...>(),
            initDoc)
        .def(pybind11::init<
            const LPInitialTableaux<LPConstraintNonSpun>&, BanArgs...>(),
            initDoc)
        .def("enforceBans",
            &BanConstraint::template enforceBans<LPConstraintNone, Integer>,
            rdoc::enforceBans)
        .def("enforceBans",
            &BanConstraint::template enforceBans<LPConstraintEulerPositive,
                Integer>,
            rdoc::enforceBans)
        .def("enforceBans",
            &BanConstraint::template enforceBans<LPConstraintEulerZero,
                Integer>,
            rdoc::enforceBans)
        .def("enforceBans",
            &BanConstraint::template enforceBans<LPConstraintNonSpun, Integer>,
            rdoc::enforceBans)
        .def("marked", &BanConstraint::marked, rdoc::marked)
        .def_static("supported", &BanConstraint::supported, rdoc::supported)
        ;
    regina::python::add_output_rich(c);
    regina::python::add_eq_operators(c, rdoc::__eq);

    RDOC_SCOPE_END
}

void addTreeConstraint(pybind11::module_& m) {
    // The constraint classes do not have their own method docs, and so
    // the class-specific doc namespaces are unavailable.
    RDOC_SCOPE_BEGIN_MAIN

    addLPConstraint<LPConstraintNone, rdoc::LPConstraintNone>(
        m, "LPConstraintNone");
    addLPConstraint<LPConstraintEulerPositive, rdoc::LPConstraintEulerPositive>(
        m, "LPConstraintEulerPositive");
    addLPConstraint<LPConstraintEulerZero, rdoc::LPConstraintEulerZero>(
        m, "LPConstraintEulerZero");
    addLPConstraint<LPConstraintNonSpun, rdoc::LPConstraintNonSpun>(
        m, "LPConstraintNonSpun");

    // BanNone does not have its own constructor documentation, since it
    // behaves identically to the base class constructor.
    addBanConstraint<BanNone>(m, "BanNone", rdoc::BanNone::__class,
        rdoc::BanConstraintBase::__init);

    RDOC_SCOPE_SWITCH(BanBoundary)
    addBanConstraint<BanBoundary>(m, "BanBoundary", rdoc::__class,
        rdoc::__init);

    RDOC_SCOPE_SWITCH(BanEdge)
    addBanConstraint<BanEdge, regina::Edge<3>*>(m, "BanEdge", rdoc::__class,
        rdoc::__init);

    RDOC_SCOPE_SWITCH(BanTorusBoundary)
    addBanConstraint<BanTorusBoundary>(m, "BanTorusBoundary", rdoc::__class,
        rdoc::__init);

    RDOC_SCOPE_END
}

