
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "link/link.h"
#include "treewidth/treedecomposition-impl.h"
#include "triangulation/facetpairing.h"
#include "triangulation/facetpairing3.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "../helpers.h"
#include "../docstrings/treewidth/treedecomposition.h"

using pybind11::overload_cast;
using regina::TreeBag;
using regina::TreeDecomposition;

void addTreeDecomposition(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(TreeDecompositionAlg)

    pybind11::enum_<regina::TreeDecompositionAlg>(m, "TreeDecompositionAlg",
            rdoc_scope)
        .value("Upper", regina::TreeDecompositionAlg::Upper, rdoc::Upper)
        .value("UpperGreedyFillIn",
            regina::TreeDecompositionAlg::UpperGreedyFillIn,
            rdoc::UpperGreedyFillIn)
        ;

    // Deprecated constants:
    m.attr("TD_UPPER") = regina::TreeDecompositionAlg::Upper;
    m.attr("TD_UPPER_GREEDY_FILL_IN") =
        regina::TreeDecompositionAlg::UpperGreedyFillIn;

    RDOC_SCOPE_SWITCH(BagComparison)

    pybind11::enum_<regina::BagComparison>(m, "BagComparison", rdoc_scope)
        .value("Equal", regina::BagComparison::Equal, rdoc::Equal)
        .value("Subset", regina::BagComparison::Subset, rdoc::Subset)
        .value("Superset", regina::BagComparison::Superset, rdoc::Superset)
        .value("Unrelated", regina::BagComparison::Unrelated, rdoc::Unrelated)
        ;

    // Deprecated constants:
    m.attr("BAG_EQUAL") = regina::BagComparison::Equal;
    m.attr("BAG_SUBSET") = regina::BagComparison::Subset;
    m.attr("BAG_SUPERSET") = regina::BagComparison::Superset;
    m.attr("BAG_UNRELATED") = regina::BagComparison::Unrelated;

    RDOC_SCOPE_SWITCH(NiceType)

    pybind11::enum_<regina::NiceType>(m, "NiceType", rdoc_scope)
        .value("Introduce", regina::NiceType::Introduce, rdoc::Introduce)
        .value("Forget", regina::NiceType::Forget, rdoc::Forget)
        .value("Join", regina::NiceType::Join, rdoc::Join)
        ;

    // Deprecated constants:
    m.attr("NICE_INTRODUCE") = regina::NiceType::Introduce;
    m.attr("NICE_FORGET") = regina::NiceType::Forget;
    m.attr("NICE_JOIN") = regina::NiceType::Join;
    RDOC_SCOPE_SWITCH(TreeBag)

    auto tb = pybind11::class_<TreeBag>(m, "TreeBag", rdoc_scope)
        .def("size", &TreeBag::size, rdoc::size)
        .def("element", &TreeBag::element, rdoc::element)
        .def("contains", &TreeBag::contains, rdoc::contains)
        .def("index", &TreeBag::index, rdoc::index)
        .def("niceType", &TreeBag::niceType, rdoc::niceType)
        .def("type", &TreeBag::niceType, rdoc::type) // deprecated
        .def("niceIndex", &TreeBag::niceIndex, rdoc::niceIndex)
        .def("subtype", &TreeBag::niceIndex, rdoc::subtype) // deprecated
        .def("compare", &TreeBag::compare, rdoc::compare)
        .def("next", &TreeBag::next,
            pybind11::return_value_policy::reference, rdoc::next)
        .def("nextPrefix", &TreeBag::nextPrefix,
            pybind11::return_value_policy::reference, rdoc::nextPrefix)
        .def("parent", &TreeBag::parent,
            pybind11::return_value_policy::reference, rdoc::parent)
        .def("children", &TreeBag::children,
            pybind11::return_value_policy::reference, rdoc::children)
        .def("sibling", &TreeBag::sibling,
            pybind11::return_value_policy::reference, rdoc::sibling)
        .def("isLeaf", &TreeBag::isLeaf, rdoc::isLeaf)
    ;
    regina::python::add_output(tb);
    regina::python::add_eq_operators(tb);

    RDOC_SCOPE_SWITCH(TreeDecomposition)

    auto td = pybind11::class_<TreeDecomposition>(m, "TreeDecomposition",
            rdoc_scope)
        .def(pybind11::init<const regina::TreeDecomposition&>(), rdoc::__copy)
        .def(pybind11::init<const regina::Triangulation<2>&>(), rdoc::__init)
        .def(pybind11::init<const regina::Triangulation<2>&,
            regina::TreeDecompositionAlg>(), rdoc::__init)
        .def(pybind11::init<const regina::Triangulation<3>&>(), rdoc::__init)
        .def(pybind11::init<const regina::Triangulation<3>&,
            regina::TreeDecompositionAlg>(), rdoc::__init)
        .def(pybind11::init<const regina::Triangulation<4>&>(), rdoc::__init)
        .def(pybind11::init<const regina::Triangulation<4>&,
            regina::TreeDecompositionAlg>(), rdoc::__init)
        .def(pybind11::init<const regina::FacetPairing<3>&>(), rdoc::__init_2)
        .def(pybind11::init<const regina::FacetPairing<3>&,
            regina::TreeDecompositionAlg>(), rdoc::__init_2)
        .def(pybind11::init<const regina::FacetPairing<2>&>(), rdoc::__init_2)
        .def(pybind11::init<const regina::FacetPairing<2>&,
            regina::TreeDecompositionAlg>(), rdoc::__init_2)
        .def(pybind11::init<const regina::FacetPairing<4>&>(), rdoc::__init_2)
        .def(pybind11::init<const regina::FacetPairing<4>&,
            regina::TreeDecompositionAlg>(), rdoc::__init_2)
        .def(pybind11::init<const regina::Link&>(), rdoc::__init_3)
        .def(pybind11::init<const regina::Link&,
            regina::TreeDecompositionAlg>(), rdoc::__init_3)
        .def(pybind11::init<const regina::Matrix<bool>&>(), rdoc::__init_4)
        .def(pybind11::init<const regina::Matrix<bool>&,
            regina::TreeDecompositionAlg>(), rdoc::__init_4)
        .def(pybind11::init<const std::vector<std::vector<bool>>&>(),
            rdoc::__init_5)
        .def(pybind11::init<const std::vector<std::vector<bool>>&,
            regina::TreeDecompositionAlg>(), rdoc::__init_5)
        .def("swap", &TreeDecomposition::swap, rdoc::swap)
        .def("width", &TreeDecomposition::width, rdoc::width)
        .def("size", &TreeDecomposition::size, rdoc::size)
        .def("root", &TreeDecomposition::root,
            pybind11::return_value_policy::reference_internal, rdoc::root)
        .def("first", &TreeDecomposition::first,
            pybind11::return_value_policy::reference_internal, rdoc::first)
        .def("firstPrefix", &TreeDecomposition::firstPrefix,
            pybind11::return_value_policy::reference_internal,
            rdoc::firstPrefix)
        .def("bag", &TreeDecomposition::bag,
            pybind11::return_value_policy::reference_internal, rdoc::bag)
        .def("compress", &TreeDecomposition::compress, rdoc::compress)
        .def("makeNice", [](TreeDecomposition& t) {
            t.makeNice();
        }, rdoc::makeNice)
        .def("makeNice", [](TreeDecomposition& t, std::nullptr_t) {
            t.makeNice();
        }, rdoc::makeNice)
        .def("makeNice", [](TreeDecomposition& t, const std::vector<int>& h) {
            // We cannot sanity-check the size of h, since a tree decomposition
            // does not know how many nodes are in its underlying graph.
            t.makeNice(h.data());
        }, rdoc::makeNice)
        // overload_cast cannot handle template vs non-template overloads.
        .def("reroot", static_cast<void (TreeDecomposition::*)(TreeBag*)>(
            &TreeDecomposition::reroot), rdoc::reroot)
        .def("reroot", [](TreeDecomposition& t,
                const std::vector<double>& costSame,
                const std::vector<double>& costReverse) {
            if (costSame.size() != t.size())
                throw regina::InvalidArgument(
                    "Argument costSame is a list of the wrong size");
            if (costReverse.size() != t.size())
                throw regina::InvalidArgument(
                    "Argument costReverse is a list of the wrong size");

            t.reroot(costSame.data(), costReverse.data());
        }, rdoc::reroot_2)
        .def("reroot", [](TreeDecomposition& t,
                const std::vector<double>& costSame,
                const std::vector<double>& costReverse,
                std::nullptr_t) {
            if (costSame.size() != t.size())
                throw regina::InvalidArgument(
                    "Argument costSame is a list of the wrong size");
            if (costReverse.size() != t.size())
                throw regina::InvalidArgument(
                    "Argument costReverse is a list of the wrong size");

            t.reroot(costSame.data(), costReverse.data());
        }, rdoc::reroot_2)
        .def("reroot", [](TreeDecomposition& t,
                const std::vector<double>& costSame,
                const std::vector<double>& costReverse,
                const std::vector<double>& costRoot) {
            if (costSame.size() != t.size())
                throw regina::InvalidArgument(
                    "Argument costSame is a list of the wrong size");
            if (costReverse.size() != t.size())
                throw regina::InvalidArgument(
                    "Argument costReverse is a list of the wrong size");
            if (costRoot.size() != t.size())
                throw regina::InvalidArgument(
                    "Argument costRoot is a list of the wrong size");

            t.reroot(costSame.data(), costReverse.data(), costRoot.data());
        }, rdoc::reroot_2)
        .def("dot", &TreeDecomposition::dot, rdoc::dot)
        .def("pace", &TreeDecomposition::pace, rdoc::pace)
        .def_static("fromPACE",
            overload_cast<const std::string&>(&TreeDecomposition::fromPACE),
            rdoc::fromPACE)
    ;
    regina::python::add_output(td);
    regina::python::add_eq_operators(td, rdoc::__eq, rdoc::__ne);

    regina::python::add_global_swap<TreeDecomposition>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

