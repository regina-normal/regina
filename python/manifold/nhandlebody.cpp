
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

#include "manifold/nhandlebody.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NHandlebody;

void addNHandlebody() {
    class_<NHandlebody, bases<regina::NManifold>,
            std::auto_ptr<NHandlebody>, boost::noncopyable>
            ("NHandlebody", init<unsigned long, bool>())
        .def(init<const NHandlebody&>())
        .def("getHandles", &NHandlebody::getHandles)
        .def("isOrientable", &NHandlebody::isOrientable)
        .def(self == self)
    ;

    implicitly_convertible<std::auto_ptr<NHandlebody>,
        std::auto_ptr<regina::NManifold> >();
}

