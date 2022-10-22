
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file triangulation/dim4/edge4.h
 *  \brief Internal header for edges in a 4-manifold triangulation.
 *
 *  This file is automatically included from triangulation/dim4.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation4.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation4.h.
#include "triangulation/dim4/triangulation4.h"

#ifndef __REGINA_EDGE4_H
#ifndef __DOXYGEN
#define __REGINA_EDGE4_H
#endif

namespace regina {

/**
 * Represents an edge in the skeleton of a 4-manifold triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * documentation for Face for a general overview of how this class works.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions"
 * offer significant extra functionality.
 *
 * Edges do not support value semantics: they cannot be copied, swapped,
 * or manually constructed.  Their location in memory defines them, and
 * they are often passed and compared by pointer.  End users are never
 * responsible for their memory management; this is all taken care of by
 * the Triangulation to which they belong.
 *
 * \headerfile triangulation/dim4.h
 *
 * \ingroup dim4
 */
template <>
class Face<4, 1> : public detail::FaceBase<4, 1> {
    private:
        Triangulation<2>* link_;
            /**< A triangulation of the edge link.  This will only be
                 constructed on demand; until then it will be null.
                 We keep this as a pointer to avoid instantiating the
                 lower-dimensional triangulation classes here in the header. */

    public:
        /**
         * Default destructor.
         */
        ~Face();

        /**
         * Returns a full 2-manifold triangulation describing
         * the link of this edge.
         *
         * This routine is fast (it uses a pre-computed triangulation
         * where possible).  The downside is that the triangulation is
         * read-only (though you can always clone it).
         *
         * Regarding the labelling of triangles in the edge link:
         *
         * - The triangles of the edge link are numbered as follows.
         *   Let \a i lie between 0 and degree()-1 inclusive, let
         *   \a pent represent `embedding(i).pentachoron()`,
         *   and let \a e represent `embedding(i).edge()`.
         *   Then `buildLink()->triangle(i)` is the triangle
         *   in the edge link that links edge \a e of pentachoron \a pent.
         *   In other words, `buildLink()->triangle(i)` in the edge link
         *   is parallel to triangle `pent->triangle(e)` in the
         *   surrounding 4-manifold triangulation.
         *
         * - The vertices of each triangle in the edge link are
         *   numbered as follows.  Following the discussion above,
         *   suppose that `buildLink()->triangle(i)`
         *   sits within \c pent and is parallel to
         *   `pent->triangle(e)`.
         *   Then vertices 0,1,2 of the triangle in the link will be
         *   parallel to vertices 0,1,2 of the corresponding Triangle<4>.
         *   The permutation `pent->triangleMapping(e)` will map
         *   vertices 0,1,2 of the triangle in the link to the
         *   corresponding vertices of \c pent (those opposite \c e),
         *   and will map 3 and 4 to the vertices of \c e itself.
         *
         * - If you need this labelling data in a format that is easy to
         *   compute with, you can call buildLinkInclusion() to retrieve
         *   this information as an isomorphism.
         *
         * \ifacespython Since Python does not distinguish between const and
         * non-const, this routine will return by value (thus making a
         * deep copy of the edge link).  You are free to modify the
         * triangulation that is returned.
         *
         * \return the read-only triangulated link of this edge.
         */
        const Triangulation<2>& buildLink() const;

        /**
         * Returns details of how the triangles are labelled in the link
         * of this edge.  This is a companion function to buildLink(),
         * which returns a full 2-manifold triangulation of the edge link.
         *
         * The documentation for buildLink() describes in plain English
         * exactly how the edge link will be triangulated.  This function
         * essentially returns the same information in a machine-readable form.
         *
         * Specifically, this function returns an Isomorphism<4> that describes
         * how the individual triangles of the link sit within the pentachora
         * of the original triangulation.  If \a p is the isomorphism returned,
         * then `p.pentImage(i)` will indicate which pentachoron
         * \a pent of the 4-manifold triangulation contains the <i>i</i>th
         * triangle of the link.  Moreover, `p.facetPerm(i)` will
         * indicate exactly where the <i>i</i>th triangle sits within
         * \a pent: (i) it will send 3,4 to the vertices of \a pent that lie
         * on the edge that the triangle links, with 3 and 4 mapping to
         * vertices 0 and 1 respectively of the corresponding Edge<4>;
         * and (ii) it will send 0,1,2 to the vertices of \a pent that
         * are parallel to vertices 0,1,2 of this triangle.
         *
         * Strictly speaking, this is an abuse of the Isomorphism<4> class
         * (the domain is a triangulation of the wrong dimension, and
         * the map is not 1-to-1 into the range pentachora).  We use
         * it anyway, but you should not attempt to call any high-level
         * routines (such as Isomorphism<4>::apply).
         *
         * This is the same isomorphism that was accessible through the
         * old buildLinkDetail() function in Regina 6.0.1 and earlier.
         *
         * \return details of how buildLink() labels the triangles of
         * the edge link.
         */
        Isomorphism<4> buildLinkInclusion() const;

        /**
         * Returns the link of this edge as a normal hypersurface.
         *
         * Constructing the link of a edge begins with building the frontier
         * of a regular neighbourhood of the edge.  If this is already a
         * normal hypersurface, then then link is called _thin_.  Otherwise
         * some basic normalisation steps are performed until the hypersurface
         * becomes normal; note that these normalisation steps could
         * change the topology of the hypersurface, and in some pathological
         * cases could even reduce it to the empty hypersurface.
         *
         * Although normalisation of arbitrary embedded 3-manifolds is messy,
         * for edge links the process is thankfully simpler.  Essentially,
         * any changes will be limited to operations analagous to compressions
         * and boundary compressions along discs and 3-balls, as well as
         * removing trivial 4-sphere components.
         *
         * \return a pair (\a s, \a thin), where \a s is the edge linking
         * normal hypersurface, and \a thin is \c true if and only if this link
         * is thin (i.e., no additional normalisation steps were required).
         */
        std::pair<NormalHypersurface, bool> linkingSurface() const;

    private:
        /**
         * Creates a new edge and marks it as belonging to the
         * given triangulation component.
         *
         * \param component the triangulation component to which this
         * edge belongs.
         */
        Face(Component<4>* component);

    friend class Triangulation<4>;
    friend class detail::TriangulationBase<4>;
};

// Inline functions for Edge<4>

inline Face<4, 1>::Face(Component<4>* component) :
        detail::FaceBase<4, 1>(component), link_(nullptr) {
}

inline std::pair<NormalHypersurface, bool> Face<4, 1>::linkingSurface() const {
    return triangulation().linkingSurface(*this);
}

} // namespace regina

#endif

