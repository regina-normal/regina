
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

#include <boost/python.hpp>

#include "regina-config.h"
#include "engine.h"
#include "helpers.h"

void addGlobalArray();

void addAlgebraClasses();
void addAngleClasses();
void addCensusClasses();
void addDim2Classes();
void addDim4Classes();
void addFileClasses();
void addForeignClasses();
void addGenericClasses();
void addHypersurfaceClasses();
void addManifoldClasses();
void addMathsClasses();
void addPacketClasses();
void addProgressClasses();
void addSnapPeaClasses();
void addSplitClasses();
void addSubcomplexClasses();
void addSurfacesClasses();
void addTreewidthClasses();
void addTriangulationClasses();
void addUtilitiesClasses();

using boost::python::self;

namespace {
    std::string welcome() {
        return std::string(PACKAGE_STRING) +
            "\nSoftware for low-dimensional topology" +
            "\nCopyright (c) 1999-2016, The Regina development team";
    }
}

#ifndef REGINA_LINK_MODULE_INTO_EXECUTABLE
// This is the normal situation: the C++ module is built as the extension
// regina/engine.so, which is loaded at runtime from regina/__init__.py.
// All of regina's classes live in the module regina.engine, and are
// automatically imported into the module regina by regina/__init__.py.
BOOST_PYTHON_MODULE(engine) {
#else
// This is a special case where the C++ module is linked into Regina's main
// executable at compile time (specifically, this happens on iOS).
// Nothing is loaded at runtime from the filesystem; there is no __init__.py,
// and all of Regina's classes live directly in the module regina.
BOOST_PYTHON_MODULE(regina) {
#endif
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

    boost::python::def("versionString", regina::versionString);
    boost::python::def("versionMajor", regina::versionMajor);
    boost::python::def("versionMinor", regina::versionMinor);
    boost::python::def("versionUsesUTF8", regina::versionUsesUTF8);
    boost::python::def("versionSnapPy", regina::versionSnapPy);
    boost::python::def("versionSnapPea", regina::versionSnapPea);
    boost::python::def("hasInt128", regina::hasInt128);
    boost::python::def("testEngine", regina::testEngine);

    // Components from subdirectories (in approximate dependency order):

    addUtilitiesClasses();
    addProgressClasses();
    addMathsClasses();
    addAlgebraClasses();
    addPacketClasses();
    addTriangulationClasses();
    addCensusClasses();
    addDim4Classes();
    addFileClasses();
    addForeignClasses();
    addSplitClasses();
    addSnapPeaClasses();
    addSubcomplexClasses();
    addManifoldClasses();
    addAngleClasses();
    addSurfacesClasses();
    addHypersurfaceClasses();
    addDim2Classes();
    addGenericClasses();
    addTreewidthClasses();
}

