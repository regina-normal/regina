
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

#include "manifold/ngraphloop.h"
#include "manifold/nsfs.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NGraphLoop;
using regina::NMatrix2;
using regina::NSFSpace;

void addNGraphLoop() {
    class_<NGraphLoop, bases<regina::NManifold>,
            std::auto_ptr<NGraphLoop>, boost::noncopyable>
            ("NGraphLoop", init<NSFSpace*, long, long, long, long>())
        .def(init<NSFSpace*, const NMatrix2&>())
        .def("sfs", &NGraphLoop::sfs,
            return_internal_reference<>())
        .def("matchingReln", &NGraphLoop::matchingReln,
            return_internal_reference<>())
        .def(self < self)
    ;

    implicitly_convertible<std::auto_ptr<NGraphLoop>,
        std::auto_ptr<regina::NManifold> >();
}

