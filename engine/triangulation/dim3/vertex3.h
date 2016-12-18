
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

#ifndef __VERTEX3_H
#ifndef __DOXYGEN
#define __VERTEX3_H
#endif

namespace regina {

/**
 * \weakgroup dim3
 * @{
 */

/**
 * Represents a vertex in the skeleton of a 3-manifold triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * documentation for Face for a general overview of how this class works.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions"
 * offer significant extra functionality.
 *
 * \headerfile triangulation/dim3.h
 */
template <>
class REGINA_API Face<3, 0> : public detail::FaceBase<3, 0> {
    public:
        /**
         * Categorises the possible links of a vertex into a small number
         * of common types.  Here a vertex link is considered only up to its
         * topology (not the combinatorics of its triangulation).
         *
         * @see link
         */
        enum LinkType {
            SPHERE = 1,
                /**< Specifies a vertex link that is a sphere.
                     In other words, the vertex is internal. */
            DISC = 2,
                /**< Specifies a vertex link that is a disc.
                     In other words, the vertex lies on a real boundary
                     component. */
            TORUS = 3,
                /**< Specifies a vertex link that is a torus.
                     In other words, this is an ideal vertex
                     representing a torus cusp. */
            KLEIN_BOTTLE = 4,
                /**< Specifies a vertex link that is a Klein bottle.
                     In other words, this is an ideal vertex
                     representing a Klein bottle cusp. */
            NON_STANDARD_CUSP = 5,
                /**< Specifies a vertex link that is closed and is not a
                     sphere, torus or Klein bottle.
                     In other words, this is an ideal vertex but not one
                     of the standard ideal vertex types. */
            INVALID = 6
                /**< Specifies a vertex link that has boundary and is not a
                     disc.  In other words, this vertex makes the
                     triangulation invalid. */
        };
    private:
        LinkType link_;
            /**< A broad categorisation of the topology of the vertex link. */
        long linkEulerChar_;
            /**< Specifies the Euler characteristic of the vertex link. */
        Triangulation<2>* linkTri_;
            /**< A triangulation of the vertex link.  This will only be
                 constructed on demand; until then it will be null. */

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
         * @return a broad categorisation of the vertex link.
         */
        LinkType link() const;

        /**
         * Returns a full 2-manifold triangulation describing
         * the link of this vertex.
         *
         * This routine is fast (it uses a pre-computed triangulation if
         * possible).  The downside is that the triangulation is read-only,
         * and does not contain any information on how the triangles in the
         * link correspond to tetrahedra in the original triangulation
         * (though this is easily deduced; see below).
         * If you want a writable triangulation, or one with this extra
         * information, then call buildLinkDetail() instead.
         *
         * The triangulation of the vertex link is built as follows.
         * Let \a i lie between 0 and degree()-1 inclusive, let
         * \a tet represent <tt>embedding(i).tetrahedron()</tt>,
         * and let \a v represent <tt>embedding(i).vertex()</tt>.
         * Then <tt>buildLink()->triangle(i)</tt> is the triangle
         * in the vertex link that "slices off" vertex \a v from
         * tetrahedron \a tet.  In other words,
         * <tt>buildLink()->triangle(i)</tt> in the vertex link
         * is parallel to triangle <tt>tet->triangle(v)</tt> in the
         * surrounding 3-manifold triangulation.
         *
         * The vertices of each triangle in the vertex link are
         * numbered as follows.  Following the discussion above,
         * suppose that <tt>buildLink()->triangle(i)</tt> sits within
         * \c tet and is parallel to <tt>tet->triangle(v)</tt>.
         * Then vertices 0,1,2 of the triangle in the link will be
         * parallel to vertices 0,1,2 of the corresponding Triangle<3>.
         * The permutation <tt>tet->triangleMapping(v)</tt> will map
         * vertices 0,1,2 of the triangle in the link to the
         * corresponding vertices of \c tet (those opposite \c v),
         * and will map 3 to \c v itself.
         *
         * This Vertex<3> object will retain ownership of the triangulation
         * that is returned.  If you wish to edit the triangulation, you
         * should make a new clone and edit the clone instead.
         *
         * \ifacespython Since Python does not distinguish between const and
         * non-const, this routine will make a deep copy of the vertex link.
         * You are free to modify the triangulation that is returned.
         *
         * @return the read-only triangulated link of the vertex.
         */
        const Triangulation<2>* buildLink() const;

