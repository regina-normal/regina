
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "regina-config.h" // for REGINA_PYBIND11_VERSION
#include "pybind11/pybind11.h"
#if REGINA_PYBIND11_VERSION == 3
#include <pybind11/native_enum.h>
#endif
#include "file/fileformat.h"
#include "../helpers.h"
#include "../docstrings/file/fileformat.h"

void addFileFormat(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(FileFormat)

#if REGINA_PYBIND11_VERSION == 3
    pybind11::native_enum<regina::FileFormat>(m, "FileFormat", "enum.Enum",
            rdoc_scope)
#elif REGINA_PYBIND11_VERSION == 2
    pybind11::enum_<regina::FileFormat>(m, "FileFormat", rdoc_scope)
#else
    #error "Unsupported pybind11 version"
#endif
        .value("BinaryGen1", regina::FileFormat::BinaryGen1, rdoc::BinaryGen1)
        .value("XmlGen2", regina::FileFormat::XmlGen2, rdoc::XmlGen2)
        .value("XmlGen3", regina::FileFormat::XmlGen3, rdoc::XmlGen3)
        .value("Current", regina::FileFormat::Current, rdoc::Current)
#if REGINA_PYBIND11_VERSION == 3
        .finalize()
#endif
        ;

    // Deprecated constants:
    m.attr("REGINA_BINARY_GEN_1") = regina::FileFormat::BinaryGen1;
    m.attr("REGINA_XML_GEN_2") = regina::FileFormat::XmlGen2;
    m.attr("REGINA_XML_GEN_3") = regina::FileFormat::XmlGen3;
    m.attr("REGINA_CURRENT_FILE_FORMAT") = regina::FileFormat::Current;

    RDOC_SCOPE_END
}

