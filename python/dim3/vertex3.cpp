
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
#include "../pybind11/stl.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using regina::Face;
using regina::FaceEmbedding;
using regina::Vertex;
using regina::VertexEmbedding;

void addVertex3(pybind11::module_& m) {
    auto e = pybind11::class_<FaceEmbedding<3, 0>>(m, "FaceEmbedding3_0")
        .def(pybind11::init<regina::Tetrahedron<3>*, regina::Perm<4>>())
        .def(pybind11::init<const VertexEmbedding<3>&>())
        .def("simplex", &VertexEmbedding<3>::simplex,
            pybind11::return_value_policy::reference)
        .def("tetrahedron", &VertexEmbedding<3>::tetrahedron,
            pybind11::return_value_policy::reference)
        .def("face", &VertexEmbedding<3>::face)
        .def("vertex", &VertexEmbedding<3>::vertex)
        .def("vertices", &VertexEmbedding<3>::vertices)
    ;
    regina::python::add_output(e, true /* __repr__ */);
    regina::python::add_eq_operators(e);

    auto c = pybind11::class_<Face<3, 0>>(m, "Face3_0")
        .def("index", &Vertex<3>::index)
        // Make all embeddings functions return by value in Python, since
        // embeddings are lightweight and we wish to enforce constness.
        .def("embeddings", [](const Vertex<3>& v) {
            pybind11::list ans;
            for (const auto& emb : v)
                ans.append(emb);
            return ans;
        })
        .def("embedding", &Vertex<3>::embedding)
        .def("front", &Vertex<3>::front)
        .def("back", &Vertex<3>::back)
        .def("triangulation", &Vertex<3>::triangulation)
        .def("component", &Vertex<3>::component,
            pybind11::return_value_policy::reference)
        .def("boundaryComponent", &Vertex<3>::boundaryComponent,
            pybind11::return_value_policy::reference)
        .def("degree", &Vertex<3>::degree)
        .def("linkType", &Vertex<3>::linkType)
        .def("link", [](const Vertex<3>&) {
            PyErr_SetString(PyExc_RuntimeError,
                "Vertex3::link() has been renamed to Vertex3::linkType().  "
                "You should change your code now, because the name link() "
                "will be used for a different function in the future.");
        })
        .def("buildLink", [](const Vertex<3>& v) {
            // Return a clone of the resulting triangulation.
            // This is because Python cannot enforce the constness of
            // the reference that would normally be returned.
            return new regina::Triangulation<2>(v.buildLink());
        })
        .def("buildLinkInclusion", &Vertex<3>::buildLinkInclusion)
        .def("isLinkClosed", &Vertex<3>::isLinkClosed)
        .def("isIdeal", &Vertex<3>::isIdeal)
        .def("isBoundary", &Vertex<3>::isBoundary)
        .def("isStandard", &Vertex<3>::isStandard)
        .def("isValid", &Vertex<3>::isValid)
        .def("hasBadIdentification", &Vertex<3>::hasBadIdentification)
        .def("hasBadLink", &Vertex<3>::hasBadLink)
        .def("isLinkOrientable", &Vertex<3>::isLinkOrientable)
        .def("linkEulerChar", &Vertex<3>::linkEulerChar)
        .def_static("ordering", &Vertex<3>::ordering)
        .def_static("faceNumber", &Vertex<3>::faceNumber)
        .def_static("containsVertex", &Vertex<3>::containsVertex)
        // On some systems we cannot take addresses of the following
        // inline class constants (e.g., this fails with gcc10 on windows).
        // We therefore define getter functions instead.
        .def_property_readonly_static("nFaces", [](pybind11::object) {
            return Vertex<3>::nFaces;
        })
        .def_property_readonly_static("lexNumbering", [](pybind11::object) {
            return Vertex<3>::lexNumbering;
        })
        .def_property_readonly_static("oppositeDim", [](pybind11::object) {
            return Vertex<3>::oppositeDim;
        })
        .def_property_readonly_static("dimension", [](pybind11::object) {
            return Vertex<3>::dimension;
        })
        .def_property_readonly_static("subdimension", [](pybind11::object) {
            return Vertex<3>::subdimension;
        })
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    pybind11::enum_<regina::Vertex<3>::LinkType>(c, "LinkType")
        .value("SPHERE", regina::Vertex<3>::SPHERE)
        .value("DISC", regina::Vertex<3>::DISC)
        .value("TORUS", regina::Vertex<3>::TORUS)
        .value("KLEIN_BOTTLE", regina::Vertex<3>::KLEIN_BOTTLE)
        .value("NON_STANDARD_CUSP", regina::Vertex<3>::NON_STANDARD_CUSP)
        .value("INVALID", regina::Vertex<3>::INVALID)
        .export_values();

    m.attr("VertexEmbedding3") = m.attr("FaceEmbedding3_0");
    m.attr("Vertex3") = m.attr("Face3_0");
}

