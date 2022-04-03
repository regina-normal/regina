
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
#include "census/gluingperms.h"
#include "triangulation/facetpairing.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::FacetPairing;
using regina::FacetSpec;
using regina::GluingPerms;
using regina::Perm;

template <int dim>
void addGluingPerms(pybind11::module_& m, const char* name) {
    using Index = typename GluingPerms<dim>::Index;

    auto c = pybind11::class_<GluingPerms<dim>>(m, name)
        .def(pybind11::init<const FacetPairing<dim>&>())
        .def(pybind11::init<const GluingPerms<dim>&>())
        .def("swap", &GluingPerms<dim>::swap)
        .def("size", &GluingPerms<dim>::size)
        .def("pairing", &GluingPerms<dim>::pairing,
            pybind11::return_value_policy::reference_internal)
        .def("facetPairing", &GluingPerms<dim>::pairing, // deprecated
            pybind11::return_value_policy::reference_internal)
        .def("perm", overload_cast<const FacetSpec<dim>&>(
            &GluingPerms<dim>::perm, pybind11::const_))
        .def("perm", overload_cast<size_t, int>(
            &GluingPerms<dim>::perm, pybind11::const_))
        .def("gluingPerm", overload_cast<const FacetSpec<dim>&>( // deprecated
            &GluingPerms<dim>::perm, pybind11::const_))
        .def("gluingPerm", overload_cast<size_t, int>( // deprecated
            &GluingPerms<dim>::perm, pybind11::const_))
        .def("permIndex",
            overload_cast<const FacetSpec<dim>&>(
            &GluingPerms<dim>::permIndex, pybind11::const_))
        .def("permIndex",
            overload_cast<size_t, int>(
            &GluingPerms<dim>::permIndex, pybind11::const_))
        .def("setPermIndex", [](GluingPerms<dim>& g,
                const FacetSpec<dim>& s, Index val) {
            g.permIndex(s) = val;
        })
        .def("setPermIndex", [](GluingPerms<dim>& g,
                size_t s, int f, Index val) {
            g.permIndex(s, f) = val;
        })
        .def("triangulate", &GluingPerms<dim>::triangulate)
        .def("data", &GluingPerms<dim>::data)
        .def("gluingToIndex",
            overload_cast<const FacetSpec<dim>&, const Perm<dim+1>&>(
            &GluingPerms<dim>::gluingToIndex, pybind11::const_))
        .def("gluingToIndex",
            overload_cast<size_t, int, const Perm<dim+1>&>(
            &GluingPerms<dim>::gluingToIndex, pybind11::const_))
        .def("indexToGluing",
            overload_cast<const FacetSpec<dim>&, Index>(
            &GluingPerms<dim>::indexToGluing, pybind11::const_))
        .def("indexToGluing",
            overload_cast<size_t, int, Index>(
            &GluingPerms<dim>::indexToGluing, pybind11::const_))
        .def_static("fromData", &GluingPerms<dim>::fromData)
        ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.def("swap",
        (void(*)(GluingPerms<dim>&, GluingPerms<dim>&))(regina::swap));
}

