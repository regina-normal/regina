
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include <boost/python.hpp>
#include "census/dim2edgepairing.h"
#include "dim2/dim2triangulation.h"

using namespace boost::python;
using regina::Dim2EdgePairing;
using regina::Dim2TriangleEdge;
using regina::Dim2Triangulation;

namespace {
    const Dim2TriangleEdge& (Dim2EdgePairing::*dest_facet)(
        const Dim2TriangleEdge&) const = &Dim2EdgePairing::dest;
    const Dim2TriangleEdge& (Dim2EdgePairing::*dest_unsigned)(
        unsigned, unsigned) const = &Dim2EdgePairing::dest;
    bool (Dim2EdgePairing::*isUnmatched_facet)(const Dim2TriangleEdge&) const =
        &Dim2EdgePairing::isUnmatched;
    bool (Dim2EdgePairing::*isUnmatched_unsigned)(unsigned, unsigned) const =
        &Dim2EdgePairing::isUnmatched;

    const Dim2TriangleEdge& getItem(const Dim2EdgePairing& p,
            const Dim2TriangleEdge& index) {
        return p[index];
    }

    void writeDot_stdout(const Dim2EdgePairing& p, const char* prefix = 0,
            bool subgraph = false, bool labels = false) {
        p.writeDot(std::cout, prefix, subgraph, labels);
    }

    void writeDotHeader_stdout(const char* graphName = 0) {
        Dim2EdgePairing::writeDotHeader(std::cout, graphName);
    }

    // Write dot() and dotHeader() as standalone functions: it seems
    // difficult using boost overloads in a way that keeps both gcc and
    // LLVM happy. :/
    std::string dot_standalone(const Dim2EdgePairing& p,
            const char* prefix = 0, bool subgraph = false,
            bool labels = false) {
        return p.dot(prefix, subgraph, labels);
    }

    std::string dotHeader_standalone(const char* graphName = 0) {
        return Dim2EdgePairing::dotHeader(graphName);
    }

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeDot, writeDot_stdout, 1, 4);
    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeDotHeader, writeDotHeader_stdout,
        0, 1);

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_dot, dot_standalone, 1, 4);
    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_dotHeader, dotHeader_standalone, 0, 1);
}

void addDim2EdgePairing() {
    class_<Dim2EdgePairing, std::auto_ptr<Dim2EdgePairing>,
            boost::noncopyable>
            ("Dim2EdgePairing", init<const Dim2EdgePairing&>())
        .def(init<const Dim2Triangulation&>())
        .def("size", &Dim2EdgePairing::size)
        .def("dest", dest_facet,
            return_value_policy<reference_existing_object>())
        .def("dest", dest_unsigned,
            return_value_policy<reference_existing_object>())
        .def("__getitem__", getItem,
            return_value_policy<reference_existing_object>())
        .def("isUnmatched", isUnmatched_facet)
        .def("isUnmatched", isUnmatched_unsigned)
        .def("isCanonical", &Dim2EdgePairing::isCanonical)
        .def("toString", &Dim2EdgePairing::toString)
        .def("toTextRep", &Dim2EdgePairing::toTextRep)
        .def("fromTextRep", &Dim2EdgePairing::fromTextRep,
            return_value_policy<manage_new_object>())
        .def("writeDot", writeDot_stdout, OL_writeDot())
        .def("dot", dot_standalone, OL_dot())
        .def("writeDotHeader", writeDotHeader_stdout, OL_writeDotHeader())
        .def("dotHeader", dotHeader_standalone, OL_dotHeader())
        .def("isClosed", &Dim2EdgePairing::isClosed)
        .def("__str__", &Dim2EdgePairing::toString)
        .staticmethod("fromTextRep")
        .staticmethod("writeDotHeader")
        .staticmethod("dotHeader")
    ;
}

