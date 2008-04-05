
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "subcomplex/nblockedsfstriple.h"
#include "subcomplex/nsatregion.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NBlockedSFSTriple;

void addNBlockedSFSTriple() {
    class_<NBlockedSFSTriple, bases<regina::NStandardTriangulation>,
            std::auto_ptr<NBlockedSFSTriple>, boost::noncopyable>
            ("NBlockedSFSTriple", no_init)
        .def("end", &NBlockedSFSTriple::end,
            return_internal_reference<>())
        .def("centre", &NBlockedSFSTriple::centre,
            return_internal_reference<>())
        .def("matchingReln", &NBlockedSFSTriple::matchingReln,
            return_internal_reference<>())
        .def("isBlockedSFSTriple", &NBlockedSFSTriple::isBlockedSFSTriple,
            return_value_policy<manage_new_object>())
        .staticmethod("isBlockedSFSTriple")
    ;

    implicitly_convertible<std::auto_ptr<NBlockedSFSTriple>,
        std::auto_ptr<regina::NStandardTriangulation> >();
}

