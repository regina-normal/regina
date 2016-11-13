
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
#include "triangulation/dim4.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using namespace boost::python;
using regina::Component;

namespace {
    boost::python::list pentachora_list(Component<4>& t) {
        boost::python::list ans;
        for (std::vector<regina::Simplex<4>*>::const_iterator it =
                t.simplices().begin(); it != t.simplices().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list bc_list(Component<4>& t) {
        boost::python::list ans;
        for (auto s : t.boundaryComponents())
            ans.append(boost::python::ptr(s));
        return ans;
    }
}

void addComponent4() {
    class_<Component<4>, std::auto_ptr<Component<4>>, boost::noncopyable>
            ("Component4", no_init)
        .def("index", &Component<4>::index)
        .def("size", &Component<4>::size)
        .def("countPentachora", &Component<4>::countPentachora)
        .def("countFaces", &regina::python::countFaces<Component<4>, 4>)
        .def("countTetrahedra", &Component<4>::countTetrahedra)
        .def("countTriangles", &Component<4>::countTriangles)
        .def("countEdges", &Component<4>::countEdges)
        .def("countVertices", &Component<4>::countVertices)
        .def("countBoundaryComponents", &Component<4>::countBoundaryComponents)
        .def("simplices", pentachora_list)
        .def("pentachora", pentachora_list)
        .def("faces", &regina::python::faces<Component<4>, 4>)
        .def("vertices", regina::python::faces_list<Component<4>, 4, 0>)
        .def("edges", regina::python::faces_list<Component<4>, 4, 1>)
        .def("triangles", regina::python::faces_list<Component<4>, 4, 2>)
        .def("tetrahedra", regina::python::faces_list<Component<4>, 4, 3>)
        .def("boundaryComponents", bc_list)
        .def("simplex", &Component<4>::simplex,
            return_value_policy<reference_existing_object>())
        .def("pentachoron", &Component<4>::pentachoron,
            return_value_policy<reference_existing_object>())
        .def("face", &regina::python::face<Component<4>, 4, size_t>)
        .def("tetrahedron", &Component<4>::tetrahedron,
            return_value_policy<reference_existing_object>())
        .def("triangle", &Component<4>::triangle,
            return_value_policy<reference_existing_object>())
        .def("edge", &Component<4>::edge,
            return_value_policy<reference_existing_object>())
        .def("vertex", &Component<4>::vertex,
            return_value_policy<reference_existing_object>())
        .def("boundaryComponent", &Component<4>::boundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("isIdeal", &Component<4>::isIdeal)
        .def("isValid", &Component<4>::isValid)
        .def("isOrientable", &Component<4>::isOrientable)
        .def("isClosed", &Component<4>::isClosed)
        .def("hasBoundaryFacets", &Component<4>::hasBoundaryFacets)
        .def("hasBoundaryTetrahedra", &Component<4>::hasBoundaryTetrahedra)
        .def("countBoundaryFacets", &Component<4>::countBoundaryFacets)
        .def("countBoundaryTetrahedra", &Component<4>::countBoundaryTetrahedra)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("Dim4Component") = scope().attr("Component4");
}

