
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

#include "subcomplex/nlayering.h"
#include "triangulation/ntetrahedron.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NLayering;
using regina::NPerm;
using regina::NTetrahedron;

void addNLayering() {
    class_<NLayering, boost::noncopyable, std::auto_ptr<NLayering> >
            ("NLayering", init<NTetrahedron*, NPerm, NTetrahedron*, NPerm>())
        .def("getSize", &NLayering::getSize)
        .def("getOldBoundaryTet", &NLayering::getOldBoundaryTet,
            return_value_policy<reference_existing_object>())
        .def("getOldBoundaryRoles", &NLayering::getOldBoundaryRoles)
        .def("getNewBoundaryTet", &NLayering::getNewBoundaryTet,
            return_value_policy<reference_existing_object>())
        .def("getNewBoundaryRoles", &NLayering::getNewBoundaryRoles)
        .def("boundaryReln", &NLayering::boundaryReln,
            return_internal_reference<>())
        .def("extendOne", &NLayering::extendOne)
        .def("extend", &NLayering::extend)
        .def("matchesTop", &NLayering::matchesTop)
    ;
}

