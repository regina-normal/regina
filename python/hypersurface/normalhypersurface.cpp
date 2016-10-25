
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

#include <boost/python.hpp>
#include "hypersurface/normalhypersurface.h"
#include "hypersurface/normalhypersurfaces.h" // for makeZeroVector()
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::NormalHypersurface;
using regina::Triangulation;

namespace {
    void writeRawVector_stdio(const NormalHypersurface& s) {
        s.writeRawVector(std::cout);
    }

    /**
     * A python-only constructor that lets users build a normal hypersurface
     * from a hand-crafted list of integers.
     */
    NormalHypersurface* fromCoordinates(Triangulation<4>* t,
            regina::HyperCoords coords, boost::python::list values) {
        regina::NormalHypersurfaceVector* v =
            regina::makeZeroVector(t, coords);

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
            extract<regina::LargeInteger&> x_large(values[i]);
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

        return new NormalHypersurface(t, v);
    }
}

void addNormalHypersurface() {
    class_<NormalHypersurface, std::auto_ptr<NormalHypersurface>,
            boost::noncopyable>("NormalHypersurface", no_init)
        .def("__init__", make_constructor(fromCoordinates))
        .def("clone", &NormalHypersurface::clone,
            return_value_policy<manage_new_object>())
        .def("doubleHypersurface", &NormalHypersurface::doubleHypersurface,
            return_value_policy<manage_new_object>())
        .def("tetrahedra", &NormalHypersurface::tetrahedra)
        .def("prisms", &NormalHypersurface::prisms)
        .def("edgeWeight", &NormalHypersurface::edgeWeight)
        .def("countCoords", &NormalHypersurface::countCoords)
        .def("triangulation", &NormalHypersurface::triangulation,
            return_value_policy<to_held_type<>>())
        .def("name", &NormalHypersurface::name,
            return_value_policy<return_by_value>())
        .def("setName", &NormalHypersurface::setName)
        .def("writeRawVector", writeRawVector_stdio)
        .def("isEmpty", &NormalHypersurface::isEmpty)
        .def("isCompact", &NormalHypersurface::isCompact)
        .def("isOrientable", &NormalHypersurface::isOrientable)
        .def("isTwoSided", &NormalHypersurface::isTwoSided)
        .def("isConnected", &NormalHypersurface::isConnected)
        .def("hasRealBoundary", &NormalHypersurface::hasRealBoundary)
        .def("isVertexLinking", &NormalHypersurface::isVertexLinking)
        .def("isVertexLink", &NormalHypersurface::isVertexLink,
            return_value_policy<reference_existing_object>())
        .def("isThinEdgeLink", &NormalHypersurface::isThinEdgeLink,
            return_value_policy<reference_existing_object>())
        .def("homology", &NormalHypersurface::homology,
            return_internal_reference<>())
        .def("triangulate", &NormalHypersurface::triangulate,
            return_value_policy<to_held_type<>>())
        .def("sameSurface", &NormalHypersurface::sameSurface)
        .def("embedded", &NormalHypersurface::embedded)
        .def("locallyCompatible", &NormalHypersurface::locallyCompatible)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NNormalHypersurface") = scope().attr("NormalHypersurface");
}

