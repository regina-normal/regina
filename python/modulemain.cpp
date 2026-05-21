
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#include "regina-config.h" // for REGINA_PYBIND11_VERSION
#include "pybind11/pybind11.h"
#if REGINA_PYBIND11_VERSION == 3
#include <pybind11/native_enum.h>
#endif

#include "core/engine.h"
#include "triangulation/triangulation.h" // for TriangleType

#include "helpers.h"
#include "docstrings/core/engine.h"
#include "docstrings/core/regina-core.h"
#include "docstrings/python/equality.h"
#include "docstrings/triangulation/detail/face.h" // for TriangleType

// Additional headers for timeExceptions():
#include "maths/perm.h"
#include <chrono>

void addAlgebraClasses(pybind11::module_& m);
void addAngleClasses(pybind11::module_& m);
void addCensusClasses(pybind11::module_& m);
void addDim2Classes(pybind11::module_& m, pybind11::module_& internal);
void addDim4Classes(pybind11::module_& m, pybind11::module_& internal);
void addEnumerateClasses(pybind11::module_& m);
void addFileClasses(pybind11::module_& m);
void addForeignClasses(pybind11::module_& m);
void addGenericClasses(pybind11::module_& m, pybind11::module_& internal);
void addHypersurfaceClasses(pybind11::module_& m);
void addLinkClasses(pybind11::module_& m, pybind11::module_& internal);
void addManifoldClasses(pybind11::module_& m);
void addMathsClasses(pybind11::module_& m);
void addPacketClasses(pybind11::module_& m);
void addProgressClasses(pybind11::module_& m);
void addSnapPeaClasses(pybind11::module_& m, pybind11::module_& internal);
void addSplitClasses(pybind11::module_& m);
void addSubcomplexClasses(pybind11::module_& m);
void addSurfaceClasses(pybind11::module_& m, pybind11::module_& internal);
void addTreewidthClasses(pybind11::module_& m);
void addTriangulationClasses(pybind11::module_& m, pybind11::module_& internal);
void addUtilitiesClasses(pybind11::module_& m);

void addSageHacks();

namespace {
    std::string welcome() {
        return std::string(PACKAGE_STRING) +
            "\nSoftware for low-dimensional topology" +
            "\nCopyright (c) 1999-2026, The Regina development team";
    }
}

