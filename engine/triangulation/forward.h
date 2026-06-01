
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
requires (supportedDim(dim) && subdim >= 0 && subdim < dim)
class Face;

template <int dim, int subdim>
requires (supportedDim(dim) && subdim >= 0 && subdim < dim)
class FaceEmbedding;

template <int dim> requires (supportedDim(dim)) class FacetPairing;

template <int dim> requires (supportedDim(dim)) class Isomorphism;

template <int dim> requires (supportedDim(dim)) class Simplex;

template <int dim> requires (supportedDim(dim)) class Triangulation;
template <> class Triangulation<2>;
template <> class Triangulation<3>;
template <> class Triangulation<4>;

class NormalHypersurface;
class NormalHypersurfaces;
class NormalSurface;
class NormalSurfaces;

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
         * A type alias for `Face<dim, subdim>` if `0 ≤ subdim < dim`, or
         * `Simplex<dim>` if `subdim == dim`, or regina::NoSuchType for other
         * values of \a subdim.
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
    struct SafeFaceHelper<dim, dim> {
        using type = Simplex<dim>;
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
 * The point of this type alias is so that we can do arithmetic on dimensions
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
 * The point of this type alias is so that we can do arithmetic on dimensions
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
 * The point of this type alias is so that we can do arithmetic on dimensions
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
 * The point of this type alias is so that we can do arithmetic on dimensions
 * and avoid compile errors (e.g., when a type such as `Pentachoron<3>` would
 * otherwise appear in the `false` branch of a conditional).
 *
 * \nopython
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
using SafePentachoron = typename detail::SafeFaceHelper<dim, 4>::type;

namespace detail {
    /**
     * Implementation details for SafeHypersurface<dim> and
     * SafeHypersurfaces<dim>.  See those types for further information.
     *
     * \ingroup detail
     */
    template <int dim> requires (supportedDim(dim))
    struct SafeHypersurfaceHelper {
        /**
         * A type alias for a normal hypersurface within a
         * <i>dim</i>-dimensional triangulation, or NoSuchType if Regina does
         * not support such objects in this dimension.
         */
        using Hypersurface = regina::NoSuchType;
        /**
         * A type alias for a list of normal hypersurfaces within a
         * <i>dim</i>-dimensional triangulation, or NoSuchType if Regina does
         * not support such objects in this dimension.
         */
        using Hypersurfaces = regina::NoSuchType;
    };

    #ifndef __DOXYGEN
    template <>
    struct SafeHypersurfaceHelper<3> {
        using Hypersurface = NormalSurface;
        using Hypersurfaces = NormalSurfaces;
    };

    template <>
    struct SafeHypersurfaceHelper<4> {
        using Hypersurface = NormalHypersurface;
        using Hypersurfaces = NormalHypersurfaces;
    };
    #endif // ! __DOXYGEN
} // namespace detail

/**
 * A type alias for a normal hypersurface within a <i>dim</i>-dimensional
 * triangulation, or NoSuchType if Regina does not support such objects
 * in this dimension.
 *
 * In particular, this will be the type regina::NormalSurface if `dim == 3`,
 * or regina::NormalHypersurface if `dim == 4`.
 */
template <int dim> requires (supportedDim(dim))
using SafeHypersurface =
    typename detail::SafeHypersurfaceHelper<dim>::Hypersurface;

/**
 * A type alias for a list of normal hypersurfaces within a
 * <i>dim</i>-dimensional triangulation, or NoSuchType if Regina does not
 * support such objects in this dimension.
 *
 * In particular, this will be the type regina::NormalSurfaces if
 * `dim == 3`, or regina::NormalHypersurfaces if `dim == 4`.
 */
template <int dim> requires (supportedDim(dim))
using SafeHypersurfaces =
    typename detail::SafeHypersurfaceHelper<dim>::Hypersurfaces;

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
using Triangle = detail::SafeFaceHelper<dim, 2>::type;

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
using Tetrahedron = detail::SafeFaceHelper<dim, 3>::type;

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
using Pentachoron = detail::SafeFaceHelper<dim, 4>::type;

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

} // namespace regina

#endif

