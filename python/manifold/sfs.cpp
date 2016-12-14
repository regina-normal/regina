
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
#include "algebra/abeliangroup.h"
#include "manifold/lensspace.h"
#include "manifold/sfs.h"
#include "../helpers.h"

using namespace boost::python;
using regina::SFSFibre;
using regina::SFSpace;

namespace {
    unsigned long (SFSpace::*punctures_void)() const = &SFSpace::punctures;
    unsigned long (SFSpace::*punctures_bool)(bool) const =
        &SFSpace::punctures;
    unsigned long (SFSpace::*reflectors_void)() const = &SFSpace::reflectors;
    unsigned long (SFSpace::*reflectors_bool)(bool) const =
        &SFSpace::reflectors;
    void (SFSpace::*insertFibre_fibre)(const SFSFibre&) =
        &SFSpace::insertFibre;
    void (SFSpace::*insertFibre_longs)(long, long) = &SFSpace::insertFibre;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addHandle, addHandle, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addCrosscap, addCrosscap, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addPuncture, addPuncture, 0, 2);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addReflector, addReflector, 0, 2);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_reduce, reduce, 0, 1);
}

void addSFSpace() {
    class_<SFSFibre>("SFSFibre")
        .def(init<long, long>())
        .def(init<const SFSFibre&>())
        .def_readwrite("alpha", &SFSFibre::alpha)
        .def_readwrite("beta", &SFSFibre::beta)
        .def(self < self)
        .def(self_ns::str(self))
        .def(regina::python::add_eq_operators())
    ;

    class_<SFSpace, bases<regina::Manifold>,
            std::auto_ptr<SFSpace> >("SFSpace")
        .def(init<SFSpace::classType, unsigned long,
            optional<unsigned long, unsigned long,
            unsigned long, unsigned long> >())
        .def(init<const SFSpace&>())
        .def("baseClass", &SFSpace::baseClass)
        .def("baseGenus", &SFSpace::baseGenus)
        .def("baseOrientable", &SFSpace::baseOrientable)
        .def("fibreReversing", &SFSpace::fibreReversing)
        .def("fibreNegating", &SFSpace::fibreNegating)
        .def("punctures", punctures_void)
        .def("punctures", punctures_bool)
        .def("reflectors", reflectors_void)
        .def("reflectors", reflectors_bool)
        .def("fibreCount", &SFSpace::fibreCount)
        .def("fibre", &SFSpace::fibre)
        .def("obstruction", &SFSpace::obstruction)
        .def("addHandle", &SFSpace::addHandle, OL_addHandle())
        .def("addCrosscap", &SFSpace::addCrosscap, OL_addCrosscap())
        .def("addPuncture", &SFSpace::addPuncture, OL_addPuncture())
        .def("addReflector", &SFSpace::addReflector, OL_addReflector())
        .def("insertFibre", insertFibre_fibre)
        .def("insertFibre", insertFibre_longs)
        .def("reflect", &SFSpace::reflect)
        .def("complementAllFibres", &SFSpace::complementAllFibres)
        .def("reduce", &SFSpace::reduce, OL_reduce())
        .def("isLensSpace", &SFSpace::isLensSpace,
            return_value_policy<manage_new_object>())
        .def(self < self)
        .def(regina::python::add_eq_operators())
    ;

    enum_<SFSpace::classType>("classType")
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
        ;

    implicitly_convertible<std::auto_ptr<SFSpace>,
        std::auto_ptr<regina::Manifold> >();

    scope().attr("NSFSFibre") = scope().attr("SFSFibre");
    scope().attr("NSFSpace") = scope().attr("SFSpace");
}

