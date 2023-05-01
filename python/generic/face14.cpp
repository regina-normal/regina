
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

void addFace14(pybind11::module_& m) {
    addFace<14, 0>(m, "Face14_0", "FaceEmbedding14_0");
    addFace<14, 1>(m, "Face14_1", "FaceEmbedding14_1");
    addFace<14, 2>(m, "Face14_2", "FaceEmbedding14_2");
    addFace<14, 3>(m, "Face14_3", "FaceEmbedding14_3");
    addFace<14, 4>(m, "Face14_4", "FaceEmbedding14_4");
    addFace<14, 5>(m, "Face14_5", "FaceEmbedding14_5");
    addFace<14, 6>(m, "Face14_6", "FaceEmbedding14_6");
    addFace<14, 7>(m, "Face14_7", "FaceEmbedding14_7");
    addFace<14, 8>(m, "Face14_8", "FaceEmbedding14_8");
    addFace<14, 9>(m, "Face14_9", "FaceEmbedding14_9");
    addFace<14, 10>(m, "Face14_10", "FaceEmbedding14_10");
    addFace<14, 11>(m, "Face14_11", "FaceEmbedding14_11");
    addFace<14, 12>(m, "Face14_12", "FaceEmbedding14_12");
    addFace<14, 13>(m, "Face14_13", "FaceEmbedding14_13");

    m.attr("VertexEmbedding14") = m.attr("FaceEmbedding14_0");
    m.attr("EdgeEmbedding14") = m.attr("FaceEmbedding14_1");
    m.attr("TriangleEmbedding14") = m.attr("FaceEmbedding14_2");
    m.attr("TetrahedronEmbedding14") = m.attr("FaceEmbedding14_3");
    m.attr("PentachoronEmbedding14") = m.attr("FaceEmbedding14_4");

    m.attr("Vertex14") = m.attr("Face14_0");
    m.attr("Edge14") = m.attr("Face14_1");
    m.attr("Triangle14") = m.attr("Face14_2");
    m.attr("Tetrahedron14") = m.attr("Face14_3");
    m.attr("Pentachoron14") = m.attr("Face14_4");
}

