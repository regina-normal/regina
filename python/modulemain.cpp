
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "pybind11/pybind11.h"

#include "regina-config.h"
#include "core/engine.h"
#include "helpers.h"

void addGlobalArray(pybind11::module_& m);

void addAlgebraClasses(pybind11::module_& m);
void addAngleClasses(pybind11::module_& m);
void addCensusClasses(pybind11::module_& m);
void addDim2Classes(pybind11::module_& m);
void addDim4Classes(pybind11::module_& m);
void addFileClasses(pybind11::module_& m);
void addForeignClasses(pybind11::module_& m);
#ifndef REGINA_LOWDIMONLY
void addGenericClasses(pybind11::module_& m);
#endif
void addHypersurfaceClasses(pybind11::module_& m);
void addLinkClasses(pybind11::module_& m);
void addManifoldClasses(pybind11::module_& m);
void addMathsClasses(pybind11::module_& m);
void addPacketClasses(pybind11::module_& m);
void addProgressClasses(pybind11::module_& m);
void addSnapPeaClasses(pybind11::module_& m);
void addSplitClasses(pybind11::module_& m);
void addSubcomplexClasses(pybind11::module_& m);
void addSurfacesClasses(pybind11::module_& m);
void addTreewidthClasses(pybind11::module_& m);
void addTriangulationClasses(pybind11::module_& m);
void addUtilitiesClasses(pybind11::module_& m);

void addSageHacks();

namespace {
    std::string welcome() {
        return std::string(PACKAGE_STRING) +
            "\nSoftware for low-dimensional topology" +
            "\nCopyright (c) 1999-2021, The Regina development team";
    }
}

#ifndef REGINA_LINK_MODULE_INTO_EXECUTABLE
// This is the normal situation: the C++ module is built as the extension
// regina/engine.so, which is loaded at runtime from regina/__init__.py.
// All of regina's classes live in the module regina.engine, and are
// automatically imported into the module regina by regina/__init__.py.
PYBIND11_MODULE(engine, m) {
#else
// This is a special case where the C++ module is linked into Regina's main
// executable at compile time (specifically, this happens on iOS).
// Nothing is loaded at runtime from the filesystem; there is no __init__.py,
// and all of Regina's classes live directly in the module regina.
PYBIND11_MODULE(regina, m) {
#endif
    // Welcome string:

    m.def("welcome", welcome);

    // Wrappers for regina::python helpers:

    pybind11::enum_<regina::python::EqualityType>(m, "EqualityType")
        .value("BY_VALUE", regina::python::BY_VALUE)
        .value("BY_REFERENCE", regina::python::BY_REFERENCE)
        .value("NEVER_INSTANTIATED", regina::python::NEVER_INSTANTIATED)
        ;

    addGlobalArray(m);

    // Core engine routines:

    m.def("versionString", regina::versionString);
    m.def("versionMajor", regina::versionMajor);
    m.def("versionMinor", regina::versionMinor);
    m.def("versionUsesUTF8", regina::versionUsesUTF8);
    m.def("versionSnapPy", regina::versionSnapPy);
    m.def("versionSnapPea", regina::versionSnapPea);
    m.def("hasInt128", regina::hasInt128);
    m.def("politeThreads", regina::politeThreads);
    m.def("testEngine", regina::testEngine);

    pybind11::enum_<regina::Algorithm>(m, "Algorithm")
        .value("ALG_DEFAULT", regina::ALG_DEFAULT)
        .value("ALG_BACKTRACK", regina::ALG_BACKTRACK)
        .value("ALG_TREEWIDTH", regina::ALG_TREEWIDTH)
        .value("ALG_NAIVE", regina::ALG_NAIVE)
        .value("TV_DEFAULT", regina::ALG_DEFAULT)
        .value("TV_BACKTRACK", regina::ALG_BACKTRACK)
        .value("TV_TREEWIDTH", regina::ALG_TREEWIDTH)
        .value("TV_NAIVE", regina::ALG_NAIVE)
        .export_values();
        ;

    // Components from subdirectories (in approximate dependency order):

    addUtilitiesClasses(m);
    addProgressClasses(m);
    addMathsClasses(m);
    addAlgebraClasses(m);
    addPacketClasses(m);
    addTriangulationClasses(m);
    addCensusClasses(m);
    addDim4Classes(m);
    addFileClasses(m);
    addForeignClasses(m);
    addSplitClasses(m);
    addSnapPeaClasses(m);
    addSubcomplexClasses(m);
    addManifoldClasses(m);
    addAngleClasses(m);
    addSurfacesClasses(m);
    addHypersurfaceClasses(m);
    addDim2Classes(m);
    addGenericClasses(m);
    addTreewidthClasses(m);
    addLinkClasses(m);

    // This routine allows the user to import sage-related hacks, which
    // are not included by default in regina's python module.
    m.def("_addSageHacks", &addSageHacks);
}

