
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
#include <pybind11/stl.h>
#include "maths/matrixops.h"
#include "../helpers/docstrings.h"
#include "../docstrings/maths/matrixops.h"

using pybind11::overload_cast;
using regina::MatrixInt;

void addMatrixOps(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    m.def("smithNormalForm", overload_cast<MatrixInt&>(
        &regina::smithNormalForm),
        rdoc::smithNormalForm);
    m.def("smithNormalForm", overload_cast<MatrixInt&, MatrixInt&,
            MatrixInt&, MatrixInt&, MatrixInt&>(&regina::smithNormalForm),
        rdoc::smithNormalForm_2);
    m.def("metricalSmithNormalForm", &regina::metricalSmithNormalForm,
        rdoc::metricalSmithNormalForm);
    m.def("rowBasis", &regina::rowBasis, rdoc::rowBasis);
    m.def("rowBasisAndOrthComp", &regina::rowBasisAndOrthComp,
        rdoc::rowBasisAndOrthComp);
    m.def("columnEchelonForm", &regina::columnEchelonForm,
        rdoc::columnEchelonForm);
    m.def("preImageOfLattice", &regina::preImageOfLattice,
        rdoc::preImageOfLattice);
    m.def("torsionAutInverse", &regina::torsionAutInverse,
        rdoc::torsionAutInverse);

    RDOC_SCOPE_END
}

