
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
#include "link/link.h"
#include "treewidth/treedecomposition.h"
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
        .def(pybind11::init<const regina::TreeDecomposition&>())
        .def(pybind11::init([](pybind11::list graph,
                regina::TreeDecompositionAlg alg) {
            size_t len = graph.size();
            bool** g = new bool*[len];

            long i, j, k;
            pybind11::list row;
            for (i = 0; i < len; ++i) {
                try {
                    row = graph[i].cast<pybind11::list>();
                } catch (pybind11::cast_error const &) {
                    // Clean up and throw an exception.
                    for (k = 0; k < i; ++k) delete[] g[k];
                    delete[] g;
                    throw std::invalid_argument(
                        "Graph must be presented as a list of lists");
                }
                if (row.size() != len) {
                    // Clean up and throw an exception.
                    for (k = 0; k < i; ++k) delete[] g[k];
                    delete[] g;
                    throw pybind11::index_error("Initialisation list "
                        "does not describe a square matrix");
                }

                g[i] = new bool[len];
                try {
                    for (j = 0; j < len; ++j)
                        g[i][j] = row[j].cast<bool>();
                } catch (pybind11::cast_error const &) {
                    // Clean up and throw an exception.
                    for (k = 0; k <= i; ++k) delete[] g[k];
                    delete[] g;
                    throw std::invalid_argument(
                        "Matrix element not convertible to a boolean");
                }
            }
            TreeDecomposition* ans = new TreeDecomposition(
                len, const_cast<bool const**>(g), alg);

            // Clean up and return.
            for (i = 0; i < len; ++i) delete[] g[i];
            delete[] g;
            return ans;
        }), pybind11::arg(), pybind11::arg("alg") = regina::TD_UPPER)
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
            // In python we don't allow passing the heightHint array.
            t.makeNice();
        })
        .def("reroot", [](TreeDecomposition& t, TreeBag* b) {
            // overload_cast breaks since reroot is overloaded with a
            // templated and non-templated version.
            t.reroot(b);
        })
        .def("writeDot", [](const TreeDecomposition& t) {
            t.writeDot(std::cout);
        })
        .def("dot", &TreeDecomposition::dot)
        .def("writePACE", [](const TreeDecomposition& t) {
            t.writePACE(std::cout);
        })
        .def("pace", &TreeDecomposition::pace)
        .def_static("fromPACE",
            overload_cast<const std::string&>(&TreeDecomposition::fromPACE))
    ;
    regina::python::add_output(td);
    regina::python::add_eq_operators(td);

    m.attr("NTreeBag") = m.attr("TreeBag");
    m.attr("NTreeDecomposition") = m.attr("TreeDecomposition");
}

