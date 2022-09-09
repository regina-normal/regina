
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
#include "hypersurface/normalhypersurfaces.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "../helpers.h"

using regina::NormalHypersurface;
using regina::Triangulation;

void addNormalHypersurface(pybind11::module_& m) {
    auto c = pybind11::class_<NormalHypersurface>(m, "NormalHypersurface")
        .def(pybind11::init<const NormalHypersurface&>())
        .def(pybind11::init<const NormalHypersurface&,
            const Triangulation<4>&>())
        .def(pybind11::init<const Triangulation<4>&, regina::HyperEncoding,
            const regina::Vector<regina::LargeInteger>&>())
        .def(pybind11::init<const Triangulation<4>&, regina::HyperCoords,
            const regina::Vector<regina::LargeInteger>&>())
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
        }))
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
        }))
        .def("swap", &NormalHypersurface::swap)
        .def("doubleHypersurface", [](const NormalHypersurface& s) {
            // This is deprecated, so we reimplement it ourselves.
            return s * 2;
        })
        .def("tetrahedra", &NormalHypersurface::tetrahedra)
        .def("prisms", &NormalHypersurface::prisms)
        .def("edgeWeight", &NormalHypersurface::edgeWeight)
        .def("triangulation", &NormalHypersurface::triangulation,
            pybind11::return_value_policy::reference_internal)
        .def("name", &NormalHypersurface::name)
        .def("setName", &NormalHypersurface::setName)
        .def("writeXMLData", [](const NormalHypersurface& s,
                pybind11::object file, regina::FileFormat f,
                const regina::NormalHypersurfaces* list) {
            pybind11::scoped_ostream_redirect stream(std::cout, file);
            s.writeXMLData(std::cout, f, list);
        })
        .def("isEmpty", &NormalHypersurface::isEmpty)
        .def("isCompact", &NormalHypersurface::isCompact)
        .def("isOrientable", &NormalHypersurface::isOrientable)
        .def("isTwoSided", &NormalHypersurface::isTwoSided)
        .def("isConnected", &NormalHypersurface::isConnected)
        .def("hasRealBoundary", &NormalHypersurface::hasRealBoundary)
        .def("isVertexLinking", &NormalHypersurface::isVertexLinking)
        .def("isVertexLink", &NormalHypersurface::isVertexLink,
            pybind11::return_value_policy::reference)
        .def("isThinEdgeLink", &NormalHypersurface::isThinEdgeLink,
            pybind11::return_value_policy::reference)
        .def("isThinTriangleLink", &NormalHypersurface::isThinTriangleLink,
            pybind11::return_value_policy::reference)
        .def("homology", &NormalHypersurface::homology,
            pybind11::return_value_policy::reference_internal)
        .def("triangulate", &NormalHypersurface::triangulate)
        .def("embedded", &NormalHypersurface::embedded)
        .def("locallyCompatible", &NormalHypersurface::locallyCompatible)
        .def("vector", &NormalHypersurface::vector,
            pybind11::return_value_policy::reference_internal)
        .def("encoding", &NormalHypersurface::encoding)
        .def("scaleDown", &NormalHypersurface::scaleDown)
        .def_static("reconstructTetrahedra",
            &NormalHypersurface::reconstructTetrahedra)
        .def(pybind11::self + pybind11::self)
        .def(pybind11::self * regina::LargeInteger())
        .def(pybind11::self *= regina::LargeInteger())
        .def(pybind11::self < pybind11::self)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.def("swap",
        (void(*)(NormalHypersurface&, NormalHypersurface&))(regina::swap));
}

