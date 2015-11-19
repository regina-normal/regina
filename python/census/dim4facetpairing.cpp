
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

#include <boost/python.hpp>
#include "census/dim4facetpairing.h"
#include "dim4/dim4triangulation.h"
#include "../helpers.h"

using namespace boost::python;
using regina::Dim4FacetPairing;
using regina::Dim4PentFacet;
using regina::Dim4Triangulation;

namespace {
    const Dim4PentFacet& (Dim4FacetPairing::*dest_facet)(const Dim4PentFacet&)
        const = &Dim4FacetPairing::dest;
    const Dim4PentFacet& (Dim4FacetPairing::*dest_unsigned)(unsigned, unsigned)
        const = &Dim4FacetPairing::dest;
    bool (Dim4FacetPairing::*isUnmatched_facet)(const Dim4PentFacet&) const =
        &Dim4FacetPairing::isUnmatched;
    bool (Dim4FacetPairing::*isUnmatched_unsigned)(unsigned, unsigned) const =
        &Dim4FacetPairing::isUnmatched;

    const Dim4PentFacet& getItem(const Dim4FacetPairing& p,
            const Dim4PentFacet& index) {
        return p[index];
    }

    void writeDot_stdout(const Dim4FacetPairing& p, const char* prefix = 0,
            bool subgraph = false, bool labels = false) {
        p.writeDot(std::cout, prefix, subgraph, labels);
    }

    void writeDotHeader_stdout(const char* graphName = 0) {
        Dim4FacetPairing::writeDotHeader(std::cout, graphName);
    }

    // Write dot() and dotHeader() as standalone functions: it seems
    // difficult using boost overloads in a way that keeps both gcc and
    // LLVM happy. :/
    std::string dot_standalone(const Dim4FacetPairing& p,
            const char* prefix = 0, bool subgraph = false,
            bool labels = false) {
        return p.dot(prefix, subgraph, labels);
    }

    std::string dotHeader_standalone(const char* graphName = 0) {
        return Dim4FacetPairing::dotHeader(graphName);
    }

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeDot, writeDot_stdout, 1, 4);
    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeDotHeader, writeDotHeader_stdout,
        0, 1);

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_dot, dot_standalone, 1, 4);
    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_dotHeader, dotHeader_standalone, 0, 1);
}

void addDim4FacetPairing() {
    class_<Dim4FacetPairing, std::auto_ptr<Dim4FacetPairing>,
            boost::noncopyable>
            ("Dim4FacetPairing", init<const Dim4FacetPairing&>())
        .def(init<const Dim4Triangulation&>())
        .def("size", &Dim4FacetPairing::size)
        .def("dest", dest_facet,
            return_value_policy<reference_existing_object>())
        .def("dest", dest_unsigned,
            return_value_policy<reference_existing_object>())
        .def("__getitem__", getItem,
            return_value_policy<reference_existing_object>())
        .def("isUnmatched", isUnmatched_facet)
        .def("isUnmatched", isUnmatched_unsigned)
        .def("isCanonical", &Dim4FacetPairing::isCanonical)
        .def("toString", &Dim4FacetPairing::toString)
        .def("str", &Dim4FacetPairing::str)
        .def("toTextRep", &Dim4FacetPairing::toTextRep)
        .def("fromTextRep", &Dim4FacetPairing::fromTextRep,
            return_value_policy<manage_new_object>())
        .def("writeDot", writeDot_stdout, OL_writeDot())
        .def("dot", dot_standalone, OL_dot())
        .def("writeDotHeader", writeDotHeader_stdout, OL_writeDotHeader())
        .def("dotHeader", dotHeader_standalone, OL_dotHeader())
        .def("isClosed", &Dim4FacetPairing::isClosed)
        .def("__str__", &Dim4FacetPairing::str)
        .def(regina::python::add_eq_operators())
        .staticmethod("fromTextRep")
        .staticmethod("writeDotHeader")
        .staticmethod("dotHeader")
    ;
}

