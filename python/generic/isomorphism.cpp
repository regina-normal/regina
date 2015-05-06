
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

#include <boost/python.hpp>
#include "generic/isomorphism.h"
#include "generic/triangulation.h"

using namespace boost::python;
using regina::Isomorphism;

namespace {
    template <int dim>
    struct PyIsoHelper {
        typedef int (Isomorphism<dim>::*simpImage_const_type)(unsigned) const;

        typedef regina::NPerm<dim+1> (Isomorphism<dim>::*facetPerm_const_type)(
            unsigned) const;

        static regina::NFacetSpec<dim> iso_getItem(const Isomorphism<dim>& iso,
                const regina::NFacetSpec<dim>& f) {
            return iso[f];
        }
    };
}

template <int dim>
void addIsomorphism(const char* name) {
    class_<Isomorphism<dim>, std::auto_ptr<Isomorphism<dim>>,
            boost::noncopyable>(name, init<const Isomorphism<dim>&>())
        .def("size", &Isomorphism<dim>::size)
        .def("getSourceSimplices", &Isomorphism<dim>::getSourceSimplices)
        .def("simpImage", typename PyIsoHelper<dim>::simpImage_const_type(
            &Isomorphism<dim>::simpImage))
        .def("facetPerm", typename PyIsoHelper<dim>::facetPerm_const_type(
            &Isomorphism<dim>::facetPerm))
        .def("__getitem__", PyIsoHelper<dim>::iso_getItem)
        .def("isIdentity", &Isomorphism<dim>::isIdentity)
        .def("apply", &Isomorphism<dim>::apply,
            return_value_policy<manage_new_object>())
        .def("applyInPlace", &Isomorphism<dim>::applyInPlace)
        .def("random", &Isomorphism<dim>::random,
            return_value_policy<manage_new_object>())
        .def("identity", &Isomorphism<dim>::identity,
            return_value_policy<manage_new_object>())
        .def("str", &Isomorphism<dim>::str)
        .def("toString", &Isomorphism<dim>::toString)
        .def("detail", &Isomorphism<dim>::detail)
        .def("toStringLong", &Isomorphism<dim>::toStringLong)
        .def("__str__", &Isomorphism<dim>::str)
        .staticmethod("random")
        .staticmethod("identity")
    ;
}

void addIsomorphism() {
    addIsomorphism<5>("Isomorphism5");
    addIsomorphism<8>("Isomorphism8");
    addIsomorphism<15>("Isomorphism15");
}

