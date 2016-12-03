
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

void addFace12() {
    addFace<12, 0>("Face12_0", "FaceEmbedding12_0");
    addFace<12, 1>("Face12_1", "FaceEmbedding12_1");
    addFace<12, 2>("Face12_2", "FaceEmbedding12_2");
    addFace<12, 3>("Face12_3", "FaceEmbedding12_3");
    addFace<12, 4>("Face12_4", "FaceEmbedding12_4");
    addFace<12, 5>("Face12_5", "FaceEmbedding12_5");
    addFace<12, 6>("Face12_6", "FaceEmbedding12_6");
    addFace<12, 7>("Face12_7", "FaceEmbedding12_7");
    addFace<12, 8>("Face12_8", "FaceEmbedding12_8");
    addFace<12, 9>("Face12_9", "FaceEmbedding12_9");
    addFace<12, 10>("Face12_10", "FaceEmbedding12_10");
    addFace<12, 11>("Face12_11", "FaceEmbedding12_11");

    scope().attr("VertexEmbedding12") = scope().attr("FaceEmbedding12_0");
    scope().attr("EdgeEmbedding12") = scope().attr("FaceEmbedding12_1");
    scope().attr("TriangleEmbedding12") = scope().attr("FaceEmbedding12_2");
    scope().attr("TetrahedronEmbedding12") = scope().attr("FaceEmbedding12_3");
    scope().attr("PentachoronEmbedding12") = scope().attr("FaceEmbedding12_4");

    scope().attr("Vertex12") = scope().attr("Face12_0");
    scope().attr("Edge12") = scope().attr("Face12_1");
    scope().attr("Triangle12") = scope().attr("Face12_2");
    scope().attr("Tetrahedron12") = scope().attr("Face12_3");
    scope().attr("Pentachoron12") = scope().attr("Face12_4");
}

