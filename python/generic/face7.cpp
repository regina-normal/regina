
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

void addFace7(pybind11::module_& m) {
    addFace<7, 0>(m, "Face7_0", "FaceEmbedding7_0");
    addFace<7, 1>(m, "Face7_1", "FaceEmbedding7_1");
    addFace<7, 2>(m, "Face7_2", "FaceEmbedding7_2");
    addFace<7, 3>(m, "Face7_3", "FaceEmbedding7_3");
    addFace<7, 4>(m, "Face7_4", "FaceEmbedding7_4");
    addFace<7, 5>(m, "Face7_5", "FaceEmbedding7_5");
    addFace<7, 6>(m, "Face7_6", "FaceEmbedding7_6");

    m.attr("VertexEmbedding7") = m.attr("FaceEmbedding7_0");
    m.attr("EdgeEmbedding7") = m.attr("FaceEmbedding7_1");
    m.attr("TriangleEmbedding7") = m.attr("FaceEmbedding7_2");
    m.attr("TetrahedronEmbedding7") = m.attr("FaceEmbedding7_3");
    m.attr("PentachoronEmbedding7") = m.attr("FaceEmbedding7_4");

    m.attr("Vertex7") = m.attr("Face7_0");
    m.attr("Edge7") = m.attr("Face7_1");
    m.attr("Triangle7") = m.attr("Face7_2");
    m.attr("Tetrahedron7") = m.attr("Face7_3");
    m.attr("Pentachoron7") = m.attr("Face7_4");
}

