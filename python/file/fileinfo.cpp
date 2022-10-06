
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#include "../pybind11/pybind11.h"
#include "../pybind11/stl.h"
#include "file/fileinfo.h"
#include "../helpers.h"

using regina::FileInfo;

void addFileInfo(pybind11::module_& m) {
    auto c = pybind11::class_<FileInfo>(m, "FileInfo")
        .def(pybind11::init<const FileInfo&>())
        .def("pathname", &FileInfo::pathname)
        .def("format", &FileInfo::format)
        .def("formatDescription", &FileInfo::formatDescription)
        .def("engine", &FileInfo::engine)
        .def("isCompressed", &FileInfo::isCompressed)
        .def("isInvalid", &FileInfo::isInvalid)
        .def("swap", &FileInfo::swap)
        .def_static("identify", &FileInfo::identify)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    regina::python::add_global_swap<FileInfo>(m);
}

