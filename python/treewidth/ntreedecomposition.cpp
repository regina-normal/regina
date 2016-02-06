
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
#include "dim2/dim2edgepairing.h"
#include "dim2/dim2triangulation.h"
#include "dim4/dim4facetpairing.h"
#include "dim4/dim4triangulation.h"
#include "treewidth/ntreedecomposition.h"
#include "triangulation/nfacepairing.h"
#include "triangulation/ntriangulation.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NTreeBag;
using regina::NTreeDecomposition;

namespace {
    NTreeDecomposition* fromListAlg(boost::python::list graph,
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

        NTreeDecomposition* ans = new NTreeDecomposition(
            len, const_cast<bool const**>(g), alg);

        for (i = 0; i < len; ++i)
            delete[] g[i];
        delete[] g;

        return ans;
    }

    inline NTreeDecomposition* fromList(boost::python::list graph) {
        return fromListAlg(graph);
    }
}

void addNTreeDecomposition() {
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

    class_<NTreeBag, std::auto_ptr<NTreeBag>,
            boost::noncopyable>("NTreeBag", no_init)
        .def("size", &NTreeBag::size)
        .def("element", &NTreeBag::element)
        .def("contains", &NTreeBag::contains)
        .def("index", &NTreeBag::index)
        .def("type", &NTreeBag::type)
        .def("subtype", &NTreeBag::subtype)
        .def("compare", &NTreeBag::compare)
        .def("next", &NTreeBag::next,
            return_value_policy<reference_existing_object>())
        .def("nextPrefix", &NTreeBag::nextPrefix,
            return_value_policy<reference_existing_object>())
        .def("parent", &NTreeBag::parent,
            return_value_policy<reference_existing_object>())
        .def("children", &NTreeBag::children,
            return_value_policy<reference_existing_object>())
        .def("sibling", &NTreeBag::sibling,
            return_value_policy<reference_existing_object>())
        .def("isLeaf", &NTreeBag::isLeaf)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    class_<NTreeDecomposition, std::auto_ptr<NTreeDecomposition>,
            boost::noncopyable>("NTreeDecomposition", no_init)
        .def(init<const regina::NTriangulation&>())
        .def(init<const regina::NTriangulation&,
            regina::TreeDecompositionAlg>())
        .def(init<const regina::Dim2Triangulation&>())
        .def(init<const regina::Dim2Triangulation&,
            regina::TreeDecompositionAlg>())
        .def(init<const regina::Dim4Triangulation&>())
        .def(init<const regina::Dim4Triangulation&,
            regina::TreeDecompositionAlg>())
        .def(init<const regina::NFacePairing&>())
        .def(init<const regina::NFacePairing&,
            regina::TreeDecompositionAlg>())
        .def(init<const regina::Dim2EdgePairing&>())
        .def(init<const regina::Dim2EdgePairing&,
            regina::TreeDecompositionAlg>())
        .def(init<const regina::Dim4FacetPairing&>())
        .def(init<const regina::Dim4FacetPairing&,
            regina::TreeDecompositionAlg>())
        .def("__init__", make_constructor(fromListAlg))
        .def("__init__", make_constructor(fromList))
        .def("width", &NTreeDecomposition::width)
        .def("size", &NTreeDecomposition::size)
        .def("root", &NTreeDecomposition::root,
            return_value_policy<reference_existing_object>())
        .def("first", &NTreeDecomposition::first,
            return_value_policy<reference_existing_object>())
        .def("firstPrefix", &NTreeDecomposition::firstPrefix,
            return_value_policy<reference_existing_object>())
        .def("compress", &NTreeDecomposition::compress)
        .def("makeNice", &NTreeDecomposition::makeNice)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
}

