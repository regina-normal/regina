
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
#include "enumerate/treetraversal.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::Integer;
using regina::NormalEncoding;
using regina::Triangulation;

template <class LPConstraint, typename BanConstraint>
void addTreeTraversalBase(pybind11::module_& m, const char* name) {
    using Tree = regina::TreeTraversal<LPConstraint, BanConstraint, Integer>;

    auto c = pybind11::class_<Tree>(m, name)
        .def_static("supported", &Tree::supported)
        .def("nVisited", &Tree::nVisited) // TODO
        // TODO: dumpTypes
        .def("buildSurface", &Tree::buildSurface)
        .def("buildStructure", &Tree::buildStructure)
    ;
    regina::python::add_eq_operators(c);
}

template <class LPConstraint, typename BanConstraint>
void addTreeEnumeration(pybind11::module_& m, const char* name) {
    using Tree = regina::TreeEnumeration<LPConstraint, BanConstraint>;
    using Action = const std::function<bool(const Tree&)>&;

    auto c = pybind11::class_<Tree, regina::TreeTraversal<
            LPConstraint, BanConstraint, Integer>>(m, name)
        .def(pybind11::init<const Triangulation<3>&, NormalEncoding>())
        .def("nSolns", &Tree::nSolns) // TODO
        .def("run", &Tree::template run<Action>)
        .def("next", &Tree::next,
            pybind11::arg("tracker") = nullptr)
        .def_static("writeTypes", &Tree::writeTypes)
        .def_static("writeSurface", &Tree::writeSurface)
    ;
    regina::python::add_eq_operators(c);
}

template <class LPConstraint, typename BanConstraint>
void addTautEnumeration(pybind11::module_& m, const char* name) {
    using Tree = regina::TautEnumeration<LPConstraint, BanConstraint>;
    using Action = const std::function<bool(const Tree&)>&;

    auto c = pybind11::class_<Tree, regina::TreeTraversal<
            LPConstraint, BanConstraint, Integer>>(m, name)
        .def(pybind11::init<const Triangulation<3>&>())
        .def("nSolns", &Tree::nSolns) // TODO
        .def("run", &Tree::template run<Action>)
        .def("next", &Tree::next,
            pybind11::arg("tracker") = nullptr)
        .def_static("writeTypes", &Tree::writeTypes)
        .def_static("writeStructure", &Tree::writeStructure)
    ;
    regina::python::add_eq_operators(c);
}

template <class LPConstraint, typename BanConstraint>
void addTreeSingleSoln(pybind11::module_& m, const char* name) {
    using Tree = regina::TreeSingleSoln<LPConstraint, BanConstraint>;

    auto c = pybind11::class_<Tree, regina::TreeTraversal<
            LPConstraint, BanConstraint, Integer>>(m, name)
        .def(pybind11::init<const Triangulation<3>&, NormalEncoding>())
        .def("find", &Tree::find)
        .def("cancel", &Tree::cancel)
    ;
    regina::python::add_eq_operators(c);
}

void addTreeTraversal(pybind11::module_& m) {
    addTreeTraversalBase<regina::LPConstraintNone, regina::BanNone>(
        m, "TreeTraversal");
    addTreeTraversalBase<regina::LPConstraintEulerPositive, regina::BanNone>(
        m, "TreeTraversal_EulerPositive");
    addTreeTraversalBase<regina::LPConstraintNonSpun, regina::BanNone>(
        m, "TreeTraversal_NonSpun");

    // TODO: Expand list
    addTreeEnumeration<regina::LPConstraintNone, regina::BanNone>(
        m, "TreeEnumeration");
    addTreeEnumeration<regina::LPConstraintNonSpun, regina::BanNone>(
        m, "TreeEnumeration_NonSpun");

    addTautEnumeration<regina::LPConstraintNone, regina::BanNone>(
        m, "TautEnumeration");

    // TODO: Expand list
    addTreeSingleSoln<regina::LPConstraintEulerPositive, regina::BanNone>(
        m, "TreeSingleSoln_EulerPositive");

    // TODO:
    // Constraints:
    // - EulerPositive (NS_STANDARD, NS_AN_STANDARD)
    // - EulerZero (NS_STANDARD)
    // - NonSpun (NS_QUAD, NS_AN_QUAD_OCT)
    //
    // Ban:
    // - BanBoundary (NS_STANDARD, NS_AN_STANDARD)
    // - BanTorusBoundary (NS_STANDARD, NS_AN_STANDARD)
}

