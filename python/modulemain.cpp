
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include "docstrings/core/engine.h"
#include "docstrings/core/output.h"

// Docstrings that are generated once but need to be reused across many
// source files:
namespace regina::python::doc::common {
    const char* Output_detail = Output_::detail;
    const char* Output_str = Output_::str;
    const char* Output_utf8 = Output_::utf8;

    // Note: docstrings should be wrapped at 70 characters per line;
    // the hard maximum is 72.

    const char* eq_reference =
R"doc(Determines whether this and the given Python wrapper refer to the same
underlying object in Regina's calculation engine.

Note that most of Regina's classes do **not** test equality in this
way; instead they use value semantics (i.e., the == and != operators
compare the *contents* of the two objects). This class is one of the
few exceptions that uses reference semantics, as explained below.

Regina's calculation engine is written in C++, not Python. It is
therefore possible to have several different Python objects that are
all thin wrappers around the same underlying C++ object (so changes to
any one of these objects will be reflected in all of them). The
operators == and != for this class will test for exactly this scenario.

Essentially, these tests are similar in spirit to the Python test
``x is y``, but instead of looking at the Python wrappers they look at
the underlying C++ objects in the calculation engine.  In particular,
as noted above, it is possible to have two different Python wrappers
(so ``x is y`` is false) that refer to the same underlying C++ object
(so ``x == y`` is true).)doc";

    const char* neq_reference =
R"doc(Determines whether this and the given Python wrapper refer to different
underlying objects in Regina's calculation engine.

Note that most of Regina's classes do **not** test equality in this
way; instead they use value semantics (i.e., the == and != operators
compare the *contents* of the two objects). This class is one of the
few exceptions that uses reference semantics, as explained below.

Regina's calculation engine is written in C++, not Python. It is
therefore possible to have several different Python objects that are
all thin wrappers around the same underlying C++ object (so changes to
any one of these objects will be reflected in all of them). The
operators == and != for this class will test for exactly this scenario.

Essentially, these tests are similar in spirit to the Python test
``x is y``, but instead of looking at the Python wrappers they look at
the underlying C++ objects in the calculation engine.  In particular,
as noted above, it is possible to have two different Python wrappers
(so ``x is y`` is false) that refer to the same underlying C++ object
(so ``x == y`` is true).)doc";

    const char* eq_None =
R"doc(Always returns ``False``, since an object of this type is never equal
to ``None``.)doc";

    const char* neq_None =
R"doc(Always returns ``True``, since an object of this type is never equal
to ``None``.)doc";

    const char* eq_disabled =
R"doc(Disabled for objects of this type.

Objects of this type use value semantics, which means that the
operators == and != should compare by value (i.e., they test whether
two objects have the same contents). However, Regina does not
currently implement such a test for objects of this type.)doc";

    const char* eq_packet_disabled =
R"doc(Disabled for packets of this type.

The operators == and != compare packet contents by value (i.e., they
test whether two packets have the same contents). However, Regina does
not currently implement such a test for packets of this type.

To test whether two Python objects refer to the same underlying packet,
use Packet.samePacket() instead.)doc";

    const char* eq_packet_invalid =
R"doc(Disabled for packets of different types.

The operators == and != compare packet contents by value, and therefore
can only be used to compare two packets of the same type.

To test whether two Python objects refer to the same underlying packet,
use Packet.samePacket() instead.)doc";

    const char* eq_never_instantiated =
R"doc(Disabled in Regina.

Objects of this type cannot be created, and so cannot be compared.)doc";
}

void addGlobalArray(pybind11::module_& m);

