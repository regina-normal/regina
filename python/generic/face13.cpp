
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

void addFace13() {
    addFace<13, 0>("Face13_0", "FaceEmbedding13_0");
    addFace<13, 1>("Face13_1", "FaceEmbedding13_1");
    addFace<13, 2>("Face13_2", "FaceEmbedding13_2");
    addFace<13, 3>("Face13_3", "FaceEmbedding13_3");
    addFace<13, 4>("Face13_4", "FaceEmbedding13_4");
    addFace<13, 5>("Face13_5", "FaceEmbedding13_5");
    addFace<13, 6>("Face13_6", "FaceEmbedding13_6");
    addFace<13, 7>("Face13_7", "FaceEmbedding13_7");
    addFace<13, 8>("Face13_8", "FaceEmbedding13_8");
    addFace<13, 9>("Face13_9", "FaceEmbedding13_9");
    addFace<13, 10>("Face13_10", "FaceEmbedding13_10");
    addFace<13, 11>("Face13_11", "FaceEmbedding13_11");
    addFace<13, 12>("Face13_12", "FaceEmbedding13_12");

    scope().attr("VertexEmbedding13") = scope().attr("FaceEmbedding13_0");
    scope().attr("EdgeEmbedding13") = scope().attr("FaceEmbedding13_1");
    scope().attr("TriangleEmbedding13") = scope().attr("FaceEmbedding13_2");
    scope().attr("TetrahedronEmbedding13") = scope().attr("FaceEmbedding13_3");
    scope().attr("PentachoronEmbedding13") = scope().attr("FaceEmbedding13_4");

    scope().attr("Vertex13") = scope().attr("Face13_0");
    scope().attr("Edge13") = scope().attr("Face13_1");
    scope().attr("Triangle13") = scope().attr("Face13_2");
    scope().attr("Tetrahedron13") = scope().attr("Face13_3");
    scope().attr("Pentachoron13") = scope().attr("Face13_4");
}

