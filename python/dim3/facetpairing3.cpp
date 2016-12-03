
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
#include "triangulation/facepair.h"
#include "triangulation/facetpairing3.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using namespace boost::python;
using regina::FacetPairing;
using regina::FacetSpec;
using regina::Triangulation;

namespace {
    const FacetSpec<3>& (FacetPairing<3>::*dest_face)(const FacetSpec<3>&) const =
        &FacetPairing<3>::dest;
    const FacetSpec<3>& (FacetPairing<3>::*dest_unsigned)(size_t, unsigned) const =
        &FacetPairing<3>::dest;
    bool (FacetPairing<3>::*isUnmatched_face)(const FacetSpec<3>&) const =
        &FacetPairing<3>::isUnmatched;
    bool (FacetPairing<3>::*isUnmatched_unsigned)(size_t, unsigned) const =
        &FacetPairing<3>::isUnmatched;

    bool (FacetPairing<3>::*query_bdec)() const =
        &FacetPairing<3>::hasBrokenDoubleEndedChain;
    bool (FacetPairing<3>::*query_oecwdh)() const =
        &FacetPairing<3>::hasOneEndedChainWithDoubleHandle;
    bool (FacetPairing<3>::*query_wdec)() const =
        &FacetPairing<3>::hasWedgedDoubleEndedChain;
    bool (FacetPairing<3>::*query_oecwsb)() const =
        &FacetPairing<3>::hasOneEndedChainWithStrayBigon;
    bool (FacetPairing<3>::*query_toec)() const =
        &FacetPairing<3>::hasTripleOneEndedChain;

    void writeDot_stdout(const FacetPairing<3>& p, const char* prefix = 0,
            bool subgraph = false, bool labels = false) {
        p.writeDot(std::cout, prefix, subgraph, labels);
    }

    void writeDotHeader_stdout(const char* graphName = 0) {
        FacetPairing<3>::writeDotHeader(std::cout, graphName);
    }

    // Write dot() and dotHeader() as standalone functions: it seems
    // difficult using boost overloads in a way that keeps both gcc and
    // LLVM happy. :/
    std::string dot_standalone(const FacetPairing<3>& p,
            const char* prefix = 0, bool subgraph = false,
            bool labels = false) {
        return p.dot(prefix, subgraph, labels);
    }

    std::string dotHeader_standalone(const char* graphName = 0) {
        return FacetPairing<3>::dotHeader(graphName);
    }

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeDot, writeDot_stdout, 1, 4);
    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeDotHeader, writeDotHeader_stdout,
        0, 1);

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_dot, dot_standalone, 1, 4);
    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_dotHeader, dotHeader_standalone, 0, 1);
}

void addFacetPairing3() {
    class_<FacetPairing<3>, std::auto_ptr<FacetPairing<3>>, boost::noncopyable>
            ("FacetPairing3", init<const FacetPairing<3>&>())
        .def(init<const Triangulation<3>&>())
        .def("size", &FacetPairing<3>::size)
        .def("dest", dest_face,
            return_value_policy<reference_existing_object>())
        .def("dest", dest_unsigned,
            return_value_policy<reference_existing_object>())
        .def("__getitem__",
            static_cast<const FacetSpec<3>& (FacetPairing<3>::*)(
                const FacetSpec<3>&) const>(&FacetPairing<3>::operator[]),
            return_value_policy<reference_existing_object>())
        .def("isUnmatched", isUnmatched_face)
        .def("isUnmatched", isUnmatched_unsigned)
        .def("isCanonical", &FacetPairing<3>::isCanonical)
        .def("toTextRep", &FacetPairing<3>::toTextRep)
        .def("fromTextRep", &FacetPairing<3>::fromTextRep,
            return_value_policy<manage_new_object>())
        .def("writeDot", writeDot_stdout, OL_writeDot())
        .def("dot", dot_standalone, OL_dot())
        .def("writeDotHeader", writeDotHeader_stdout, OL_writeDotHeader())
        .def("dotHeader", dotHeader_standalone, OL_dotHeader())
        .def("isClosed", &FacetPairing<3>::isClosed)
        .def("hasTripleEdge", &FacetPairing<3>::hasTripleEdge)
        .def("followChain", &FacetPairing<3>::followChain)

        // For reasons I do not understand, it seems I have to declare
        // these routines separately to avoid a compile error with
        // g++ 3.3.  *shrug*
        .def("hasBrokenDoubleEndedChain", query_bdec)
        .def("hasOneEndedChainWithDoubleHandle", query_oecwdh)
        .def("hasWedgedDoubleEndedChain", query_wdec)
        .def("hasOneEndedChainWithStrayBigon", query_oecwsb)
        .def("hasTripleOneEndedChain", query_toec)

        .def("hasSingleStar", &FacetPairing<3>::hasSingleStar)
        .def("hasDoubleStar", &FacetPairing<3>::hasDoubleStar)
        .def("hasDoubleSquare", &FacetPairing<3>::hasDoubleSquare)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("fromTextRep")
        .staticmethod("writeDotHeader")
        .staticmethod("dotHeader")
    ;

    scope().attr("NFacePairing") = scope().attr("FacetPairing3");
}

