
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include <boost/python.hpp>
#include "dim2/dim2boundarycomponent.h"
#include "dim2/dim2component.h"
#include "dim2/dim2edge.h"
#include "dim2/dim2triangle.h"
#include "dim2/dim2vertex.h"
#include "../utilities/equality.h"

using namespace boost::python;
using regina::Dim2Component;

namespace {
    boost::python::list getVertices_list(Dim2Component& t) {
        boost::python::list ans;
        for (std::vector<regina::Dim2Vertex*>::const_iterator it =
                t.getVertices().begin(); it != t.getVertices().end(); it++)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list getEdges_list(Dim2Component& t) {
        boost::python::list ans;
        for (std::vector<regina::Dim2Edge*>::const_iterator it =
                t.getEdges().begin(); it != t.getEdges().end(); it++)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list getTriangles_list(Dim2Component& t) {
        boost::python::list ans;
        for (std::vector<regina::Dim2Triangle*>::const_iterator it =
                t.getTriangles().begin(); it != t.getTriangles().end(); it++)
            ans.append(boost::python::ptr(*it));
        return ans;
    }
}

void addDim2Component() {
    class_<Dim2Component, std::auto_ptr<Dim2Component>, boost::noncopyable>
            ("Dim2Component", no_init)
        .def("index", &Dim2Component::index)
        .def("getNumberOfTriangles", &Dim2Component::getNumberOfTriangles)
        .def("getNumberOfSimplices", &Dim2Component::getNumberOfSimplices)
        .def("getNumberOfEdges", &Dim2Component::getNumberOfEdges)
        .def("getNumberOfVertices", &Dim2Component::getNumberOfVertices)
        .def("getNumberOfBoundaryComponents",
            &Dim2Component::getNumberOfBoundaryComponents)
        .def("getTriangles", getTriangles_list)
        .def("getEdges", getEdges_list)
        .def("getVertices", getVertices_list)
        .def("getTriangle", &Dim2Component::getTriangle,
            return_value_policy<reference_existing_object>())
        .def("getSimplex", &Dim2Component::getSimplex,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim2Component::getEdge,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim2Component::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &Dim2Component::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("isOrientable", &Dim2Component::isOrientable)
        .def("isClosed", &Dim2Component::isClosed)
        .def("getNumberOfBoundaryEdges",
            &Dim2Component::getNumberOfBoundaryEdges)
        .def("str", &Dim2Component::str)
        .def("toString", &Dim2Component::toString)
        .def("detail", &Dim2Component::detail)
        .def("toStringLong", &Dim2Component::toStringLong)
        .def("__str__", &Dim2Component::str)
        EQUAL_BY_PTR(Dim2Component)
    ;
}

