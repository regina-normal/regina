
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

void addFace8(pybind11::module_& m) {
    addFace<8, 0>(m, "Face8_0", "FaceEmbedding8_0");
    addFace<8, 1>(m, "Face8_1", "FaceEmbedding8_1");
    addFace<8, 2>(m, "Face8_2", "FaceEmbedding8_2");
    addFace<8, 3>(m, "Face8_3", "FaceEmbedding8_3");
    addFace<8, 4>(m, "Face8_4", "FaceEmbedding8_4");
    addFace<8, 5>(m, "Face8_5", "FaceEmbedding8_5");
    addFace<8, 6>(m, "Face8_6", "FaceEmbedding8_6");
    addFace<8, 7>(m, "Face8_7", "FaceEmbedding8_7");

    m.attr("VertexEmbedding8") = m.attr("FaceEmbedding8_0");
    m.attr("EdgeEmbedding8") = m.attr("FaceEmbedding8_1");
    m.attr("TriangleEmbedding8") = m.attr("FaceEmbedding8_2");
    m.attr("TetrahedronEmbedding8") = m.attr("FaceEmbedding8_3");
    m.attr("PentachoronEmbedding8") = m.attr("FaceEmbedding8_4");

    m.attr("Vertex8") = m.attr("Face8_0");
    m.attr("Edge8") = m.attr("Face8_1");
    m.attr("Triangle8") = m.attr("Face8_2");
    m.attr("Tetrahedron8") = m.attr("Face8_3");
    m.attr("Pentachoron8") = m.attr("Face8_4");
}

