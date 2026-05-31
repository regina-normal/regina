
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

template <int dim> requires (supportedDim(dim)) class BoundaryComponent;

template <int dim> requires (supportedDim(dim)) class Component;

template <int dim> requires (supportedDim(dim)) class Example;
template <> class Example<2>;
template <> class Example<3>;
template <> class Example<4>;

template <int dim, int subdim>
requires (supportedDim(dim) && subdim >= 0 && subdim <= dim)
class Face;

template <int dim> requires (supportedDim(dim)) class Face<dim, dim>;
template <> class Face<3, 0>; // build links, output type
template <> class Face<4, 1>; // build links
template <> class Face<4, 0>; // build links, output ideal/etc.

template <int dim, int subdim>
requires (supportedDim(dim) && subdim >= 0 && subdim < dim)
class FaceEmbedding;

template <int dim> requires (supportedDim(dim)) class FacetPairing;

template <int dim> requires (supportedDim(dim)) class Isomorphism;

template <int dim> requires (supportedDim(dim)) class Triangulation;
template <> class Triangulation<2>;
template <> class Triangulation<3>;
template <> class Triangulation<4>;

class NormalHypersurface;
class NormalHypersurfaces;
class NormalSurface;
class NormalSurfaces;

/**
 * Refers to a top-dimensional simplex in a <i>dim</i>-dimensional
 * triangulation.
 *
 * This is the preferred way to refer to a top-dimensional simplex (as
 * opposed to the more clumsy notation Face<dim, dim>).
 *
 * \tparam dim the dimension of the underlying triangulation.
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
using Simplex = Face<dim, dim>;

/**
 * Refers to a vertex of a <i>dim</i>-dimensional triangulation.
 *
 * This is the preferred way to refer to a vertex of a triangulation
 * (as opposed to the more clumsy notation Face<dim, 0>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix (e.g., \c Vertex8).
 *
 * \tparam dim the dimension of the underlying triangulation.
 *
 * \ingroup triangulation
 */
template <int dim>
requires (supportedDim(dim))
using Vertex = Face<dim, 0>;

/**
 * Refers to an edge of a <i>dim</i>-dimensional triangulation.
 *
 * This is the preferred way to refer to an edge of a triangulation
 * (as opposed to the more clumsy notation Face<dim, 1>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix (e.g., \c Edge5).
 *
 * \tparam dim the dimension of the underlying triangulation.
 *
 * \ingroup triangulation
 */
template <int dim>
requires (supportedDim(dim))
using Edge = Face<dim, 1>;

/**
 * Refers to a triangular face of a <i>dim</i>-dimensional triangulation.
 * This alias is also valid for the case \a dim = 2, where it refers to
 * a top-dimensional simplex of a 2-dimensional triangulation.
 *
 * This is the preferred way to refer to a 2-face of a triangulation
 * (as opposed to the more clumsy notation Face<dim, 2>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix (e.g., \c Triangle4).
 *
 * \tparam dim the dimension of the underlying triangulation.
 *
 * \ingroup triangulation
 */
template <int dim>
requires (supportedDim(dim))
using Triangle = Face<dim, 2>;

/**
 * Refers to a tetrahedral face of a <i>dim</i>-dimensional triangulation.
 * This alias is also valid for the case \a dim = 3, where it refers to
 * a top-dimensional simplex of a 3-dimensional triangulation.
 *
 * This is the preferred way to refer to a 3-face of a triangulation
 * (as opposed to the more clumsy notation Face<dim, 3>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c Tetrahedron7).
 *
 * \tparam dim the dimension of the underlying triangulation.
 *
 * \ingroup triangulation
 */
template <int dim>
requires (supportedDim(dim) && dim >= 3)
using Tetrahedron = Face<dim, 3>;

/**
 * Refers to a pentachoron face of a <i>dim</i>-dimensional triangulation.
 * This alias is also valid for the case \a dim = 4, where it refers to
 * a top-dimensional simplex of a 4-dimensional triangulation.
 *
 * This is the preferred way to refer to a 4-face of a triangulation
 * (as opposed to the more clumsy notation Face<dim, 4>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix (e.g.,
 * \c Pentachoron9).
 *
 * \tparam dim the dimension of the underlying triangulation.
 *
 * \ingroup triangulation
 */
template <int dim>
requires (supportedDim(dim) && dim >= 4)
using Pentachoron = Face<dim, 4>;

/**
 * Details how a vertex of a <i>dim</i>-dimensional triangulation
 * appears within each top-dimensional simplex.
 *
 * This is the preferred way to refer to this class (as opposed to the
 * more clumsy notation FaceEmbedding<dim, 0>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c VertexEmbedding3).
 *
 * \tparam dim the dimension of the underlying triangulation.
 *
 * \ingroup triangulation
 */
template <int dim>
requires (supportedDim(dim))
using VertexEmbedding = FaceEmbedding<dim, 0>;

