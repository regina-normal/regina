
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

void addFace9(pybind11::module_& m) {
    addFace<9, 0>(m, "Face9_0", "FaceEmbedding9_0");
    addFace<9, 1>(m, "Face9_1", "FaceEmbedding9_1");
    addFace<9, 2>(m, "Face9_2", "FaceEmbedding9_2");
    addFace<9, 3>(m, "Face9_3", "FaceEmbedding9_3");
    addFace<9, 4>(m, "Face9_4", "FaceEmbedding9_4");
    addFace<9, 5>(m, "Face9_5", "FaceEmbedding9_5");
    addFace<9, 6>(m, "Face9_6", "FaceEmbedding9_6");
    addFace<9, 7>(m, "Face9_7", "FaceEmbedding9_7");
    addFace<9, 8>(m, "Face9_8", "FaceEmbedding9_8");

    m.attr("VertexEmbedding9") = m.attr("FaceEmbedding9_0");
    m.attr("EdgeEmbedding9") = m.attr("FaceEmbedding9_1");
    m.attr("TriangleEmbedding9") = m.attr("FaceEmbedding9_2");
    m.attr("TetrahedronEmbedding9") = m.attr("FaceEmbedding9_3");
    m.attr("PentachoronEmbedding9") = m.attr("FaceEmbedding9_4");

    m.attr("Vertex9") = m.attr("Face9_0");
    m.attr("Edge9") = m.attr("Face9_1");
    m.attr("Triangle9") = m.attr("Face9_2");
    m.attr("Tetrahedron9") = m.attr("Face9_3");
    m.attr("Pentachoron9") = m.attr("Face9_4");
}

