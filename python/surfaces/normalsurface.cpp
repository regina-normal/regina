
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
#include "../pybind11/operators.h"
#include "maths/matrix.h"
#include "surfaces/coordregistry.h"
#include "surfaces/normalsurface.h"
#include "triangulation/dim3.h"
#include "../globalarray.h"
#include "../helpers.h"

using regina::NormalSurface;
using regina::NormalSurfaceVector;
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
    GlobalArray2D<regina::Perm<4>> triDiscArcs_arr(regina::triDiscArcs, 4);
    GlobalArray2D<regina::Perm<4>> quadDiscArcs_arr(regina::quadDiscArcs, 3);
    GlobalArray2D<regina::Perm<4>> octDiscArcs_arr(regina::octDiscArcs, 3);
}

void addNormalSurface(pybind11::module_& m) {
    auto v = pybind11::class_<NormalSurfaceVector>(m, "NormalSurfaceVector")
        .def("coords", &NormalSurfaceVector::coords)
        .def("clone", &NormalSurfaceVector::clone)
        .def("size", &NormalSurfaceVector::size)
        .def("__getitem__", [](const NormalSurfaceVector& v, size_t index) {
            return v[index];
        }, pybind11::return_value_policy::reference_internal)
        .def("set", &NormalSurfaceVector::set)
        // We cannot just use pybind11::self += pybind11:;self, because
        // in that case pybind11 seems to want to return by value.
        .def("__iadd__", &NormalSurfaceVector::operator +=,
            pybind11::return_value_policy::reference_internal)
        .def("scaleDown", &NormalSurfaceVector::scaleDown)
        .def("allowsAlmostNormal", &NormalSurfaceVector::allowsAlmostNormal)
        .def("allowsSpun", &NormalSurfaceVector::allowsSpun)
        .def("allowsOriented", &NormalSurfaceVector::allowsOriented)
        .def("hasMultipleOctDiscs", &NormalSurfaceVector::hasMultipleOctDiscs)
        .def("isCompact", &NormalSurfaceVector::isCompact)
        .def("isVertexLinking", &NormalSurfaceVector::isVertexLinking)
        .def("isVertexLink", &NormalSurfaceVector::isVertexLink)
        .def("isThinEdgeLink", &NormalSurfaceVector::isThinEdgeLink)
        .def("isSplitting", &NormalSurfaceVector::isSplitting)
        .def("isCentral", &NormalSurfaceVector::isCentral)
        .def("triangles", &NormalSurfaceVector::triangles)
        .def("orientedTriangles",
        &NormalSurfaceVector::orientedTriangles)
        .def("quads", &NormalSurfaceVector::quads)
        .def("orientedQuads", &NormalSurfaceVector::orientedQuads)
        .def("octs", &NormalSurfaceVector::octs)
        .def("edgeWeight", &NormalSurfaceVector::edgeWeight)
        .def("arcs", &NormalSurfaceVector::arcs)
    ;
    regina::python::add_output(v, true /* __repr__ */);
    regina::python::add_eq_operators(v);

    auto c = pybind11::class_<NormalSurface>(m, "NormalSurface")
        .def(pybind11::init<const NormalSurface&>())
        .def(pybind11::init<const NormalSurface&, const Triangulation<3>&>())
        .def(pybind11::init([](Triangulation<3>& t, regina::NormalCoords coords,
                pybind11::list values) {
            regina::NormalSurfaceVector* v = forCoords(coords, [&](auto info) {
                typedef decltype(info) Coords;
                return static_cast<NormalSurfaceVector*>(
                    new typename Coords::Class(Coords::dimension(t.size())));
            }, nullptr);
            if (values.size() != v->size()) {
                delete v;
                throw pybind11::index_error(
                    "Incorrect number of normal coordinates");
            }
            try {
                // Accept any types that we know how to convert to a large
                // integer.
                for (size_t i = 0; i < v->size(); ++i)
                    v->set(i, values[i].cast<regina::LargeInteger>());
            } catch (pybind11::cast_error const &) {
                delete v;
                throw std::invalid_argument(
                    "List element not convertible to LargeInteger");
            }
            return new NormalSurface(t, v);
        }))
        .def("clone", [](const NormalSurface& s) {
            // Since clone() is deprecated, we reimplement it here to
            // avoid noisy compiler warnings.
            // Here we use the copy constructor, which has the side-effect of
            // cloning the surface name also (which the C++ clone() does not).
            // To ensure no change in behaviour, we revert the name change here.
            NormalSurface* ans = new NormalSurface(s);
            ans->setName(std::string());
            return ans;
        })
        .def("swap", &NormalSurface::swap)
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
        .def("isIncompressible", &NormalSurface::isIncompressible)
        .def("cutAlong", &NormalSurface::cutAlong)
        .def("crush", &NormalSurface::crush)
        .def("sameSurface", &NormalSurface::sameSurface)
        .def("normal", &NormalSurface::normal)
        .def("embedded", &NormalSurface::embedded)
        .def("locallyCompatible", &NormalSurface::locallyCompatible)
        .def("disjoint", &NormalSurface::disjoint)
        .def("boundaryIntersections", &NormalSurface::boundaryIntersections)
        .def("vector", &NormalSurface::vector,
            pybind11::return_value_policy::reference_internal)
        .def("rawVector", [](const NormalSurface& s) {
            // Since rawVector() is deprecated, we reimplement it
            // ourselves here to avoid noisy compiler warnings.
            return s.vector().coords();
        }, pybind11::return_value_policy::reference_internal)
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

    m.def("swap", (void(*)(NormalSurface&, NormalSurface&))(regina::swap));
}

