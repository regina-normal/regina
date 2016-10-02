
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
#include "file/fileinfo.h"
#include "../helpers.h"

using namespace boost::python;
using regina::FileInfo;

void addFileInfo() {
    class_<FileInfo, std::auto_ptr<FileInfo>, boost::noncopyable>
            ("FileInfo", no_init)
        .def("pathname", &FileInfo::pathname,
            return_value_policy<return_by_value>())
        .def("type", &FileInfo::type)
        .def("typeDescription", &FileInfo::typeDescription,
            return_value_policy<return_by_value>())
        .def("engine", &FileInfo::engine,
            return_value_policy<return_by_value>())
        .def("isCompressed", &FileInfo::isCompressed)
        .def("isInvalid", &FileInfo::isInvalid)
        .def("identify", &FileInfo::identify,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("identify")
        // Apparently there is no way in python to make attributes read-only.
        .attr("TYPE_XML") = FileInfo::TYPE_XML
    ;

    scope().attr("NFileInfo") = scope().attr("FileInfo");
}