/**
 * Details how a edge of a <i>dim</i>-dimensional triangulation
 * appears within each top-dimensional simplex.
 *
 * This is the preferred way to refer to this class (as opposed to the
 * more clumsy notation FaceEmbedding<dim, 1>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c EdgeEmbedding5).
 *
 * \tparam dim the dimension of the underlying triangulation.
 *
 * \ingroup triangulation
 */
template <int dim>
requires (supportedDim(dim))
using EdgeEmbedding = FaceEmbedding<dim, 1>;

/**
 * Details how a triangular face of a <i>dim</i>-dimensional triangulation
 * appears within each top-dimensional simplex.
 *
 * This is the preferred way to refer to this class (as opposed to the
 * more clumsy notation FaceEmbedding<dim, 2>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c TriangleEmbedding12).
 *
 * \tparam dim the dimension of the underlying triangulation.
 *
 * \ingroup triangulation
 */
template <int dim>
requires (supportedDim(dim) && dim > 2)
using TriangleEmbedding = FaceEmbedding<dim, 2>;

/**
 * Details how a tetrahedral face of a <i>dim</i>-dimensional triangulation
 * appears within each top-dimensional simplex.
 *
 * This is the preferred way to refer to this class (as opposed to the
 * more clumsy notation FaceEmbedding<dim, 3>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c TetrahedronEmbedding7).
 *
 * \tparam dim the dimension of the underlying triangulation.
 *
 * \ingroup triangulation
 */
template <int dim>
requires (supportedDim(dim) && dim > 3)
using TetrahedronEmbedding = FaceEmbedding<dim, 3>;

/**
 * Details how a pentachoron face of a <i>dim</i>-dimensional triangulation
 * appears within each top-dimensional simplex.
 *
 * This is the preferred way to refer to this class (as opposed to the
 * more clumsy notation FaceEmbedding<dim, 4>).
 *
 * \python Python does not support templates.  Instead this alias can
 * be used by appending the dimension \a dim as a suffix
 * (e.g., \c PentachoronEmbedding14).
 *
 * \tparam dim the dimension of the underlying triangulation.
 *
 * \ingroup triangulation
 */
template <int dim>
requires (supportedDim(dim) && dim > 4)
using PentachoronEmbedding = FaceEmbedding<dim, 4>;

namespace detail {
    /**
     * Implementation details for SafeTriangulation<dim>.
     * See SafeTriangulation for further information.
     *
     * \ingroup detail
     */
    template <int dim> requires (dim >= 0 && dim <= maxDim() + 1)
    struct SafeTriangulationHelper {
        /**
         * A type alias for `Triangulation<dim>`, or regina::NoSuchType if
         * \a dim is outside the supported range.
         */
         using type = Triangulation<dim>;
    };

    /**
     * Implementation details for SafeFace<dim, subdim>.
     * See SafeFace for further information.
     *
     * \ingroup detail
     */
    template <int dim, int subdim>
    requires (supportedDim(dim) && subdim >= -1 && subdim <= std::max(dim, 4))
    struct SafeFaceHelper {
        /**
         * A type alias for `Face<dim, subdim>`, or regina::NoSuchType if the
         * pair `(dim, subdim)` is outside the supported range.
         */
         using type = Face<dim, subdim>;
    };

    #ifndef __DOXYGEN
    template <>
    struct SafeTriangulationHelper<0> {
        using type = regina::NoSuchType;
    };
    template <>
    struct SafeTriangulationHelper<1> {
        using type = regina::NoSuchType;
    };
    template <>
    struct SafeTriangulationHelper<maxDim() + 1> {
        using type = regina::NoSuchType;
    };

    template <int dim> requires (supportedDim(dim))
    struct SafeFaceHelper<dim, -1> {
        using type = regina::NoSuchType;
    };
    template <>
    struct SafeFaceHelper<2, 3> {
        using type = regina::NoSuchType;
    };
    template <>
    struct SafeFaceHelper<2, 4> {
        using type = regina::NoSuchType;
    };
    template <>
    struct SafeFaceHelper<3, 4> {
        using type = regina::NoSuchType;
    };
    #endif // ! __DOXYGEN
} // namespace detail

/**
 * Provides safe access to triangulations of the given dimension, where the
 * template argument \a dim may be a little outside the supported range.
 *
 * For supported dimensions, `SafeTriangulation<dim>` will simply resolve to
 * `Triangulation<dim>`.  For dimensions that are _not_ supported, it will
 * resolve to regina::NoSuchType.
 *
 * The point of this traits class is so that we can do arithmetic on dimensions
 * and avoid compile errors (e.g., when `Triangulation<1>` would otherwise
 * appear in the `false` branch of a conditional).
 *
 * \nopython
 *
 * \ingroup triangulation
 */
template <int dim> requires (dim >= 0 && dim <= maxDim() + 1)
using SafeTriangulation = typename detail::SafeTriangulationHelper<dim>::type;

