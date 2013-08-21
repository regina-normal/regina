
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

// We need to see Python.h first to avoid a "portability fix" in pyport.h
// that breaks boost.python on MacOSX.
#include "Python.h"

#include <boost/python.hpp>
#include "file/nglobaldirs.h"

using namespace boost::python;
using regina::NGlobalDirs;

void addNGlobalDirs() {
    class_<NGlobalDirs>("NGlobalDirs", no_init)
        .def("home", &NGlobalDirs::home)
        .def("pythonModule", &NGlobalDirs::pythonModule)
        .def("pythonLibs", &NGlobalDirs::pythonLibs)
        .def("examples", &NGlobalDirs::examples)
        .def("engineDocs", &NGlobalDirs::engineDocs)
        .def("data", &NGlobalDirs::data)
        .def("setDirs", &NGlobalDirs::setDirs)
        .staticmethod("home")
        .staticmethod("pythonModule")
        .staticmethod("pythonLibs")
        .staticmethod("examples")
        .staticmethod("engineDocs")
        .staticmethod("data")
        .staticmethod("setDirs")
    ;
}

