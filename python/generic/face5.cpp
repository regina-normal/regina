
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

void addFace5(pybind11::module_& m) {
    addFace<5, 0>(m, "Face5_0", "FaceEmbedding5_0");
    addFace<5, 1>(m, "Face5_1", "FaceEmbedding5_1");
    addFace<5, 2>(m, "Face5_2", "FaceEmbedding5_2");
    addFace<5, 3>(m, "Face5_3", "FaceEmbedding5_3");
    addFace<5, 4>(m, "Face5_4", "FaceEmbedding5_4");

    m.attr("VertexEmbedding5") = m.attr("FaceEmbedding5_0");
    m.attr("EdgeEmbedding5") = m.attr("FaceEmbedding5_1");
    m.attr("TriangleEmbedding5") = m.attr("FaceEmbedding5_2");
    m.attr("TetrahedronEmbedding5") = m.attr("FaceEmbedding5_3");
    m.attr("PentachoronEmbedding5") = m.attr("FaceEmbedding5_4");

    m.attr("Vertex5") = m.attr("Face5_0");
    m.attr("Edge5") = m.attr("Face5_1");
    m.attr("Triangle5") = m.attr("Face5_2");
    m.attr("Tetrahedron5") = m.attr("Face5_3");
    m.attr("Pentachoron5") = m.attr("Face5_4");
}