/**
 * Provides safe access to faces of the given dimension, where the template
 * arguments \a dim and \a subdim may be a little outside the supported range.
 *
 * For supported dimensions, `SafeFace<dim, subdim>` will simply resolve to
 * `Face<dim, subdim>`.  For dimensions that are _not_ supported, it will
 * resolve to regina::NoSuchType.
 *
 * The point of this traits class is so that we can do arithmetic on dimensions
 * and avoid compile errors (e.g., when a type such as `Face<2, 3>` would
 * otherwise appear in the `false` branch of a conditional).
 *
 * \nopython
 *
 * \ingroup triangulation
 */
template <int dim, int subdim>
requires (supportedDim(dim) && subdim >= -1 && subdim <= std::max(dim, 4))
using SafeFace = typename detail::SafeFaceHelper<dim, subdim>::type;

/**
 * Provides safe access to tetrahedra in triangulations of the given dimension,
 * where the template argument \a dim may be any of Regina's supported
 * dimensions.
 *
 * For dimensions \a dim ≥ 3, `SafeTetrahedron<dim>` will simply resolve to
 * `Tetrahedron<dim>`.  For dimension 2, it will resolve to regina::NoSuchType.
 *
 * The point of this traits class is so that we can do arithmetic on dimensions
 * and avoid compile errors (e.g., when a type such as `Tetrahedron<2>` would
 * otherwise appear in the `false` branch of a conditional).
 *
 * \nopython
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
using SafeTetrahedron = typename detail::SafeFaceHelper<dim, 3>::type;

/**
 * Provides safe access to pentachora in triangulations of the given dimension,
 * where the template argument \a dim may be any of Regina's supported
 * dimensions.
 *
 * For dimensions \a dim ≥ 4, `SafePentachoron<dim>` will simply resolve to
 * `Pentachoron<dim>`.  For dimensions 2 and 3, it will resolve to
 * regina::NoSuchType.
 *
 * The point of this traits class is so that we can do arithmetic on dimensions
 * and avoid compile errors (e.g., when a type such as `Pentachoron<3>` would
 * otherwise appear in the `false` branch of a conditional).
 *
 * \nopython
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
using SafePentachoron = typename detail::SafeFaceHelper<dim, 4>::type;

/**
 * Provides safe access to related types for triangulations of the given
 * dimension.
 *
 * Typically these related types will be meaningful for some but not all
 * triangulation dimensions.  The point of this traits class is that, for
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

/**
 * Provides safe access to types for normal surfaces or hypersurfaces within
 * triangulations of the given dimension.
 *
 * Typically these types will be meaningful for some but not all triangulation
 * dimensions.  The point of this traits class is that, for the cases that are
 * _not_ meaningful, the relevant types will resolve to regina::NoSuchType
 * (and therefore avoid compiler errors by listing invalid template parameters).
 *
 * \nopython
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
struct HypersurfaceTraits {
    /**
     * A type alias for a normal hypersurface within a <i>dim</i>-dimensional
     * triangulation, or NoSuchType if Regina does not support such objects
     * in this dimension.
     *
     * In particular, this will be the type regina::NormalSurface if `dim == 3`,
     * or regina::NormalHypersurface if `dim == 4`.
     */
    using Hypersurface = regina::NoSuchType;
    /**
     * A type alias for a list of normal hypersurfaces within a
     * <i>dim</i>-dimensional triangulation, or NoSuchType if Regina does not
     * support such objects in this dimension.
     *
     * In particular, this will be the type regina::NormalSurfaces if
     * `dim == 3`, or regina::NormalHypersurfaces if `dim == 4`.
     */
    using Hypersurfaces = regina::NoSuchType;
};

#ifndef __DOXYGEN
template <>
struct HypersurfaceTraits<3> {
    using Hypersurface = NormalSurface;
    using Hypersurfaces = NormalSurfaces;
};

template <>
struct HypersurfaceTraits<4> {
    using Hypersurface = NormalHypersurface;
    using Hypersurfaces = NormalHypersurfaces;
};
#endif // ! __DOXYGEN

/**
 * A type alias for a normal hypersurface within a <i>dim</i>-dimensional
 * triangulation, or NoSuchType if Regina does not support such objects
 * in this dimension.
 *
 * In particular, this will be the type regina::NormalSurface if `dim == 3`,
 * or regina::NormalHypersurface if `dim == 4`.
 */
template <int dim>
using Hypersurface = HypersurfaceTraits<dim>::Hypersurface;

/**
 * A type alias for a list of normal hypersurfaces within a
 * <i>dim</i>-dimensional triangulation, or NoSuchType if Regina does not
 * support such objects in this dimension.
 *
 * In particular, this will be the type regina::NormalSurfaces if
 * `dim == 3`, or regina::NormalHypersurfaces if `dim == 4`.
 */
template <int dim>
using Hypersurfaces = HypersurfaceTraits<dim>::Hypersurfaces;

} // namespace regina

#endif

