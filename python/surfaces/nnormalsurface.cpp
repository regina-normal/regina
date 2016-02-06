
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

#include "maths/nmatrixint.h"
#include "surfaces/nnormalsurface.h"
#include "surfaces/nnormalsurfacelist.h" // for makeZeroVector()
#include "triangulation/ntriangulation.h"
#include "../globalarray.h"
#include "../helpers.h"
#include "../safeheldtype.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::NNormalSurface;
using regina::NTriangulation;
using regina::python::GlobalArray;
using regina::python::GlobalArray2D;
using regina::python::GlobalArray3D;

namespace {
    const char* const quadString_1D[3] = {
        regina::quadString[0],
        regina::quadString[1],
        regina::quadString[2]
    };

    GlobalArray2D<int> quadSeparating_arr(regina::quadSeparating, 4);
    GlobalArray3D<int> quadMeeting_arr(regina::quadMeeting, 4);
    GlobalArray2D<int> vertexSplitDefn_arr(regina::vertexSplitDefn, 3);
    GlobalArray2D<int> quadPartner_arr(regina::quadPartner, 3);
    GlobalArray<const char*> quadString_arr(quadString_1D, 3);
    GlobalArray2D<regina::NPerm4> triDiscArcs_arr(regina::__triDiscArcs, 4, 3);
    GlobalArray2D<regina::NPerm4> quadDiscArcs_arr(regina::__quadDiscArcs, 3, 4);
    GlobalArray2D<regina::NPerm4> octDiscArcs_arr(regina::__octDiscArcs, 3, 8);

    /**
     * A python-only constructor that lets users build a normal surface
     * from a hand-crafted list of integers.
     */
    NNormalSurface* fromCoordinates(NTriangulation* t,
            regina::NormalCoords coords, boost::python::list values) {
        regina::NNormalSurfaceVector* v = regina::makeZeroVector(t, coords);

        long len = boost::python::len(values);
        if (len != v->size()) {
            delete v;

            PyErr_SetString(PyExc_ValueError,
                "Incorrect number of normal coordinates");
            ::boost::python::throw_error_already_set();
        }

        for (long i = 0; i < len; i++) {
            // Accept any type that we know how to convert to a large
            // integer.
            extract<regina::NLargeInteger&> x_large(values[i]);
            if (x_large.check()) {
                v->setElement(i, x_large());
                continue;
            }
            extract<long> x_long(values[i]);
            if (x_long.check()) {
                v->setElement(i, x_long());
                continue;
            }
            extract<const char*> x_str(values[i]);
            if (x_str.check()) {
                v->setElement(i, x_str());
                continue;
            }

            // Throw an exception.
            x_large();
        }

        return new NNormalSurface(t, v);
    }

    void writeRawVector_stdio(const NNormalSurface& s) {
        s.writeRawVector(std::cout);
    }
    boost::python::tuple isThinEdgeLink_tuple(const NNormalSurface& s) {
        std::pair<const regina::NEdge*, const regina::NEdge*> ans =
            s.isThinEdgeLink();
        return boost::python::make_tuple(boost::python::ptr(ans.first),
            boost::python::ptr(ans.second));
    }

    NNormalSurface* findVtxOctAlmostNormalSphere1(NTriangulation* t) {
        return NNormalSurface::findVtxOctAlmostNormalSphere(t);
    }
    NNormalSurface* findVtxOctAlmostNormalSphere2(NTriangulation* t, bool b) {
        return NNormalSurface::findVtxOctAlmostNormalSphere(t, b);
    }

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_isCompressingDisc,
        NNormalSurface::isCompressingDisc, 0, 1);
}

