
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

#include "file/nfileinfo.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NFileInfo;

void addNFileInfo() {
    scope s = class_<NFileInfo, bases<regina::ShareableObject>,
            std::auto_ptr<NFileInfo>, boost::noncopyable>
            ("NFileInfo", no_init)
        .def("getPathname", &NFileInfo::getPathname,
            return_value_policy<return_by_value>())
        .def("getType", &NFileInfo::getType)
        .def("getTypeDescription", &NFileInfo::getTypeDescription,
            return_value_policy<return_by_value>())
        .def("getEngine", &NFileInfo::getEngine,
            return_value_policy<return_by_value>())
        .def("isCompressed", &NFileInfo::isCompressed)
        .def("isInvalid", &NFileInfo::isInvalid)
        .def("NFileInfo_identify", &NFileInfo::identify,
            return_value_policy<manage_new_object>())
        .staticmethod("NFileInfo_identify")
    ;

    // Apparently there is no way in python to make a module attribute
    // read-only.
    s.attr("TYPE_BINARY") = NFileInfo::TYPE_BINARY;
    s.attr("TYPE_XML") = NFileInfo::TYPE_XML;
}

