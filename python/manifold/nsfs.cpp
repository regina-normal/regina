
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "algebra/nabeliangroup.h"
#include "manifold/nlensspace.h"
#include "manifold/nsfs.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NExceptionalFibre;
using regina::NSFS;

namespace {
    void (NSFS::*insertFibre_fibre)(const NExceptionalFibre&) =
        &NSFS::insertFibre;
    void (NSFS::*insertFibre_longs)(long, long) = &NSFS::insertFibre;
}

void addNSFS() {
    class_<NExceptionalFibre>("NExceptionalFibre")
        .def(init<long, long>())
        .def(init<const NExceptionalFibre&>())
        .def_readwrite("alpha", &NExceptionalFibre::alpha)
        .def_readwrite("beta", &NExceptionalFibre::beta)
        .def(self == self)
        .def(self < self)
        .def(self_ns::str(self))
    ;

    class_<NSFS, bases<regina::NManifold>, std::auto_ptr<NSFS> >("NSFS")
        .def(init<unsigned long, bool, optional<unsigned long> >())
        .def(init<const NSFS&>())
        .def("getOrbitGenus", &NSFS::getOrbitGenus)
        .def("isOrbitOrientable", &NSFS::isOrbitOrientable)
        .def("getOrbitPunctures", &NSFS::getOrbitPunctures)
        .def("getFibreCount", &NSFS::getFibreCount)
        .def("getFibre", &NSFS::getFibre)
        .def("insertFibre", insertFibre_fibre)
        .def("insertFibre", insertFibre_longs)
        .def("reduce", &NSFS::reduce)
        .def("isLensSpace", &NSFS::isLensSpace,
            return_value_policy<manage_new_object>())
    ;

    implicitly_convertible<std::auto_ptr<NSFS>,
        std::auto_ptr<regina::NManifold> >();
}