void addNNormalSurface() {
    class_<NNormalSurface, std::auto_ptr<NNormalSurface>, boost::noncopyable>
            ("NNormalSurface", no_init)
        .def("__init__", make_constructor(fromCoordinates))
        .def("clone", &NNormalSurface::clone,
            return_value_policy<manage_new_object>())
        .def("doubleSurface", &NNormalSurface::doubleSurface,
            return_value_policy<manage_new_object>())
        .def("triangles", &NNormalSurface::triangles)
        .def("getTriangleCoord", &NNormalSurface::triangles)
        .def("orientedTriangles", &NNormalSurface::orientedTriangles)
        .def("getOrientedTriangleCoord", &NNormalSurface::orientedTriangles)
        .def("quads", &NNormalSurface::quads)
        .def("getQuadCoord", &NNormalSurface::quads)
        .def("orientedQuads", &NNormalSurface::orientedQuads)
        .def("getOrientedQuadCoord", &NNormalSurface::orientedQuads)
        .def("octs", &NNormalSurface::octs)
        .def("getOctCoord", &NNormalSurface::octs)
        .def("edgeWeight", &NNormalSurface::edgeWeight)
        .def("getEdgeWeight", &NNormalSurface::edgeWeight)
        .def("getFaceArcs", &NNormalSurface::arcs)
        .def("arcs", &NNormalSurface::arcs)
        .def("getTriangleArcs", &NNormalSurface::arcs)
        .def("octPosition", &NNormalSurface::octPosition)
        .def("getOctPosition", &NNormalSurface::octPosition)
        .def("countCoords", &NNormalSurface::countCoords)
        .def("getNumberOfCoords", &NNormalSurface::countCoords)
        .def("triangulation", &NNormalSurface::triangulation,
            return_value_policy<to_held_type<> >())
        .def("getTriangulation", &NNormalSurface::triangulation,
            return_value_policy<to_held_type<> >())
        .def("name", &NNormalSurface::name,
            return_value_policy<return_by_value>())
        .def("getName", &NNormalSurface::name,
            return_value_policy<return_by_value>())
        .def("setName", &NNormalSurface::setName)
        .def("writeRawVector", writeRawVector_stdio)
        .def("isEmpty", &NNormalSurface::isEmpty)
        .def("isCompact", &NNormalSurface::isCompact)
        .def("eulerChar", &NNormalSurface::eulerChar)
        .def("getEulerChar", &NNormalSurface::eulerChar)
        .def("getEulerCharacteristic", &NNormalSurface::eulerChar)
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
        .def("isCompressingDisc", &NNormalSurface::isCompressingDisc,
            OL_isCompressingDisc())
        .def("isIncompressible", &NNormalSurface::isIncompressible)
        .def("cutAlong", &NNormalSurface::cutAlong,
            return_value_policy<to_held_type<> >())
        .def("crush", &NNormalSurface::crush,
            return_value_policy<to_held_type<> >())
        .def("sameSurface", &NNormalSurface::sameSurface)
        .def("normal", &NNormalSurface::normal)
        .def("embedded", &NNormalSurface::embedded)
        .def("locallyCompatible", &NNormalSurface::locallyCompatible)
        .def("disjoint", &NNormalSurface::disjoint)
        .def("boundaryIntersections", &NNormalSurface::boundaryIntersections,
            return_value_policy<manage_new_object>())
        .def("findNonTrivialSphere", &NNormalSurface::findNonTrivialSphere,
            return_value_policy<manage_new_object>())
        .def("findVtxOctAlmostNormalSphere", findVtxOctAlmostNormalSphere1,
            return_value_policy<manage_new_object>())
        .def("findVtxOctAlmostNormalSphere", findVtxOctAlmostNormalSphere2,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("findNonTrivialSphere")
        .staticmethod("findVtxOctAlmostNormalSphere")
    ;

    // Global arrays:
    scope().attr("quadSeparating") = &quadSeparating_arr;
    scope().attr("vertexSplit") = &quadSeparating_arr;
    scope().attr("quadMeeting") = &quadMeeting_arr;
    scope().attr("vertexSplitMeeting") = &quadMeeting_arr;
    scope().attr("vertexSplitDefn") = &vertexSplitDefn_arr;
    scope().attr("quadPartner") = &quadPartner_arr;
    scope().attr("vertexSplitPartner") = &quadPartner_arr;
    scope().attr("quadString") = &quadString_arr;
    scope().attr("vertexSplitString") = &quadString_arr;
    scope().attr("triDiscArcs") = &triDiscArcs_arr;
    scope().attr("quadDiscArcs") = &quadDiscArcs_arr;
    scope().attr("octDiscArcs") = &octDiscArcs_arr;
}

