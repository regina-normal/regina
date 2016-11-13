
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
#include "triangulation/dim4.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::Isomorphism;

namespace {
    int (Isomorphism<4>::*simpImage_const)(unsigned) const =
        &Isomorphism<4>::simpImage;
    regina::Perm<5> (Isomorphism<4>::*facetPerm_const)(unsigned) const =
        &Isomorphism<4>::facetPerm;
}

void addIsomorphism4() {
    class_<Isomorphism<4>, std::auto_ptr<Isomorphism<4>>, boost::noncopyable>
            ("Isomorphism4", init<const Isomorphism<4>&>())
        .def("size", &Isomorphism<4>::size)
        .def("simpImage", simpImage_const)
        .def("pentImage", simpImage_const)
        .def("facetPerm", facetPerm_const)
        .def("__getitem__", &Isomorphism<4>::operator[])
        .def("isIdentity", &Isomorphism<4>::isIdentity)
        .def("apply", &Isomorphism<4>::apply,
            return_value_policy<to_held_type<>>())
        .def("applyInPlace", &Isomorphism<4>::applyInPlace)
        .def("random", &Isomorphism<4>::random,
            return_value_policy<manage_new_object>())
        .def("identity", &Isomorphism<4>::identity,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("random")
        .staticmethod("identity")
    ;

    scope().attr("Dim4Isomorphism") = scope().attr("Isomorphism4");
}

