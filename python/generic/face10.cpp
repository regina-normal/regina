
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

void addFace10() {
    addFace<10, 0>("Face10_0", "FaceEmbedding10_0");
    addFace<10, 1>("Face10_1", "FaceEmbedding10_1");
    addFace<10, 2>("Face10_2", "FaceEmbedding10_2");
    addFace<10, 3>("Face10_3", "FaceEmbedding10_3");
    addFace<10, 4>("Face10_4", "FaceEmbedding10_4");
    addFace<10, 5>("Face10_5", "FaceEmbedding10_5");
    addFace<10, 6>("Face10_6", "FaceEmbedding10_6");
    addFace<10, 7>("Face10_7", "FaceEmbedding10_7");
    addFace<10, 8>("Face10_8", "FaceEmbedding10_8");
    addFace<10, 9>("Face10_9", "FaceEmbedding10_9");

    scope().attr("VertexEmbedding10") = scope().attr("FaceEmbedding10_0");
    scope().attr("EdgeEmbedding10") = scope().attr("FaceEmbedding10_1");
    scope().attr("TriangleEmbedding10") = scope().attr("FaceEmbedding10_2");
    scope().attr("TetrahedronEmbedding10") = scope().attr("FaceEmbedding10_3");
    scope().attr("PentachoronEmbedding10") = scope().attr("FaceEmbedding10_4");

    scope().attr("Vertex10") = scope().attr("Face10_0");
    scope().attr("Edge10") = scope().attr("Face10_1");
    scope().attr("Triangle10") = scope().attr("Face10_2");
    scope().attr("Tetrahedron10") = scope().attr("Face10_3");
    scope().attr("Pentachoron10") = scope().attr("Face10_4");
}

