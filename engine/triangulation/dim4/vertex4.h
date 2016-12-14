
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

/*! \file triangulation/dim4/vertex4.h
 *  \brief Internal header for vertices in a 4-manifold triangulation.
 *
 *  This file is automatically included from triangulation/dim4.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation4.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation4.h.
#include "triangulation/dim4/triangulation4.h"

#ifndef __VERTEX4_H
#ifndef __DOXYGEN
#define __VERTEX4_H
#endif

namespace regina {

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Represents a vertex in the skeleton of a 4-manifold triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * documentation for Face for a general overview of how this class works.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions"
 * offer significant extra functionality.
 *
 * \headerfile triangulation/dim4.h
 */
template <>
class REGINA_API Face<4, 0> : public detail::FaceBase<4, 0> {
    private:
        Triangulation<3>* link_;
            /**< The link of this vertex, given as a full-blown
                 3-manifold triangulation.  It is guaranteed that 3-sphere
                 recognition has already been run over this triangulation
                 (and so future 3-sphere queries will be very fast). */
        bool ideal_;
            /**< Is this vertex ideal? */

    public:
        /**
         * Default destructor.
         */
        ~Face();

        /**
         * Returns a full 3-manifold triangulation describing
         * the link of this vertex.
         *
         * This routine is fast (it uses a pre-computed triangulation).
         * The downside is that the triangulation is read-only, and does
         * not contain any information on how the tetrahedra in the link
         * correspond to pentachora in the original triangulation
         * (though this is easily deduced; see below).  If you want a
         * writable triangulation, or one with this extra information,
         * then call buildLinkDetail() instead.
         *
         * The triangulation of the vertex link is built as follows.
         * Let \a i lie between 0 and degree()-1 inclusive, let
         * \a pent represent <tt>embedding(i).pentachoron()</tt>,
         * and let \a v represent <tt>embedding(i).vertex()</tt>.
         * Then <tt>buildLink()->tetrahedron(i)</tt> is the tetrahedron
         * in the vertex link that "slices off" vertex \a v from
         * pentachoron \a pent.  In other words,
         * <tt>buildLink()->tetrahedron(i)</tt> in the vertex link
         * is parallel to tetrahedron <tt>pent->tetrahedron(v)</tt> in the
         * surrounding 4-manifold triangulation.
         *
         * The vertices of each tetrahedron in the vertex link are
         * numbered as follows.  Following the discussion above,
         * suppose that <tt>buildLink()->tetrahedron(i)</tt>
         * sits within \c pent and is parallel to
         * <tt>pent->tetrahedron(v)</tt>.
         * Then vertices 0,1,2,3 of the tetrahedron in the link will be
         * parallel to vertices 0,1,2,3 of the corresponding Tetrahedron<4>.
         * The permutation <tt>pent->tetrahedronMapping(v)</tt> will map
         * vertices 0,1,2,3 of the tetrahedron in the link to the
         * corresponding vertices of \c pent (those opposite \c v),
         * and will map 4 to \c v itself.
         *
         * This Vertex<4> object will retain ownership of the triangulation
         * that is returned.  If you wish to edit the triangulation, you
         * should make a new clone and edit the clone instead.
         *
         * \ifacespython Since Python does not distinguish between const and
         * non-const, this routine will make a deep copy of the vertex link.
         * You are free to modify the triangulation that is returned.
         *
         * @return the read-only triangulated link of this vertex.
         */
        REGINA_INLINE_REQUIRED
        const Triangulation<3>* buildLink() const;

        /**
         * Returns a full 3-manifold triangulation describing
         * the link of this vertex.
         *
         * This routine is heavyweight (it computes a new triangulation
         * each time).  The benefit is that the triangulation is writeable,
         * and optionally contain detailed information on how the tetrahedra
         * in the link correspond to pentachora in the original triangulation.
         * If you do not need this extra information, consider using the
         * faster buildLink() instead.
         *
         * See the buildLink() documentation for an explanation of
         * exactly how the triangulation will be constructed.
         *
         * If \a labels is passed as \c true, each tetrahedron of the new
         * vertex link will be given a text description of the form
         * <tt>p&nbsp;(v)</tt>, where \c p is the index of the pentachoron
         * the tetrahedron is from, and \c v is the vertex of that pentachoron
         * that this tetrahedron links.
         *
         * If \a inclusion is non-null (i.e., it points to some
         * Isomorphism<4> pointer \a p), then it will be modified to
         * point to a new Isomorphism<4> that describes in detail how the
         * individual tetrahedra of the link sit within pentachora of
         * the original triangulation.  Specifically, after this routine
         * is called, <tt>p->pentImage(i)</tt> will indicate which pentachoron
         * \a pent of the 4-manifold triangulation contains the <i>i</i>th
         * tetrahedron of the link.  Moreover, <tt>p->facetPerm(i)</tt> will
         * indicate exactly where the <i>i</i>th tetrahedron sits within
         * \a pent: it will send 4 to the vertex of \a pent that the
         * tetrahedron links, and it will send 0,1,2,3 to the vertices of
         * \a pent that are parallel to vertices 0,1,2,3 of this tetrahedron.
         *
         * The triangulation that is returned, as well as the isomorphism
         * if one was requested, will be newly allocated.  The caller of
         * this routine is responsible for destroying these objects.
         *
         * Strictly speaking, this is an abuse of the Isomorphism<4> class
         * (the domain is a triangulation of the wrong dimension, and
         * the map is not 1-to-1 into the range pentachora).  We use
         * it anyway, but you should not attempt to call any high-level
         * routines (such as Isomorphism<4>::apply).
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
        Triangulation<3>* buildLinkDetail(bool labels = true,
            Isomorphism<4>** inclusion = 0) const;

        /**
         * Determines if this vertex is an ideal vertex.
         * To be an ideal, a vertex must (i) be valid, and (ii) have
         * a closed vertex link that is not a 3-sphere.
         *
         * @return \c true if and only if this is an ideal vertex.
         */
        bool isIdeal() const;

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
         * @param component the triangulation component to which this
         * vertex belongs.
         */
        Face(Component<4>* component);

    friend class Triangulation<4>;
    friend class detail::TriangulationBase<4>;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4VertexEmbedding, you should
 * use either the new alias VertexEmbedding<4>, or the full class name
 * FaceEmbedding<4, 0>.
 */
REGINA_DEPRECATED typedef FaceEmbedding<4, 0> Dim4VertexEmbedding;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4Vertex, you should use
 * either the new alias Vertex<4>, or the full class name Face<4, 0>.
 */
REGINA_DEPRECATED typedef Face<4, 0> Dim4Vertex;

/*@}*/

// Inline functions for Vertex<4>

inline Face<4, 0>::Face(Component<4>* component) :
        detail::FaceBase<4, 0>(component), link_(0), ideal_(false) {
}

inline const Triangulation<3>* Face<4, 0>::buildLink() const {
    return link_;
}

inline bool Face<4, 0>::isIdeal() const {
    return ideal_;
}

} // namespace regina

#endif

