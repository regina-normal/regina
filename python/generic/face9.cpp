
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

void addFace9() {
    addFace<9, 0>("Face9_0", "FaceEmbedding9_0");
    addFace<9, 1>("Face9_1", "FaceEmbedding9_1");
    addFace<9, 2>("Face9_2", "FaceEmbedding9_2");
    addFace<9, 3>("Face9_3", "FaceEmbedding9_3");
    addFace<9, 4>("Face9_4", "FaceEmbedding9_4");
    addFace<9, 5>("Face9_5", "FaceEmbedding9_5");
    addFace<9, 6>("Face9_6", "FaceEmbedding9_6");
    addFace<9, 7>("Face9_7", "FaceEmbedding9_7");
    addFace<9, 8>("Face9_8", "FaceEmbedding9_8");

    scope().attr("VertexEmbedding9") = scope().attr("FaceEmbedding9_0");
    scope().attr("EdgeEmbedding9") = scope().attr("FaceEmbedding9_1");
    scope().attr("TriangleEmbedding9") = scope().attr("FaceEmbedding9_2");
    scope().attr("TetrahedronEmbedding9") = scope().attr("FaceEmbedding9_3");
    scope().attr("PentachoronEmbedding9") = scope().attr("FaceEmbedding9_4");

    scope().attr("Vertex9") = scope().attr("Face9_0");
    scope().attr("Edge9") = scope().attr("Face9_1");
    scope().attr("Triangle9") = scope().attr("Face9_2");
    scope().attr("Tetrahedron9") = scope().attr("Face9_3");
    scope().attr("Pentachoron9") = scope().attr("Face9_4");
}

