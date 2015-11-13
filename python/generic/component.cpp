
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
#include "generic/component.h"
#include "generic/simplex.h"

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
    };
}

template <int dim>
void addComponent(const char* name) {
    class_<Component<dim>, std::auto_ptr<Component<dim>>, boost::noncopyable>
            (name, no_init)
        .def("index", &Component<dim>::index)
        .def("size", &Component<dim>::size)
        .def("getNumberOfSimplices", &Component<dim>::getNumberOfSimplices)
        .def("simplices", &PyComponentHelper<dim>::simplices_list)
        .def("getSimplices", &PyComponentHelper<dim>::simplices_list)
        .def("simplex", &Component<dim>::simplex,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &Component<dim>::getSimplex,
            return_value_policy<reference_existing_object>())
        .def("isOrientable", &Component<dim>::isOrientable)
        .def("countBoundaryFacets", &Component<dim>::countBoundaryFacets)
        .def("getNumberOfBoundaryFacets",
            &Component<dim>::getNumberOfBoundaryFacets)
        .def("str", &Component<dim>::str)
        .def("toString", &Component<dim>::toString)
        .def("detail", &Component<dim>::detail)
        .def("toStringLong", &Component<dim>::toStringLong)
        .def("__str__", &Component<dim>::str)
    ;
}

void addComponent() {
    addComponent<4>("Component4");
    addComponent<5>("Component5");
    addComponent<6>("Component6");
    addComponent<7>("Component7");
    addComponent<8>("Component8");
    addComponent<9>("Component9");
    addComponent<10>("Component10");
    addComponent<11>("Component11");
    addComponent<12>("Component12");
    addComponent<13>("Component13");
    addComponent<14>("Component14");
    addComponent<15>("Component15");
}
