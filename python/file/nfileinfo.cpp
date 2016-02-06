
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

#include <boost/python.hpp>
#include "file/nfileinfo.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NFileInfo;

void addNFileInfo() {
    scope s = class_<NFileInfo, std::auto_ptr<NFileInfo>, boost::noncopyable>
            ("NFileInfo", no_init)
        .def("pathname", &NFileInfo::pathname,
            return_value_policy<return_by_value>())
        .def("getPathname", &NFileInfo::pathname,
            return_value_policy<return_by_value>())
        .def("type", &NFileInfo::type)
        .def("getType", &NFileInfo::type)
        .def("typeDescription", &NFileInfo::typeDescription,
            return_value_policy<return_by_value>())
        .def("getTypeDescription", &NFileInfo::typeDescription,
            return_value_policy<return_by_value>())
        .def("engine", &NFileInfo::engine,
            return_value_policy<return_by_value>())
        .def("getEngine", &NFileInfo::engine,
            return_value_policy<return_by_value>())
        .def("isCompressed", &NFileInfo::isCompressed)
        .def("isInvalid", &NFileInfo::isInvalid)
        .def("identify", &NFileInfo::identify,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("identify")
    ;

    // Apparently there is no way in python to make a module attribute
    // read-only.
    s.attr("TYPE_XML") = NFileInfo::TYPE_XML;
}

