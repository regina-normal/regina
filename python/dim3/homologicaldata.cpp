
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
#include "../pybind11/stl.h"
#include "triangulation/homologicaldata.h"
#include "../helpers.h"

using regina::HomologicalData;
using regina::Triangulation;

void addHomologicalData(pybind11::module_& m) {
    auto c = pybind11::class_<HomologicalData>(m, "HomologicalData")
        .def(pybind11::init<const Triangulation<3>&>())
        .def(pybind11::init<const HomologicalData&>())
        .def("homology", &HomologicalData::homology,
            pybind11::return_value_policy::reference_internal)
        .def("bdryHomology", &HomologicalData::bdryHomology,
            pybind11::return_value_policy::reference_internal)
        .def("bdryHomologyMap", &HomologicalData::bdryHomologyMap,
            pybind11::return_value_policy::reference_internal)
        .def("dualHomology", &HomologicalData::dualHomology,
            pybind11::return_value_policy::reference_internal)
        .def("h1CellAp", &HomologicalData::h1CellAp,
            pybind11::return_value_policy::reference_internal)
        .def("countStandardCells", &HomologicalData::countStandardCells)
        .def("countDualCells", &HomologicalData::countDualCells)
        .def("countBdryCells", &HomologicalData::countBdryCells)
        .def("eulerChar", &HomologicalData::eulerChar)
        .def("torsionRankVector", &HomologicalData::torsionRankVector)
        .def("torsionRankVectorString",
            &HomologicalData::torsionRankVectorString)
        .def("torsionSigmaVector", &HomologicalData::torsionSigmaVector)
        .def("torsionSigmaVectorString",
            &HomologicalData::torsionSigmaVectorString)
        .def("torsionLegendreSymbolVector",
            &HomologicalData::torsionLegendreSymbolVector)
        .def("torsionLegendreSymbolVectorString",
            &HomologicalData::torsionLegendreSymbolVectorString)
        .def("formIsHyperbolic", &HomologicalData::formIsHyperbolic)
        .def("formIsSplit", &HomologicalData::formIsSplit)
        .def("formSatKK", &HomologicalData::formSatKK)
        .def("embeddabilityComment",
            &HomologicalData::embeddabilityComment)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("NHomologicalData") = m.attr("HomologicalData");
}

