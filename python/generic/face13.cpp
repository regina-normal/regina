
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

void addFace13(pybind11::module_& m) {
    addFace<13, 0>(m, "Face13_0", "FaceEmbedding13_0");
    addFace<13, 1>(m, "Face13_1", "FaceEmbedding13_1");
    addFace<13, 2>(m, "Face13_2", "FaceEmbedding13_2");
    addFace<13, 3>(m, "Face13_3", "FaceEmbedding13_3");
    addFace<13, 4>(m, "Face13_4", "FaceEmbedding13_4");
    addFace<13, 5>(m, "Face13_5", "FaceEmbedding13_5");
    addFace<13, 6>(m, "Face13_6", "FaceEmbedding13_6");
    addFace<13, 7>(m, "Face13_7", "FaceEmbedding13_7");
    addFace<13, 8>(m, "Face13_8", "FaceEmbedding13_8");
    addFace<13, 9>(m, "Face13_9", "FaceEmbedding13_9");
    addFace<13, 10>(m, "Face13_10", "FaceEmbedding13_10");
    addFace<13, 11>(m, "Face13_11", "FaceEmbedding13_11");
    addFace<13, 12>(m, "Face13_12", "FaceEmbedding13_12");

    m.attr("VertexEmbedding13") = m.attr("FaceEmbedding13_0");
    m.attr("EdgeEmbedding13") = m.attr("FaceEmbedding13_1");
    m.attr("TriangleEmbedding13") = m.attr("FaceEmbedding13_2");
    m.attr("TetrahedronEmbedding13") = m.attr("FaceEmbedding13_3");
    m.attr("PentachoronEmbedding13") = m.attr("FaceEmbedding13_4");

    m.attr("Vertex13") = m.attr("Face13_0");
    m.attr("Edge13") = m.attr("Face13_1");
    m.attr("Triangle13") = m.attr("Face13_2");
    m.attr("Tetrahedron13") = m.attr("Face13_3");
    m.attr("Pentachoron13") = m.attr("Face13_4");
}

