
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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
#include "maths/matrix.h"
#include "surfaces/normalsurface.h"
#include "surfaces/normalsurfaces.h" // for makeZeroVector()
#include "triangulation/dim3.h"
#include "../globalarray.h"
#include "../helpers.h"

using regina::NormalSurface;
using regina::Triangulation;
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
    GlobalArray2D<int> quadDefn_arr(regina::quadDefn, 3);
    GlobalArray2D<int> quadPartner_arr(regina::quadPartner, 3);
    GlobalArray<const char*> quadString_arr(quadString_1D, 3);
    GlobalArray2D<regina::Perm<4>> triDiscArcs_arr(regina::__triDiscArcs, 4, 3);
    GlobalArray2D<regina::Perm<4>> quadDiscArcs_arr(regina::__quadDiscArcs, 3, 4);
    GlobalArray2D<regina::Perm<4>> octDiscArcs_arr(regina::__octDiscArcs, 3, 8);
}

void addNormalSurface(pybind11::module& m) {
    auto c = pybind11::class_<NormalSurface>(m, "NormalSurface")
        .def(pybind11::init([](Triangulation<3>* t, regina::NormalCoords coords,
                pybind11::list values) {
            regina::NormalSurfaceVector* v = regina::makeZeroVector(t, coords);
            if (values.size() != v->size()) {
                delete v;
                throw pybind11::index_error(
                    "Incorrect number of normal coordinates");
            }
            try {
                // Accept any types that we know how to convert to a large
                // integer.
                for (size_t i = 0; i < v->size(); ++i)
                    v->setElement(i, values[i].cast<regina::LargeInteger>());
            } catch (pybind11::cast_error const &) {
                delete v;
                throw std::invalid_argument(
                    "List element not convertible to LargeInteger");
            }
            return new NormalSurface(t, v);
        }))
        .def("clone", &NormalSurface::clone)
        .def("doubleSurface", &NormalSurface::doubleSurface)
        .def("triangles", &NormalSurface::triangles)
        .def("orientedTriangles", &NormalSurface::orientedTriangles)
        .def("quads", &NormalSurface::quads)
        .def("orientedQuads", &NormalSurface::orientedQuads)
        .def("octs", &NormalSurface::octs)
        .def("edgeWeight", &NormalSurface::edgeWeight)
        .def("arcs", &NormalSurface::arcs)
        .def("octPosition", &NormalSurface::octPosition)
        .def("countCoords", &NormalSurface::countCoords)
        .def("triangulation", &NormalSurface::triangulation)
        .def("name", &NormalSurface::name)
        .def("setName", &NormalSurface::setName)
        .def("writeRawVector", [](const NormalSurface& s) {
            s.writeRawVector(std::cout);
        })
        .def("isEmpty", &NormalSurface::isEmpty)
        .def("isCompact", &NormalSurface::isCompact)
        .def("eulerChar", &NormalSurface::eulerChar)
        .def("isOrientable", &NormalSurface::isOrientable)
        .def("isTwoSided", &NormalSurface::isTwoSided)
        .def("isConnected", &NormalSurface::isConnected)
        .def("hasRealBoundary", &NormalSurface::hasRealBoundary)
        .def("isVertexLinking", &NormalSurface::isVertexLinking)
        .def("isVertexLink", &NormalSurface::isVertexLink,
            pybind11::return_value_policy::reference)
        .def("isThinEdgeLink", &NormalSurface::isThinEdgeLink,
            pybind11::return_value_policy::reference)
        .def("isSplitting", &NormalSurface::isSplitting)
        .def("isCentral", &NormalSurface::isCentral)
        .def("countBoundaries", &NormalSurface::countBoundaries)
        .def("isCompressingDisc", &NormalSurface::isCompressingDisc,
            pybind11::arg("knownConnected") = false)
        .def("separates", &NormalSurface::separates)
        .def("isEssentialSphere", &NormalSurface::isEssentialSphere)
        .def("isEssentialTorus", &NormalSurface::isEssentialTorus)
        .def("isEssentialKleinBottle", &NormalSurface::isEssentialKleinBottle)
        .def("isSolidTorusAnnulus", &NormalSurface::isSolidTorusAnnulus)
        .def("isIncompressible", &NormalSurface::isIncompressible)
        .def("cutAlong", &NormalSurface::cutAlong)
        .def("crush", &NormalSurface::crush)
        .def("sameSurface", &NormalSurface::sameSurface)
        .def("normal", &NormalSurface::normal)
        .def("embedded", &NormalSurface::embedded)
        .def("locallyCompatible", &NormalSurface::locallyCompatible)
        .def("disjoint", &NormalSurface::disjoint)
        .def("boundaryIntersections", &NormalSurface::boundaryIntersections)
        .def("systemAllowsAlmostNormal",
            &NormalSurface::systemAllowsAlmostNormal)
        .def("systemAllowsSpun", &NormalSurface::systemAllowsSpun)
        .def("systemAllowsOriented", &NormalSurface::systemAllowsOriented)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    // Global arrays:
    m.attr("quadSeparating") = &quadSeparating_arr;
    m.attr("quadMeeting") = &quadMeeting_arr;
    m.attr("quadDefn") = &quadDefn_arr;
    m.attr("quadPartner") = &quadPartner_arr;
    m.attr("quadString") = &quadString_arr;
    m.attr("triDiscArcs") = &triDiscArcs_arr;
    m.attr("quadDiscArcs") = &quadDiscArcs_arr;
    m.attr("octDiscArcs") = &octDiscArcs_arr;
}

