
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

void addFace15(pybind11::module_& m) {
    addFace<15, 0>(m, "Face15_0", "FaceEmbedding15_0");
    addFace<15, 1>(m, "Face15_1", "FaceEmbedding15_1");
    addFace<15, 2>(m, "Face15_2", "FaceEmbedding15_2");
    addFace<15, 3>(m, "Face15_3", "FaceEmbedding15_3");
    addFace<15, 4>(m, "Face15_4", "FaceEmbedding15_4");
    addFace<15, 5>(m, "Face15_5", "FaceEmbedding15_5");
    addFace<15, 6>(m, "Face15_6", "FaceEmbedding15_6");
    addFace<15, 7>(m, "Face15_7", "FaceEmbedding15_7");
    addFace<15, 8>(m, "Face15_8", "FaceEmbedding15_8");
    addFace<15, 9>(m, "Face15_9", "FaceEmbedding15_9");
    addFace<15, 10>(m, "Face15_10", "FaceEmbedding15_10");
    addFace<15, 11>(m, "Face15_11", "FaceEmbedding15_11");
    addFace<15, 12>(m, "Face15_12", "FaceEmbedding15_12");
    addFace<15, 13>(m, "Face15_13", "FaceEmbedding15_13");
    addFace<15, 14>(m, "Face15_14", "FaceEmbedding15_14");

    m.attr("VertexEmbedding15") = m.attr("FaceEmbedding15_0");
    m.attr("EdgeEmbedding15") = m.attr("FaceEmbedding15_1");
    m.attr("TriangleEmbedding15") = m.attr("FaceEmbedding15_2");
    m.attr("TetrahedronEmbedding15") = m.attr("FaceEmbedding15_3");
    m.attr("PentachoronEmbedding15") = m.attr("FaceEmbedding15_4");

    m.attr("Vertex15") = m.attr("Face15_0");
    m.attr("Edge15") = m.attr("Face15_1");
    m.attr("Triangle15") = m.attr("Face15_2");
    m.attr("Tetrahedron15") = m.attr("Face15_3");
    m.attr("Pentachoron15") = m.attr("Face15_4");
}

