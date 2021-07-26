
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
#include "hypersurface/hscoordregistry.h"
#include "hypersurface/normalhypersurface.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "../helpers.h"

using regina::NormalHypersurface;
using regina::NormalHypersurfaceVector;
using regina::Triangulation;

void addNormalHypersurface(pybind11::module_& m) {
    auto v = pybind11::class_<NormalHypersurfaceVector>(m,
            "NormalHypersurfaceVector")
        .def("coords", &NormalHypersurfaceVector::coords)
        .def("clone", &NormalHypersurfaceVector::clone)
        .def("size", &NormalHypersurfaceVector::size)
        .def("__getitem__", [](const NormalHypersurfaceVector& v,
                size_t index) {
            return v[index];
        }, pybind11::return_value_policy::reference_internal)
        .def("set", &NormalHypersurfaceVector::set)
        // We cannot just use pybind11::self += pybind11:;self, because
        // in that case pybind11 seems to want to return by value.
        .def("__iadd__", &NormalHypersurfaceVector::operator +=,
            pybind11::return_value_policy::reference_internal)
        .def("scaleDown", &NormalHypersurfaceVector::scaleDown)
        .def("isCompact", &NormalHypersurfaceVector::isCompact)
        .def("isVertexLinking", &NormalHypersurfaceVector::isVertexLinking)
        .def("isVertexLink", &NormalHypersurfaceVector::isVertexLink)
        .def("isThinEdgeLink", &NormalHypersurfaceVector::isThinEdgeLink)
        .def("tetrahedra", &NormalHypersurfaceVector::tetrahedra)
        .def("prisms", &NormalHypersurfaceVector::prisms)
        .def("edgeWeight", &NormalHypersurfaceVector::edgeWeight)
    ;
    regina::python::add_output(v, true /* __repr__ */);
    regina::python::add_eq_operators(v);

    auto c = pybind11::class_<NormalHypersurface>(m, "NormalHypersurface")
        .def(pybind11::init<const NormalHypersurface&>())
        .def(pybind11::init<const NormalHypersurface&,
            const Triangulation<4>&>())
        .def(pybind11::init([](Triangulation<4>& t, regina::HyperCoords coords,
                pybind11::list values) {
            regina::NormalHypersurfaceVector* v =
                forCoords(coords, [&](auto info) {
                    typedef decltype(info) Coords;
                    return static_cast<NormalHypersurfaceVector*>(
                        new typename Coords::Class(Coords::dimension(t.size())));
                }, nullptr);
            if (values.size() != v->size()) {
                delete v;
                throw pybind11::index_error(
                    "Incorrect number of normal coordinates");
            }
            try {
                // Accept any type that we know how to convert to a large
                // integer.
                for (size_t i = 0; i < v->size(); ++i)
                    v->set(i, values[i].cast<regina::LargeInteger>());
            } catch (pybind11::cast_error const &) {
                delete v;
                throw std::invalid_argument(
                    "List element not convertible to LargeInteger");
            }
            return new NormalHypersurface(t, v);
        }))
        .def("clone", [](const NormalHypersurface& s) {
            // Since clone() is deprecated, we reimplement it here to
            // avoid noisy compiler warnings.
            // Here we use the copy constructor, which has the side-effect of
            // cloning the surface name also (which the C++ clone() does not).
            // To ensure no change in behaviour, we revert the name change here.
            NormalHypersurface* ans = new NormalHypersurface(s);
            ans->setName(std::string());
            return ans;
        })
        .def("swap", &NormalHypersurface::swap)
        .def("doubleHypersurface", &NormalHypersurface::doubleHypersurface)
        .def("tetrahedra", &NormalHypersurface::tetrahedra)
        .def("prisms", &NormalHypersurface::prisms)
        .def("edgeWeight", &NormalHypersurface::edgeWeight)
        .def("countCoords", &NormalHypersurface::countCoords)
        .def("triangulation", &NormalHypersurface::triangulation)
        .def("name", &NormalHypersurface::name)
        .def("setName", &NormalHypersurface::setName)
        .def("writeRawVector", [](const NormalHypersurface& s) {
            s.writeRawVector(std::cout);
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
        .def("homology", &NormalHypersurface::homology,
            pybind11::return_value_policy::reference_internal)
        .def("triangulate", &NormalHypersurface::triangulate)
        .def("sameSurface", &NormalHypersurface::sameSurface)
        .def("embedded", &NormalHypersurface::embedded)
        .def("locallyCompatible", &NormalHypersurface::locallyCompatible)
        .def("vector", &NormalHypersurface::vector,
            pybind11::return_value_policy::reference_internal)
        .def("rawVector", [](const NormalHypersurface& s) {
            // Since rawVector() is deprecated, we reimplement it
            // ourselves here to avoid noisy compiler warnings.
            return s.vector().coords();
        }, pybind11::return_value_policy::reference_internal)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.def("swap",
        (void(*)(NormalHypersurface&, NormalHypersurface&))(regina::swap));
}

