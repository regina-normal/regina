
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
#include "hypersurface/normalhypersurfaces.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "../helpers.h"
#include "../docstrings/hypersurface/normalhypersurface.h"

using regina::NormalHypersurface;
using regina::Triangulation;

void addNormalHypersurface(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(NormalHypersurface)

    auto c = pybind11::class_<NormalHypersurface>(m, "NormalHypersurface",
            rdoc_scope)
        .def(pybind11::init<const NormalHypersurface&>(), rdoc::__init)
        .def(pybind11::init<const NormalHypersurface&,
            const Triangulation<4>&>(), rdoc::__init_2)
        .def(pybind11::init<const Triangulation<4>&, regina::HyperEncoding,
            const regina::Vector<regina::LargeInteger>&>(), rdoc::__init_3)
        .def(pybind11::init<const Triangulation<4>&, regina::HyperCoords,
            const regina::Vector<regina::LargeInteger>&>(), rdoc::__init_4)
        .def(pybind11::init([](const Triangulation<4>& t,
                regina::HyperEncoding enc, pybind11::list values) {
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
            return new NormalHypersurface(t, enc, std::move(v));
        }), rdoc::__init_3)
        .def(pybind11::init([](const Triangulation<4>& t,
                regina::HyperCoords coords, pybind11::list values) {
            regina::HyperEncoding enc(coords);
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
            return new NormalHypersurface(t, enc, std::move(v));
        }), rdoc::__init_4)
        .def("swap", &NormalHypersurface::swap, rdoc::swap)
        .def("doubleHypersurface", [](const NormalHypersurface& s) {
            // This is deprecated, so we reimplement it ourselves.
            return s * 2;
        }, rdoc::doubleHypersurface)
        .def("tetrahedra", &NormalHypersurface::tetrahedra, rdoc::tetrahedra)
        .def("prisms", &NormalHypersurface::prisms, rdoc::prisms)
        .def("edgeWeight", &NormalHypersurface::edgeWeight, rdoc::edgeWeight)
        .def("triangulation", &NormalHypersurface::triangulation,
            pybind11::return_value_policy::reference_internal,
            rdoc::triangulation)
        .def("name", &NormalHypersurface::name, rdoc::name)
        .def("setName", &NormalHypersurface::setName, rdoc::setName)
        .def("writeXMLData", [](const NormalHypersurface& s,
                pybind11::object file, regina::FileFormat f,
                const regina::NormalHypersurfaces* list) {
            pybind11::scoped_ostream_redirect stream(std::cout, file);
            s.writeXMLData(std::cout, f, list);
        }, rdoc::writeXMLData)
        .def("isEmpty", &NormalHypersurface::isEmpty, rdoc::isEmpty)
        .def("isCompact", &NormalHypersurface::isCompact, rdoc::isCompact)
        .def("isOrientable", &NormalHypersurface::isOrientable,
            rdoc::isOrientable)
        .def("isTwoSided", &NormalHypersurface::isTwoSided, rdoc::isTwoSided)
        .def("isConnected", &NormalHypersurface::isConnected, rdoc::isConnected)
        .def("hasRealBoundary", &NormalHypersurface::hasRealBoundary,
            rdoc::hasRealBoundary)
        .def("isVertexLinking", &NormalHypersurface::isVertexLinking,
            rdoc::isVertexLinking)
        .def("isVertexLink", &NormalHypersurface::isVertexLink,
            pybind11::return_value_policy::reference, rdoc::isVertexLink)
        .def("isThinEdgeLink", &NormalHypersurface::isThinEdgeLink,
            pybind11::return_value_policy::reference, rdoc::isThinEdgeLink)
        .def("isNormalEdgeLink", &NormalHypersurface::isNormalEdgeLink,
            pybind11::return_value_policy::reference, rdoc::isNormalEdgeLink)
        .def("isThinTriangleLink", &NormalHypersurface::isThinTriangleLink,
            pybind11::return_value_policy::reference, rdoc::isThinTriangleLink)
        .def("isNormalTriangleLink", &NormalHypersurface::isNormalTriangleLink,
            pybind11::return_value_policy::reference,
            rdoc::isNormalTriangleLink)
        .def("isThinTetrahedronLink",
            &NormalHypersurface::isThinTetrahedronLink,
            pybind11::return_value_policy::reference,
            rdoc::isThinTetrahedronLink)
        .def("isNormalTetrahedronLink",
            &NormalHypersurface::isNormalTetrahedronLink,
            pybind11::return_value_policy::reference,
            rdoc::isNormalTetrahedronLink)
        .def("homology", &NormalHypersurface::homology,
            pybind11::return_value_policy::reference_internal, rdoc::homology)
        .def("triangulate", &NormalHypersurface::triangulate, rdoc::triangulate)
        .def("embedded", &NormalHypersurface::embedded, rdoc::embedded)
        .def("locallyCompatible",
            &NormalHypersurface::locallyCompatible, rdoc::locallyCompatible)
        .def("vector", &NormalHypersurface::vector,
            pybind11::return_value_policy::reference_internal, rdoc::vector)
        .def("encoding", &NormalHypersurface::encoding, rdoc::encoding)
        .def("scaleDown", &NormalHypersurface::scaleDown, rdoc::scaleDown)
        .def_static("reconstructTetrahedra",
            &NormalHypersurface::reconstructTetrahedra,
            rdoc::reconstructTetrahedra)
        .def(pybind11::self + pybind11::self, rdoc::__add)
        .def(pybind11::self * regina::LargeInteger(), rdoc::__mul)
        .def(pybind11::self *= regina::LargeInteger(), rdoc::__imul)
        .def(pybind11::self < pybind11::self, rdoc::__lt)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);

    regina::python::add_global_swap<NormalHypersurface>(m, rdoc_global::swap);

    RDOC_SCOPE_END
}

