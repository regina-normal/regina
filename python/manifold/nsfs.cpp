
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

#include "algebra/nabeliangroup.h"
#include "manifold/nlensspace.h"
#include "manifold/nsfs.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NSFSFibre;
using regina::NSFSpace;

namespace {
    unsigned long (NSFSpace::*punctures_void)() const = &NSFSpace::punctures;
    unsigned long (NSFSpace::*punctures_bool)(bool) const =
        &NSFSpace::punctures;
    unsigned long (NSFSpace::*reflectors_void)() const = &NSFSpace::reflectors;
    unsigned long (NSFSpace::*reflectors_bool)(bool) const =
        &NSFSpace::reflectors;
    void (NSFSpace::*insertFibre_fibre)(const NSFSFibre&) =
        &NSFSpace::insertFibre;
    void (NSFSpace::*insertFibre_longs)(long, long) = &NSFSpace::insertFibre;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addHandle, addHandle, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addCrosscap, addCrosscap, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addPuncture, addPuncture, 0, 2);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addReflector, addReflector, 0, 2);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_reduce, reduce, 0, 1);
}

void addNSFSpace() {
    class_<NSFSFibre>("NSFSFibre")
        .def(init<long, long>())
        .def(init<const NSFSFibre&>())
        .def_readwrite("alpha", &NSFSFibre::alpha)
        .def_readwrite("beta", &NSFSFibre::beta)
        .def(self == self)
        .def(self < self)
        .def(self_ns::str(self))
    ;

    scope s = class_<NSFSpace, bases<regina::NManifold>,
            std::auto_ptr<NSFSpace> >("NSFSpace")
        .def(init<NSFSpace::classType, unsigned long,
            optional<unsigned long, unsigned long,
            unsigned long, unsigned long> >())
        .def(init<const NSFSpace&>())
        .def("baseClass", &NSFSpace::baseClass)
        .def("baseGenus", &NSFSpace::baseGenus)
        .def("baseOrientable", &NSFSpace::baseOrientable)
        .def("fibreReversing", &NSFSpace::fibreReversing)
        .def("fibreNegating", &NSFSpace::fibreNegating)
        .def("punctures", punctures_void)
        .def("punctures", punctures_bool)
        .def("reflectors", reflectors_void)
        .def("reflectors", reflectors_bool)
        .def("fibreCount", &NSFSpace::fibreCount)
        .def("fibre", &NSFSpace::fibre)
        .def("obstruction", &NSFSpace::obstruction)
        .def("addHandle", &NSFSpace::addHandle, OL_addHandle())
        .def("addCrosscap", &NSFSpace::addCrosscap, OL_addCrosscap())
        .def("addPuncture", &NSFSpace::addPuncture, OL_addPuncture())
        .def("addReflector", &NSFSpace::addReflector, OL_addReflector())
        .def("insertFibre", insertFibre_fibre)
        .def("insertFibre", insertFibre_longs)
        .def("complementAllFibres", &NSFSpace::complementAllFibres)
        .def("reduce", &NSFSpace::reduce, OL_reduce())
        .def("isLensSpace", &NSFSpace::isLensSpace,
            return_value_policy<manage_new_object>())
        .def(self < self)
    ;

    enum_<NSFSpace::classType>("classType")
        .value("o1", NSFSpace::o1)
        .value("o2", NSFSpace::o2)
        .value("n1", NSFSpace::n1)
        .value("n2", NSFSpace::n2)
        .value("n3", NSFSpace::n3)
        .value("n4", NSFSpace::n4)
        .value("bo1", NSFSpace::bo1)
        .value("bo2", NSFSpace::bo2)
        .value("bn1", NSFSpace::bn1)
        .value("bn2", NSFSpace::bn2)
        .value("bn3", NSFSpace::bn3)
        ;

    implicitly_convertible<std::auto_ptr<NSFSpace>,
        std::auto_ptr<regina::NManifold> >();
}

