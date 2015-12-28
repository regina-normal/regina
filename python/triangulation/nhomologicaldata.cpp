
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

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
        .def("getHomology", &NHomologicalData::getHomology,
            return_internal_reference<>())
        .def("getBdryHomology", &NHomologicalData::getBdryHomology,
            return_internal_reference<>())
        .def("getBdryHomologyMap", &NHomologicalData::getBdryHomologyMap,
            return_internal_reference<>())
        .def("getDualHomology", &NHomologicalData::getDualHomology,
            return_internal_reference<>())
        .def("getH1CellAp", &NHomologicalData::getH1CellAp,
            return_internal_reference<>())
        .def("getNumStandardCells", &NHomologicalData::getNumStandardCells)
        .def("getNumDualCells", &NHomologicalData::getNumDualCells)
        .def("getNumBdryCells", &NHomologicalData::getNumBdryCells)
        .def("getEulerChar", &NHomologicalData::getEulerChar)
        .def("getTorsionRankVectorString",
            &NHomologicalData::getTorsionRankVectorString,
            return_value_policy<copy_const_reference>())
        .def("getTorsionSigmaVectorString",
            &NHomologicalData::getTorsionSigmaVectorString,
            return_value_policy<copy_const_reference>())
        .def("getTorsionLegendreSymbolVectorString",
            &NHomologicalData::getTorsionLegendreSymbolVectorString,
            return_value_policy<copy_const_reference>())
        .def("formIsHyperbolic", &NHomologicalData::formIsHyperbolic)
        .def("formIsSplit", &NHomologicalData::formIsSplit)
        .def("formSatKK", &NHomologicalData::formSatKK)
        .def("getEmbeddabilityComment",
            &NHomologicalData::getEmbeddabilityComment,
            return_value_policy<copy_const_reference>())
        .def("str", &NHomologicalData::str)
        .def("toString", &NHomologicalData::toString)
        .def("detail", &NHomologicalData::detail)
        .def("toStringLong", &NHomologicalData::toStringLong)
        .def("__str__", &NHomologicalData::str)
        .def(regina::python::add_eq_operators())
    ;
}

