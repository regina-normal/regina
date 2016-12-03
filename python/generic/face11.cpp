
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

void addFace11() {
    addFace<11, 0>("Face11_0", "FaceEmbedding11_0");
    addFace<11, 1>("Face11_1", "FaceEmbedding11_1");
    addFace<11, 2>("Face11_2", "FaceEmbedding11_2");
    addFace<11, 3>("Face11_3", "FaceEmbedding11_3");
    addFace<11, 4>("Face11_4", "FaceEmbedding11_4");
    addFace<11, 5>("Face11_5", "FaceEmbedding11_5");
    addFace<11, 6>("Face11_6", "FaceEmbedding11_6");
    addFace<11, 7>("Face11_7", "FaceEmbedding11_7");
    addFace<11, 8>("Face11_8", "FaceEmbedding11_8");
    addFace<11, 9>("Face11_9", "FaceEmbedding11_9");
    addFace<11, 10>("Face11_10", "FaceEmbedding11_10");

    scope().attr("VertexEmbedding11") = scope().attr("FaceEmbedding11_0");
    scope().attr("EdgeEmbedding11") = scope().attr("FaceEmbedding11_1");
    scope().attr("TriangleEmbedding11") = scope().attr("FaceEmbedding11_2");
    scope().attr("TetrahedronEmbedding11") = scope().attr("FaceEmbedding11_3");
    scope().attr("PentachoronEmbedding11") = scope().attr("FaceEmbedding11_4");

    scope().attr("Vertex11") = scope().attr("Face11_0");
    scope().attr("Edge11") = scope().attr("Face11_1");
    scope().attr("Triangle11") = scope().attr("Face11_2");
    scope().attr("Tetrahedron11") = scope().attr("Face11_3");
    scope().attr("Pentachoron11") = scope().attr("Face11_4");
}

