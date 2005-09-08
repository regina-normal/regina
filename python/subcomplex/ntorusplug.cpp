
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

#include "subcomplex/ntorusplug.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NPerm;
using regina::NTetrahedron;
using regina::NTorusPlug;

namespace {
    NTorusPlug* (*isPlugged_public)(NTetrahedron*, NPerm,
        NTetrahedron*, NPerm) = &NTorusPlug::isPlugged;
}

void addNTorusPlug() {
    class_<NTorusPlug, bases<regina::NStandardTriangulation>,
            std::auto_ptr<NTorusPlug>, boost::noncopyable>
            ("NTorusPlug", no_init)
        .def("isPlugged", isPlugged_public,
            return_value_policy<manage_new_object>())
        .staticmethod("isPlugged")
    ;

    implicitly_convertible<std::auto_ptr<NTorusPlug>,
        std::auto_ptr<regina::NStandardTriangulation> >();
}