#ifndef REGINA_LINK_MODULE_INTO_EXECUTABLE
// This is the normal situation: the C++ module is built as the extension
// regina/engine.so, which is loaded at runtime from regina/__init__.py.
// All of regina's classes live in the module regina.engine, and are
// automatically imported into the module regina by regina/__init__.py.
#if REGINA_PYBIND11_VERSION == 3
PYBIND11_MODULE(engine, m,
        pybind11::multiple_interpreters::per_interpreter_gil()) {
#elif REGINA_PYBIND11_VERSION == 2
PYBIND11_MODULE(engine, m) {
#else
    #error "Unsupported pybind11 version"
#endif
#else
// This is a special case where the C++ module is linked into Regina's main
// executable at compile time (specifically, this happens on iOS).
// Nothing is loaded at runtime from the filesystem; there is no __init__.py,
// and all of Regina's classes live directly in the module regina.
#if REGINA_PYBIND11_VERSION == 3
PYBIND11_MODULE(regina, m
        pybind11::multiple_interpreters::per_interpreter_gil()) {
#elif REGINA_PYBIND11_VERSION == 2
PYBIND11_MODULE(regina, m) {
#else
#error "Unsupported pybind11 version"
#endif
#endif

    auto internal = m.def_submodule("internal",
R"doc(Implementation details for Regina.

End users should not need to explicitly refer to any of the classes
or functions within the submodule regina.internal.)doc");

    // Welcome string:

    m.def("welcome", welcome,
R"doc(Returns a multi-line welcome string that can be used as a banner for
a new Python session.)doc");

    // Wrappers for regina::python helpers:

    RDOC_SCOPE_BEGIN(python::EqualityType)

    using EqualityType = regina::python::EqualityType;
#if REGINA_PYBIND11_VERSION == 3
    pybind11::native_enum<EqualityType>(m, "EqualityType", "enum.Enum",
            rdoc::__class)
#elif REGINA_PYBIND11_VERSION == 2
    pybind11::enum_<EqualityType>(m, "EqualityType", rdoc::__class)
#endif
        .value("ByValue", EqualityType::ByValue, rdoc::ByValue)
        .value("ByReference", EqualityType::ByReference, rdoc::ByReference)
        .value("NeverInstantiated", EqualityType::NeverInstantiated,
            rdoc::NeverInstantiated)
        .value("Disabled", EqualityType::Disabled, rdoc::Disabled)
        // The following values are all deprecated:
        .value("BY_VALUE", EqualityType::ByValue, rdoc::BY_VALUE)
        .value("BY_REFERENCE", EqualityType::ByReference, rdoc::BY_REFERENCE)
        .value("NEVER_INSTANTIATED", EqualityType::NeverInstantiated,
            rdoc::NEVER_INSTANTIATED)
        .value("DISABLED", EqualityType::Disabled, rdoc::DISABLED)
#if REGINA_PYBIND11_VERSION == 3
        .finalize()
#endif
        ;

    // Core engine routines:

    RDOC_SCOPE_SWITCH_MAIN

    // From regina-core.h:
    m.def("supportedDim", regina::supportedDim, rdoc::supportedDim);
    m.def("standardDim", regina::standardDim, rdoc::standardDim);
    m.def("maxDim", regina::maxDim, rdoc::maxDim);
    m.def("maxPermDegree", regina::maxPermDegree, rdoc::maxPermDegree);

    // From core/engine.h:
    m.def("versionString", regina::versionString, rdoc::versionString);
    m.def("versionMajor", regina::versionMajor, rdoc::versionMajor);
    m.def("versionMinor", regina::versionMinor, rdoc::versionMinor);
    m.def("buildInfo", regina::buildInfo, rdoc::buildInfo);
    m.def("versionUsesUTF8", regina::versionUsesUTF8, rdoc::versionUsesUTF8);
    m.def("versionSnapPy", regina::versionSnapPy, rdoc::versionSnapPy);
    m.def("versionSnapPea", regina::versionSnapPea, rdoc::versionSnapPea);
    m.def("versionPybind11Major", []() {
        return REGINA_PYBIND11_VERSION;
    }, rdoc::versionPybind11Major);
    m.def("hasInt128", regina::hasInt128, rdoc::hasInt128);
    m.def("politeThreads", regina::politeThreads, rdoc::politeThreads);
    m.def("testEngine", regina::testEngine, rdoc::testEngine);

    // Python-only:
    m.def("timeExceptions", []() {
        auto t0 = std::chrono::system_clock::now();
        try {
            // Use a routine that does a bit of work and throws an exception.
            // We can be reasonably confident that the compiler hasn't
            // optimised away the try/catch block.
            regina::Perm<2>::tightDecoding("_");
        } catch (const regina::InvalidArgument&) {
        }
        auto t1 = std::chrono::system_clock::now();
        {
            // A case where no exception gets thrown, for comparison.
            regina::Perm<2>::tightDecoding("!"); // identity permutation
        }
        auto t2 = std::chrono::system_clock::now();
        try {
            throw regina::FailedPrecondition("Oops!");
        } catch (const regina::FailedPrecondition&) {
        }
        auto t3 = std::chrono::system_clock::now();
        try {
            throw pybind11::stop_iteration();
        } catch (const pybind11::stop_iteration&) {
        }
        auto t4 = std::chrono::system_clock::now();

        using tick = std::chrono::microseconds;
        return std::make_tuple(
            std::chrono::duration_cast<tick>(t1 - t0).count(),
            std::chrono::duration_cast<tick>(t2 - t1).count(),
            std::chrono::duration_cast<tick>(t3 - t2).count(),
            std::chrono::duration_cast<tick>(t4 - t3).count());
    }, R"doc(Diagnostic routine to test the performance of C++ exceptions.

This routine performs several C++ operations, most involving try/catch
blocks using either Regina or pybind11 exceptions, and measures their
running times.

Returns:
    A tuple giving the elapsed time for each operation, measured in
    microseconds.  The size of this tuple, as well as the specific
    operations performed, are subject to change in future versions of
    Regina.)doc");

    RDOC_SCOPE_SWITCH(Algorithm)

#if REGINA_PYBIND11_VERSION == 3
    pybind11::native_enum<regina::Algorithm>(m, "Algorithm", "enum.Enum",
            rdoc::__class)
#elif REGINA_PYBIND11_VERSION == 2
    pybind11::enum_<regina::Algorithm>(m, "Algorithm", rdoc::__class)
#endif
        .value("Default", regina::Algorithm::Default, rdoc::Default)
        .value("Backtrack", regina::Algorithm::Backtrack, rdoc::Backtrack)
        .value("Treewidth", regina::Algorithm::Treewidth, rdoc::Treewidth)
        .value("Naive", regina::Algorithm::Naive, rdoc::Naive)
#if REGINA_PYBIND11_VERSION == 3
        .finalize()
#endif
        ;

    // Deprecated constants:
    m.attr("ALG_DEFAULT") = regina::Algorithm::Default;
    m.attr("ALG_BACKTRACK") = regina::Algorithm::Backtrack;
    m.attr("ALG_TREEWIDTH") = regina::Algorithm::Treewidth;
    m.attr("ALG_NAIVE") = regina::Algorithm::Naive;

    RDOC_SCOPE_SWITCH(Language)

#if REGINA_PYBIND11_VERSION == 3
    pybind11::native_enum<regina::Language>(m, "Language", "enum.Enum",
            rdoc::__class)
#elif REGINA_PYBIND11_VERSION == 2
    pybind11::enum_<regina::Language>(m, "Language", rdoc::__class)
#endif
        .value("Cxx", regina::Language::Cxx, rdoc::Cxx)
        .value("Python", regina::Language::Python, rdoc::Python)
        .value("Current",
            // In Python this should evaluate to Language::Python, but when
            // writing C++ code (as we are here) it evaluates to Language::Cxx.
            // We therefore hard-code its value as Language::Python below.
            regina::Language::Python,
            rdoc::Current)
#if REGINA_PYBIND11_VERSION == 3
        .finalize()
#endif
        ;

    RDOC_SCOPE_SWITCH(TriangleType)

#if REGINA_PYBIND11_VERSION == 3
    pybind11::native_enum<regina::TriangleType>(m, "TriangleType",
            "enum.Enum", rdoc::__class)
#elif REGINA_PYBIND11_VERSION == 2
    pybind11::enum_<regina::TriangleType>(m, "TriangleType", rdoc::__class)
#endif
        .value("Unknown", regina::TriangleType::Unknown, rdoc::Unknown)
        .value("Triangle", regina::TriangleType::Triangle, rdoc::Triangle)
        .value("Scarf", regina::TriangleType::Scarf, rdoc::Scarf)
        .value("Parachute", regina::TriangleType::Parachute, rdoc::Parachute)
        .value("Cone", regina::TriangleType::Cone, rdoc::Cone)
        .value("Mobius", regina::TriangleType::Mobius, rdoc::Mobius)
        .value("Horn", regina::TriangleType::Horn, rdoc::Horn)
        .value("DunceHat", regina::TriangleType::DunceHat, rdoc::DunceHat)
        .value("L31", regina::TriangleType::L31, rdoc::L31)
#if REGINA_PYBIND11_VERSION == 3
        .finalize()
#endif
        ;

    RDOC_SCOPE_END

    // Components from subdirectories, which appear in order of dependency:
    //
    // - In some cases the bindings require this; in particular, when a value
    //   has to be converted to Python immediately.  This can happen (for
    //   instance) when setting default arguments or class constants, in
    //   which case the type of the value being set must already be available.
    //   A failure here will mean the Python module cannot load (and so will
    //   be picked up by the test suite).
    //
    // - For docstrings with abbreviated type names (e.g., Integer instead of
    //   IntegerBase<false>), the type should be already available in order
    //   for the docstrings to use them in the function signatures.
    //   A failure here will simply lead to less-readable docstrints (and so
    //   will not be picked up automatically by our tests).

    addFileClasses(m); // Provides FileFormat, which is used later
    addMathsClasses(m);
    addUtilitiesClasses(m);
    addProgressClasses(m);
    addAlgebraClasses(m);
    addPacketClasses(m);
    addDim2Classes(m, internal);
    addTriangulationClasses(m, internal);
    addLinkClasses(m, internal); // Needs to come _before_ dim4 classes
    addDim4Classes(m, internal);
    addGenericClasses(m, internal);
    addCensusClasses(m);
    addForeignClasses(m);
    addSplitClasses(m);
    addSnapPeaClasses(m, internal);
    addSubcomplexClasses(m);
    addManifoldClasses(m);
    addAngleClasses(m);
    addSurfaceClasses(m, internal);
    addHypersurfaceClasses(m);
    addTreewidthClasses(m);
    addEnumerateClasses(m);

    // This routine allows the user to import sage-related hacks, which
    // are not included by default in regina's python module.
    m.def("_addSageHacks", &addSageHacks,
R"doc(Modifies Regina's Python module to be suitable for use within SageMath.

Since Regina 5.96, this routine does nothing at all. The future of
this function is uncertain: it may be given a purpose again in some
future release of Regina, or it may eventually be removed completely.)doc");
}

