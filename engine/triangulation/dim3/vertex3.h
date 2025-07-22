
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

/*! \file triangulation/dim3/vertex3.h
 *  \brief Internal header for vertices in a 3-manifold triangulation.
 *
 *  This file is automatically included from triangulation/dim3.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation3.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation3.h.
#include "triangulation/dim3/triangulation3.h"

#ifndef __REGINA_VERTEX3_H
#ifndef __DOXYGEN
#define __REGINA_VERTEX3_H
#endif

namespace regina {

/**
 * Represents a vertex in the skeleton of a 3-manifold triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * generic documentation for Face for a general overview of how the face
 * classes work.  In Python, you can read this generic documentation by
 * looking at faces in a higher dimension: try `help(Vertex5)`.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions"
 * offer significant extra functionality.
 *
 * Vertices do not support value semantics: they cannot be copied, swapped,
 * or manually constructed.  Their location in memory defines them, and
 * they are often passed and compared by pointer.  End users are never
 * responsible for their memory management; this is all taken care of by
 * the Triangulation to which they belong.
 *
 * \headerfile triangulation/dim3.h
 *
 * \ingroup dim3
 */
template <>
class Face<3, 0> : public detail::FaceBase<3, 0> {
    public:
        /**
         * Categorises the possible links of a vertex into a small number
         * of common types.  Here a vertex link is considered only up to its
         * topology (not the combinatorics of its triangulation).
         *
         * \see link
         */
        enum class Link {
            /**
             * Specifies a vertex link that is a sphere.  In other words, the
             * vertex is internal.
             */
            Sphere = 1,
            /**
             * Specifies a vertex link that is a disc.  In other words, the
             * vertex lies on a real boundary component.
             */
            Disc = 2,
            /**
             * Specifies a vertex link that is a torus.  In other words, this
             * is an ideal vertex representing a torus cusp.
             */
            Torus = 3,
            /**
             * Specifies a vertex link that is a Klein bottle.  In other words,
             * this is an ideal vertex representing a Klein bottle cusp.
             */
            KleinBottle = 4,
            /**
             * Specifies a vertex link that is closed and is not a sphere,
             * torus or Klein bottle.  In other words, this is an ideal vertex
             * but not one of the standard ideal vertex types.
             */
            NonStandardCusp = 5,
            /**
             * Specifies a vertex link that has boundary and is not a disc.
             * In other words, this vertex makes the triangulation invalid.
             */
            Invalid = 6
        };

        /**
         * A deprecated type alias that represents a type of vertex link.
         *
         * \deprecated This has been renamed to Link, and is now a scoped
         * enumeration.
         */
        using LinkType [[deprecated]] = Link;

        /**
         * A deprecated constant indicating one of the vertex link types.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Link::Sphere.
         */
        [[deprecated]] inline static constexpr Link SPHERE = Link::Sphere;

        /**
         * A deprecated constant indicating one of the vertex link types.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Link::Disc.
         */
        [[deprecated]] inline static constexpr Link DISC = Link::Disc;

        /**
         * A deprecated constant indicating one of the vertex link types.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Link::Torus.
         */
        [[deprecated]] inline static constexpr Link TORUS = Link::Torus;

        /**
         * A deprecated constant indicating one of the vertex link types.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Link::KleinBottle.
         */
        [[deprecated]] inline static constexpr Link KLEIN_BOTTLE =
            Link::KleinBottle;

        /**
         * A deprecated constant indicating one of the vertex link types.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Link::NonStandardCusp.
         */
        [[deprecated]] inline static constexpr Link NON_STANDARD_CUSP =
            Link::NonStandardCusp;

        /**
         * A deprecated constant indicating one of the vertex link types.
         *
         * \deprecated This has been renamed to the scoped enumeration constant
         * Link::Invalid.
         */
        [[deprecated]] inline static constexpr Link INVALID = Link::Invalid;

    private:
        Link link_;
            /**< A broad categorisation of the topology of the vertex link. */
        long linkEulerChar_;
            /**< Specifies the Euler characteristic of the vertex link. */
        Triangulation<2>* linkTri_;
            /**< A triangulation of the vertex link.  This will only be
                 constructed on demand; until then it will be null.
                 We keep this as a pointer to avoid instantiating the
                 lower-dimensional triangulation classes here in the header. */

    public:
        /**
         * Default destructor.
         */
        ~Face();

        /**
         * Returns a broad categorisation of the link of the vertex.
         * This considers topological information only, not the
         * combinatorics of how the link is triangulated.
         *
         * This routine does not require a full triangulation of the
         * vertex link, and so can be much faster than analysing the
         * result of buildLink().
         *
         * This is the routine that used to be called link() in Regina 6.0.1
         * and earlier.  It was renamed to linkType() in Regina 7.0, to clear
         * the way for a different routine to be called link() in the future.
         *
         * \return a broad categorisation of the vertex link.
         */
        Link linkType() const;

