
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

#include <pybind11/pybind11.h>
#include "snappea/snappeatriangulation.h"
#include "utilities/exception.h"
#include "utilities/snapshot.h"
#include "../helpers.h"
#include "../helpers/exception.h"
#include "../docstrings/utilities/exception.h"
#include "../docstrings/utilities/snapshot.h"

using regina::python::registerReginaException;

// Bring in all of the exception classes defined in utilities/*.h.
//
// The SnapPea kernel exceptions are added elsewhere (since they are
// defined in snappea/*.h).

void addException(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    // Derived from ReginaException:
    auto base = registerReginaException<regina::ReginaException,
        rdoc::ReginaException>(m, "ReginaException", PyExc_RuntimeError);
    registerReginaException<regina::FailedPrecondition,
        rdoc::FailedPrecondition>(m, "FailedPrecondition", base);
    registerReginaException<regina::InvalidArgument,
        rdoc::InvalidArgument>(m, "InvalidArgument", base);
    registerReginaException<regina::InvalidInput,
        rdoc::InvalidInput>(m, "InvalidInput", base);
    registerReginaException<regina::NotImplemented,
        rdoc::NotImplemented>(m, "NotImplemented", base);
    registerReginaException<regina::FileError,
        rdoc::FileError>(m, "FileError", base);
    registerReginaException<regina::NoSolution,
        rdoc::NoSolution>(m, "NoSolution", base);
    auto unsolved = registerReginaException<regina::UnsolvedCase,
        rdoc::UnsolvedCase>(m, "UnsolvedCase", base);
    registerReginaException<regina::LockViolation,
        rdoc::LockViolation>(m, "LockViolation", base);
    registerReginaException<regina::ImpossibleScenario,
        rdoc::ImpossibleScenario>(m, "ImpossibleScenario", base);
    auto numerical = registerReginaException<regina::NumericalError,
        rdoc::NumericalError>(m, "NumericalError", base);
    registerReginaException<regina::IntegerOverflow,
        rdoc::IntegerOverflow>(m, "IntegerOverflow", numerical);
    registerReginaException<regina::DivisionByZero,
        rdoc::DivisionByZero>(m, "DivisionByZero", numerical);
    registerReginaException<regina::SnapPeaUnsolvedCase,
        rdoc::SnapPeaUnsolvedCase>(m, "SnapPeaUnsolvedCase", unsolved);
    registerReginaException<regina::SnapPeaIsNull,
        rdoc::SnapPeaIsNull>(m, "SnapPeaIsNull", base);

    // Snapshotting machinery:
    registerReginaException<regina::SnapshotWriteError,
        rdoc::SnapshotWriteError>(m, "SnapshotWriteError", base);

    RDOC_SCOPE_END
}

