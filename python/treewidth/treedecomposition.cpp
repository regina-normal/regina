
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
#include "../pybind11/stl.h"
#include "link/link.h"
#include "treewidth/treedecomposition-impl.h"
#include "triangulation/facetpairing.h"
#include "triangulation/facetpairing3.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::TreeBag;
using regina::TreeDecomposition;

void addTreeDecomposition(pybind11::module_& m) {
    pybind11::enum_<regina::TreeDecompositionAlg>(m, "TreeDecompositionAlg")
        .value("TD_UPPER", regina::TD_UPPER)
        .value("TD_UPPER_GREEDY_FILL_IN", regina::TD_UPPER_GREEDY_FILL_IN)
        .export_values()
        ;

    pybind11::enum_<regina::BagComparison>(m, "BagComparison")
        .value("BAG_EQUAL", regina::BAG_EQUAL)
        .value("BAG_SUBSET", regina::BAG_SUBSET)
        .value("BAG_SUPERSET", regina::BAG_SUPERSET)
        .value("BAG_UNRELATED", regina::BAG_UNRELATED)
        .export_values()
        ;

    pybind11::enum_<regina::NiceType>(m, "NiceType")
        .value("NICE_INTRODUCE", regina::NICE_INTRODUCE)
        .value("NICE_FORGET", regina::NICE_FORGET)
        .value("NICE_JOIN", regina::NICE_JOIN)
        .export_values()
        ;

    auto tb = pybind11::class_<TreeBag>(m, "TreeBag")
        .def("size", &TreeBag::size)
        .def("element", &TreeBag::element)
        .def("contains", &TreeBag::contains)
        .def("index", &TreeBag::index)
        .def("type", &TreeBag::type)
        .def("subtype", &TreeBag::subtype)
        .def("compare", &TreeBag::compare)
        .def("next", &TreeBag::next,
            pybind11::return_value_policy::reference)
        .def("nextPrefix", &TreeBag::nextPrefix,
            pybind11::return_value_policy::reference)
        .def("parent", &TreeBag::parent,
            pybind11::return_value_policy::reference)
        .def("children", &TreeBag::children,
            pybind11::return_value_policy::reference)
        .def("sibling", &TreeBag::sibling,
            pybind11::return_value_policy::reference)
        .def("isLeaf", &TreeBag::isLeaf)
    ;
    regina::python::add_output(tb);
    regina::python::add_eq_operators(tb);

    auto td = pybind11::class_<TreeDecomposition>(m, "TreeDecomposition")
        .def(pybind11::init<const regina::Triangulation<2>&>())
        .def(pybind11::init<const regina::Triangulation<2>&,
            regina::TreeDecompositionAlg>())
        .def(pybind11::init<const regina::Triangulation<3>&>())
        .def(pybind11::init<const regina::Triangulation<3>&,
            regina::TreeDecompositionAlg>())
        .def(pybind11::init<const regina::Triangulation<4>&>())
        .def(pybind11::init<const regina::Triangulation<4>&,
            regina::TreeDecompositionAlg>())
        .def(pybind11::init<const regina::FacetPairing<3>&>())
        .def(pybind11::init<const regina::FacetPairing<3>&,
            regina::TreeDecompositionAlg>())
        .def(pybind11::init<const regina::FacetPairing<2>&>())
        .def(pybind11::init<const regina::FacetPairing<2>&,
            regina::TreeDecompositionAlg>())
        .def(pybind11::init<const regina::FacetPairing<4>&>())
        .def(pybind11::init<const regina::FacetPairing<4>&,
            regina::TreeDecompositionAlg>())
        .def(pybind11::init<const regina::Link&>())
        .def(pybind11::init<const regina::Link&,
            regina::TreeDecompositionAlg>())
        .def(pybind11::init<const regina::Matrix<bool>&>())
        .def(pybind11::init<const regina::Matrix<bool>&,
            regina::TreeDecompositionAlg>())
        .def(pybind11::init<const std::vector<std::vector<bool>>&>())
        .def(pybind11::init<const std::vector<std::vector<bool>>&,
            regina::TreeDecompositionAlg>())
        .def(pybind11::init<const regina::TreeDecomposition&>())
        .def("swap", &TreeDecomposition::swap)
        .def("width", &TreeDecomposition::width)
        .def("size", &TreeDecomposition::size)
        .def("root", &TreeDecomposition::root,
            pybind11::return_value_policy::reference_internal)
        .def("first", &TreeDecomposition::first,
            pybind11::return_value_policy::reference_internal)
        .def("firstPrefix", &TreeDecomposition::firstPrefix,
            pybind11::return_value_policy::reference_internal)
        .def("bag", &TreeDecomposition::bag,
            pybind11::return_value_policy::reference_internal)
        .def("compress", &TreeDecomposition::compress)
        .def("makeNice", [](TreeDecomposition& t) {
            t.makeNice();
        })
        .def("makeNice", [](TreeDecomposition& t, std::nullptr_t) {
            t.makeNice();
        })
        .def("makeNice", [](TreeDecomposition& t, const std::vector<int>& h) {
            // We cannot sanity-check the size of h, since a tree decomposition
            // does not know how many nodes are in its underlying graph.
            t.makeNice(h.data());
        })
        // overload_cast cannot handle template vs non-template overloads.
        .def("reroot", (void (TreeDecomposition::*)(TreeBag*))(
            &TreeDecomposition::reroot))
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
        })
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
        })
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
        })
        .def("dot", &TreeDecomposition::dot)
        .def("pace", &TreeDecomposition::pace)
        .def_static("fromPACE",
            overload_cast<const std::string&>(&TreeDecomposition::fromPACE))
    ;
    regina::python::add_output(td);
    regina::python::add_eq_operators(td);

    regina::python::add_global_swap<TreeDecomposition>(m);
}

