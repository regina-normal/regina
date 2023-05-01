
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

#include "../pybind11/pybind11.h"
#include "../pybind11/stl.h"
#include "utilities/stringutils.h"
#include "../helpers/docstrings.h"
#include "../docstrings/utilities/stringutils.h"

using pybind11::overload_cast;

void addStringUtils(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    m.def("startsWith", &regina::startsWith, rdoc::startsWith);
    m.def("stripWhitespace", &regina::stripWhitespace, rdoc::stripWhitespace);
    m.def("stringToToken", &regina::stringToToken, rdoc::stringToToken);

    // overload_cast has trouble with templated vs non-templated overloads.
    // Just cast directly.
    m.def("basicTokenise", static_cast<std::vector<std::string>(&)(
        const std::string&)>(regina::basicTokenise), rdoc::basicTokenise);

    m.def("subscript",
        &regina::subscript<long>, rdoc::subscript);
    m.def("subscript", overload_cast<const regina::Integer&>(
        &regina::subscript<false>), rdoc::subscript);
    m.def("subscript", overload_cast<const regina::LargeInteger&>(
        &regina::subscript<true>), rdoc::subscript);
    m.def("superscript",
        &regina::superscript<long>, rdoc::superscript);
    m.def("superscript", overload_cast<const regina::Integer&>(
        &regina::superscript<false>), rdoc::superscript);
    m.def("superscript", overload_cast<const regina::LargeInteger&>(
        &regina::superscript<true>), rdoc::superscript);

    RDOC_SCOPE_END
}

