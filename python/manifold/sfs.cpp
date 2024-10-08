
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

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "algebra/abeliangroup.h"
#include "manifold/lensspace.h"
#include "manifold/sfs.h"
#include "../helpers.h"
#include "../docstrings/manifold/sfs.h"

using pybind11::overload_cast;
using regina::SFSFibre;
using regina::SFSpace;

void addSFSpace(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(SFSFibre)

    auto f = pybind11::class_<SFSFibre>(m, "SFSFibre", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<long, long>(), rdoc::__init)
        .def(pybind11::init<const SFSFibre&>(), rdoc::__copy)
        .def_readwrite("alpha", &SFSFibre::alpha)
        .def_readwrite("beta", &SFSFibre::beta)
    ;
    regina::python::add_output_ostream(f);
    regina::python::add_eq_operators(f, rdoc::__eq);
    regina::python::add_cmp_operators(f, rdoc::__cmp);

    RDOC_SCOPE_SWITCH(SFSpace)

    auto s = pybind11::class_<SFSpace, regina::Manifold>(m, "SFSpace",
            rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<SFSpace::ClassType, unsigned long,
            unsigned long, unsigned long,
            unsigned long, unsigned long>(),
            pybind11::arg(), pybind11::arg(),
            pybind11::arg("punctures") = 0,
            pybind11::arg("puncturesTwisted") = 0,
            pybind11::arg("reflectors") = 0,
            pybind11::arg("reflectorsTwisted") = 0,
            rdoc::__init)
        .def(pybind11::init<const SFSpace&>(), rdoc::__copy)
        .def("swap", &SFSpace::swap, rdoc::swap)
        .def("baseClass", &SFSpace::baseClass, rdoc::baseClass)
        .def("baseGenus", &SFSpace::baseGenus, rdoc::baseGenus)
        .def("baseOrientable", &SFSpace::baseOrientable, rdoc::baseOrientable)
        .def("fibreReversing", &SFSpace::fibreReversing, rdoc::fibreReversing)
        .def("fibreNegating", &SFSpace::fibreNegating, rdoc::fibreNegating)
        .def("punctures",
            overload_cast<>(&SFSpace::punctures, pybind11::const_),
            rdoc::punctures)
        .def("punctures",
            overload_cast<bool>(&SFSpace::punctures, pybind11::const_),
            rdoc::punctures_2)
        .def("reflectors",
            overload_cast<>(&SFSpace::reflectors, pybind11::const_),
            rdoc::reflectors)
        .def("reflectors",
            overload_cast<bool>(&SFSpace::reflectors, pybind11::const_),
            rdoc::reflectors_2)
        .def("fibreCount", &SFSpace::fibreCount, rdoc::fibreCount)
        .def("fibre", &SFSpace::fibre, rdoc::fibre)
        .def("obstruction", &SFSpace::obstruction, rdoc::obstruction)
        .def("addHandle", &SFSpace::addHandle,
            pybind11::arg("fibreReversing") = false, rdoc::addHandle)
        .def("addCrosscap", &SFSpace::addCrosscap,
            pybind11::arg("fibreReversing") = false, rdoc::addCrosscap)
        .def("addPuncture", &SFSpace::addPuncture,
            pybind11::arg("twisted") = false,
            pybind11::arg("nPunctures") = 1, rdoc::addPuncture)
        .def("addReflector", &SFSpace::addReflector,
            pybind11::arg("twisted") = false,
            pybind11::arg("nReflectors") = 1, rdoc::addReflector)
        .def("insertFibre",
            overload_cast<const SFSFibre&>(&SFSpace::insertFibre),
            rdoc::insertFibre)
        .def("insertFibre",
            overload_cast<long, long>(&SFSpace::insertFibre),
            rdoc::insertFibre_2)
        .def("reflect", &SFSpace::reflect, rdoc::reflect)
        .def("complementAllFibres", &SFSpace::complementAllFibres,
            rdoc::complementAllFibres)
        .def("reduce", &SFSpace::reduce,
            pybind11::arg("mayReflect") = true, rdoc::reduce)
        .def("isLensSpace", &SFSpace::isLensSpace, rdoc::isLensSpace)
    ;
    regina::python::add_eq_operators(s, rdoc::__eq);
    // Do not bind comparison operators, since these are already inherited
    // from Manifold and we do not want to hide those more general versions.
    regina::python::add_output(s);

    regina::python::add_global_swap<SFSpace>(m, rdoc::global_swap);

    RDOC_SCOPE_INNER_BEGIN(ClassType)

    pybind11::enum_<SFSpace::ClassType>(s, "ClassType", rdoc_inner_scope)
        .value("o1", SFSpace::o1, rdoc_inner::o1)
        .value("o2", SFSpace::o2, rdoc_inner::o2)
        .value("n1", SFSpace::n1, rdoc_inner::n1)
        .value("n2", SFSpace::n2, rdoc_inner::n2)
        .value("n3", SFSpace::n3, rdoc_inner::n3)
        .value("n4", SFSpace::n4, rdoc_inner::n4)
        .value("bo1", SFSpace::bo1, rdoc_inner::bo1)
        .value("bo2", SFSpace::bo2, rdoc_inner::bo2)
        .value("bn1", SFSpace::bn1, rdoc_inner::bn1)
        .value("bn2", SFSpace::bn2, rdoc_inner::bn2)
        .value("bn3", SFSpace::bn3, rdoc_inner::bn3)
        .export_values()
        ;

    RDOC_SCOPE_INNER_END
    RDOC_SCOPE_END
}

