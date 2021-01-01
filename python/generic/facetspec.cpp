
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "../pybind11/pybind11.h"
#include "../pybind11/operators.h"
#include "triangulation/facetspec.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::FacetSpec;

template <int dim>
void addFacetSpec(pybind11::module_& m, const char* name) {
    auto c = pybind11::class_<FacetSpec<dim>>(m, name)
        .def(pybind11::init<>())
        .def(pybind11::init<int, int>())
        .def(pybind11::init<const FacetSpec<dim>&>())
        .def_readwrite("simp", &FacetSpec<dim>::simp)
        .def_readwrite("facet", &FacetSpec<dim>::facet)
        .def("isBoundary", &FacetSpec<dim>::isBoundary)
        .def("isBeforeStart", &FacetSpec<dim>::isBeforeStart)
        .def("isPastEnd", &FacetSpec<dim>::isPastEnd)
        .def("setFirst", &FacetSpec<dim>::setFirst)
        .def("setBoundary", &FacetSpec<dim>::setBoundary)
        .def("setBeforeStart", &FacetSpec<dim>::setBeforeStart)
        .def("setPastEnd", &FacetSpec<dim>::setPastEnd)
        .def("inc", [](FacetSpec<dim>& s) {
            return s++;
        })
        .def("dec", [](FacetSpec<dim>& s) {
            return s--;
        })
        .def(pybind11::self < pybind11::self)
        .def(pybind11::self <= pybind11::self)
    ;
    regina::python::add_output_ostream(c, true /* __repr__ */);
    regina::python::add_eq_operators(c);
}

void addFacetSpec(pybind11::module_& m) {
    addFacetSpec<2>(m, "FacetSpec2");
    addFacetSpec<3>(m, "FacetSpec3");
    addFacetSpec<4>(m, "FacetSpec4");
#ifndef REGINA_LOWDIMONLY
    addFacetSpec<5>(m, "FacetSpec5");
    addFacetSpec<6>(m, "FacetSpec6");
    addFacetSpec<7>(m, "FacetSpec7");
    addFacetSpec<8>(m, "FacetSpec8");
    addFacetSpec<9>(m, "FacetSpec9");
    addFacetSpec<10>(m, "FacetSpec10");
    addFacetSpec<11>(m, "FacetSpec11");
    addFacetSpec<12>(m, "FacetSpec12");
    addFacetSpec<13>(m, "FacetSpec13");
    addFacetSpec<14>(m, "FacetSpec14");
    addFacetSpec<15>(m, "FacetSpec15");
#endif

    m.attr("Dim2TriangleEdge") = m.attr("FacetSpec2");
    m.attr("NTetFace") = m.attr("FacetSpec3");
    m.attr("Dim4PentFacet") = m.attr("FacetSpec4");
}

