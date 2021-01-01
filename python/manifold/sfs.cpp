
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
#include "algebra/abeliangroup.h"
#include "manifold/lensspace.h"
#include "manifold/sfs.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::SFSFibre;
using regina::SFSpace;

void addSFSpace(pybind11::module_& m) {
    auto f = pybind11::class_<SFSFibre>(m, "SFSFibre")
        .def(pybind11::init<>())
        .def(pybind11::init<long, long>())
        .def(pybind11::init<const SFSFibre&>())
        .def_readwrite("alpha", &SFSFibre::alpha)
        .def_readwrite("beta", &SFSFibre::beta)
        .def(pybind11::self < pybind11::self)
    ;
    regina::python::add_output_ostream(f);
    regina::python::add_eq_operators(f);

    auto s = pybind11::class_<SFSpace, regina::Manifold>(m, "SFSpace")
        .def(pybind11::init<>())
        .def(pybind11::init<SFSpace::classType, unsigned long,
            unsigned long, unsigned long,
            unsigned long, unsigned long>(),
            pybind11::arg(), pybind11::arg(),
            pybind11::arg("punctures") = 0,
            pybind11::arg("puncturesTwisted") = 0,
            pybind11::arg("reflectors") = 0,
            pybind11::arg("reflectorsTwisted") = 0)
        .def(pybind11::init<const SFSpace&>())
        .def("baseClass", &SFSpace::baseClass)
        .def("baseGenus", &SFSpace::baseGenus)
        .def("baseOrientable", &SFSpace::baseOrientable)
        .def("fibreReversing", &SFSpace::fibreReversing)
        .def("fibreNegating", &SFSpace::fibreNegating)
        .def("punctures",
            overload_cast<>(&SFSpace::punctures, pybind11::const_))
        .def("punctures",
            overload_cast<bool>(&SFSpace::punctures, pybind11::const_))
        .def("reflectors",
            overload_cast<>(&SFSpace::reflectors, pybind11::const_))
        .def("reflectors",
            overload_cast<bool>(&SFSpace::reflectors, pybind11::const_))
        .def("fibreCount", &SFSpace::fibreCount)
        .def("fibre", &SFSpace::fibre)
        .def("obstruction", &SFSpace::obstruction)
        .def("addHandle", &SFSpace::addHandle,
            pybind11::arg("fibreReversing") = false)
        .def("addCrosscap", &SFSpace::addCrosscap,
            pybind11::arg("fibreReversing") = false)
        .def("addPuncture", &SFSpace::addPuncture,
            pybind11::arg("twisted") = false,
            pybind11::arg("nPunctures") = 1)
        .def("addReflector", &SFSpace::addReflector,
            pybind11::arg("twisted") = false,
            pybind11::arg("nReflectors") = 1)
        .def("insertFibre",
            overload_cast<const SFSFibre&>(&SFSpace::insertFibre))
        .def("insertFibre",
            overload_cast<long, long>(&SFSpace::insertFibre))
        .def("reflect", &SFSpace::reflect)
        .def("complementAllFibres", &SFSpace::complementAllFibres)
        .def("reduce", &SFSpace::reduce,
            pybind11::arg("mayReflect") = true)
        .def("isLensSpace", &SFSpace::isLensSpace)
    ;
    // The SFSpace subclass defines its own equality tests, so we
    // should not just inherit the compare-by-pointer test from Manifold.
    regina::python::add_eq_operators(s);

    pybind11::enum_<SFSpace::classType>(s, "classType")
        .value("o1", SFSpace::o1)
        .value("o2", SFSpace::o2)
        .value("n1", SFSpace::n1)
        .value("n2", SFSpace::n2)
        .value("n3", SFSpace::n3)
        .value("n4", SFSpace::n4)
        .value("bo1", SFSpace::bo1)
        .value("bo2", SFSpace::bo2)
        .value("bn1", SFSpace::bn1)
        .value("bn2", SFSpace::bn2)
        .value("bn3", SFSpace::bn3)
        .export_values()
        ;

    m.attr("NSFSFibre") = m.attr("SFSFibre");
    m.attr("NSFSpace") = m.attr("SFSpace");
}

