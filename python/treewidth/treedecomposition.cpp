
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "treewidth/treedecomposition.h"
#include "triangulation/facetpairing.h"
#include "triangulation/facetpairing3.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "../helpers.h"

using namespace boost::python;
using regina::TreeBag;
using regina::TreeDecomposition;

namespace {
    TreeDecomposition* fromListAlg(boost::python::list graph,
            regina::TreeDecompositionAlg alg = regina::TD_UPPER) {
        long len = boost::python::len(graph);

        long i, j, k;
        bool** g = new bool*[len];
        boost::python::list row;

        for (i = 0; i < len; ++i) {
            g[i] = new bool[len];

            extract<boost::python::list> x_list(graph[i]);
            if (! x_list.check()) {
                // Throw an exception.
                for (k = 0; k <= i; ++k)
                    delete[] g[k];
                delete[] g;
                x_list();
            }

            row = x_list();
            if (boost::python::len(row) != len) {
                // Throw an exception.
                for (k = 0; k <= i; ++k)
                    delete[] g[k];
                delete[] g;

                PyErr_SetString(PyExc_ValueError,
                    "Initialisation list does not describe a square matrix.");
                boost::python::throw_error_already_set();
            }

            for (j = 0; j < len; ++j) {
                extract<bool> val(row[j]);
                if (val.check()) {
                    g[i][j] = val();
                    continue;
                }

                // Throw an exception.
                for (k = 0; k <= i; ++k)
                    delete[] g[k];
                delete[] g;
                val();
            }
        }

        TreeDecomposition* ans = new TreeDecomposition(
            len, const_cast<bool const**>(g), alg);

        for (i = 0; i < len; ++i)
            delete[] g[i];
        delete[] g;

        return ans;
    }

    inline TreeDecomposition* fromList(boost::python::list graph) {
        return fromListAlg(graph);
    }

    void writeDot_stdio(const TreeDecomposition& t) {
        t.writeDot(std::cout);
    }
}

void addTreeDecomposition() {
    scope global;

    enum_<regina::TreeDecompositionAlg>("TreeDecompositionAlg")
        .value("TD_UPPER", regina::TD_UPPER)
        .value("TD_UPPER_GREEDY_FILL_IN", regina::TD_UPPER_GREEDY_FILL_IN)
        ;

    global.attr("TD_UPPER") = regina::TD_UPPER;
    global.attr("TD_UPPER_GREEDY_FILL_IN") = regina::TD_UPPER_GREEDY_FILL_IN;

    enum_<regina::BagComparison>("BagComparison")
        .value("BAG_EQUAL", regina::BAG_EQUAL)
        .value("BAG_SUBSET", regina::BAG_SUBSET)
        .value("BAG_SUPERSET", regina::BAG_SUPERSET)
        .value("BAG_UNRELATED", regina::BAG_UNRELATED)
        ;

    global.attr("BAG_EQUAL") = regina::BAG_EQUAL;
    global.attr("BAG_SUBSET") = regina::BAG_SUBSET;
    global.attr("BAG_SUPERSET") = regina::BAG_SUPERSET;
    global.attr("BAG_UNRELATED") = regina::BAG_UNRELATED;

    enum_<regina::NiceType>("NiceType")
        .value("NICE_INTRODUCE", regina::NICE_INTRODUCE)
        .value("NICE_FORGET", regina::NICE_FORGET)
        .value("NICE_JOIN", regina::NICE_JOIN)
        ;

    global.attr("NICE_INTRODUCE") = regina::NICE_INTRODUCE;
    global.attr("NICE_FORGET") = regina::NICE_FORGET;
    global.attr("NICE_JOIN") = regina::NICE_JOIN;

    class_<TreeBag, std::auto_ptr<TreeBag>,
            boost::noncopyable>("TreeBag", no_init)
        .def("size", &TreeBag::size)
        .def("element", &TreeBag::element)
        .def("contains", &TreeBag::contains)
        .def("index", &TreeBag::index)
        .def("type", &TreeBag::type)
        .def("subtype", &TreeBag::subtype)
        .def("compare", &TreeBag::compare)
        .def("next", &TreeBag::next,
            return_value_policy<reference_existing_object>())
        .def("nextPrefix", &TreeBag::nextPrefix,
            return_value_policy<reference_existing_object>())
        .def("parent", &TreeBag::parent,
            return_value_policy<reference_existing_object>())
        .def("children", &TreeBag::children,
            return_value_policy<reference_existing_object>())
        .def("sibling", &TreeBag::sibling,
            return_value_policy<reference_existing_object>())
        .def("isLeaf", &TreeBag::isLeaf)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    class_<TreeDecomposition, std::auto_ptr<TreeDecomposition>,
            boost::noncopyable>("TreeDecomposition", no_init)
        .def(init<const regina::Triangulation<2>&>())
        .def(init<const regina::Triangulation<2>&,
            regina::TreeDecompositionAlg>())
        .def(init<const regina::Triangulation<3>&>())
        .def(init<const regina::Triangulation<3>&,
            regina::TreeDecompositionAlg>())
        .def(init<const regina::Triangulation<4>&>())
        .def(init<const regina::Triangulation<4>&,
            regina::TreeDecompositionAlg>())
        .def(init<const regina::FacetPairing<3>&>())
        .def(init<const regina::FacetPairing<3>&,
            regina::TreeDecompositionAlg>())
        .def(init<const regina::FacetPairing<2>&>())
        .def(init<const regina::FacetPairing<2>&,
            regina::TreeDecompositionAlg>())
        .def(init<const regina::FacetPairing<4>&>())
        .def(init<const regina::FacetPairing<4>&,
            regina::TreeDecompositionAlg>())
        .def("__init__", make_constructor(fromListAlg))
        .def("__init__", make_constructor(fromList))
        .def("width", &TreeDecomposition::width)
        .def("size", &TreeDecomposition::size)
        .def("root", &TreeDecomposition::root,
            return_value_policy<reference_existing_object>())
        .def("first", &TreeDecomposition::first,
            return_value_policy<reference_existing_object>())
        .def("firstPrefix", &TreeDecomposition::firstPrefix,
            return_value_policy<reference_existing_object>())
        .def("compress", &TreeDecomposition::compress)
        .def("makeNice", &TreeDecomposition::makeNice)
        .def("writeDot", writeDot_stdio)
        .def("dot", &TreeDecomposition::dot)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NTreeBag") = scope().attr("TreeBag");
    scope().attr("NTreeDecomposition") = scope().attr("TreeDecomposition");
}

