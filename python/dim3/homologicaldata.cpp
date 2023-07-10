
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "../pybind11/stl.h"
#include "triangulation/dim3/homologicaldata.h"
#include "../helpers.h"
#include "../docstrings/triangulation/dim3/homologicaldata.h"

using regina::HomologicalData;
using regina::Triangulation;

void addHomologicalData(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(HomologicalData)

    auto c = pybind11::class_<HomologicalData>(m, "HomologicalData", rdoc_scope)
        .def(pybind11::init<const Triangulation<3>&>(), rdoc::__init)
        .def(pybind11::init<const HomologicalData&>(), rdoc::__copy)
        .def("triangulation", &HomologicalData::triangulation,
            pybind11::return_value_policy::reference_internal,
            rdoc::triangulation)
        .def("swap", &HomologicalData::swap, rdoc::swap)
        .def("homology", &HomologicalData::homology,
            pybind11::return_value_policy::reference_internal, rdoc::homology)
        .def("bdryHomology", &HomologicalData::bdryHomology,
            pybind11::return_value_policy::reference_internal,
            rdoc::bdryHomology)
        .def("bdryHomologyMap", &HomologicalData::bdryHomologyMap,
            pybind11::return_value_policy::reference_internal,
            rdoc::bdryHomologyMap)
        .def("dualHomology", &HomologicalData::dualHomology,
            pybind11::return_value_policy::reference_internal,
            rdoc::dualHomology)
        .def("h1CellAp", &HomologicalData::h1CellAp,
            pybind11::return_value_policy::reference_internal, rdoc::h1CellAp)
        .def("countStandardCells", &HomologicalData::countStandardCells,
            rdoc::countStandardCells)
        .def("countDualCells", &HomologicalData::countDualCells,
            rdoc::countDualCells)
        .def("countBdryCells", &HomologicalData::countBdryCells,
            rdoc::countBdryCells)
        .def("eulerChar", &HomologicalData::eulerChar, rdoc::eulerChar)
        .def("torsionRankVector", &HomologicalData::torsionRankVector,
            rdoc::torsionRankVector)
        .def("torsionRankVectorString",
            &HomologicalData::torsionRankVectorString,
            rdoc::torsionRankVectorString)
        .def("torsionSigmaVector", &HomologicalData::torsionSigmaVector,
            rdoc::torsionSigmaVector)
        .def("torsionSigmaVectorString",
            &HomologicalData::torsionSigmaVectorString,
            rdoc::torsionSigmaVectorString)
        .def("torsionLegendreSymbolVector",
            &HomologicalData::torsionLegendreSymbolVector,
            rdoc::torsionLegendreSymbolVector)
        .def("torsionLegendreSymbolVectorString",
            &HomologicalData::torsionLegendreSymbolVectorString,
            rdoc::torsionLegendreSymbolVectorString)
        .def("formIsHyperbolic", &HomologicalData::formIsHyperbolic,
            rdoc::formIsHyperbolic)
        .def("formIsSplit", &HomologicalData::formIsSplit, rdoc::formIsSplit)
        .def("formSatKK", &HomologicalData::formSatKK, rdoc::formSatKK)
        .def("embeddabilityComment",
            &HomologicalData::embeddabilityComment, rdoc::embeddabilityComment)
    ;
    regina::python::add_output(c);
    regina::python::disable_eq_operators(c);

    regina::python::add_global_swap<HomologicalData>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

