
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file triangulation/nvertex.h
 *  \brief Deals with vertices in a triangulation.
 */

#ifndef __NVERTEX_H
#ifndef __DOXYGEN
#define __NVERTEX_H
#endif

#include "regina-core.h"
#include "generic/face.h"
#include "maths/nperm4.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

class NBoundaryComponent;

template <int> class Component;
template <int> class Isomorphism;
template <int> class Simplex;
template <int> class Triangulation;
typedef Component<3> NComponent;
typedef Isomorphism<3> NIsomorphism;
typedef Simplex<3> NTetrahedron;
typedef Triangulation<2> Dim2Triangulation;
typedef Triangulation<3> NTriangulation;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * A convenience typedef for FaceEmbedding<3, 0>.
 */
typedef FaceEmbedding<3, 0> NVertexEmbedding;

/**
 * Represents a vertex in the skeleton of a 3-manifold triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * documentation for Face for a general overview of how this class works.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions"
 * offer significant extra functionality.
 */
template <>
class REGINA_API Face<3, 0> : public detail::FaceBase<3, 0>,
        public Output<Face<3, 0>> {
    public:
        /**
         * Categorises the possible links of a vertex into a small number
         * of common types.  Here a vertex link is considered only up to its
         * topology (not the combinatorics of its triangulation).
         *
         * @see getLink
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
            NON_STANDARD_BDRY = 6
                /**< Specifies a vertex link that has boundary and is not a
                     disc.  In other words, this vertex makes the
                     triangulation invalid. */
        };
    private:
        NBoundaryComponent* boundaryComponent_;
            /**< The boundary component that this vertex is a part of,
                 or 0 if this vertex is internal. */
        LinkType link_;
            /**< A broad categorisation of the topology of the vertex link. */
        long linkEulerChar_;
            /**< Specifies the Euler characteristic of the vertex link. */
        Dim2Triangulation* linkTri_;
            /**< A triangulation of the vertex link.  This will only be
                 constructed on demand; until then it will be null. */

    public:
        /**
         * Default destructor.
         */
        ~Face();

        /**
         * Returns the boundary component of the triangulation to which
         * this vertex belongs.
         *
         * See the note in the NBoundaryComponent overview regarding what
         * happens if the vertex link is a multiply punctured surface.
         * Note that this makes the vertex non-standard and the
         * triangulation invalid.
         *
         * An ideal vertex will have its own individual boundary
         * component to which it belongs.
         *
         * @return the boundary component containing this vertex,
         * or 0 if this vertex is not on the boundary of the triangulation
         * as determined by isBoundary().
         */
        NBoundaryComponent* boundaryComponent() const;
        /**
         * Deprecated routine that returns the boundary component of the
         * triangulation to which this vertex belongs.
         *
         * \deprecated This routine has been renamed as boundaryComponent().
         * See the boundaryComponent() documentation for further details.
         */
        NBoundaryComponent* getBoundaryComponent() const;

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
        LinkType getLink() const;

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
         * \a tet represent <tt>getEmbedding(i).getTetrahedron()</tt>,
         * and let \a v represent <tt>getEmbedding(i).getVertex()</tt>.
         * Then <tt>buildLink()->getTriangle(i)</tt> is the triangle
         * in the vertex link that "slices off" vertex \a v from
         * tetrahedron \a tet.  In other words,
         * <tt>buildLink()->getTriangle(i)</tt> in the vertex link
         * is parallel to triangle <tt>tet->getTriangle(v)</tt> in the
         * surrounding 3-manifold triangulation.
         *
         * The vertices of each triangle in the vertex link are
         * numbered as follows.  Following the discussion above,
         * suppose that <tt>buildLink()->getTriangle(i)</tt> sits within
         * \c tet and is parallel to <tt>tet->getTriangle(v)</tt>.
         * Then vertices 0,1,2 of the triangle in the link will be
         * parallel to vertices 0,1,2 of the corresponding NTriangle.
         * The permutation <tt>tet->getTriangleMapping(v)</tt> will map
         * vertices 0,1,2 of the triangle in the link to the
         * corresponding vertices of \c tet (those opposite \c v),
         * and will map 3 to \c v itself.
         *
         * This NVertex object will retain ownership of the triangulation
         * that is returned.  If you wish to edit the triangulation, you
         * should make a new clone and edit the clone instead.
         *
         * @return the read-only triangulated link of the vertex.
         */
        const Dim2Triangulation* buildLink() const;

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
         * NIsomorphism pointer \a p), then it will be modified to
         * point to a new NIsomorphism that describes in detail how the
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
         * Strictly speaking, this is an abuse of the NIsomorphism class
         * (the domain is a triangulation of the wrong dimension, and
         * the map is not 1-to-1 into the range tetrahedra).  We use
         * it anyway, but you should not attempt to call any high-level
         * routines (such as NIsomorphism::apply).
         *
         * \ifacespython The second (isomorphism) argument is not present.
         * Instead this routine returns a pair (triangulation, isomorphism).
         * As a side-effect, the isomorphism will always be constructed
         * (i.e., it is not optional).
         *
         * @return a newly constructed triangulation of the link of this vertex.
         */
        Dim2Triangulation* buildLinkDetail(bool labels = true,
            NIsomorphism** inclusion = 0) const;

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
         * Determines if this vertex lies on the boundary of the
         * triangulation.  Ideal vertices are included as
         * being on the boundary.  In fact, the only vertices not
         * considered as on the boundary are those whose links are
         * spheres.
         *
         * @return \c true if and only if this vertex lies on the boundary.
         * @see isIdeal()
         */
        bool isBoundary() const;

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
        long getLinkEulerChar() const;

        /**
         * A deprecated alias for getLinkEulerChar().
         *
         * Returns the Euler characteristic of the vertex link.
         *
         * This routine does not require a full triangulation of the
         * vertex link, and so can be much faster than calling
         * buildLink().eulerChar().
         *
         * \deprecated This routine will be removed in a future version of
         * Regina.  Please use the identical routine getLinkEulerChar() instead.
         *
         * @return the Euler characteristic of the vertex link.
         */
        long getLinkEulerCharacteristic() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Creates a new vertex and marks it as belonging to the
         * given triangulation component.
         *
         * @param myComponent the triangulation component to which this
         * vertex belongs.
         */
        Face(NComponent* component);

    friend class Triangulation<3>;
    friend class detail::TriangulationBase<3>;
};

