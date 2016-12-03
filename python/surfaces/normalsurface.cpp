
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

#include "maths/matrix.h"
#include "surfaces/normalsurface.h"
#include "surfaces/normalsurfaces.h" // for makeZeroVector()
#include "triangulation/dim3.h"
#include "../globalarray.h"
#include "../helpers.h"
#include "../safeheldtype.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
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

    /**
     * A python-only constructor that lets users build a normal surface
     * from a hand-crafted list of integers.
     */
    NormalSurface* fromCoordinates(Triangulation<3>* t,
            regina::NormalCoords coords, boost::python::list values) {
        regina::NormalSurfaceVector* v = regina::makeZeroVector(t, coords);

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

        return new NormalSurface(t, v);
    }

    void writeRawVector_stdio(const NormalSurface& s) {
        s.writeRawVector(std::cout);
    }
    boost::python::tuple isThinEdgeLink_tuple(const NormalSurface& s) {
        std::pair<const regina::Edge<3>*, const regina::Edge<3>*> ans =
            s.isThinEdgeLink();
        return boost::python::make_tuple(boost::python::ptr(ans.first),
            boost::python::ptr(ans.second));
    }

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_isCompressingDisc,
        NormalSurface::isCompressingDisc, 0, 1);
}

void addNormalSurface() {
    class_<NormalSurface, std::auto_ptr<NormalSurface>, boost::noncopyable>
            ("NormalSurface", no_init)
        .def("__init__", make_constructor(fromCoordinates))
        .def("clone", &NormalSurface::clone,
            return_value_policy<manage_new_object>())
        .def("doubleSurface", &NormalSurface::doubleSurface,
            return_value_policy<manage_new_object>())
        .def("triangles", &NormalSurface::triangles)
        .def("orientedTriangles", &NormalSurface::orientedTriangles)
        .def("quads", &NormalSurface::quads)
        .def("orientedQuads", &NormalSurface::orientedQuads)
        .def("octs", &NormalSurface::octs)
        .def("edgeWeight", &NormalSurface::edgeWeight)
        .def("arcs", &NormalSurface::arcs)
        .def("octPosition", &NormalSurface::octPosition)
        .def("countCoords", &NormalSurface::countCoords)
        .def("triangulation", &NormalSurface::triangulation,
            return_value_policy<to_held_type<> >())
        .def("name", &NormalSurface::name,
            return_value_policy<return_by_value>())
        .def("setName", &NormalSurface::setName)
        .def("writeRawVector", writeRawVector_stdio)
        .def("isEmpty", &NormalSurface::isEmpty)
        .def("isCompact", &NormalSurface::isCompact)
        .def("eulerChar", &NormalSurface::eulerChar)
        .def("isOrientable", &NormalSurface::isOrientable)
        .def("isTwoSided", &NormalSurface::isTwoSided)
        .def("isConnected", &NormalSurface::isConnected)
        .def("hasRealBoundary", &NormalSurface::hasRealBoundary)
        .def("isVertexLinking", &NormalSurface::isVertexLinking)
        .def("isVertexLink", &NormalSurface::isVertexLink,
            return_value_policy<reference_existing_object>())
        .def("isThinEdgeLink", isThinEdgeLink_tuple)
        .def("isSplitting", &NormalSurface::isSplitting)
        .def("isCentral", &NormalSurface::isCentral)
        .def("isCompressingDisc", &NormalSurface::isCompressingDisc,
            OL_isCompressingDisc())
        .def("isIncompressible", &NormalSurface::isIncompressible)
        .def("cutAlong", &NormalSurface::cutAlong,
            return_value_policy<to_held_type<> >())
        .def("crush", &NormalSurface::crush,
            return_value_policy<to_held_type<> >())
        .def("sameSurface", &NormalSurface::sameSurface)
        .def("normal", &NormalSurface::normal)
        .def("embedded", &NormalSurface::embedded)
        .def("locallyCompatible", &NormalSurface::locallyCompatible)
        .def("disjoint", &NormalSurface::disjoint)
        .def("boundaryIntersections", &NormalSurface::boundaryIntersections,
            return_value_policy<manage_new_object>())
        .def("systemAllowsAlmostNormal",
            &NormalSurface::systemAllowsAlmostNormal)
        .def("systemAllowsSpun", &NormalSurface::systemAllowsSpun)
        .def("systemAllowsOriented", &NormalSurface::systemAllowsOriented)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    // Global arrays:
    scope().attr("quadSeparating") = &quadSeparating_arr;
    scope().attr("quadMeeting") = &quadMeeting_arr;
    scope().attr("quadDefn") = &quadDefn_arr;
    scope().attr("quadPartner") = &quadPartner_arr;
    scope().attr("quadString") = &quadString_arr;
    scope().attr("triDiscArcs") = &triDiscArcs_arr;
    scope().attr("quadDiscArcs") = &quadDiscArcs_arr;
    scope().attr("octDiscArcs") = &octDiscArcs_arr;

    scope().attr("NNormalSurface") = scope().attr("NormalSurface");
}

