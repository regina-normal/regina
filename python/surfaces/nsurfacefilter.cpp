
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

#include "surfaces/nnormalsurface.h"
#include "surfaces/nsurfacefilter.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NSurfaceFilter;

void addNSurfaceFilter() {
    scope s = class_<NSurfaceFilter, bases<regina::NPacket>,
            std::auto_ptr<NSurfaceFilter>, boost::noncopyable>
            ("NSurfaceFilter")
        .def(init<const NSurfaceFilter&>())
        .def("accept", &NSurfaceFilter::accept)
        .def("getFilterID", &NSurfaceFilter::getFilterID)
        .def("getFilterName", &NSurfaceFilter::getFilterName)
    ;

    s.attr("packetType") = NSurfaceFilter::packetType;
    s.attr("filterID") = NSurfaceFilter::filterID;

    implicitly_convertible<std::auto_ptr<NSurfaceFilter>,
        std::auto_ptr<regina::NPacket> >();
}