void addAlgebraClasses(pybind11::module_& m);
void addAngleClasses(pybind11::module_& m);
void addCensusClasses(pybind11::module_& m);
void addDim2Classes(pybind11::module_& m);
void addDim4Classes(pybind11::module_& m);
void addEnumerateClasses(pybind11::module_& m);
void addFileClasses(pybind11::module_& m);
void addForeignClasses(pybind11::module_& m);
void addGenericClasses(pybind11::module_& m);
void addHypersurfaceClasses(pybind11::module_& m);
void addLinkClasses(pybind11::module_& m);
void addManifoldClasses(pybind11::module_& m);
void addMathsClasses(pybind11::module_& m);
void addPacketClasses(pybind11::module_& m);
void addProgressClasses(pybind11::module_& m);
void addSnapPeaClasses(pybind11::module_& m);
void addSplitClasses(pybind11::module_& m);
void addSubcomplexClasses(pybind11::module_& m);
void addSurfaceClasses(pybind11::module_& m);
void addTreewidthClasses(pybind11::module_& m);
void addTriangulationClasses(pybind11::module_& m);
void addUtilitiesClasses(pybind11::module_& m);

void addSageHacks();

namespace {
    std::string welcome() {
        return std::string(PACKAGE_STRING) +
            "\nSoftware for low-dimensional topology" +
            "\nCopyright (c) 1999-2022, The Regina development team";
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

    m.def("welcome", welcome,
R"doc(Returns a multi-line welcome string that can be used as a banner for
a new Python session.)doc");

    // Wrappers for regina::python helpers:

    pybind11::enum_<regina::python::EqualityType>(m, "EqualityType")
        .value("BY_VALUE", regina::python::BY_VALUE)
        .value("BY_REFERENCE", regina::python::BY_REFERENCE)
        .value("NEVER_INSTANTIATED", regina::python::NEVER_INSTANTIATED)
        .value("DISABLED", regina::python::DISABLED)
        ;

    addGlobalArray(m);

    // Core engine routines:

    RDOC_SCOPE_BEGIN_MAIN

    m.def("versionString", regina::versionString, rdoc::versionString);
    m.def("versionMajor", regina::versionMajor, rdoc::versionMajor);
    m.def("versionMinor", regina::versionMinor, rdoc::versionMinor);
    m.def("versionUsesUTF8", regina::versionUsesUTF8, rdoc::versionUsesUTF8);
    m.def("versionSnapPy", regina::versionSnapPy, rdoc::versionSnapPy);
    m.def("versionSnapPea", regina::versionSnapPea, rdoc::versionSnapPea);
    m.def("hasInt128", regina::hasInt128, rdoc::hasInt128);
    m.def("politeThreads", regina::politeThreads, rdoc::politeThreads);
    m.def("testEngine", regina::testEngine, rdoc::testEngine);

    RDOC_SCOPE_END

    pybind11::enum_<regina::Algorithm>(m, "Algorithm")
        .value("ALG_DEFAULT", regina::ALG_DEFAULT)
        .value("ALG_BACKTRACK", regina::ALG_BACKTRACK)
        .value("ALG_TREEWIDTH", regina::ALG_TREEWIDTH)
        .value("ALG_NAIVE", regina::ALG_NAIVE)
        .export_values();
        ;

    // Components from subdirectories (in approximate dependency order):

    addUtilitiesClasses(m);
    addFileClasses(m); // needs to come before Packet
    addProgressClasses(m);
    addMathsClasses(m);
    addAlgebraClasses(m);
    addPacketClasses(m);
    addTriangulationClasses(m);
    addCensusClasses(m);
    addDim4Classes(m);
    addForeignClasses(m);
    addSplitClasses(m);
    addSnapPeaClasses(m);
    addSubcomplexClasses(m);
    addManifoldClasses(m);
    addAngleClasses(m);
    addSurfaceClasses(m);
    addHypersurfaceClasses(m);
    addDim2Classes(m);
    addGenericClasses(m);
    addTreewidthClasses(m);
    addLinkClasses(m);
    addEnumerateClasses(m);

    // This routine allows the user to import sage-related hacks, which
    // are not included by default in regina's python module.
    m.def("_addSageHacks", &addSageHacks,
R"doc(Modifies Regina's Python module to be suitable for use within SageMath.

Since Regina 5.96, this routine does nothing at all. The future of
this function is uncertain: it may be given a purpose again in some
future release of Regina, or it may eventually be removed completely.)doc");
}

