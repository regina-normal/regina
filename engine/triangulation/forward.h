
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

#include "regina-core.h"

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

template <int dim, int subdim>
requires (supportedDim(dim) && subdim >= 0 && subdim <= dim)
class Face;

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
template <int dim> requires (supportedDim(dim))
using Simplex = Face<dim, dim>;

template <int dim, int subdim>
requires (supportedDim(dim) && subdim >= 0 && subdim < dim)
class FaceEmbedding;

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
 * Provides safe access to related types for triangulations of the given
 * dimension.
 *
 * Typically these related types will be meaningful for some but not all
 * triangulation dimensions.  The point of TriangulationTraits is that, for
 * the cases that are _not_ meaningful, the relevant types will resolve to
 * regina::NoSuchType (and therefore avoid compiler errors by listing invalid
 * template parameters, such as `Triangulation<1>` or `Face<dim, dim + 1>`).
 *
 * \nopython
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
struct TriangulationTraits {
    /**
     * A type alias for a vertex of a <i>dim</i>-dimensional triangulation.
     */
    using Vertex = Face<dim, 0>;
    /**
     * A type alias for an edge of a <i>dim</i>-dimensional triangulation.
     */
    using Edge = Face<dim, 1>;
    /**
     * A type alias for a triangle of a <i>dim</i>-dimensional triangulation.
     */
    using Triangle = Face<dim, 2>;
    /**
     * A type alias for a tetrahedron of a <i>dim</i>-dimensional triangulation,
     * or NoSuchType if `dim < 3`.
     */
    using Tetrahedron = Face<dim, 3>;
    /**
     * A type alias for a pentachoron of a <i>dim</i>-dimensional triangulation,
     * or NoSuchType if `dim < 4`.
     */
    using Pentachoron = Face<dim, 4>;

    /**
     * A type alias for a triangulation of dimension `dim + 1`, or NoSuchType
     * if \a dim is the largest supported dimension in Regina.
     */
    using Higher = Triangulation<dim + 1>;
    /**
     * A type alias for a triangulation of dimension `dim - 1`, or NoSuchType
     * if \a dim is 2 (i.e., the lowest supported dimension in Regina).
     */
    using Lower = Triangulation<dim - 1>;
};

#ifndef __DOXYGEN
template <>
struct TriangulationTraits<2> {
    using Vertex = Face<2, 0>;
    using Edge = Face<2, 1>;
    using Triangle = Face<2, 2>;
    using Tetrahedron = regina::NoSuchType;
    using Pentachoron = regina::NoSuchType;

    using Higher = Triangulation<3>;
    using Lower = regina::NoSuchType;
};

template <>
struct TriangulationTraits<3> {
    using Vertex = Face<3, 0>;
    using Edge = Face<3, 1>;
    using Triangle = Face<3, 2>;
    using Tetrahedron = Face<3, 3>;
    using Pentachoron = regina::NoSuchType;

    using Higher = Triangulation<4>;
    using Lower = Triangulation<2>;
};

template <>
struct TriangulationTraits<maxDim()> {
    using Vertex = Face<maxDim(), 0>;
    using Edge = Face<maxDim(), 1>;
    using Triangle = Face<maxDim(), 2>;
    using Tetrahedron = Face<maxDim(), 3>;
    using Pentachoron = Face<maxDim(), 4>;

    using Higher = regina::NoSuchType;
    using Lower = Triangulation<maxDim() - 1>;
};
#endif // ! __DOXYGEN

} // namespace regina

#endif

