
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
#include "triangulation/nfacetspec.h"

using namespace boost::python;
using regina::Dim2TriangleEdge;

namespace {
    Dim2TriangleEdge triangleedge_inc_operator(Dim2TriangleEdge& p) {
        return p++;
    }

    Dim2TriangleEdge triangleedge_dec_operator(Dim2TriangleEdge& p) {
        return p--;
    }
}

void addDim2TriangleEdge() {
    class_<Dim2TriangleEdge>("Dim2TriangleEdge")
        .def(init<int, int>())
        .def(init<const Dim2TriangleEdge&>())
        .def_readwrite("simp", &Dim2TriangleEdge::simp)
        .def_readwrite("facet", &Dim2TriangleEdge::facet)
        .def("isBoundary", &Dim2TriangleEdge::isBoundary)
        .def("isBeforeStart", &Dim2TriangleEdge::isBeforeStart)
        .def("isPastEnd", &Dim2TriangleEdge::isPastEnd)
        .def("setFirst", &Dim2TriangleEdge::setFirst)
        .def("setBoundary", &Dim2TriangleEdge::setBoundary)
        .def("setBeforeStart", &Dim2TriangleEdge::setBeforeStart)
        .def("setPastEnd", &Dim2TriangleEdge::setPastEnd)
        .def("inc", triangleedge_inc_operator)
        .def("dec", triangleedge_dec_operator)
        .def(self == self)
        .def(self < self)
        .def(self <= self)
    ;
}

