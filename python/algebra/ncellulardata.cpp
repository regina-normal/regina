
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "algebra/ncellulardata.h"

using namespace boost::python;
using regina::NCellularData;
using regina::NTriangulation;

// These are the old NHomologicalData python bindings that I'm 
// attempting to modify to be NCellularData python bindings -RB

void addNCellularData() {
    class_<NCellularData, bases<regina::ShareableObject>,
            std::auto_ptr<NCellularData>, boost::noncopyable>
            ("NCellularData", init<const NTriangulation&>())
        .def(init<const NCellularData&>())
//        .def("standardHomology", &NCellularData::standardHomology,
//            return_internal_reference<>())
//        .def("boundaryHomology", &NCellularData::boundaryHomology,
//            return_internal_reference<>())
//        .def("boundaryHomologyMap", &NCellularData::boundaryHomologyMap,
//            return_internal_reference<>())
//        .def("dualHomology", &NCellularData::dualHomology,
//            return_internal_reference<>())
//        .def("mixedHomology", &NCellularData::mixedHomology, 
//            return_internal_reference<>())
//        .def("standardToMixedHom", &NCellularData::standardToMixedHom, 
//            return_internal_reference<>())
//        .def("dualToMixedHom", &NCellularData::dualToMixedHom, 
//            return_internal_reference<>())
//        .def("fastDualToStandardH1", &NCellularData::fastDualToStandardH1,
//            return_internal_reference<>())
//        .def("standardCellCount", &NCellularData::standardCellCount)
//        .def("dualCellCount", &NCellularData::dualCellCount)
//        .def("boundaryCellCount", &NCellularData::boundaryCellCount)
//        .def("mixedCellCount", &NCellularData::mixedCellCount)
        .def("eulerChar", &NCellularData::eulerChar)
//        .def("torsionRankVectorString",
//            &NCellularData::torsionRankVectorString,
//            return_value_policy<copy_const_reference>())
//        .def("torsionSigmaVectorString",
//            &NCellularData::torsionSigmaVectorString,
//            return_value_policy<copy_const_reference>())
//        .def("torsionLegendreSymbolVectorString",
//            &NCellularData::torsionLegendreSymbolVectorString,
//            return_value_policy<copy_const_reference>())
//        .def("formIsHyperbolic", &NCellularData::formIsHyperbolic)
//        .def("formIsSplit", &NCellularData::formIsSplit)
//        .def("formSatKK", &NCellularData::formSatKK)
//        .def("embeddabilityComment",
//            &NCellularData::embeddabilityComment,
//            return_value_policy<copy_const_reference>())
//        .def("verifyChainComplexes", &NCellularData::verifyChainComplexes)
//        .def("verifyCoordinateIsomorphisms", &NCellularData::verifyCoordinateIsomorphisms)
//        .def("imgH2form", &NCellularData::imgH2form,    // no rush on implementing this as 
//            return_internal_reference<>())                 // I'll probably change the internal rep
    ;
}

