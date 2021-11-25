
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
#include "enumerate/treetraversal-impl.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::Integer;
using regina::NormalEncoding;
using regina::Triangulation;

using regina::LPConstraintNone;
using regina::LPConstraintEulerPositive;
using regina::LPConstraintEulerZero;
using regina::LPConstraintNonSpun;

using regina::BanNone;
using regina::BanBoundary;
using regina::BanTorusBoundary;

template <class LPConstraint, typename BanConstraint>
void addTreeTraversalBase(pybind11::module_& m, const char* name) {
    using Tree = regina::TreeTraversal<LPConstraint, BanConstraint, Integer>;

    auto c = pybind11::class_<Tree>(m, name)
        .def_static("supported", &Tree::supported)
        .def("visited", &Tree::visited)
        .def("typeString", &Tree::typeString)
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
        .def("solutions", &Tree::solutions)
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
        .def("solutions", &Tree::solutions)
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
    addTreeTraversalBase<LPConstraintNone, BanNone>(
        m, "TreeTraversal");
    addTreeTraversalBase<LPConstraintEulerPositive, BanNone>(
        m, "TreeTraversal_EulerPositive");
    addTreeTraversalBase<LPConstraintEulerZero, BanNone>(
        m, "TreeTraversal_EulerZero");
    addTreeTraversalBase<LPConstraintNonSpun, BanNone>(
        m, "TreeTraversal_NonSpun");
    addTreeTraversalBase<LPConstraintNone, BanBoundary>(
        m, "TreeTraversal_BanBoundary");
    addTreeTraversalBase<LPConstraintEulerPositive, BanBoundary>(
        m, "TreeTraversal_EulerPositive_BanBoundary");
    addTreeTraversalBase<LPConstraintEulerZero, BanBoundary>(
        m, "TreeTraversal_EulerZero_BanBoundary");
    addTreeTraversalBase<LPConstraintNone, BanTorusBoundary>(
        m, "TreeTraversal_BanTorusBoundary");
    addTreeTraversalBase<LPConstraintEulerPositive, BanTorusBoundary>(
        m, "TreeTraversal_EulerPositive_BanTorusBoundary");
    addTreeTraversalBase<LPConstraintEulerZero, BanTorusBoundary>(
        m, "TreeTraversal_EulerZero_BanTorusBoundary");

    addTreeEnumeration<LPConstraintNone, BanNone>(
        m, "TreeEnumeration");
    addTreeEnumeration<LPConstraintEulerPositive, BanNone>(
        m, "TreeEnumeration_EulerPositive");
    addTreeEnumeration<LPConstraintEulerZero, BanNone>(
        m, "TreeEnumeration_EulerZero");
    addTreeEnumeration<LPConstraintNonSpun, BanNone>(
        m, "TreeEnumeration_NonSpun");
    addTreeEnumeration<LPConstraintNone, BanBoundary>(
        m, "TreeEnumeration_BanBoundary");
    addTreeEnumeration<LPConstraintEulerPositive, BanBoundary>(
        m, "TreeEnumeration_EulerPositive_BanBoundary");
    addTreeEnumeration<LPConstraintEulerZero, BanBoundary>(
        m, "TreeEnumeration_EulerZero_BanBoundary");
    addTreeEnumeration<LPConstraintNone, BanTorusBoundary>(
        m, "TreeEnumeration_BanTorusBoundary");
    addTreeEnumeration<LPConstraintEulerPositive, BanTorusBoundary>(
        m, "TreeEnumeration_EulerPositive_BanTorusBoundary");
    addTreeEnumeration<LPConstraintEulerZero, BanTorusBoundary>(
        m, "TreeEnumeration_EulerZero_BanTorusBoundary");

    addTautEnumeration<LPConstraintNone, BanNone>(
        m, "TautEnumeration");

    addTreeSingleSoln<LPConstraintNone, BanNone>(
        m, "TreeSingleSoln");
    addTreeSingleSoln<LPConstraintEulerPositive, BanNone>(
        m, "TreeSingleSoln_EulerPositive");
    addTreeSingleSoln<LPConstraintEulerZero, BanNone>(
        m, "TreeSingleSoln_EulerZero");
    addTreeSingleSoln<LPConstraintNonSpun, BanNone>(
        m, "TreeSingleSoln_NonSpun");
    addTreeSingleSoln<LPConstraintNone, BanBoundary>(
        m, "TreeSingleSoln_BanBoundary");
    addTreeSingleSoln<LPConstraintEulerPositive, BanBoundary>(
        m, "TreeSingleSoln_EulerPositive_BanBoundary");
    addTreeSingleSoln<LPConstraintEulerZero, BanBoundary>(
        m, "TreeSingleSoln_EulerZero_BanBoundary");
    addTreeSingleSoln<LPConstraintNone, BanTorusBoundary>(
        m, "TreeSingleSoln_BanTorusBoundary");
    addTreeSingleSoln<LPConstraintEulerPositive, BanTorusBoundary>(
        m, "TreeSingleSoln_EulerPositive_BanTorusBoundary");
    addTreeSingleSoln<LPConstraintEulerZero, BanTorusBoundary>(
        m, "TreeSingleSoln_EulerZero_BanTorusBoundary");
}

