
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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

#include "subcomplex/ntrisolidtorus.h"
#include "triangulation/ntetrahedron.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NTriSolidTorus;

void addNTriSolidTorus() {
    class_<NTriSolidTorus, bases<regina::NStandardTriangulation>,
            std::auto_ptr<NTriSolidTorus>, boost::noncopyable>
            ("NTriSolidTorus", no_init)
        .def("clone", &NTriSolidTorus::clone,
            return_value_policy<manage_new_object>())
        .def("getTetrahedron", &NTriSolidTorus::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("getVertexRoles", &NTriSolidTorus::getVertexRoles)
        .def("isAnnulusSelfIdentified",
            &NTriSolidTorus::isAnnulusSelfIdentified)
        .def("areAnnuliLinkedMajor", &NTriSolidTorus::areAnnuliLinkedMajor)
        .def("areAnnuliLinkedAxis", &NTriSolidTorus::areAnnuliLinkedAxis)
        .def("formsTriSolidTorus", &NTriSolidTorus::formsTriSolidTorus,
            return_value_policy<manage_new_object>())
        .staticmethod("formsTriSolidTorus")
    ;

    implicitly_convertible<std::auto_ptr<NTriSolidTorus>,
        std::auto_ptr<regina::NStandardTriangulation> >();
}

