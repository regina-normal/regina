
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

#ifndef __EDGE4_H
#ifndef __DOXYGEN
#define __EDGE4_H
#endif

namespace regina {

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Represents an edge in the skeleton of a 4-manifold triangulation.
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
class REGINA_API Face<4, 1> : public detail::FaceBase<4, 1> {
    private:
        Triangulation<2>* link_;
            /**< A triangulation of the edge link.  This will only be
             * constructed on demand; until then it will be null. */

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
         * read-only, and does not contain any information on how the
         * triangles in the link correspond to pentachora in the original
         * triangulation (though this is easily deduced; see below).
         * If you want a writable triangulation, or one with this extra
         * information, then call buildLinkDetail() instead.
         *
         * The triangulation of the edge link is built as follows.
         * Let \a i lie between 0 and degree()-1 inclusive, let
         * \a pent represent <tt>embedding(i).pentachoron()</tt>,
         * and let \a e represent <tt>embedding(i).edge()</tt>.
         * Then <tt>buildLink()->triangle(i)</tt> is the triangle
         * in the edge link that links edge \a e of pentachoron \a pent.
         * In other words, <tt>buildLink()->triangle(i)</tt> in the edge link
         * is parallel to triangle <tt>pent->triangle(e)</tt> in the
         * surrounding 4-manifold triangulation.
         *
         * The vertices of each triangle in the edge link are
         * numbered as follows.  Following the discussion above,
         * suppose that <tt>buildLink()->triangle(i)</tt>
         * sits within \c pent and is parallel to
         * <tt>pent->triangle(e)</tt>.
         * Then vertices 0,1,2 of the triangle in the link will be
         * parallel to vertices 0,1,2 of the corresponding Triangle<4>.
         * The permutation <tt>pent->triangleMapping(e)</tt> will map
         * vertices 0,1,2 of the triangle in the link to the
         * corresponding vertices of \c pent (those opposite \c e),
         * and will map 3 and 4 to the vertices of \c e itself.
         *
         * This Edge<4> object will retain ownership of the triangulation
         * that is returned.  If you wish to edit the triangulation, you
         * should make a new clone and edit the clone instead.
         *
         * \ifacespython Since Python does not distinguish between const and
         * non-const, this routine will make a deep copy of the edge link.
         * You are free to modify the triangulation that is returned.
         *
         * @return the read-only triangulated link of this edge.
         */
        const Triangulation<2>* buildLink() const;

        /**
         * Returns a full 2-manifold triangulation describing
         * the link of this edge.
         *
         * This routine is heavyweight (it computes a new triangulation
         * each time).  The benefit is that the triangulation is writeable,
         * and optionally contain detailed information on how the triangles
         * in the link correspond to pentachora in the original triangulation.
         * If you do not need this extra information, consider using the
         * faster buildLink() instead.
         *
         * See the buildLink() documentation for an explanation of
         * exactly how the triangulation will be constructed.
         *
         * If \a labels is passed as \c true, each triangle of the new
         * edge link will be given a text description of the form
         * <tt>p&nbsp;(e)</tt>, where \c p is the index of the pentachoron
         * the triangle is from, and \c e is the edge of that pentachoron
         * that this triangle links.
         *
         * If \a inclusion is non-null (i.e., it points to some
         * Isomorphism<4> pointer \a p), then it will be modified to
         * point to a new Isomorphism<4> that describes in detail how the
         * individual triangles of the link sit within pentachora of
         * the original triangulation.  Specifically, after this routine
         * is called, <tt>p->pentImage(i)</tt> will indicate which pentachoron
         * \a pent of the 4-manifold triangulation contains the <i>i</i>th
         * triangle of the link.  Moreover, <tt>p->facetPerm(i)</tt> will
         * indicate exactly where the <i>i</i>th triangle sits within
         * \a pent: (i) it will send 3,4 to the vertices of \a pent that lie
         * on the edge that the triangle links, with 3 and 4 mapping to
         * vertices 0 and 1 respectively of the corresponding Edge<4>;
         * and (ii) it will send 0,1,2 to the vertices of \a pent that
         * are parallel to vertices 0,1,2 of this triangle.
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
         * non-const, this routine will make a deep copy of the edge link.
         * You are free to modify the triangulation that is returned.
         *
         * @return a newly constructed triangulation of the link of this edge.
         */
        Triangulation<2>* buildLinkDetail(bool labels = true,
            Isomorphism<4>** inclusion = 0) const;

    private:
        /**
         * Creates a new edge and marks it as belonging to the
         * given triangulation component.
         *
         * @param component the triangulation component to which this
         * edge belongs.
         */
        Face(Component<4>* component);

    friend class Triangulation<4>;
    friend class detail::TriangulationBase<4>;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4EdgeEmbedding, you should
 * use either the new alias EdgeEmbedding<4>, or the full class name
 * FaceEmbedding<4, 1>.
 */
REGINA_DEPRECATED typedef FaceEmbedding<4, 1> Dim4EdgeEmbedding;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4Edge, you should use
 * either the new alias Edge<4>, or the full class name Face<4, 1>.
 */
REGINA_DEPRECATED typedef Face<4, 1> Dim4Edge;

/*@}*/

// Inline functions for Edge<4>

inline Face<4, 1>::Face(Component<4>* component) :
        detail::FaceBase<4, 1>(component), link_(0) {
}

inline const Triangulation<2>* Face<4, 1>::buildLink() const {
    if (! link_) {
        // This is a construct-on-demand member; cast away constness to
        // set it here.
        const_cast<Edge<4>*>(this)->link_ = buildLinkDetail(false, 0);
    }
    return link_;
}

} // namespace regina

#endif

