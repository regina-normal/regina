
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

void addFace7() {
    addFace<7, 0>("Face7_0", "FaceEmbedding7_0");
    addFace<7, 1>("Face7_1", "FaceEmbedding7_1");
    addFace<7, 2>("Face7_2", "FaceEmbedding7_2");
    addFace<7, 3>("Face7_3", "FaceEmbedding7_3");
    addFace<7, 4>("Face7_4", "FaceEmbedding7_4");
    addFace<7, 5>("Face7_5", "FaceEmbedding7_5");
    addFace<7, 6>("Face7_6", "FaceEmbedding7_6");

    scope().attr("VertexEmbedding7") = scope().attr("FaceEmbedding7_0");
    scope().attr("EdgeEmbedding7") = scope().attr("FaceEmbedding7_1");
    scope().attr("TriangleEmbedding7") = scope().attr("FaceEmbedding7_2");
    scope().attr("TetrahedronEmbedding7") = scope().attr("FaceEmbedding7_3");
    scope().attr("PentachoronEmbedding7") = scope().attr("FaceEmbedding7_4");

    scope().attr("Vertex7") = scope().attr("Face7_0");
    scope().attr("Edge7") = scope().attr("Face7_1");
    scope().attr("Triangle7") = scope().attr("Face7_2");
    scope().attr("Tetrahedron7") = scope().attr("Face7_3");
    scope().attr("Pentachoron7") = scope().attr("Face7_4");
}

