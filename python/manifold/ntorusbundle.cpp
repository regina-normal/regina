
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

#include "algebra/nabeliangroup.h"
#include "manifold/ntorusbundle.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NMatrix2;
using regina::NTorusBundle;

void addNTorusBundle() {
    class_<NTorusBundle, bases<regina::NManifold>,
            std::auto_ptr<NTorusBundle> >("NTorusBundle")
        .def(init<const NMatrix2&>())
        .def(init<long, long, long, long>())
        .def(init<const NTorusBundle&>())
        .def("getMonodromy", &NTorusBundle::getMonodromy,
            return_internal_reference<>())
    ;

    implicitly_convertible<std::auto_ptr<NTorusBundle>,
        std::auto_ptr<regina::NManifold> >();
}

