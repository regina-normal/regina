
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
#include "file/globaldirs.h"
#include "../helpers.h"
#include "../docstrings/file/globaldirs.h"

using pybind11::overload_cast;
using regina::GlobalDirs;

void addGlobalDirs(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(GlobalDirs)

    auto c = pybind11::class_<GlobalDirs>(m, "GlobalDirs", rdoc_scope)
        .def_static("home", &GlobalDirs::home, rdoc::home)
        .def_static("pythonModule", &GlobalDirs::pythonModule,
            rdoc::pythonModule)
        .def_static("census", &GlobalDirs::census, rdoc::census)
        .def_static("examples", &GlobalDirs::examples, rdoc::examples)
        .def_static("engineDocs", &GlobalDirs::engineDocs, rdoc::engineDocs)
        .def_static("data", &GlobalDirs::data, rdoc::data)
        .def_static("setDirs", overload_cast<const std::string&,
                const std::string&, const std::string&>(
            &GlobalDirs::setDirs),
            pybind11::arg(), pybind11::arg(),
            pybind11::arg("censusDir") = std::string(),
            rdoc::setDirs)
        .def_static("deduceDirs", &GlobalDirs::deduceDirs, rdoc::deduceDirs)
    ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_END
}

