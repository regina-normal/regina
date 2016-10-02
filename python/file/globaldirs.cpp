
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

// We need to see Python.h first to avoid a "portability fix" in pyport.h
// that breaks boost.python on MacOSX.
#include "Python.h"

#include <boost/python.hpp>
#include "file/globaldirs.h"
#include "../helpers.h"

using namespace boost::python;
using regina::GlobalDirs;

namespace {
    void setDirs_2(const std::string& x, const std::string& y) {
        GlobalDirs::setDirs(x, y);
    }

    void setDirs_3(const std::string& x, const std::string& y,
            const std::string& z) {
        GlobalDirs::setDirs(x, y, z);
    }
}

void addGlobalDirs() {
    class_<GlobalDirs>("GlobalDirs", no_init)
        .def("home", &GlobalDirs::home)
        .def("pythonModule", &GlobalDirs::pythonModule)
        .def("census", &GlobalDirs::census)
        .def("pythonLibs", &GlobalDirs::pythonLibs)
        .def("examples", &GlobalDirs::examples)
        .def("engineDocs", &GlobalDirs::engineDocs)
        .def("data", &GlobalDirs::data)
        .def("setDirs", setDirs_2)
        .def("setDirs", setDirs_3)
        .def(regina::python::no_eq_operators())
        .staticmethod("home")
        .staticmethod("pythonModule")
        .staticmethod("census")
        .staticmethod("pythonLibs")
        .staticmethod("examples")
        .staticmethod("engineDocs")
        .staticmethod("data")
        .staticmethod("setDirs")
    ;

    scope().attr("NGlobalDirs") = scope().attr("GlobalDirs");
}