        /**
         * Returns a full 2-manifold triangulation describing
         * the link of this vertex.
         *
         * This routine is heavyweight (it computes a new triangulation
         * each time).  The benefit is that the triangulation is writeable,
         * and optionally contain detailed information on how the triangles
         * in the link correspond to tetrahedra in the original triangulation.
         * If you do not need this extra information, consider using the
         * faster buildLink() instead.
         *
         * See the buildLink() documentation for an explanation of
         * exactly how the triangulation will be constructed.
         *
         * If \a labels is passed as \c true, each triangle of the new
         * vertex link will be given a text description of the form
         * <tt>t&nbsp;(v)</tt>, where \c t is the index of the tetrahedron
         * the triangle is from, and \c v is the vertex of that tetrahedron
         * that this triangle links.
         *
         * If \a inclusion is non-null (i.e., it points to some
         * Isomorphism<3> pointer \a p), then it will be modified to
         * point to a new Isomorphism<3> that describes in detail how the
         * individual triangles of the link sit within tetrahedra of
         * the original triangulation.  Specifically, after this routine
         * is called, <tt>p->tetImage(i)</tt> will indicate which tetrahedron
         * \a tet of the 3-manifold triangulation contains the <i>i</i>th
         * triangle of the link.  Moreover, <tt>p->facePerm(i)</tt> will
         * indicate exactly where the <i>i</i>th triangle sits within \a tet:
         * it will send 3 to the vertex of \a t that the triangle links,
         * and it will send 0,1,2 to the vertices of \a tet that are
         * parallel to vertices 0,1,2 of this triangle.
         *
         * The triangulation that is returned, as well as the isomorphism
         * if one was requested, will be newly allocated.  The caller of
         * this routine is responsible for destroying these objects.
         *
         * Strictly speaking, this is an abuse of the Isomorphism<3> class
         * (the domain is a triangulation of the wrong dimension, and
         * the map is not 1-to-1 into the range tetrahedra).  We use
         * it anyway, but you should not attempt to call any high-level
         * routines (such as Isomorphism<3>::apply).
         *
         * \ifacespython The second (isomorphism) argument is not present.
         * Instead this routine returns a pair (triangulation, isomorphism).
         * As a side-effect, the isomorphism will always be constructed
         * (i.e., it is not optional).
         *
         * \ifacespython Since Python does not distinguish between const and
         * non-const, this routine will make a deep copy of the vertex link.
         * You are free to modify the triangulation that is returned.
         *
         * @return a newly constructed triangulation of the link of this vertex.
         */
        Triangulation<2>* buildLinkDetail(bool labels = true,
            Isomorphism<3>** inclusion = 0) const;

        /**
         * Determines if the link of this vertex is closed.
         *
         * @return \c true if and only if the link of this vertex is
         * closed.
         */
        bool isLinkClosed() const;

        /**
         * Determines if this vertex is an ideal vertex.
         * This requires the vertex link to be closed and not a
         * 2-sphere.
         *
         * @return \c true if and only if this is an ideal vertex.
         */
        bool isIdeal() const;

        /**
         * Determines if this vertex is standard.
         * This requires the vertex link to be a sphere, disc, torus or
         * Klein bottle.
         *
         * @return \c true if and only if this vertex is standard.
         */
        bool isStandard() const;

        /**
         * Returns the Euler characteristic of the vertex link.
         *
         * This routine does not require a full triangulation of the
         * vertex link, and so can be much faster than calling
         * buildLink().eulerChar().
         *
         * @return the Euler characteristic of the vertex link.
         */
        REGINA_INLINE_REQUIRED
        long linkEulerChar() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new vertex and marks it as belonging to the
         * given triangulation component.
         *
         * @param myComponent the triangulation component to which this
         * vertex belongs.
         */
        Face(Component<3>* component);

    friend class Triangulation<3>;
    friend class detail::TriangulationBase<3>;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef NVertexEmbedding, you should
 * use either the new alias VertexEmbedding<3>, or the full class name
 * FaceEmbedding<3, 0>.
 */
REGINA_DEPRECATED typedef FaceEmbedding<3, 0> NVertexEmbedding;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef NVertex, you should use
 * either the new alias Vertex<3>, or the full class name Face<3, 0>.
 */
REGINA_DEPRECATED typedef Face<3, 0> NVertex;

/*@}*/

// Inline functions for Vertex<3>

inline Face<3, 0>::Face(Component<3>* component) :
        detail::FaceBase<3, 0>(component), linkEulerChar_(0), linkTri_(0) {
}

inline Vertex<3>::LinkType Face<3, 0>::link() const {
    return link_;
}

inline const Triangulation<2>* Face<3, 0>::buildLink() const {
    if (! linkTri_) {
        // This is a construct-on-demand member: cast away constness to
        // set it here.
        const_cast<Vertex<3>*>(this)->linkTri_ = buildLinkDetail(false, 0);
    }
    return linkTri_;
}

inline bool Face<3, 0>::isLinkClosed() const {
    return (link_ != DISC && link_ != INVALID);
}

inline bool Face<3, 0>::isIdeal() const {
    return (link_ == TORUS || link_ == KLEIN_BOTTLE ||
        link_ == NON_STANDARD_CUSP);
}

inline bool Face<3, 0>::isStandard() const {
    return (link_ != NON_STANDARD_CUSP && link_ != INVALID);
}

inline long Face<3, 0>::linkEulerChar() const {
    return linkEulerChar_;
}

} // namespace regina

#endif

