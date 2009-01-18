
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "census/dim4facetpairing.h"
#include "dim4/dim4triangulation.h"
#include <boost/python.hpp>

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
            bool subgraph = false) {
        p.writeDot(std::cout, prefix, subgraph);
    }

    void writeDotHeader_stdout(const char* graphName = 0) {
        Dim4FacetPairing::writeDotHeader(std::cout, graphName);
    }

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeDot, writeDot_stdout, 1, 3);
    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeDotHeader, writeDotHeader_stdout,
        0, 1);
}

void addDim4FacetPairing() {
    class_<Dim4FacetPairing, std::auto_ptr<Dim4FacetPairing>,
            boost::noncopyable>
            ("Dim4FacetPairing", init<const Dim4FacetPairing&>())
        .def(init<const Dim4Triangulation&>())
        .def("getNumberOfPentachora", &Dim4FacetPairing::getNumberOfPentachora)
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
        .def("toTextRep", &Dim4FacetPairing::toTextRep)
        .def("fromTextRep", &Dim4FacetPairing::fromTextRep,
            return_value_policy<manage_new_object>())
        .def("writeDot", writeDot_stdout, OL_writeDot())
        .def("writeDotHeader", writeDotHeader_stdout, OL_writeDotHeader())
        .def("isClosed", &Dim4FacetPairing::isClosed)
        .def("__str__", &Dim4FacetPairing::toString)
        .staticmethod("fromTextRep")
        .staticmethod("writeDotHeader")
    ;
}

