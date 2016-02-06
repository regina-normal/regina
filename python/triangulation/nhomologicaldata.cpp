
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "triangulation/nhomologicaldata.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NHomologicalData;
using regina::NTriangulation;

void addNHomologicalData() {
    class_<NHomologicalData, std::auto_ptr<NHomologicalData>,
            boost::noncopyable>
            ("NHomologicalData", init<const NTriangulation&>())
        .def(init<const NHomologicalData&>())
        .def("homology", &NHomologicalData::homology,
            return_internal_reference<>())
        .def("getHomology", &NHomologicalData::homology,
            return_internal_reference<>())
        .def("bdryHomology", &NHomologicalData::bdryHomology,
            return_internal_reference<>())
        .def("getBdryHomology", &NHomologicalData::bdryHomology,
            return_internal_reference<>())
        .def("bdryHomologyMap", &NHomologicalData::bdryHomologyMap,
            return_internal_reference<>())
        .def("getBdryHomologyMap", &NHomologicalData::bdryHomologyMap,
            return_internal_reference<>())
        .def("dualHomology", &NHomologicalData::dualHomology,
            return_internal_reference<>())
        .def("getDualHomology", &NHomologicalData::dualHomology,
            return_internal_reference<>())
        .def("h1CellAp", &NHomologicalData::h1CellAp,
            return_internal_reference<>())
        .def("getH1CellAp", &NHomologicalData::h1CellAp,
            return_internal_reference<>())
        .def("countStandardCells", &NHomologicalData::countStandardCells)
        .def("getNumStandardCells", &NHomologicalData::countStandardCells)
        .def("countDualCells", &NHomologicalData::countDualCells)
        .def("getNumDualCells", &NHomologicalData::countDualCells)
        .def("countBdryCells", &NHomologicalData::countBdryCells)
        .def("getNumBdryCells", &NHomologicalData::countBdryCells)
        .def("eulerChar", &NHomologicalData::eulerChar)
        .def("getEulerChar", &NHomologicalData::eulerChar)
        .def("torsionRankVectorString",
            &NHomologicalData::torsionRankVectorString,
            return_value_policy<copy_const_reference>())
        .def("getTorsionRankVectorString",
            &NHomologicalData::torsionRankVectorString,
            return_value_policy<copy_const_reference>())
        .def("torsionSigmaVectorString",
            &NHomologicalData::torsionSigmaVectorString,
            return_value_policy<copy_const_reference>())
        .def("getTorsionSigmaVectorString",
            &NHomologicalData::torsionSigmaVectorString,
            return_value_policy<copy_const_reference>())
        .def("torsionLegendreSymbolVectorString",
            &NHomologicalData::torsionLegendreSymbolVectorString,
            return_value_policy<copy_const_reference>())
        .def("getTorsionLegendreSymbolVectorString",
            &NHomologicalData::torsionLegendreSymbolVectorString,
            return_value_policy<copy_const_reference>())
        .def("formIsHyperbolic", &NHomologicalData::formIsHyperbolic)
        .def("formIsSplit", &NHomologicalData::formIsSplit)
        .def("formSatKK", &NHomologicalData::formSatKK)
        .def("embeddabilityComment",
            &NHomologicalData::embeddabilityComment,
            return_value_policy<copy_const_reference>())
        .def("getEmbeddabilityComment",
            &NHomologicalData::embeddabilityComment,
            return_value_policy<copy_const_reference>())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
}

