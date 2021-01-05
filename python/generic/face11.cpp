
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

void addFace11(pybind11::module_& m) {
    addFace<11, 0>(m, "Face11_0", "FaceEmbedding11_0");
    addFace<11, 1>(m, "Face11_1", "FaceEmbedding11_1");
    addFace<11, 2>(m, "Face11_2", "FaceEmbedding11_2");
    addFace<11, 3>(m, "Face11_3", "FaceEmbedding11_3");
    addFace<11, 4>(m, "Face11_4", "FaceEmbedding11_4");
    addFace<11, 5>(m, "Face11_5", "FaceEmbedding11_5");
    addFace<11, 6>(m, "Face11_6", "FaceEmbedding11_6");
    addFace<11, 7>(m, "Face11_7", "FaceEmbedding11_7");
    addFace<11, 8>(m, "Face11_8", "FaceEmbedding11_8");
    addFace<11, 9>(m, "Face11_9", "FaceEmbedding11_9");
    addFace<11, 10>(m, "Face11_10", "FaceEmbedding11_10");

    m.attr("VertexEmbedding11") = m.attr("FaceEmbedding11_0");
    m.attr("EdgeEmbedding11") = m.attr("FaceEmbedding11_1");
    m.attr("TriangleEmbedding11") = m.attr("FaceEmbedding11_2");
    m.attr("TetrahedronEmbedding11") = m.attr("FaceEmbedding11_3");
    m.attr("PentachoronEmbedding11") = m.attr("FaceEmbedding11_4");

    m.attr("Vertex11") = m.attr("Face11_0");
    m.attr("Edge11") = m.attr("Face11_1");
    m.attr("Triangle11") = m.attr("Face11_2");
    m.attr("Tetrahedron11") = m.attr("Face11_3");
    m.attr("Pentachoron11") = m.attr("Face11_4");
}

