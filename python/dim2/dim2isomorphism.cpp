
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
#include "dim2/dim2isomorphism.h"
#include "dim2/dim2triangulation.h"

using namespace boost::python;
using regina::Dim2Isomorphism;

namespace {
    int (Dim2Isomorphism::*simpImage_const)(unsigned) const =
        &Dim2Isomorphism::simpImage;
    regina::NPerm3 (Dim2Isomorphism::*facetPerm_const)(unsigned) const =
        &Dim2Isomorphism::facetPerm;

    /*
    regina::Dim2PentFacet Dim2iso_getItem(const Dim2Isomorphism& iso,
            const regina::Dim2PentFacet& f) {
        return iso[f];
    }
    */
}

void addDim2Isomorphism() {
    class_<Dim2Isomorphism, bases<regina::ShareableObject>,
            std::auto_ptr<Dim2Isomorphism>, boost::noncopyable>
            ("Dim2Isomorphism", init<const Dim2Isomorphism&>())
        .def("getSourceSimplices", &Dim2Isomorphism::getSourceSimplices)
        .def("getSourceTriangles", &Dim2Isomorphism::getSourceTriangles)
        .def("simpImage", simpImage_const)
        .def("triImage", simpImage_const)
        .def("facetPerm", facetPerm_const)
        .def("edgePerm", facetPerm_const)
        // .def("__getitem__", Dim2iso_getItem)
        .def("isIdentity", &Dim2Isomorphism::isIdentity)
        .def("apply", &Dim2Isomorphism::apply,
            return_value_policy<manage_new_object>())
        .def("applyInPlace", &Dim2Isomorphism::applyInPlace)
        .def("random", &Dim2Isomorphism::random,
            return_value_policy<manage_new_object>())
        .staticmethod("random")
    ;
}