/**
 * A convenience typedef for Face<3, 0>.
 */
typedef Face<3, 0> NVertex;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "triangulation/ntetrahedron.h"
namespace regina {

// Inline functions for NVertex

inline Face<3, 0>::Face(NComponent* component) :
        detail::FaceBase<3, 0>(component),
        boundaryComponent_(0), linkEulerChar_(0), linkTri_(0) {
}

inline NBoundaryComponent* Face<3, 0>::boundaryComponent() const {
    return boundaryComponent_;
}

inline NBoundaryComponent* Face<3, 0>::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline NVertex::LinkType Face<3, 0>::getLink() const {
    return link_;
}

inline const Dim2Triangulation* Face<3, 0>::buildLink() const {
    if (! linkTri_) {
        // This is a construct-on-demand member: cast away constness to
        // set it here.
        const_cast<NVertex*>(this)->linkTri_ = buildLinkDetail(false, 0);
    }
    return linkTri_;
}

inline bool Face<3, 0>::isLinkClosed() const {
    return (link_ != DISC && link_ != NON_STANDARD_BDRY);
}

inline bool Face<3, 0>::isIdeal() const {
    return (link_ == TORUS || link_ == KLEIN_BOTTLE ||
        link_ == NON_STANDARD_CUSP);
}

inline bool Face<3, 0>::isBoundary() const {
    return (boundaryComponent_ != 0);
}

inline bool Face<3, 0>::isStandard() const {
    return (link_ != NON_STANDARD_CUSP && link_ != NON_STANDARD_BDRY);
}

inline long Face<3, 0>::getLinkEulerChar() const {
    return linkEulerChar_;
}

inline long Face<3, 0>::getLinkEulerCharacteristic() const {
    return linkEulerChar_;
}

} // namespace regina

#endif

