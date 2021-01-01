
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
#include "hypersurface/normalhypersurface.h"
#include "hypersurface/normalhypersurfaces.h" // for makeZeroVector()
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "../helpers.h"

using regina::NormalHypersurface;
using regina::Triangulation;

void addNormalHypersurface(pybind11::module_& m) {
    auto c = pybind11::class_<NormalHypersurface>(m, "NormalHypersurface")
        .def(pybind11::init([](Triangulation<4>* t, regina::HyperCoords coords,
                pybind11::list values) {
            regina::NormalHypersurfaceVector* v =
                regina::makeZeroVector(t, coords);
            if (values.size() != v->size()) {
                delete v;
                throw pybind11::index_error(
                    "Incorrect number of normal coordinates");
            }
            try {
                // Accept any type that we know how to convert to a large
                // integer.
                for (size_t i = 0; i < v->size(); ++i)
                    v->setElement(i, values[i].cast<regina::LargeInteger>());
            } catch (pybind11::cast_error const &) {
                delete v;
                throw std::invalid_argument(
                    "List element not convertible to LargeInteger");
            }
            return new NormalHypersurface(t, v);
        }))
        .def("clone", &NormalHypersurface::clone)
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
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.attr("NNormalHypersurface") = m.attr("NormalHypersurface");
}

