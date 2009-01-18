
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

#include "surfaces/nnormalsurface.h"
#include "triangulation/ntriangulation.h"
#include "../globalarray.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NNormalSurface;
using regina::python::GlobalArray;
using regina::python::GlobalArray2D;
using regina::python::GlobalArray3D;

namespace {
    const char* const vertexSplitString_1D[3] = {
        regina::vertexSplitString[0],
        regina::vertexSplitString[1],
        regina::vertexSplitString[2]
    };

    GlobalArray2D<int> vertexSplit_arr(regina::vertexSplit, 4);
    GlobalArray3D<int> vertexSplitMeeting_arr(regina::vertexSplitMeeting, 4);
    GlobalArray2D<int> vertexSplitDefn_arr(regina::vertexSplitDefn, 3);
    GlobalArray2D<int> vertexSplitPartner_arr(regina::vertexSplitPartner, 3);
    GlobalArray<const char*> vertexSplitString_arr(vertexSplitString_1D, 3);
    GlobalArray2D<regina::NPerm> triDiscArcs_arr(regina::__triDiscArcs, 4, 3);
    GlobalArray2D<regina::NPerm> quadDiscArcs_arr(regina::__quadDiscArcs, 3, 4);
    GlobalArray2D<regina::NPerm> octDiscArcs_arr(regina::__octDiscArcs, 3, 8);

    void writeTextShort_stdio(const NNormalSurface& s) {
        s.writeTextShort(std::cout);
    }
    void writeRawVector_stdio(const NNormalSurface& s) {
        s.writeRawVector(std::cout);
    }
    boost::python::tuple isThinEdgeLink_tuple(const NNormalSurface& s) {
        std::pair<const regina::NEdge*, const regina::NEdge*> ans =
            s.isThinEdgeLink();
        return make_tuple(boost::python::ptr(ans.first),
            boost::python::ptr(ans.second));
    }
}

void addNNormalSurface() {
    class_<NNormalSurface, bases<regina::ShareableObject>,
            std::auto_ptr<NNormalSurface>, boost::noncopyable>
            ("NNormalSurface", no_init)
        .def("clone", &NNormalSurface::clone,
            return_value_policy<manage_new_object>())
        .def("doubleSurface", &NNormalSurface::doubleSurface,
            return_value_policy<manage_new_object>())
        .def("getTriangleCoord", &NNormalSurface::getTriangleCoord)
        .def("getQuadCoord", &NNormalSurface::getQuadCoord)
        .def("getOctCoord", &NNormalSurface::getOctCoord)
        .def("getEdgeWeight", &NNormalSurface::getEdgeWeight)
        .def("getFaceArcs", &NNormalSurface::getFaceArcs)
        .def("getNumberOfCoords", &NNormalSurface::getNumberOfCoords)
        .def("getTriangulation", &NNormalSurface::getTriangulation,
            return_value_policy<reference_existing_object>())
        .def("getName", &NNormalSurface::getName,
            return_value_policy<return_by_value>())
        .def("setName", &NNormalSurface::setName)
        .def("writeTextShort", writeTextShort_stdio)
        .def("writeRawVector", writeRawVector_stdio)
        .def("isEmpty", &NNormalSurface::isEmpty)
        .def("isCompact", &NNormalSurface::isCompact)
        .def("getEulerCharacteristic", &NNormalSurface::getEulerCharacteristic)
        .def("isOrientable", &NNormalSurface::isOrientable)
        .def("isTwoSided", &NNormalSurface::isTwoSided)
        .def("isConnected", &NNormalSurface::isConnected)
        .def("hasRealBoundary", &NNormalSurface::hasRealBoundary)
        .def("isVertexLinking", &NNormalSurface::isVertexLinking)
        .def("isVertexLink", &NNormalSurface::isVertexLink,
            return_value_policy<reference_existing_object>())
        .def("isThinEdgeLink", isThinEdgeLink_tuple)
        .def("isSplitting", &NNormalSurface::isSplitting)
        .def("isCentral", &NNormalSurface::isCentral)
        .def("cutAlong", &NNormalSurface::cutAlong,
            return_value_policy<manage_new_object>())
        .def("crush", &NNormalSurface::crush,
            return_value_policy<manage_new_object>())
        .def("sameSurface", &NNormalSurface::sameSurface)
        .def("locallyCompatible", &NNormalSurface::locallyCompatible)
        .def("disjoint", &NNormalSurface::disjoint)
        .def("findNonTrivialSphere", &NNormalSurface::findNonTrivialSphere,
            return_value_policy<manage_new_object>())
        .def("findVtxOctAlmostNormalSphere",
            &NNormalSurface::findVtxOctAlmostNormalSphere,
            return_value_policy<manage_new_object>())
        .staticmethod("findNonTrivialSphere")
        .staticmethod("findVtxOctAlmostNormalSphere")
    ;

    // Global arrays:
    scope().attr("vertexSplit") = &vertexSplit_arr;
    scope().attr("vertexSplitMeeting") = &vertexSplitMeeting_arr;
    scope().attr("vertexSplitDefn") = &vertexSplitDefn_arr;
    scope().attr("vertexSplitPartner") = &vertexSplitPartner_arr;
    scope().attr("vertexSplitString") = &vertexSplitString_arr;
    scope().attr("triDiscArcs") = &triDiscArcs_arr;
    scope().attr("quadDiscArcs") = &quadDiscArcs_arr;
    scope().attr("octDiscArcs") = &octDiscArcs_arr;
}

