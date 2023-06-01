
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include "triangulation/facenumbering.h"

using regina::FaceNumbering;

template <int dim, int subdim, int face>
void testFaceNumbering() {
    static_assert(FaceNumbering<dim, subdim>::faceNumber(
        FaceNumbering<dim, subdim>::ordering(face)) == face);
    static_assert(FaceNumbering<dim, subdim>::containsVertex(face,
        FaceNumbering<dim, subdim>::ordering(face)[0]));
    static_assert(FaceNumbering<dim, subdim>::containsVertex(face,
        FaceNumbering<dim, subdim>::ordering(face)[subdim]));
    static_assert(! FaceNumbering<dim, subdim>::containsVertex(face,
        FaceNumbering<dim, subdim>::ordering(face)[subdim + 1]));
    static_assert(! FaceNumbering<dim, subdim>::containsVertex(face,
        FaceNumbering<dim, subdim>::ordering(face)[dim]));

    if constexpr (subdim + subdim + 1 != dim) {
        static_assert(FaceNumbering<dim, dim - 1 - subdim>::faceNumber(
            FaceNumbering<dim, subdim>::ordering(face).reverse()) == face);
    } else {
        static_assert(FaceNumbering<dim, subdim>::faceNumber(
            FaceNumbering<dim, subdim>::ordering(face).reverse()) ==
            FaceNumbering<dim, subdim>::nFaces - 1 - face);
    }

    if constexpr (subdim == 1) {
        if constexpr (subdim + subdim + 1 != dim) {
            static_assert(regina::faceOppositeEdge<dim>(
                FaceNumbering<dim, subdim>::ordering(face)[0],
                FaceNumbering<dim, subdim>::ordering(face)[1]) == face);
        } else {
            static_assert(regina::faceOppositeEdge<dim>(
                FaceNumbering<dim, subdim>::ordering(face)[0],
                FaceNumbering<dim, subdim>::ordering(face)[1]) ==
                FaceNumbering<dim, subdim>::nFaces - 1 - face);
        }
    }
}

template void testFaceNumbering<1, 0, 1>();

template void testFaceNumbering<2, 0, 1>();
template void testFaceNumbering<2, 1, 1>();

template void testFaceNumbering<3, 0, 2>();
template void testFaceNumbering<3, 1, 4>();
template void testFaceNumbering<3, 2, 1>();

template void testFaceNumbering<4, 0, 2>();
template void testFaceNumbering<4, 1, 7>();
template void testFaceNumbering<4, 2, 8>();
template void testFaceNumbering<4, 3, 3>();

template void testFaceNumbering<5, 0, 3>();
template void testFaceNumbering<5, 1, 7>();
template void testFaceNumbering<5, 2, 11>();
template void testFaceNumbering<5, 3, 8>();
template void testFaceNumbering<5, 4, 4>();

template void testFaceNumbering<15, 0, 9>();
template void testFaceNumbering<15, 1, 20>();
template void testFaceNumbering<15, 2, 40>();
template void testFaceNumbering<15, 5, 100>();
template void testFaceNumbering<15, 7, 1000>();
template void testFaceNumbering<15, 10, 120>();
template void testFaceNumbering<15, 12, 45>();
template void testFaceNumbering<15, 13, 27>();
template void testFaceNumbering<15, 14, 8>();
