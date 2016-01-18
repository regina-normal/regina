
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include "face-bindings.h"

void addFace() {
    def("binomSmall", regina::binomSmall);

    // Note: FaceEmbedding<4, subdim> does not get the aliases
    // pentachoron() or getPentachoron().
    // This is fixed in the 4-manifolds branch, since dim=4 is handled
    // separately.
    addFace<4, 0>("Face4_0", "FaceEmbedding4_0");
    addFace<4, 1>("Face4_1", "FaceEmbedding4_1");
    addFace<4, 2>("Face4_2", "FaceEmbedding4_2");
    addFace<4, 3>("Face4_3", "FaceEmbedding4_3");

    // The bindings for dim >= 5 are in separate files, one per dimension.
    // This is to help avoid excessive memory consumption by the compiler.
}

