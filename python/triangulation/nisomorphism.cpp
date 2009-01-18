
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

#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NIsomorphism;

namespace {
    int (NIsomorphism::*tetImage_const)(unsigned) const =
        &NIsomorphism::tetImage;

    regina::NTetFace iso_getItem(const NIsomorphism& iso,
            const regina::NTetFace& f) {
        return iso[f];
    }
}

void addNIsomorphism() {
    class_<NIsomorphism, bases<regina::ShareableObject>,
            std::auto_ptr<NIsomorphism>, boost::noncopyable>
            ("NIsomorphism", no_init)
        .def("getSourceTetrahedra", &NIsomorphism::getSourceTetrahedra)
        .def("tetImage", tetImage_const)
        .def("facePerm", &NIsomorphism::facePerm)
        .def("__getitem__", iso_getItem)
        .def("isIdentity", &NIsomorphism::isIdentity)
        .def("apply", &NIsomorphism::apply,
            return_value_policy<manage_new_object>())
        .def("random", &NIsomorphism::random,
            return_value_policy<manage_new_object>())
        .staticmethod("random")
    ;
}

