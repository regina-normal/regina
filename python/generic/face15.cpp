
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

void addFace15() {
    addFace<15, 0>("Face15_0", "FaceEmbedding15_0");
    addFace<15, 1>("Face15_1", "FaceEmbedding15_1");
    addFace<15, 2>("Face15_2", "FaceEmbedding15_2");
    addFace<15, 3>("Face15_3", "FaceEmbedding15_3");
    addFace<15, 4>("Face15_4", "FaceEmbedding15_4");
    addFace<15, 5>("Face15_5", "FaceEmbedding15_5");
    addFace<15, 6>("Face15_6", "FaceEmbedding15_6");
    addFace<15, 7>("Face15_7", "FaceEmbedding15_7");
    addFace<15, 8>("Face15_8", "FaceEmbedding15_8");
    addFace<15, 9>("Face15_9", "FaceEmbedding15_9");
    addFace<15, 10>("Face15_10", "FaceEmbedding15_10");
    addFace<15, 11>("Face15_11", "FaceEmbedding15_11");
    addFace<15, 12>("Face15_12", "FaceEmbedding15_12");
    addFace<15, 13>("Face15_13", "FaceEmbedding15_13");
    addFace<15, 14>("Face15_14", "FaceEmbedding15_14");

    scope().attr("VertexEmbedding15") = scope().attr("FaceEmbedding15_0");
    scope().attr("EdgeEmbedding15") = scope().attr("FaceEmbedding15_1");
    scope().attr("TriangleEmbedding15") = scope().attr("FaceEmbedding15_2");
    scope().attr("TetrahedronEmbedding15") = scope().attr("FaceEmbedding15_3");
    scope().attr("PentachoronEmbedding15") = scope().attr("FaceEmbedding15_4");

    scope().attr("Vertex15") = scope().attr("Face15_0");
    scope().attr("Edge15") = scope().attr("Face15_1");
    scope().attr("Triangle15") = scope().attr("Face15_2");
    scope().attr("Tetrahedron15") = scope().attr("Face15_3");
    scope().attr("Pentachoron15") = scope().attr("Face15_4");
}

