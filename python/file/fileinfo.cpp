
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "file/fileinfo.h"
#include "../helpers.h"
#include "../docstrings/file/fileinfo.h"

using regina::FileInfo;

void addFileInfo(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(FileInfo)

    auto c = pybind11::class_<FileInfo>(m, "FileInfo", rdoc_scope)
        .def(pybind11::init<const FileInfo&>(), rdoc::__copy)
        .def("pathname", &FileInfo::pathname, rdoc::pathname)
        .def("format", &FileInfo::format, rdoc::format)
        .def("formatDescription", &FileInfo::formatDescription,
            rdoc::formatDescription)
        .def("engine", &FileInfo::engine, rdoc::engine)
        .def("isCompressed", &FileInfo::isCompressed, rdoc::isCompressed)
        .def("isInvalid", &FileInfo::isInvalid, rdoc::isInvalid)
        .def("swap", &FileInfo::swap, rdoc::swap)
        .def_static("identify", &FileInfo::identify, rdoc::identify)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq);

    regina::python::add_global_swap<FileInfo>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

