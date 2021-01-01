
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

#include "face-bindings.h"

void addFace6(pybind11::module_& m) {
    addFace<6, 0>(m, "Face6_0", "FaceEmbedding6_0");
    addFace<6, 1>(m, "Face6_1", "FaceEmbedding6_1");
    addFace<6, 2>(m, "Face6_2", "FaceEmbedding6_2");
    addFace<6, 3>(m, "Face6_3", "FaceEmbedding6_3");
    addFace<6, 4>(m, "Face6_4", "FaceEmbedding6_4");
    addFace<6, 5>(m, "Face6_5", "FaceEmbedding6_5");

    m.attr("VertexEmbedding6") = m.attr("FaceEmbedding6_0");
    m.attr("EdgeEmbedding6") = m.attr("FaceEmbedding6_1");
    m.attr("TriangleEmbedding6") = m.attr("FaceEmbedding6_2");
    m.attr("TetrahedronEmbedding6") = m.attr("FaceEmbedding6_3");
    m.attr("PentachoronEmbedding6") = m.attr("FaceEmbedding6_4");

    m.attr("Vertex6") = m.attr("Face6_0");
    m.attr("Edge6") = m.attr("Face6_1");
    m.attr("Triangle6") = m.attr("Face6_2");
    m.attr("Tetrahedron6") = m.attr("Face6_3");
    m.attr("Pentachoron6") = m.attr("Face6_4");
}

