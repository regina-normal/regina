
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
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"

using namespace boost::python;
using regina::NIsomorphism;

namespace {
    int (NIsomorphism::*simpImage_const)(unsigned) const =
        &NIsomorphism::simpImage;
    regina::NPerm4 (NIsomorphism::*facetPerm_const)(unsigned) const =
        &NIsomorphism::facetPerm;

    regina::NTetFace iso_getItem(const NIsomorphism& iso,
            const regina::NTetFace& f) {
        return iso[f];
    }
}

void addNIsomorphism() {
    class_<NIsomorphism, bases<regina::ShareableObject>,
            std::auto_ptr<NIsomorphism>, boost::noncopyable>
            ("NIsomorphism", init<const NIsomorphism&>())
        .def("getSourceSimplices", &NIsomorphism::getSourceSimplices)
        .def("getSourceTetrahedra", &NIsomorphism::getSourceTetrahedra)
        .def("simpImage", simpImage_const)
        .def("tetImage", simpImage_const)
        .def("facetPerm", facetPerm_const)
        .def("facePerm", facetPerm_const)
        .def("__getitem__", iso_getItem)
        .def("isIdentity", &NIsomorphism::isIdentity)
        .def("apply", &NIsomorphism::apply,
            return_value_policy<manage_new_object>())
        .def("applyInPlace", &NIsomorphism::applyInPlace)
        .def("random", &NIsomorphism::random,
            return_value_policy<manage_new_object>())
        .staticmethod("random")
    ;
}

