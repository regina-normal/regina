
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

#include "triangulation/nhomologicaldata.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NHomologicalData;
using regina::NTriangulation;

void addNHomologicalData() {
    class_<NHomologicalData, bases<regina::ShareableObject>,
            std::auto_ptr<NHomologicalData>, boost::noncopyable>
            ("NHomologicalData", init<const NTriangulation&>())
        .def(init<const NHomologicalData&>())
        .def("standardHomology", &NHomologicalData::standardHomology,
            return_internal_reference<>())
        .def("boundaryHomology", &NHomologicalData::boundaryHomology,
            return_internal_reference<>())
        .def("boundaryHomologyMap", &NHomologicalData::boundaryHomologyMap,
            return_internal_reference<>())
        .def("dualHomology", &NHomologicalData::dualHomology,
            return_internal_reference<>())
        .def("h1CellAp", &NHomologicalData::h1CellAp,
            return_internal_reference<>())
        .def("standardCellCount", &NHomologicalData::standardCellCount)
        .def("dualCellCount", &NHomologicalData::dualCellCount)
        .def("bdryCellCount", &NHomologicalData::bdryCellCount)
        .def("eulerChar", &NHomologicalData::eulerChar)
        .def("torsionRankVectorString",
            &NHomologicalData::torsionRankVectorString,
            return_value_policy<copy_const_reference>())
        .def("torsionSigmaVectorString",
            &NHomologicalData::torsionSigmaVectorString,
            return_value_policy<copy_const_reference>())
        .def("torsionLegendreSymbolVectorString",
            &NHomologicalData::torsionLegendreSymbolVectorString,
            return_value_policy<copy_const_reference>())
        .def("formIsHyperbolic", &NHomologicalData::formIsHyperbolic)
        .def("formIsSplit", &NHomologicalData::formIsSplit)
        .def("formSatKK", &NHomologicalData::formSatKK)
        .def("embeddabilityComment",
            &NHomologicalData::embeddabilityComment,
            return_value_policy<copy_const_reference>())
    ;
}

