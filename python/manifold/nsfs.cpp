
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
    void (NSFSpace::*insertFibre_fibre)(const NSFSFibre&) =
        &NSFSpace::insertFibre;
    void (NSFSpace::*insertFibre_longs)(long, long) = &NSFSpace::insertFibre;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addHandle, addHandle, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addCrosscap, addCrosscap, 0, 1);
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
            optional<unsigned long, unsigned long> >())
        .def(init<const NSFSpace&>())
        .def("getBaseClass", &NSFSpace::getBaseClass)
        .def("getBaseGenus", &NSFSpace::getBaseGenus)
        .def("isBaseOrientable", &NSFSpace::isBaseOrientable)
        .def("getBasePunctures", &NSFSpace::getBasePunctures)
        .def("getBaseReflectors", &NSFSpace::getBaseReflectors)
        .def("getFibreCount", &NSFSpace::getFibreCount)
        .def("getFibre", &NSFSpace::getFibre)
        .def("getObstruction", &NSFSpace::getObstruction)
        .def("addHandle", &NSFSpace::addHandle, OL_addHandle())
        .def("addCrosscap", &NSFSpace::addHandle, OL_addCrosscap())
        .def("addPuncture", &NSFSpace::addPuncture)
        .def("addReflector", &NSFSpace::addReflector)
        .def("insertFibre", insertFibre_fibre)
        .def("insertFibre", insertFibre_longs)
        .def("reduce", &NSFSpace::reduce)
        .def("isLensSpace", &NSFSpace::isLensSpace,
            return_value_policy<manage_new_object>())
    ;

    enum_<NSFSpace::classType>("classType")
        .value("o1", NSFSpace::o1)
        .value("o2", NSFSpace::o2)
        .value("n1", NSFSpace::n1)
        .value("n2", NSFSpace::n2)
        .value("n3", NSFSpace::n3)
        .value("n4", NSFSpace::n4)
        ;

    implicitly_convertible<std::auto_ptr<NSFSpace>,
        std::auto_ptr<regina::NManifold> >();
}

