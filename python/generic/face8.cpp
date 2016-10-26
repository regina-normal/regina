
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

void addFace8() {
    addFace<8, 0>("Face8_0", "FaceEmbedding8_0");
    addFace<8, 1>("Face8_1", "FaceEmbedding8_1");
    addFace<8, 2>("Face8_2", "FaceEmbedding8_2");
    addFace<8, 3>("Face8_3", "FaceEmbedding8_3");
    addFace<8, 4>("Face8_4", "FaceEmbedding8_4");
    addFace<8, 5>("Face8_5", "FaceEmbedding8_5");
    addFace<8, 6>("Face8_6", "FaceEmbedding8_6");
    addFace<8, 7>("Face8_7", "FaceEmbedding8_7");

    scope().attr("VertexEmbedding8") = scope().attr("FaceEmbedding8_0");
    scope().attr("EdgeEmbedding8") = scope().attr("FaceEmbedding8_1");
    scope().attr("TriangleEmbedding8") = scope().attr("FaceEmbedding8_2");
    scope().attr("TetrahedronEmbedding8") = scope().attr("FaceEmbedding8_3");
    scope().attr("PentachoronEmbedding8") = scope().attr("FaceEmbedding8_4");

    scope().attr("Vertex8") = scope().attr("Face8_0");
    scope().attr("Edge8") = scope().attr("Face8_1");
    scope().attr("Triangle8") = scope().attr("Face8_2");
    scope().attr("Tetrahedron8") = scope().attr("Face8_3");
    scope().attr("Pentachoron8") = scope().attr("Face8_4");
}

