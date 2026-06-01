
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

ENSURE_ESSENTIAL_REGINA_HEADERS

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
 * \pyclassname{Vertex3}
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

    private:
        Link linkType_;
            /**< A broad categorisation of the topology of the vertex link. */

    public:
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
        detail::FaceBase<3, 0>(component) {
}

inline Vertex<3>::Link Face<3, 0>::linkType() const {
    return linkType_;
}

inline bool Face<3, 0>::isIdeal() const {
    return (linkType_ == Link::Torus || linkType_ == Link::KleinBottle ||
        linkType_ == Link::NonStandardCusp);
}

inline bool Face<3, 0>::isStandard() const {
    return (linkType_ != Link::NonStandardCusp && linkType_ != Link::Invalid);
}

} // namespace regina

#endif