        /**
         * Returns a full 2-manifold triangulation describing
         * the link of this vertex.
         *
         * This routine is fast (it uses a pre-computed triangulation if
         * possible).  The downside is that the triangulation is read-only
         * (though you can always clone it).
         *
         * Regarding the labelling of triangles in the vertex link:
         *
         * - The triangles of the vertex link are numbered as follows.
         *   Let \a i lie between 0 and degree()-1 inclusive, let
         *   \a tet represent `embedding(i).tetrahedron()`,
         *   and let \a v represent `embedding(i).vertex()`.
         *   Then `buildLink()->triangle(i)` is the triangle
         *   in the vertex link that "slices off" vertex \a v from
         *   tetrahedron \a tet.  In other words,
         *   `buildLink()->triangle(i)` in the vertex link
         *   is parallel to triangle `tet->triangle(v)` in the
         *   surrounding 3-manifold triangulation.
         *
         * - The vertices of each triangle in the vertex link are
         *   numbered as follows.  Following the discussion above,
         *   suppose that `buildLink()->triangle(i)` sits within
         *   \c tet and is parallel to `tet->triangle(v)`.
         *   Then vertices 0,1,2 of the triangle in the link will be
         *   parallel to vertices 0,1,2 of the corresponding Triangle<3>.
         *   The permutation `tet->triangleMapping(v)` will map
         *   vertices 0,1,2 of the triangle in the link to the
         *   corresponding vertices of \c tet (those opposite \c v),
         *   and will map 3 to \c v itself.
         *
         * - If you need this labelling data in a format that is easy to
         *   compute with, you can call buildLinkInclusion() to retrieve
         *   this information as an isomorphism.
         *
         * \python Since Python does not distinguish between const and
         * non-const, this routine will return by value (thus making a
         * deep copy of the vertex link).  You are free to modify the
         * triangulation that is returned.
         *
         * \return the read-only triangulated link of the vertex.
         */
        const Triangulation<2>& buildLink() const;

        /**
         * Returns details of how the triangles are labelled in the link
         * of this vertex.  This is a companion function to buildLink(),
         * which returns a full 2-manifold triangulation of the vertex link.
         *
         * The documentation for buildLink() describes in plain English
         * exactly how the vertex link will be triangulated.  This function
         * essentially returns the same information in a machine-readable form.
         *
         * Specifically, this function returns an Isomorphism<3> that describes
         * how the individual triangles of the link sit within the tetrahedra
         * of the original triangulation.  If \a p is the isomorphism returned,
         * then `p.tetImage(i)` will indicate which tetrahedron
         * \a tet of the 3-manifold triangulation contains the <i>i</i>th
         * triangle of the link.  Moreover, `p.facePerm(i)` will
         * indicate exactly where the <i>i</i>th triangle sits within \a tet:
         * it will send 3 to the vertex of \a t that the triangle links,
         * and it will send 0,1,2 to the vertices of \a tet that are
         * parallel to vertices 0,1,2 of this triangle.
         *
         * Strictly speaking, this is an abuse of the Isomorphism<3> class
         * (the domain is a triangulation of the wrong dimension, and
         * the map is not 1-to-1 into the range tetrahedra).  We use
         * it anyway, but you should not attempt to call any high-level
         * routines (such as Isomorphism<3>::apply).
         *
         * This is the same isomorphism that was accessible through the
         * old buildLinkDetail() function in Regina 6.0.1 and earlier.
         *
         * \return details of how buildLink() labels the triangles of
         * the vertex link.
         */
        Isomorphism<3> buildLinkInclusion() const;

        /**
         * Determines if the link of this vertex is closed.
         *
         * \return \c true if and only if the link of this vertex is
         * closed.
         */
        bool isLinkClosed() const;

        /**
         * Determines if this vertex is an ideal vertex.
         * This requires the vertex link to be closed and not a
         * 2-sphere.
         *
         * \return \c true if and only if this is an ideal vertex.
         */
        bool isIdeal() const;

        /**
         * Determines if this vertex is standard.
         * This requires the vertex link to be a sphere, disc, torus or
         * Klein bottle.
         *
         * \return \c true if and only if this vertex is standard.
         */
        bool isStandard() const;

        /**
         * Returns the Euler characteristic of the vertex link.
         *
         * This routine does not require a full triangulation of the
         * vertex link, and so can be much faster than calling
         * buildLink().eulerChar().
         *
         * \return the Euler characteristic of the vertex link.
         */
        long linkEulerChar() const;

        /**
         * Returns the link of this vertex as a normal surface.
         *
         * Note that vertex linking surfaces only ever contain triangles
         * (not quadrilaterals).  Moreover, vertex links are always thin
         * (i.e., after constructing the frontier of a regular neighbourhood
         * of the vertex, no further normalisation steps are required).
         *
         * \return the corresponding vertex linking normal surface.
         */
        NormalSurface linkingSurface() const;

    private:
        /**
         * Creates a new vertex and marks it as belonging to the
         * given triangulation component.
         *
         * \param component the triangulation component to which this
         * vertex belongs.
         */
        Face(Component<3>* component);

    friend class Triangulation<3>;
    friend class detail::TriangulationBase<3>;
};

// Inline functions for Vertex<3>

inline Face<3, 0>::Face(Component<3>* component) :
        detail::FaceBase<3, 0>(component),
        linkEulerChar_(0), linkTri_(nullptr) {
}

inline Vertex<3>::Link Face<3, 0>::linkType() const {
    return link_;
}

inline bool Face<3, 0>::isLinkClosed() const {
    return (link_ != Link::Disc && link_ != Link::Invalid);
}

inline bool Face<3, 0>::isIdeal() const {
    return (link_ == Link::Torus || link_ == Link::KleinBottle ||
        link_ == Link::NonStandardCusp);
}

inline bool Face<3, 0>::isStandard() const {
    return (link_ != Link::NonStandardCusp && link_ != Link::Invalid);
}

inline long Face<3, 0>::linkEulerChar() const {
    return linkEulerChar_;
}

inline NormalSurface Face<3, 0>::linkingSurface() const {
    return std::move(triangulation().linkingSurface(*this).first);
}

} // namespace regina

#endif

