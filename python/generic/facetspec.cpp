
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "regina-config.h" // for REGINA_HIGHDIM
#include "triangulation/facetspec.h"
#include "../helpers.h"
#include "../python/docstrings/triangulation/facetspec.h"

using pybind11::overload_cast;
using regina::FacetSpec;

template <int dim>
void addFacetSpec(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(FacetSpec)

    auto c = pybind11::class_<FacetSpec<dim>>(m, name, rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<ssize_t, int>(), rdoc::__init)
        .def(pybind11::init<const FacetSpec<dim>&>(), rdoc::__copy)
        .def_readwrite("simp", &FacetSpec<dim>::simp)
        .def_readwrite("facet", &FacetSpec<dim>::facet)
        .def("isBoundary", &FacetSpec<dim>::isBoundary, rdoc::isBoundary)
        .def("isBeforeStart", &FacetSpec<dim>::isBeforeStart,
            rdoc::isBeforeStart)
        .def("isPastEnd", &FacetSpec<dim>::isPastEnd, rdoc::isPastEnd)
        .def("setFirst", &FacetSpec<dim>::setFirst, rdoc::setFirst)
        .def("setBoundary", &FacetSpec<dim>::setBoundary, rdoc::setBoundary)
        .def("setBeforeStart", &FacetSpec<dim>::setBeforeStart,
            rdoc::setBeforeStart)
        .def("setPastEnd", &FacetSpec<dim>::setPastEnd, rdoc::setPastEnd)
        .def("inc", [](FacetSpec<dim>& s) {
            return s++;
        }, rdoc::__inc)
        .def("dec", [](FacetSpec<dim>& s) {
            return s--;
        }, rdoc::__dec)
    ;
    regina::python::add_output_ostream(c);
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);

    RDOC_SCOPE_END
}

void addFacetSpec(pybind11::module_& m) {
    addFacetSpec<2>(m, "FacetSpec2");
    addFacetSpec<3>(m, "FacetSpec3");
    addFacetSpec<4>(m, "FacetSpec4");
    addFacetSpec<5>(m, "FacetSpec5");
    addFacetSpec<6>(m, "FacetSpec6");
    addFacetSpec<7>(m, "FacetSpec7");
    addFacetSpec<8>(m, "FacetSpec8");
#ifdef REGINA_HIGHDIM
    addFacetSpec<9>(m, "FacetSpec9");
    addFacetSpec<10>(m, "FacetSpec10");
    addFacetSpec<11>(m, "FacetSpec11");
    addFacetSpec<12>(m, "FacetSpec12");
    addFacetSpec<13>(m, "FacetSpec13");
    addFacetSpec<14>(m, "FacetSpec14");
    addFacetSpec<15>(m, "FacetSpec15");
#endif
}

