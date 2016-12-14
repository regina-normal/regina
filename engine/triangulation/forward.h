
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file triangulation/forward.h
 *  \brief Provides forward declarations of Regina's triangulation-related
 *  classes.
 *
 *  It is better to include this file rather than manually declare classes
 *  that you plan to use, for several reasons:
 *
 *  - this header declares all template specialisations, so the compiler
 *    knows not to use generic implementations when these are not approrpiate;
 *
 *  - this header declares templated type alias (e.g., Simplex<dim>,
 *    Vertex<dim>, etc.) in one central location, which is needed for
 *    compilers that will not accept multiple declarations of the same
 *    type alias (as seen for example in some versions of gcc).
 */

#ifndef __FORWARD_H
#ifndef __DOXYGEN
#define __FORWARD_H
#endif

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

template <int> class BoundaryComponent;

template <int> class Component;
template <> class Component<2>;
template <> class Component<3>;
template <> class Component<4>;

template <int> class Example;
template <> class Example<2>;
template <> class Example<3>;
template <> class Example<4>;

template <int, int> class Face;
template <int dim> class Face<dim, dim>;
template <> class Face<2, 2>;
template <> class Face<3, 3>;
template <> class Face<3, 2>; // test shape
template <> class Face<3, 0>; // build links, output type
template <> class Face<4, 4>;
template <> class Face<4, 1>; // build links
template <> class Face<4, 0>; // build links, output ideal/etc.
template <int dim> using Vertex = Face<dim, 0>;
template <int dim> using Edge = Face<dim, 1>;
template <int dim> using Triangle = Face<dim, 2>;
template <int dim> using Tetrahedron = Face<dim, 3>;
template <int dim> using Pentachoron = Face<dim, 4>;
template <int dim> using Simplex = Face<dim, dim>;

template <int, int> class FaceEmbedding;
template <int dim> using VertexEmbedding = FaceEmbedding<dim, 0>;
template <int dim> using EdgeEmbedding = FaceEmbedding<dim, 1>;
template <int dim> using TriangleEmbedding = FaceEmbedding<dim, 2>;
template <int dim> using TetrahedronEmbedding = FaceEmbedding<dim, 3>;
template <int dim> using PentachoronEmbedding = FaceEmbedding<dim, 4>;

template <int> class FacetPairing;
template <> class FacetPairing<3>;

template <int> class Isomorphism;

template <int> class Triangulation;
template <> class Triangulation<2>;
template <> class Triangulation<3>;
template <> class Triangulation<4>;

/*@}*/

} // namespace regina

#endif

