
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
#include "triangulation/facetspec.h"
#include "../helpers.h"

using namespace boost::python;
using regina::FacetSpec;

template <int dim>
void addFacetSpec(const char* name) {
    class_<FacetSpec<dim>>(name)
        .def(init<int, int>())
        .def(init<const FacetSpec<dim>&>())
        .def_readwrite("simp", &FacetSpec<dim>::simp)
        .def_readwrite("facet", &FacetSpec<dim>::facet)
        .def("isBoundary", &FacetSpec<dim>::isBoundary)
        .def("isBeforeStart", &FacetSpec<dim>::isBeforeStart)
        .def("isPastEnd", &FacetSpec<dim>::isPastEnd)
        .def("setFirst", &FacetSpec<dim>::setFirst)
        .def("setBoundary", &FacetSpec<dim>::setBoundary)
        .def("setBeforeStart", &FacetSpec<dim>::setBeforeStart)
        .def("setPastEnd", &FacetSpec<dim>::setPastEnd)
        .def("inc", static_cast<FacetSpec<dim> (FacetSpec<dim>::*)(int)>(
            &FacetSpec<dim>::operator++))
        .def("dec", static_cast<FacetSpec<dim> (FacetSpec<dim>::*)(int)>(
            &FacetSpec<dim>::operator--))
        .def(self < self)
        .def(self <= self)
        .def(regina::python::add_eq_operators())
    ;
}

void addFacetSpec() {
    addFacetSpec<2>("FacetSpec2");
    addFacetSpec<3>("FacetSpec3");
    addFacetSpec<4>("FacetSpec4");
    addFacetSpec<5>("FacetSpec5");
    addFacetSpec<6>("FacetSpec6");
    addFacetSpec<7>("FacetSpec7");
    addFacetSpec<8>("FacetSpec8");
    addFacetSpec<9>("FacetSpec9");
    addFacetSpec<10>("FacetSpec10");
    addFacetSpec<11>("FacetSpec11");
    addFacetSpec<12>("FacetSpec12");
    addFacetSpec<13>("FacetSpec13");
    addFacetSpec<14>("FacetSpec14");
    addFacetSpec<15>("FacetSpec15");

    scope().attr("Dim2TriangleEdge") = scope().attr("FacetSpec2");
    scope().attr("NTetFace") = scope().attr("FacetSpec3");
    scope().attr("Dim4PentFacet") = scope().attr("FacetSpec4");
}

