
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

#include "../pybind11/pybind11.h"
#include "hypersurface/normalhypersurfaces.h"
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "triangulation/dim4.h"
#include "utilities/safeptr.h"
#include "../helpers.h"

using regina::HyperCoords;
using regina::NormalHypersurfaces;

void addNormalHypersurfaces(pybind11::module_& m) {
    m.def("makeMatchingEquations", regina::makeMatchingEquations);

    pybind11::class_<NormalHypersurfaces, regina::Packet,
            regina::SafePtr<NormalHypersurfaces>>(m, "NormalHypersurfaces")
        .def_static("enumerate", &NormalHypersurfaces::enumerate,
            pybind11::arg(), pybind11::arg(),
            pybind11::arg("which") = regina::HS_LIST_DEFAULT,
            pybind11::arg("algHints") = regina::HS_ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr)
        .def("recreateMatchingEquations",
            &NormalHypersurfaces::recreateMatchingEquations)
        .def("coords", &NormalHypersurfaces::coords)
        .def("which", &NormalHypersurfaces::which)
        .def("algorithm", &NormalHypersurfaces::algorithm)
        .def("isEmbeddedOnly", &NormalHypersurfaces::isEmbeddedOnly)
        .def("triangulation", &NormalHypersurfaces::triangulation)
        .def("size", &NormalHypersurfaces::size)
        .def("hypersurface", &NormalHypersurfaces::hypersurface,
            pybind11::return_value_policy::reference_internal)
        .def_property_readonly_static("typeID", [](pybind11::object) {
            // We cannot take the address of typeID, so use a getter function.
            return NormalHypersurfaces::typeID;
        })
    ;

    m.attr("NNormalHypersurfaceList") = m.attr("NormalHypersurfaces");
}

