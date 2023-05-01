
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
#include "utilities/i18nutils.h"
#include "../helpers.h"
#include "../docstrings/utilities/i18nutils.h"

void addLocale(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(i18n::Locale)

    auto c = pybind11::class_<regina::i18n::Locale>(m, "Locale", rdoc_scope)
        .def_static("codeset", &regina::i18n::Locale::codeset, rdoc::codeset)
    ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_SWITCH_MAIN

    m.def("utf8ValidTo", [](const char* s) {
        const char* ans = regina::i18n::utf8ValidTo(s);
        return (ans - s);
    }, rdoc::i18n::utf8ValidTo);

    RDOC_SCOPE_END
}

