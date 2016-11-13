
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
#include "triangulation/homologicaldata.h"
#include "../helpers.h"

using namespace boost::python;
using regina::HomologicalData;
using regina::Triangulation;

void addHomologicalData() {
    class_<HomologicalData, std::auto_ptr<HomologicalData>,
            boost::noncopyable>
            ("HomologicalData", init<const Triangulation<3>&>())
        .def(init<const HomologicalData&>())
        .def("homology", &HomologicalData::homology,
            return_internal_reference<>())
        .def("bdryHomology", &HomologicalData::bdryHomology,
            return_internal_reference<>())
        .def("bdryHomologyMap", &HomologicalData::bdryHomologyMap,
            return_internal_reference<>())
        .def("dualHomology", &HomologicalData::dualHomology,
            return_internal_reference<>())
        .def("h1CellAp", &HomologicalData::h1CellAp,
            return_internal_reference<>())
        .def("countStandardCells", &HomologicalData::countStandardCells)
        .def("countDualCells", &HomologicalData::countDualCells)
        .def("countBdryCells", &HomologicalData::countBdryCells)
        .def("eulerChar", &HomologicalData::eulerChar)
        .def("torsionRankVectorString",
            &HomologicalData::torsionRankVectorString,
            return_value_policy<copy_const_reference>())
        .def("torsionSigmaVectorString",
            &HomologicalData::torsionSigmaVectorString,
            return_value_policy<copy_const_reference>())
        .def("torsionLegendreSymbolVectorString",
            &HomologicalData::torsionLegendreSymbolVectorString,
            return_value_policy<copy_const_reference>())
        .def("formIsHyperbolic", &HomologicalData::formIsHyperbolic)
        .def("formIsSplit", &HomologicalData::formIsSplit)
        .def("formSatKK", &HomologicalData::formSatKK)
        .def("embeddabilityComment",
            &HomologicalData::embeddabilityComment,
            return_value_policy<copy_const_reference>())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NHomologicalData") = scope().attr("HomologicalData");
}

