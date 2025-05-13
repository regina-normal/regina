
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include "snappea/snappeatriangulation.h"
#include "utilities/exception.h"
#include "utilities/snapshot.h"
#include "../helpers.h"
#include "../docstrings/utilities/exception.h"
#include "../docstrings/utilities/snapshot.h"

// Bring in all of the exception classes defined in utilities/*.h.
//
// The SnapPea kernel exceptions are added elsewhere (since they are
// defined in snappea/*.h).

void addException(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    // Derived from ReginaException:
    pybind11::register_exception<regina::ReginaException>(m,
        "ReginaException", PyExc_RuntimeError)
        .doc() = rdoc::ReginaException;
    pybind11::register_exception<regina::FailedPrecondition>(m,
        "FailedPrecondition", PyExc_RuntimeError)
        .doc() = rdoc::FailedPrecondition;
    pybind11::register_exception<regina::InvalidArgument>(m,
        "InvalidArgument", PyExc_RuntimeError)
        .doc() = rdoc::InvalidArgument;
    pybind11::register_exception<regina::InvalidInput>(m,
        "InvalidInput", PyExc_RuntimeError)
        .doc() = rdoc::InvalidInput;
    pybind11::register_exception<regina::NotImplemented>(m,
        "NotImplemented", PyExc_RuntimeError)
        .doc() = rdoc::NotImplemented;
    pybind11::register_exception<regina::FileError>(m,
        "FileError", PyExc_RuntimeError)
        .doc() = rdoc::FileError;
    pybind11::register_exception<regina::NoSolution>(m,
        "NoSolution", PyExc_RuntimeError)
        .doc() = rdoc::NoSolution;
    pybind11::register_exception<regina::UnsolvedCase>(m,
        "UnsolvedCase", PyExc_RuntimeError)
        .doc() = rdoc::UnsolvedCase;
    pybind11::register_exception<regina::LockViolation>(m,
        "LockViolation", PyExc_RuntimeError)
        .doc() = rdoc::LockViolation;
    pybind11::register_exception<regina::ImpossibleScenario>(m,
        "ImpossibleScenario", PyExc_RuntimeError)
        .doc() = rdoc::ImpossibleScenario;
    pybind11::register_exception<regina::SnapPeaUnsolvedCase>(m,
        "SnapPeaUnsolvedCase", PyExc_RuntimeError)
        .doc() = rdoc::SnapPeaUnsolvedCase;
    pybind11::register_exception<regina::SnapPeaIsNull>(m,
        "SnapPeaIsNull", PyExc_RuntimeError)
        .doc() = rdoc::SnapPeaIsNull;

    // Snapshotting machinery:
    pybind11::register_exception<regina::SnapshotWriteError>(m,
        "SnapshotWriteError", PyExc_RuntimeError)
        .doc() = rdoc::SnapshotWriteError;

    RDOC_SCOPE_END
}

