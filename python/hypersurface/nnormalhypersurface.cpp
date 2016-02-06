
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
#include "dim4/dim4triangulation.h"
#include "hypersurface/nnormalhypersurface.h"
#include "hypersurface/nnormalhypersurfacelist.h" // for makeZeroVector()
#include "triangulation/ntriangulation.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::NNormalHypersurface;
using regina::Dim4Triangulation;

namespace {
    void writeRawVector_stdio(const NNormalHypersurface& s) {
        s.writeRawVector(std::cout);
    }

    /**
     * A python-only constructor that lets users build a normal hypersurface
     * from a hand-crafted list of integers.
     */
    NNormalHypersurface* fromCoordinates(Dim4Triangulation* t,
            regina::HyperCoords coords, boost::python::list values) {
        regina::NNormalHypersurfaceVector* v =
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

        return new NNormalHypersurface(t, v);
    }
}

void addNNormalHypersurface() {
    class_<NNormalHypersurface, std::auto_ptr<NNormalHypersurface>,
            boost::noncopyable>("NNormalHypersurface", no_init)
        .def("__init__", make_constructor(fromCoordinates))
        .def("clone", &NNormalHypersurface::clone,
            return_value_policy<manage_new_object>())
        .def("doubleHypersurface", &NNormalHypersurface::doubleHypersurface,
            return_value_policy<manage_new_object>())
        .def("tetrahedra", &NNormalHypersurface::tetrahedra)
        .def("prisms", &NNormalHypersurface::prisms)
        .def("edgeWeight", &NNormalHypersurface::edgeWeight)
        .def("countCoords", &NNormalHypersurface::countCoords)
        .def("triangulation", &NNormalHypersurface::triangulation,
            return_value_policy<to_held_type<>>())
        .def("name", &NNormalHypersurface::name,
            return_value_policy<return_by_value>())
        .def("setName", &NNormalHypersurface::setName)
        .def("writeRawVector", writeRawVector_stdio)
        .def("isEmpty", &NNormalHypersurface::isEmpty)
        .def("isCompact", &NNormalHypersurface::isCompact)
        .def("isOrientable", &NNormalHypersurface::isOrientable)
        .def("isTwoSided", &NNormalHypersurface::isTwoSided)
        .def("isConnected", &NNormalHypersurface::isConnected)
        .def("hasRealBoundary", &NNormalHypersurface::hasRealBoundary)
        .def("isVertexLinking", &NNormalHypersurface::isVertexLinking)
        .def("isVertexLink", &NNormalHypersurface::isVertexLink,
            return_value_policy<reference_existing_object>())
        .def("isThinEdgeLink", &NNormalHypersurface::isThinEdgeLink,
            return_value_policy<reference_existing_object>())
        .def("homology", &NNormalHypersurface::homology,
            return_internal_reference<>())
        .def("triangulate", &NNormalHypersurface::triangulate,
            return_value_policy<to_held_type<>>())
        .def("sameSurface", &NNormalHypersurface::sameSurface)
        .def("embedded", &NNormalHypersurface::embedded)
        .def("locallyCompatible", &NNormalHypersurface::locallyCompatible)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
}

