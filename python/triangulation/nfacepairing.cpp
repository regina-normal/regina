
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
#include "triangulation/nfacepair.h"
#include "triangulation/nfacepairing.h"
#include "triangulation/ntriangulation.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NFacePairing;
using regina::NTetFace;
using regina::NTriangulation;

namespace {
    const NTetFace& (NFacePairing::*dest_face)(const NTetFace&) const =
        &NFacePairing::dest;
    const NTetFace& (NFacePairing::*dest_unsigned)(size_t, unsigned) const =
        &NFacePairing::dest;
    bool (NFacePairing::*isUnmatched_face)(const NTetFace&) const =
        &NFacePairing::isUnmatched;
    bool (NFacePairing::*isUnmatched_unsigned)(size_t, unsigned) const =
        &NFacePairing::isUnmatched;

    bool (NFacePairing::*query_bdec)() const =
        &NFacePairing::hasBrokenDoubleEndedChain;
    bool (NFacePairing::*query_oecwdh)() const =
        &NFacePairing::hasOneEndedChainWithDoubleHandle;
    bool (NFacePairing::*query_wdec)() const =
        &NFacePairing::hasWedgedDoubleEndedChain;
    bool (NFacePairing::*query_oecwsb)() const =
        &NFacePairing::hasOneEndedChainWithStrayBigon;
    bool (NFacePairing::*query_toec)() const =
        &NFacePairing::hasTripleOneEndedChain;

    void writeDot_stdout(const NFacePairing& p, const char* prefix = 0,
            bool subgraph = false, bool labels = false) {
        p.writeDot(std::cout, prefix, subgraph, labels);
    }

    void writeDotHeader_stdout(const char* graphName = 0) {
        NFacePairing::writeDotHeader(std::cout, graphName);
    }

    // Write dot() and dotHeader() as standalone functions: it seems
    // difficult using boost overloads in a way that keeps both gcc and
    // LLVM happy. :/
    std::string dot_standalone(const NFacePairing& p,
            const char* prefix = 0, bool subgraph = false,
            bool labels = false) {
        return p.dot(prefix, subgraph, labels);
    }

    std::string dotHeader_standalone(const char* graphName = 0) {
        return NFacePairing::dotHeader(graphName);
    }

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeDot, writeDot_stdout, 1, 4);
    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeDotHeader, writeDotHeader_stdout,
        0, 1);

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_dot, dot_standalone, 1, 4);
    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_dotHeader, dotHeader_standalone, 0, 1);
}

void addNFacePairing() {
    class_<NFacePairing, std::auto_ptr<NFacePairing>, boost::noncopyable>
            ("NFacePairing", init<const NFacePairing&>())
        .def(init<const NTriangulation&>())
        .def("size", &NFacePairing::size)
        .def("getNumberOfTetrahedra", &NFacePairing::size)
        .def("dest", dest_face,
            return_value_policy<reference_existing_object>())
        .def("dest", dest_unsigned,
            return_value_policy<reference_existing_object>())
        .def("__getitem__",
            static_cast<const NTetFace& (NFacePairing::*)(
                const NTetFace&) const>(&NFacePairing::operator[]),
            return_value_policy<reference_existing_object>())
        .def("isUnmatched", isUnmatched_face)
        .def("isUnmatched", isUnmatched_unsigned)
        .def("isCanonical", &NFacePairing::isCanonical)
        .def("toTextRep", &NFacePairing::toTextRep)
        .def("fromTextRep", &NFacePairing::fromTextRep,
            return_value_policy<manage_new_object>())
        .def("writeDot", writeDot_stdout, OL_writeDot())
        .def("dot", dot_standalone, OL_dot())
        .def("writeDotHeader", writeDotHeader_stdout, OL_writeDotHeader())
        .def("dotHeader", dotHeader_standalone, OL_dotHeader())
        .def("isClosed", &NFacePairing::isClosed)
        .def("hasTripleEdge", &NFacePairing::hasTripleEdge)
        .def("followChain", &NFacePairing::followChain)

        // For reasons I do not understand, it seems I have to declare
        // these routines separately to avoid a compile error with
        // g++ 3.3.  *shrug*
        .def("hasBrokenDoubleEndedChain", query_bdec)
        .def("hasOneEndedChainWithDoubleHandle", query_oecwdh)
        .def("hasWedgedDoubleEndedChain", query_wdec)
        .def("hasOneEndedChainWithStrayBigon", query_oecwsb)
        .def("hasTripleOneEndedChain", query_toec)

        .def("hasSingleStar", &NFacePairing::hasSingleStar)
        .def("hasDoubleStar", &NFacePairing::hasDoubleStar)
        .def("hasDoubleSquare", &NFacePairing::hasDoubleSquare)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("fromTextRep")
        .staticmethod("writeDotHeader")
        .staticmethod("dotHeader")
    ;
}

