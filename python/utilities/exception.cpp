
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

// Bring in all of the exception classes defined in utilities/*.h.
//
// The SnapPea kernel exceptions are added elsewhere (since they are
// defined in snappea/*.h).

void addException(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    // Derived from ReginaException:
    regina::python::registerReginaException<regina::ReginaException>(m,
        "ReginaException", rdoc::ReginaException);
    regina::python::registerReginaException<regina::FailedPrecondition>(m,
        "FailedPrecondition", rdoc::FailedPrecondition);
    regina::python::registerReginaException<regina::InvalidArgument>(m,
        "InvalidArgument", rdoc::InvalidArgument);
    regina::python::registerReginaException<regina::InvalidInput>(m,
        "InvalidInput", rdoc::InvalidInput);
    regina::python::registerReginaException<regina::NotImplemented>(m,
        "NotImplemented", rdoc::NotImplemented);
    regina::python::registerReginaException<regina::FileError>(m,
        "FileError", rdoc::FileError);
    regina::python::registerReginaException<regina::NoSolution>(m,
        "NoSolution", rdoc::NoSolution);
    regina::python::registerReginaException<regina::UnsolvedCase>(m,
        "UnsolvedCase", rdoc::UnsolvedCase);
    regina::python::registerReginaException<regina::LockViolation>(m,
        "LockViolation", rdoc::LockViolation);
    regina::python::registerReginaException<regina::ImpossibleScenario>(m,
        "ImpossibleScenario", rdoc::ImpossibleScenario);
    regina::python::registerReginaException<regina::NumericalError>(m,
        "NumericalError", rdoc::NumericalError);
    regina::python::registerReginaException<regina::IntegerOverflow>(m,
        "IntegerOverflow", rdoc::IntegerOverflow);
    regina::python::registerReginaException<regina::DivisionByZero>(m,
        "DivisionByZero", rdoc::DivisionByZero);
    regina::python::registerReginaException<regina::SnapPeaUnsolvedCase>(m,
        "SnapPeaUnsolvedCase", rdoc::SnapPeaUnsolvedCase);
    regina::python::registerReginaException<regina::SnapPeaIsNull>(m,
        "SnapPeaIsNull", rdoc::SnapPeaIsNull);

    // Snapshotting machinery:
    regina::python::registerReginaException<regina::SnapshotWriteError>(m,
        "SnapshotWriteError", rdoc::SnapshotWriteError);

    RDOC_SCOPE_END
}

