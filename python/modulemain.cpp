
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

#include <boost/python.hpp>

#include "regina-config.h"
#include "engine.h"
#include "helpers.h"

void addGlobalArray();

void addAlgebra();
void addAngle();
void addCensus();
void addDim2();
void addDim4();
void addFile();
void addForeign();
void addHypersurface();
void addManifold();
void addMaths();
void addPacket();
void addProgress();
void addSnapPea();
void addSplit();
void addSubcomplex();
void addSurfaces();
void addTreewidth();
void addTriangulation();
void addUtilities();

using boost::python::self;

namespace {
    std::string welcome() {
        return std::string(PACKAGE_STRING) +
            "\nSoftware for 3-manifolds, 4-manifolds and normal surface theory" +
            "\nCopyright (c) 1999-2014, The Regina development team";
    }
}

BOOST_PYTHON_MODULE(engine) {
    // Welcome string:

    boost::python::def("welcome", welcome);

    // Wrappers for regina::python helpers:

    boost::python::enum_<regina::python::EqualityType>("EqualityType")
        .value("BY_VALUE", regina::python::BY_VALUE)
        .value("BY_REFERENCE", regina::python::BY_REFERENCE)
        .value("NEVER_INSTANTIATED", regina::python::NEVER_INSTANTIATED)
        ;

    addGlobalArray();

    // Core engine routines:

    boost::python::def("getVersionString", regina::getVersionString);
    boost::python::def("getVersionMajor", regina::getVersionMajor);
    boost::python::def("getVersionMinor", regina::getVersionMinor);
    boost::python::def("versionUsesUTF8", regina::versionUsesUTF8);
    boost::python::def("testEngine", regina::testEngine);

    // Components from subdirectories (in approximate dependency order):

    addUtilities();
    addProgress();
    addMaths();
    addAlgebra();
    addPacket();
    addTriangulation();
    addCensus();
    addDim4();
    addFile();
    addForeign();
    addSplit();
    addSnapPea();
    addSubcomplex();
    addManifold();
    addAngle();
    addSurfaces();
    addHypersurface();
    addDim2();
    addTreewidth();
}

