
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

void addFace5() {
    addFace<5, 0>("Face5_0", "FaceEmbedding5_0");
    addFace<5, 1>("Face5_1", "FaceEmbedding5_1");
    addFace<5, 2>("Face5_2", "FaceEmbedding5_2");
    addFace<5, 3>("Face5_3", "FaceEmbedding5_3");
    addFace<5, 4>("Face5_4", "FaceEmbedding5_4");

    scope().attr("VertexEmbedding5") = scope().attr("FaceEmbedding5_0");
    scope().attr("EdgeEmbedding5") = scope().attr("FaceEmbedding5_1");
    scope().attr("TriangleEmbedding5") = scope().attr("FaceEmbedding5_2");
    scope().attr("TetrahedronEmbedding5") = scope().attr("FaceEmbedding5_3");
    scope().attr("PentachoronEmbedding5") = scope().attr("FaceEmbedding5_4");

    scope().attr("Vertex5") = scope().attr("Face5_0");
    scope().attr("Edge5") = scope().attr("Face5_1");
    scope().attr("Triangle5") = scope().attr("Face5_2");
    scope().attr("Tetrahedron5") = scope().attr("Face5_3");
    scope().attr("Pentachoron5") = scope().attr("Face5_4");
}

