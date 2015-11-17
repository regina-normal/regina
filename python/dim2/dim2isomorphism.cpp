
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

#include "dim2/dim2isomorphism.h"
#include "dim2/dim2triangulation.h"
#include "../safeheldtype.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Dim2Isomorphism;

namespace {
    int (Dim2Isomorphism::*simpImage_const)(unsigned) const =
        &Dim2Isomorphism::simpImage;
    regina::NPerm3 (Dim2Isomorphism::*facetPerm_const)(unsigned) const =
        &Dim2Isomorphism::facetPerm;

    regina::Dim2TriangleEdge Dim2iso_getItem(const Dim2Isomorphism& iso,
            const regina::Dim2TriangleEdge& e) {
        return iso[e];
    }
}

void addDim2Isomorphism() {
    class_<Dim2Isomorphism, std::auto_ptr<Dim2Isomorphism>, boost::noncopyable>
            ("Dim2Isomorphism", init<const Dim2Isomorphism&>())
        .def("getSourceSimplices", &Dim2Isomorphism::getSourceSimplices)
        .def("getSourceTriangles", &Dim2Isomorphism::getSourceTriangles)
        .def("simpImage", simpImage_const)
        .def("triImage", simpImage_const)
        .def("facetPerm", facetPerm_const)
        .def("edgePerm", facetPerm_const)
        .def("__getitem__", Dim2iso_getItem)
        .def("isIdentity", &Dim2Isomorphism::isIdentity)
        .def("apply", &Dim2Isomorphism::apply,
            return_value_policy<to_held_type<> >())
        .def("applyInPlace", &Dim2Isomorphism::applyInPlace)
        .def("random", &Dim2Isomorphism::random,
            return_value_policy<manage_new_object>())
        .def("str", &Dim2Isomorphism::str)
        .def("toString", &Dim2Isomorphism::toString)
        .def("detail", &Dim2Isomorphism::detail)
        .def("toStringLong", &Dim2Isomorphism::toStringLong)
        .def("__str__", &Dim2Isomorphism::str)
        .staticmethod("random")
    ;
}

