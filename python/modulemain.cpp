
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <boost/python.hpp>

#include "engine.h"
#include "shareableobject.h"
#include "regina-config.h"

void addGlobalArray();

void addAlgebra();
void addAngle();
void addCensus();
void addFile();
void addForeign();
void addManifold();
void addMaths();
void addPacket();
void addProgress();
void addSplit();
void addSubcomplex();
void addSurfaces();
void addTriangulation();
void addUtilities();

using regina::ShareableObject;

namespace {
    std::string welcome() {
        return std::string(PACKAGE_STRING) +
            "\nA Normal Surface Theory Calculator" +
            "\nCopyright (c) 1999-2003, Ben Burton";
    }

    void shareableWriteTextShort(const ShareableObject& obj) {
        obj.writeTextShort(std::cout);
    }

    void shareableWriteTextLong(const ShareableObject& obj) {
        obj.writeTextLong(std::cout);
    }
}

BOOST_PYTHON_MODULE(regina) {
    // Welcome string:

    boost::python::def("welcome", welcome);

    // Wrappers for global array classes:

    addGlobalArray();

    // Core engine routines:

    boost::python::def("getVersionString", regina::getVersionString);
    boost::python::def("getVersionMajor", regina::getVersionMajor);
    boost::python::def("getVersionMinor", regina::getVersionMinor);
    boost::python::def("testEngine", regina::testEngine);

    // ShareableObject class:

    boost::python::class_<ShareableObject, boost::noncopyable>
            ("ShareableObject", boost::python::no_init)
        .def("writeTextShort", &shareableWriteTextShort)
        .def("writeTextLong", &shareableWriteTextLong)
        .def("toString", &ShareableObject::toString)
        .def("toStringLong", &ShareableObject::toStringLong)
        .def("__str__", &ShareableObject::toString)
    ;

    // Components from subdirectories (in approximate dependency order):

    addUtilities();
    addProgress();
    addMaths();
    addAlgebra();
    addPacket();
    addTriangulation();
    addCensus();
    addFile();
    addForeign();
    addSplit();
    addSubcomplex();
    addManifold();
    addAngle();
    addSurfaces();
}

