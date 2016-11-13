
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

using namespace boost::python;
using regina::Component;

namespace {
    template <int dim>
    struct PyComponentHelper {
        boost::python::list simplices_list(Component<dim>& t) {
            boost::python::list ans;
            for (auto it = t.simplices().begin();
                    it != t.simplices().end(); ++it)
                ans.append(boost::python::ptr(*it));
            return ans;
        }

        boost::python::list bc_list(Component<dim>& t) {
            boost::python::list ans;
            for (auto s : t.boundaryComponents())
                ans.append(boost::python::ptr(s));
            return ans;
        }
    };
}

template <int dim>
void addComponent(const char* name) {
    class_<Component<dim>, std::auto_ptr<Component<dim>>, boost::noncopyable>
            (name, no_init)
        .def("index", &Component<dim>::index)
        .def("size", &Component<dim>::size)
        .def("countBoundaryComponents",
            &Component<dim>::countBoundaryComponents)
        .def("simplices", &PyComponentHelper<dim>::simplices_list)
        .def("simplex", &Component<dim>::simplex,
            return_value_policy<reference_existing_object>())
        .def("boundaryComponents", &PyComponentHelper<dim>::bc_list)
        .def("boundaryComponent", &Component<dim>::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("isValid", &Component<dim>::isValid)
        .def("isOrientable", &Component<dim>::isOrientable)
        .def("hasBoundaryFacets", &Component<dim>::hasBoundaryFacets)
        .def("countBoundaryFacets", &Component<dim>::countBoundaryFacets)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
}

