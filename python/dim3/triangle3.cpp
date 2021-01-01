
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
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../generic/facehelper.h"

using regina::Face;
using regina::FaceEmbedding;
using regina::Triangle;
using regina::TriangleEmbedding;

void addTriangle3(pybind11::module_& m) {
    auto e = pybind11::class_<FaceEmbedding<3, 2>>(m, "FaceEmbedding3_2")
        .def(pybind11::init<regina::Tetrahedron<3>*, int>())
        .def(pybind11::init<const TriangleEmbedding<3>&>())
        .def("simplex", &TriangleEmbedding<3>::simplex,
            pybind11::return_value_policy::reference)
        .def("tetrahedron", &TriangleEmbedding<3>::tetrahedron,
            pybind11::return_value_policy::reference)
        .def("face", &TriangleEmbedding<3>::face)
        .def("triangle", &TriangleEmbedding<3>::triangle)
        .def("vertices", &TriangleEmbedding<3>::vertices)
    ;
    regina::python::add_output(e);
    regina::python::add_eq_operators(e);

    auto c = pybind11::class_<Face<3, 2>>(m, "Face3_2")
        .def("index", &Triangle<3>::index)
        .def("embeddings", [](const Triangle<3>& t) {
            pybind11::list ans;
            for (const auto& emb : t)
                ans.append(emb);
            return ans;
        }, pybind11::return_value_policy::reference_internal)
        .def("embedding", &Triangle<3>::embedding,
            pybind11::return_value_policy::reference_internal)
        .def("isBoundary", &Triangle<3>::isBoundary)
        .def("inMaximalForest", &Triangle<3>::inMaximalForest)
        .def("type", &Triangle<3>::type)
        .def("subtype", &Triangle<3>::subtype)
        .def("isMobiusBand", &Triangle<3>::isMobiusBand)
        .def("isCone", &Triangle<3>::isCone)
        .def("isValid", &Triangle<3>::isValid)
        .def("isLinkOrientable", &Triangle<3>::isLinkOrientable)
        .def("degree", &Triangle<3>::degree)
        .def("front", &Triangle<3>::front,
            pybind11::return_value_policy::reference_internal)
        .def("back", &Triangle<3>::back,
            pybind11::return_value_policy::reference_internal)
        .def("triangulation", &Triangle<3>::triangulation)
        .def("component", &Triangle<3>::component,
            pybind11::return_value_policy::reference)
        .def("boundaryComponent", &Triangle<3>::boundaryComponent,
            pybind11::return_value_policy::reference)
        .def("face", &regina::python::face<Triangle<3>, 2, int,
            pybind11::return_value_policy::reference>)
        .def("vertex", &Triangle<3>::vertex,
            pybind11::return_value_policy::reference)
        .def("edge", &Triangle<3>::edge,
            pybind11::return_value_policy::reference)
        .def("faceMapping", &regina::python::faceMapping<Triangle<3>, 2, 4>)
        .def("vertexMapping", &Triangle<3>::vertexMapping)
        .def("edgeMapping", &Triangle<3>::edgeMapping)
        .def_static("ordering", &Triangle<3>::ordering)
        .def_static("faceNumber", &Triangle<3>::faceNumber)
        .def_static("containsVertex", &Triangle<3>::containsVertex)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    // Embed this enum in the Triangle3 class.
    pybind11::enum_<regina::Triangle<3>::Type>(c, "Type")
        .value("UNKNOWN_TYPE", regina::Triangle<3>::UNKNOWN_TYPE)
        .value("TRIANGLE", regina::Triangle<3>::TRIANGLE)
        .value("SCARF", regina::Triangle<3>::SCARF)
        .value("PARACHUTE", regina::Triangle<3>::PARACHUTE)
        .value("CONE", regina::Triangle<3>::CONE)
        .value("MOBIUS", regina::Triangle<3>::MOBIUS)
        .value("HORN", regina::Triangle<3>::HORN)
        .value("DUNCEHAT", regina::Triangle<3>::DUNCEHAT)
        .value("L31", regina::Triangle<3>::L31)
        .export_values();

    m.attr("NTriangleEmbedding") = m.attr("FaceEmbedding3_2");
    m.attr("TriangleEmbedding3") = m.attr("FaceEmbedding3_2");
    m.attr("NTriangle") = m.attr("Face3_2");
    m.attr("Triangle3") = m.attr("Face3_2");
}

