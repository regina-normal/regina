
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

#include "triangulation/dim2.h"
#include "../helpers.h"
#include "../safeheldtype.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Isomorphism;

namespace {
    int (Isomorphism<2>::*simpImage_const)(unsigned) const =
        &Isomorphism<2>::simpImage;
    regina::Perm<3> (Isomorphism<2>::*facetPerm_const)(unsigned) const =
        &Isomorphism<2>::facetPerm;
}

void addIsomorphism2() {
    class_<Isomorphism<2>, std::auto_ptr<Isomorphism<2>>, boost::noncopyable>
            ("Isomorphism2", init<const Isomorphism<2>&>())
        .def("size", &Isomorphism<2>::size)
        .def("simpImage", simpImage_const)
        .def("triImage", simpImage_const)
        .def("facetPerm", facetPerm_const)
        .def("edgePerm", facetPerm_const)
        .def("__getitem__", &Isomorphism<2>::operator[])
        .def("isIdentity", &Isomorphism<2>::isIdentity)
        .def("apply", &Isomorphism<2>::apply,
            return_value_policy<to_held_type<> >())
        .def("applyInPlace", &Isomorphism<2>::applyInPlace)
        .def("random", &Isomorphism<2>::random,
            return_value_policy<manage_new_object>())
        .def("identity", &Isomorphism<2>::identity,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("random")
        .staticmethod("identity")
    ;

    scope().attr("Dim2Isomorphism") = scope().attr("Isomorphism2");
}

