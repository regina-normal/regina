
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include "../docstrings/enumerate/treetraversal.h"

using regina::Integer;
using regina::NormalEncoding;
using regina::Triangulation;

using regina::LPConstraintNone;
using regina::LPConstraintEulerPositive;
using regina::LPConstraintEulerZero;
using regina::LPConstraintNonSpun;

using regina::BanNone;
using regina::BanBoundary;
using regina::BanEdge;
using regina::BanTorusBoundary;

template <class LPConstraint, typename BanConstraint>
void addTreeTraversalBase(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(TreeTraversal)

    using Tree = regina::TreeTraversal<LPConstraint, BanConstraint, Integer>;

    auto c = pybind11::class_<Tree>(m, name, rdoc_scope)
        .def_static("supported", &Tree::supported, rdoc::supported)
        .def("visited", &Tree::visited, rdoc::visited)
        .def("typeString", &Tree::typeString, rdoc::typeString)
        .def("buildSurface", &Tree::buildSurface, rdoc::buildSurface)
        .def("buildStructure", &Tree::buildStructure, rdoc::buildStructure)
    ;
    // Leave the output routines for subclasses to wrap, since __repr__
    // will include the (derived) class name.
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END
}

template <class LPConstraint, typename BanConstraint, typename... BanArgs>
void addTreeEnumeration(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(TreeEnumeration)

    using Tree = regina::TreeEnumeration<LPConstraint, BanConstraint>;
    using Action = const std::function<bool(const Tree&)>&;

    auto c = pybind11::class_<Tree, regina::TreeTraversal<
            LPConstraint, BanConstraint, Integer>>(m, name, rdoc_scope)
        .def(pybind11::init<const Triangulation<3>&, NormalEncoding,
            BanArgs...>(), rdoc::__init)
        .def("solutions", &Tree::solutions, rdoc::solutions)
        .def("run", &Tree::template run<Action>,
            pybind11::arg("action"), rdoc::run)
        .def("next", &Tree::next,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::next)
        .def_static("writeTypes", &Tree::writeTypes, rdoc::writeTypes)
        .def_static("writeSurface", &Tree::writeSurface, rdoc::writeSurface)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END
}

template <class LPConstraint, typename BanConstraint, typename... BanArgs>
void addTautEnumeration(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(TautEnumeration)

    using Tree = regina::TautEnumeration<LPConstraint, BanConstraint>;
    using Action = const std::function<bool(const Tree&)>&;

    auto c = pybind11::class_<Tree, regina::TreeTraversal<
            LPConstraint, BanConstraint, Integer>>(m, name, rdoc_scope)
        .def(pybind11::init<const Triangulation<3>&, BanArgs...>(),
            rdoc::__init)
        .def("solutions", &Tree::solutions, rdoc::solutions)
        .def("run", &Tree::template run<Action>,
            pybind11::arg("action"), rdoc::run)
        .def("next", &Tree::next,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::next)
        .def_static("writeTypes", &Tree::writeTypes, rdoc::writeTypes)
        .def_static("writeStructure", &Tree::writeStructure,
            rdoc::writeStructure)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END
}

template <class LPConstraint, typename BanConstraint, typename... BanArgs>
void addTreeSingleSoln(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(TreeSingleSoln)

    using Tree = regina::TreeSingleSoln<LPConstraint, BanConstraint>;

    auto c = pybind11::class_<Tree, regina::TreeTraversal<
            LPConstraint, BanConstraint, Integer>>(m, name, rdoc_scope)
        .def(pybind11::init<const Triangulation<3>&, NormalEncoding,
            BanArgs...>(), rdoc::__init)
        .def("find", &Tree::find, rdoc::find)
        .def("cancel", &Tree::cancel, rdoc::cancel)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    RDOC_SCOPE_END
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
    addTreeTraversalBase<LPConstraintNone, BanEdge>(
        m, "TreeTraversal_BanEdge");
    addTreeTraversalBase<LPConstraintEulerPositive, BanEdge>(
        m, "TreeTraversal_EulerPositive_BanEdge");
    addTreeTraversalBase<LPConstraintEulerZero, BanEdge>(
        m, "TreeTraversal_EulerZero_BanEdge");
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
    addTreeEnumeration<LPConstraintNone, BanEdge, regina::Edge<3>*>(
        m, "TreeEnumeration_BanEdge");
    addTreeEnumeration<LPConstraintEulerPositive, BanEdge, regina::Edge<3>*>(
        m, "TreeEnumeration_EulerPositive_BanEdge");
    addTreeEnumeration<LPConstraintEulerZero, BanEdge, regina::Edge<3>*>(
        m, "TreeEnumeration_EulerZero_BanEdge");
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
    addTreeSingleSoln<LPConstraintNone, BanEdge, regina::Edge<3>*>(
        m, "TreeSingleSoln_BanEdge");
    addTreeSingleSoln<LPConstraintEulerPositive, BanEdge, regina::Edge<3>*>(
        m, "TreeSingleSoln_EulerPositive_BanEdge");
    addTreeSingleSoln<LPConstraintEulerZero, BanEdge, regina::Edge<3>*>(
        m, "TreeSingleSoln_EulerZero_BanEdge");
    addTreeSingleSoln<LPConstraintNone, BanTorusBoundary>(
        m, "TreeSingleSoln_BanTorusBoundary");
    addTreeSingleSoln<LPConstraintEulerPositive, BanTorusBoundary>(
        m, "TreeSingleSoln_EulerPositive_BanTorusBoundary");
    addTreeSingleSoln<LPConstraintEulerZero, BanTorusBoundary>(
        m, "TreeSingleSoln_EulerZero_BanTorusBoundary");
}

