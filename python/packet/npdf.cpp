
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

#include "packet/npdf.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NPDF;

namespace {
    void (NPDF::*reset_empty)() = &NPDF::reset;
}

void addNPDF() {
    scope s = class_<NPDF, bases<regina::NPacket>,
            std::auto_ptr<NPDF>, boost::noncopyable>("NPDF", init<>())
        .def("size", &NPDF::size)
        .def("reset", reset_empty)
    ;

    s.attr("packetType") = NPDF::packetType;

    implicitly_convertible<std::auto_ptr<NPDF>,
        std::auto_ptr<regina::NPacket> >();
}

