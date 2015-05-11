
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
#include "dim4/dim4isomorphism.h"
#include "dim4/dim4triangulation.h"

using namespace boost::python;
using regina::Dim4Isomorphism;
using regina::Isomorphism;

namespace {
    int (Dim4Isomorphism::*simpImage_const)(unsigned) const =
        &Dim4Isomorphism::simpImage;
    regina::NPerm5 (Dim4Isomorphism::*facetPerm_const)(unsigned) const =
        &Dim4Isomorphism::facetPerm;

    regina::Dim4PentFacet Dim4iso_getItem(const Dim4Isomorphism& iso,
            const regina::Dim4PentFacet& f) {
        return iso[f];
    }
}

void addDim4Isomorphism() {
    class_<Isomorphism<4>, std::auto_ptr<Isomorphism<4>>, boost::noncopyable>
            ("Isomorphism4", init<const Dim4Isomorphism&>())
        .def("size", &Dim4Isomorphism::size)
        .def("getSourceSimplices", &Dim4Isomorphism::getSourceSimplices)
        .def("simpImage", simpImage_const)
        .def("pentImage", simpImage_const)
        .def("facetPerm", facetPerm_const)
        .def("__getitem__", Dim4iso_getItem)
        .def("isIdentity", &Dim4Isomorphism::isIdentity)
        .def("apply", &Dim4Isomorphism::apply,
            return_value_policy<manage_new_object>())
        .def("applyInPlace", &Dim4Isomorphism::applyInPlace)
        .def("random", &Dim4Isomorphism::random,
            return_value_policy<manage_new_object>())
        .def("identity", &Dim4Isomorphism::identity,
            return_value_policy<manage_new_object>())
        .def("str", &Dim4Isomorphism::str)
        .def("toString", &Dim4Isomorphism::toString)
        .def("detail", &Dim4Isomorphism::detail)
        .def("toStringLong", &Dim4Isomorphism::toStringLong)
        .def("__str__", &Dim4Isomorphism::str)
        .staticmethod("random")
        .staticmethod("identity")
    ;

    scope().attr("Dim4Isomorphism") = scope().attr("Isomorphism4");
}

