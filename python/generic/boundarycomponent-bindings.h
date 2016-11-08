
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
#include "triangulation/generic.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::BoundaryComponent;

namespace {
    template <int dim>
    struct PyBoundaryComponentHelper {
        boost::python::list facets_list(BoundaryComponent<dim>& t) {
            boost::python::list ans;
            for (auto it = t.facets().begin();
                    it != t.facets().end(); ++it)
                ans.append(boost::python::ptr(*it));
            return ans;
        }
    };
}

template <int dim>
void addBoundaryComponent(const char* name) {
    // In higher dimensions:
    // - we do not store lower-dimensional faces;
    // - we do not recognise ideal or invalid vertices;
    // - we can still triangulate a real boundary component.

    class_<BoundaryComponent<dim>, std::auto_ptr<BoundaryComponent<dim>>,
            boost::noncopyable>(name, no_init)
        .def("index", &BoundaryComponent<dim>::index)
        .def("size", &BoundaryComponent<dim>::size)
        .def("facets", &PyBoundaryComponentHelper<dim>::facets_list)
        .def("facet", &BoundaryComponent<dim>::facet,
            return_value_policy<reference_existing_object>())
        .def("component", &BoundaryComponent<dim>::component,
            return_value_policy<reference_existing_object>())
        .def("triangulation", &BoundaryComponent<dim>::triangulation,
            return_value_policy<to_held_type<>>())
        .def("build", &BoundaryComponent<dim>::build,
            return_internal_reference<>())
        .def("isOrientable", &BoundaryComponent<dim>::isOrientable)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
}

