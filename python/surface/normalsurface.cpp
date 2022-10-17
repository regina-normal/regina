
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include "../pybind11/iostream.h"
#include "../pybind11/operators.h"
#include "../pybind11/stl.h"
#include "maths/matrix.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../helpers/tableview.h"
#include "../docstrings/surface/normalsurface.h"

using regina::NormalSurface;
using regina::Triangulation;
using regina::python::wrapTableView;

void addNormalSurface(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(NormalSurface)

    auto c = pybind11::class_<NormalSurface>(m, "NormalSurface", rdoc_scope)
        .def(pybind11::init<const NormalSurface&>(), rdoc::__copy)
        .def(pybind11::init<const NormalSurface&, const Triangulation<3>&>(),
            rdoc::__init)
        .def(pybind11::init<const Triangulation<3>&>(), rdoc::__init_2)
        .def(pybind11::init<const Triangulation<3>&, regina::NormalEncoding,
            const regina::Vector<regina::LargeInteger>&>(), rdoc::__init_3)
        .def(pybind11::init<const Triangulation<3>&, regina::NormalCoords,
            const regina::Vector<regina::LargeInteger>&>(), rdoc::__init_4)
        .def(pybind11::init([](const Triangulation<3>& t,
                regina::NormalEncoding enc, pybind11::list values) {
            regina::Vector<regina::LargeInteger> v(enc.block() * t.size());
            if (values.size() != v.size())
                throw pybind11::index_error(
                    "Incorrect number of normal coordinates");
            try {
                for (size_t i = 0; i < v.size(); ++i)
                    v[i] = values[i].cast<regina::LargeInteger>();
            } catch (pybind11::cast_error const &) {
                throw regina::InvalidArgument(
                    "List element not convertible to LargeInteger");
            }
            return new NormalSurface(t, enc, std::move(v));
        }), rdoc::__init_3)
        .def(pybind11::init([](const Triangulation<3>& t,
                regina::NormalCoords coords, pybind11::list values) {
            regina::NormalEncoding enc(coords);
            regina::Vector<regina::LargeInteger> v(enc.block() * t.size());
            if (values.size() != v.size())
                throw pybind11::index_error(
                    "Incorrect number of normal coordinates");
            try {
                for (size_t i = 0; i < v.size(); ++i)
                    v[i] = values[i].cast<regina::LargeInteger>();
            } catch (pybind11::cast_error const &) {
                throw regina::InvalidArgument(
                    "List element not convertible to LargeInteger");
            }
            return new NormalSurface(t, enc, std::move(v));
        }), rdoc::__init_4)
        .def("swap", &NormalSurface::swap, rdoc::swap)
        .def("doubleSurface", [](const NormalSurface& s) {
            // This is deprecated, so we reimplement it ourselves.
            return s * 2;
        }, rdoc::doubleSurface)
        .def("triangles", &NormalSurface::triangles, rdoc::triangles)
        .def("quads", &NormalSurface::quads, rdoc::quads)
        .def("octs", &NormalSurface::octs, rdoc::octs)
        .def("edgeWeight", &NormalSurface::edgeWeight, rdoc::edgeWeight)
        .def("arcs", &NormalSurface::arcs, rdoc::arcs)
        .def("octPosition", &NormalSurface::octPosition, rdoc::octPosition)
        .def("triangulation", &NormalSurface::triangulation,
            pybind11::return_value_policy::reference_internal,
            rdoc::triangulation)
        .def("name", &NormalSurface::name, rdoc::name)
        .def("setName", &NormalSurface::setName, rdoc::setName)
        .def("writeXMLData", [](const NormalSurface& s, pybind11::object file,
                regina::FileFormat f, const regina::NormalSurfaces* list) {
            pybind11::scoped_ostream_redirect stream(std::cout, file);
            s.writeXMLData(std::cout, f, list);
        }, rdoc::writeXMLData)
        .def("isEmpty", &NormalSurface::isEmpty, rdoc::isEmpty)
        .def("hasMultipleOctDiscs", &NormalSurface::hasMultipleOctDiscs,
            rdoc::hasMultipleOctDiscs)
        .def("isCompact", &NormalSurface::isCompact, rdoc::isCompact)
        .def("eulerChar", &NormalSurface::eulerChar, rdoc::eulerChar)
        .def("isOrientable", &NormalSurface::isOrientable, rdoc::isOrientable)
        .def("isTwoSided", &NormalSurface::isTwoSided, rdoc::isTwoSided)
        .def("isConnected", &NormalSurface::isConnected, rdoc::isConnected)
        .def("hasRealBoundary", &NormalSurface::hasRealBoundary,
            rdoc::hasRealBoundary)
        .def("components", &NormalSurface::components, rdoc::components)
        .def("isVertexLinking", &NormalSurface::isVertexLinking,
            rdoc::isVertexLinking)
        .def("isVertexLink", &NormalSurface::isVertexLink,
            pybind11::return_value_policy::reference, rdoc::isVertexLink)
        .def("isThinEdgeLink", &NormalSurface::isThinEdgeLink,
            pybind11::return_value_policy::reference, rdoc::isThinEdgeLink)
        .def("isNormalEdgeLink", &NormalSurface::isNormalEdgeLink,
            pybind11::return_value_policy::reference, rdoc::isNormalEdgeLink)
        .def("isThinTriangleLink", &NormalSurface::isThinTriangleLink,
            pybind11::return_value_policy::reference, rdoc::isThinTriangleLink)
        .def("isNormalTriangleLink", &NormalSurface::isNormalTriangleLink,
            pybind11::return_value_policy::reference,
            rdoc::isNormalTriangleLink)
        .def("isSplitting", &NormalSurface::isSplitting, rdoc::isSplitting)
        .def("isCentral", &NormalSurface::isCentral, rdoc::isCentral)
        .def("countBoundaries", &NormalSurface::countBoundaries,
            rdoc::countBoundaries)
        .def("isCompressingDisc", &NormalSurface::isCompressingDisc,
            pybind11::arg("knownConnected") = false,
            rdoc::isCompressingDisc)
        .def("isIncompressible", &NormalSurface::isIncompressible,
            rdoc::isIncompressible)
        .def("cutAlong", &NormalSurface::cutAlong, rdoc::cutAlong)
        .def("crush", &NormalSurface::crush, rdoc::crush)
        .def("removeOcts", &NormalSurface::removeOcts, rdoc::removeOcts)
        .def("normal", &NormalSurface::normal, rdoc::normal)
        .def("embedded", &NormalSurface::embedded, rdoc::embedded)
        .def("locallyCompatible", &NormalSurface::locallyCompatible,
            rdoc::locallyCompatible)
        .def("disjoint", &NormalSurface::disjoint, rdoc::disjoint)
        .def("boundaryIntersections", &NormalSurface::boundaryIntersections,
            rdoc::boundaryIntersections)
        .def("vector", &NormalSurface::vector,
            pybind11::return_value_policy::reference_internal, rdoc::vector)
        .def("encoding", &NormalSurface::encoding, rdoc::encoding)
        .def("couldBeAlmostNormal", &NormalSurface::couldBeAlmostNormal,
            rdoc::couldBeAlmostNormal)
        .def("couldBeNonCompact", &NormalSurface::couldBeNonCompact,
            rdoc::couldBeNonCompact)
        .def("scaleDown", &NormalSurface::scaleDown, rdoc::scaleDown)
        .def_static("reconstructTriangles",
            &NormalSurface::reconstructTriangles, rdoc::reconstructTriangles)
        .def(pybind11::self + pybind11::self, rdoc::__add)
        .def(pybind11::self * regina::LargeInteger(), rdoc::__mul)
        .def(pybind11::self *= regina::LargeInteger(), rdoc::__imul)
        .def(pybind11::self < pybind11::self, rdoc::__lt)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);

    regina::python::add_global_swap<NormalSurface>(m, rdoc::global_swap);

    RDOC_SCOPE_END

    // Global arrays:
    m.attr("quadSeparating") = wrapTableView(m, regina::quadSeparating);
    m.attr("quadMeeting") = wrapTableView(m, regina::quadMeeting);
    m.attr("quadDefn") = wrapTableView(m, regina::quadDefn);
    m.attr("quadPartner") = wrapTableView(m, regina::quadPartner);
    m.attr("triDiscArcs") = wrapTableView(m, regina::triDiscArcs);
    m.attr("quadDiscArcs") = wrapTableView(m, regina::quadDiscArcs);
    m.attr("octDiscArcs") = wrapTableView(m, regina::octDiscArcs);

    // Make sure that quadString is treated as a 1-D array of strings, not
    // a 2-D array of chars.
    regina::python::addTableView<char[6], 3>(m);
    m.attr("quadString") = regina::TableView<char[6], 3>(regina::quadString);
}

