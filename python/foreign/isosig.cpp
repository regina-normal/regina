
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
#include "foreign/isosig.h"
#include "link/link.h"
#include "packet/container.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/generic.h"
#include "utilities/typeutils.h"
#include "../helpers.h"
#include "../docstrings/foreign/isosig.h"

using pybind11::overload_cast;

void addForeignIsoSig(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    m.def("readSigList", [](int dimension, const char* filename,
            int colSigs, int colLabels, size_t ignoreLines) {
        if (dimension == 0) {
            return regina::readSigList<regina::Link>(
                filename, colSigs, colLabels, ignoreLines);
        } else {
            try {
                return regina::select_constexpr<2, regina::maxDim() + 1,
                        std::shared_ptr<regina::Container>>(dimension,
                        [=](auto dim) {
                    return regina::readSigList<regina::Triangulation<dim>>(
                        filename, colSigs, colLabels, ignoreLines);
                });
            } catch (const std::runtime_error&) {
                throw regina::InvalidArgument(
                    "The given dimension is out of range.");
            }
        }
    },
    pybind11::arg("dimension"),
    pybind11::arg("filename"),
    pybind11::arg("colSigs") = 0,
    pybind11::arg("colLabels") = -1,
    pybind11::arg("ignoreLines") = 0,
    rdoc::readSigList);

    RDOC_SCOPE_END
}

