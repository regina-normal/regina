
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file triangulation/forward.h
 *  \brief Provides forward declarations for Regina's triangulation-related
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

#ifndef __REGINA_TRIANGULATION_FORWARD_H
#ifndef __DOXYGEN
#define __REGINA_TRIANGULATION_FORWARD_H
#endif

namespace regina {

template <int dim> requires (supportedDim(dim)) class BoundaryComponent;

template <int dim> requires (supportedDim(dim)) class Component;
template <> class Component<2>;
template <> class Component<3>;
template <> class Component<4>;

template <int dim> requires (supportedDim(dim)) class Example;
template <> class Example<2>;
template <> class Example<3>;
template <> class Example<4>;

template <int, int> class Face;
template <int dim> requires (supportedDim(dim)) class Face<dim, dim>;
template <> class Face<2, 2>;
template <> class Face<3, 3>;
template <> class Face<3, 2>; // test shape, linking surface
template <> class Face<3, 1>; // linking surface
template <> class Face<3, 0>; // build links, linking surface, output type
template <> class Face<4, 4>;
template <> class Face<4, 3>; // linking surface
template <> class Face<4, 2>; // linking surface
template <> class Face<4, 1>; // build links, linking surface
template <> class Face<4, 0>; // build links, linking surface, output ideal/etc.
template <int dim> requires (supportedDim(dim))
using Vertex = Face<dim, 0>;
template <int dim> requires (supportedDim(dim))
using Edge = Face<dim, 1>;
template <int dim> requires (supportedDim(dim))
using Triangle = Face<dim, 2>;
template <int dim> requires (supportedDim(dim) && dim >= 3)
using Tetrahedron = Face<dim, 3>;
template <int dim> requires (supportedDim(dim) && dim >= 4)
using Pentachoron = Face<dim, 4>;
template <int dim> using Simplex = Face<dim, dim>;

template <int, int> class FaceEmbedding;
template <int dim> requires (supportedDim(dim))
using VertexEmbedding = FaceEmbedding<dim, 0>;
template <int dim> requires (supportedDim(dim))
using EdgeEmbedding = FaceEmbedding<dim, 1>;
template <int dim> requires (supportedDim(dim) && dim > 2)
using TriangleEmbedding = FaceEmbedding<dim, 2>;
template <int dim> requires (supportedDim(dim) && dim > 3)
using TetrahedronEmbedding = FaceEmbedding<dim, 3>;
template <int dim> requires (supportedDim(dim) && dim > 4)
using PentachoronEmbedding = FaceEmbedding<dim, 4>;

template <int dim> requires (supportedDim(dim)) class FacetPairing;
template <> class FacetPairing<3>;

template <int dim> requires (supportedDim(dim)) class Isomorphism;

template <int dim> requires (supportedDim(dim)) class Triangulation;
template <> class Triangulation<2>;
template <> class Triangulation<3>;
template <> class Triangulation<4>;

/**
 * Provides safe access to types related to triangulations of a given dimension.
 *
 * Currently _related types_ simply refers to triangulations in one dimension
 * higher or lower.  These types are useful in generic code when working with
 * (for example) triangulated boundary components, cobordisms, and so on.
 *
 * By _safe access_, we mean that all of the type aliases in TriangulationTraits
 * will be valid non-void types, even if Regina does not support triangulations
 * of the corresponding dimension.  For example, the type
 * `TriangulationTraits<2>::Lower` is valid even though Regina does not support
 * `Triangulation<1>` (in this case, `TriangulationTraits<2>::Lower` resolves to
 * an empty struct).
 *
 * \nopython
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
struct TriangulationTraits {
    /**
     * A type alias for a triangulation of dimension `dim + 1`, or an empty
     * struct if \a dim is the largest supported dimension in Regina.
     */
    using Higher = Triangulation<dim + 1>;
    /**
     * A type alias for a triangulation of dimension `dim - 1`, or an empty
     * struct if \a dim is 2 (i.e., the lowest supported dimension in Regina).
     */
    using Lower = Triangulation<dim - 1>;
};

#ifndef __DOXYGEN
template <>
struct TriangulationTraits<2> {
    using Higher = Triangulation<3>;
    struct Lower {};
};

template <>
struct TriangulationTraits<maxDim()> {
    struct Higher {};
    using Lower = Triangulation<maxDim() - 1>;
};
#endif // ! __DOXYGEN

} // namespace regina

#endif

