
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include <pybind11/pybind11.h>
#include "file/fileformat.h"
#include "../helpers.h"
#include "../docstrings/file/fileformat.h"

void addFileFormat(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(FileFormat)

    pybind11::enum_<regina::FileFormat>(m, "FileFormat", rdoc_scope)
        .value("REGINA_BINARY_GEN_1", regina::REGINA_BINARY_GEN_1,
            rdoc::REGINA_BINARY_GEN_1)
        .value("REGINA_XML_GEN_2", regina::REGINA_XML_GEN_2,
            rdoc::REGINA_XML_GEN_2)
        .value("REGINA_XML_GEN_3", regina::REGINA_XML_GEN_3,
            rdoc::REGINA_XML_GEN_3)
        .value("REGINA_CURRENT_FILE_FORMAT", regina::REGINA_CURRENT_FILE_FORMAT,
            rdoc::REGINA_CURRENT_FILE_FORMAT)
        .export_values()
        ;

    RDOC_SCOPE_END
}

