
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

/*! \file triangulation/ntetrahedron.h
 *  \brief Deals with tetrahedra in a triangulation.
 */

#ifndef __NTETRAHEDRON_H
#ifndef __DOXYGEN
#define __NTETRAHEDRON_H
#endif

#include "regina-core.h"
#include "output.h"
#include "maths/nperm4.h"
#include "utilities/nmarkedvector.h"
#include <boost/noncopyable.hpp>
// NOTE: More #includes follow after the class declarations.

namespace regina {

class NTriangle;
class NEdge;
class NVertex;
class NComponent;
class NTriangulation;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Represents a tetrahedron in a triangulation.
 *
 * With each tetrahedron is stored various pieces of information
 * regarding the overall skeletal structure and component structure of
 * the triangulation.  This skeletal information will be allocated, calculated
 * and deallocated by the NTriangulation object containing the
 * corresponding tetrahedra.
 *
 * A tetrahedron must always belong to a 3-manifold triangulation.  You
 * can construct new tetrahedra using either NTriangulation::newTetrahedron()
 * or NTriangulation::newTetrahedron(const std::string&), which will
 * automatically add the new tetrahedron to the corresponding triangulation.
 * You can destroy tetrahedra by calling NTriangulation::removeTetrahedron(),
 * NTriangulation::removeTetrahedronAt() or
 * NTriangulation::removeAllTetrahedra(); these routines will automatically
 * destroy the tetrahedra as they are removed.
 */
class REGINA_API NTetrahedron :
        public Output<NTetrahedron>,
        public boost::noncopyable,
        public NMarkedElement {
    private:
        NTetrahedron* tetrahedra_[4];
            /**< Stores the tetrahedra glued to each face of this
                 tetrahedron.  Specifically, <tt>tetrahedra[f]</tt>
                 represents the tetrahedron joined to triangular face \c f
                 of this tetrahedron, or is 0
                 if face \c f lies on the triangulation boundary.  Faces are
                 numbered from 0 to 3 inclusive, where face \c i is opposite
                 vertex \c i. */
        NPerm4 tetrahedronPerm_[4];
            /**< Stores the corresponence between vertices of this
                 tetrahedron and adjacent tetrahedra.  If face \c f is
                 joined to another tetrahedron, <tt>tetrahedronPerm[f]</tt>
                 represents the permutation \c p whereby vertex \c v of
                 this tetrahedron is identified with vertex <tt>p[v]</tt> of
                 the adjacent tetrahedron along face \c f. */
        std::string description_;
            /**< A text description of this tetrahedron.
                 Descriptions are not mandatory and need not be unique. */

        NVertex* vertices_[4];
            /**< Vertices in the triangulation skeleton that are
                 vertices of this tetrahedron. */
        NEdge* edges_[6];
            /**< Edges in the triangulation skeleton that are
                 edges of this tetrahedron. */
        NTriangle* triangles_[4];
            /**< Triangles in the triangulation skeleton that are
                 faces of this tetrahedron. */

        int tmpOrientation_[4];
            /**< Temporary array used to represent orientations
                 of triangles and vertex link triangles when calculating
                 orientability of boundary components and vertex links.
                 Each orientation will be +/-1.
                 The array should only be used within these
                 orientability routines, and its contents afterwards are
                 unpredictable. */
        NPerm4 vertexMapping_[4];
            /**< Maps 0 to each vertex of this tetrahedron in turn whilst
                 mapping (1,2,3) in a suitably "orientation-preserving" way,
                 as described in getVertexMapping(). */
        NPerm4 edgeMapping_[6];
            /**< Maps (0,1) to the vertices of this tetrahedron that form
                 each edge whilst mapping (2,3) in a suitably "orientation-
                 preserving" way, as described in getEdgeMapping(). */
        NPerm4 triMapping_[4];
            /**< Maps (0,1,2) to the vertices of this tetrahedron that form
                 each triangular face, as described in getTriangleMapping(). */
        int tetOrientation_;
            /**< The orientation of this tetrahedron in the triangulation.
                 This will either be 1 or -1. */
        NTriangulation* tri_;
            /**< The triangulation to which this tetrahedron belongs. */
        NComponent* component_;
            /**< The component to which this tetrahedron belongs in the
                 triangulation. */

    public:
        /**
         * Returns the text description associated with this
         * tetrahedron.
         *
         * @return the description of this tetrahedron.
         */
        const std::string& getDescription() const;

        /**
         * Sets the text description associated with this tetrahedron.
         * Note that descriptions need not be unique, and may be empty.
         *
         * @param desc the new description to assign to this
         * tetrahedron.
         */
        void setDescription(const std::string& desc);

        /**
         * Returns the index of this tetrahedron in the underlying
         * triangulation.  This is identical to calling
         * <tt>getTriangulation()->tetrahedronIndex(this)</tt>.
         *
         * Note that tetrahedron indexing may change when a tetrahedron is
         * added or removed from the underlying triangulation.
         *
         * @return the index of this tetrahedron.
         */
        unsigned long index() const;

        /**
         * Returns the adjacent tetrahedron glued to the given face of this
         * tetrahedron, or 0 if the given face is on the triangulation
         * boundary.
         *
         * @param face the face of this tetrahedron to examine.  This
         * should be between 0 and 3 inclusive, where face \c i is
         * opposite vertex \c i of the tetrahedron.
         * @return the adjacent tetrahedron glued to the given face, or 0
         * if the given face lies on the boundary.
         */
        NTetrahedron* adjacentTetrahedron(int face) const;
        /**
         * A dimension-agnostic alias for adjacentTetrahedron().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         * 
         * Here "simplex" refers to a top-dimensional simplex (which for
         * 3-manifold triangulations means a tetrahedron).
         * 
         * See adjacentTetrahedron() for further information.
         */
        NTetrahedron* adjacentSimplex(int face) const;
        /**
         * Deprecated in favour of adjacentTetrahedron().  The old routine
         * getAdjacentTetrahedron() has been renamed to adjacentTetrahedron()
         * as part of an effort to make programming and scripting with
         * Regina a little less work on the fingers.
         *
         * \deprecated This routine will eventually be removed in some future
         * version of Regina.  Users are advised to use adjacentTetrahedron()
         * instead, which is an identical routine with a shorter name.
         *
         * @param face the face of this tetrahedron to examine.  This
         * should be between 0 and 3 inclusive, where face \c i is
         * opposite vertex \c i of the tetrahedron.
         * @return the adjacent tetrahedron glued to the given face, or 0
         * if the given face lies on the boundary.
         */
        NTetrahedron* getAdjacentTetrahedron(int face) const;
        /**
         * Returns a permutation describing the correspondence between
         * vertices of this tetrahedron and vertices of the adjacent
         * tetrahedron glued to the given face of this tetrahedron.
         *
         * If we call this permutation \c p, then for each vertex \c v of this
         * tetrahedron, <tt>p[v]</tt> will be the vertex of the adjacent
         * tetrahedron that is identified with \c v according to the gluing
         * along the given face of this tetrahedron.
         *
         * \pre The given face of this tetrahedron has some tetrahedron
         * (possibly this one) glued to it.
         *
         * @param face the face of this tetrahedron whose gluing we
         * will examine.  This should be between 0 and 3 inclusive, where
         * face \c i is opposite vertex \c i of the tetrahedron.
         * @return a permutation mapping the vertices of this
         * tetrahedron to the vertices of the tetrahedron adjacent along
         * the given face.
         */
        NPerm4 adjacentGluing(int face) const;
        /**
         * Deprecated in favour of adjacentGluing().  The old routine
         * getAdjacentTetrahedronGluing() has been renamed to adjacentGluing()
         * as part of an effort to make programming and scripting with
         * Regina a little less work on the fingers.
         *
         * \deprecated This routine will eventually be removed in some future
         * version of Regina.  Users are advised to use adjacentGluing()
         * instead, which is an identical routine with a shorter name.
         *
         * @param face the face of this tetrahedron whose gluing we
         * will examine.  This should be between 0 and 3 inclusive, where
         * face \c i is opposite vertex \c i of the tetrahedron.
         * @return a permutation mapping the vertices of this
         * tetrahedron to the vertices of the tetrahedron adjacent along
         * the given face.
         */
        NPerm4 getAdjacentTetrahedronGluing(int face) const;
        /**
         * Examines the tetrahedron glued to the given face of this
         * tetrahedron, and returns the corresponding face of that
         * tetrahedron.  That is, the returned face of the adjacent
         * tetrahedron is glued to the given face of this tetrahedron.
         *
         * \pre The given face of this tetrahedron has some tetrahedron
         * (possibly this one) glued to it.
         *
         * @param face the face of this tetrahedron whose gluing we
         * will examine.  This
         * should be between 0 and 3 inclusive, where face \c i is
         * opposite vertex \c i of the tetrahedron.
         * @return the face of the tetrahedron adjacent along the given
         * face that is in fact glued to the given face of this
         * tetrahedron.
         */
        int adjacentFace(int face) const;
        /**
         * A dimension-agnostic alias for adjacentFace().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         * 
         * Here "facet" refers to a facet of a top-dimensional simplex
         * (which for 3-manifold triangulations means a face of a tetrahedron).
         * 
         * See adjacentFace() for further information.
         */
        int adjacentFacet(int facet) const;
        /**
         * Deprecated in favour of adjacentFace().  The old routine
         * getAdjacentFace() has been renamed to adjacentFace()
         * as part of an effort to make programming and scripting with
         * Regina a little less work on the fingers.
         *
         * \deprecated This routine will eventually be removed in some future
         * version of Regina.  Users are advised to use adjacentFace()
         * instead, which is an identical routine with a shorter name.
         *
         * @param face the face of this tetrahedron whose gluing we
         * will examine.  This should be between 0 and 3 inclusive, where
         * face \c i is opposite vertex \c i of the tetrahedron.
         * @return the face of the tetrahedron adjacent along the given
         * face that is in fact glued to the given face of this tetrahedron.
         */
        int getAdjacentFace(int face) const;
        /**
         * Determines if this tetrahedron has any faces that are
         * boundary triangles.
         *
         * @return \c true if and only if this tetrahedron has any
         * boundary triangles.
         */
        bool hasBoundary() const;

        /**
         * Joins the given face of this tetrahedron to another
         * tetrahedron.  The other tetrahedron involved will be
         * automatically updated.
         *
         * \pre This and the given tetrahedron belong to the same triangulation.
         * \pre The given face of this tetrahedron is not currently glued to
         * anything.
         * \pre The face of the other tetrahedron that will be glued to the
         * given face of this tetrahedron is not currently glued to anything.
         * \pre If the other tetrahedron involved is this tetrahedron, we are
         * not attempting to glue a face to itself.
         *
         * \warning If one tetrahedron belongs to a triangulation but
         * the other does not, the missing tetrahedron (along with anything
         * that it is joined to, directly or indirectly) will be automatically
         * added to the triangulation.  This is new behaviour as of
         * Regina 4.90; see the NTetrahedron class notes for details.
         *
         * @param myFace the face of this tetrahedron that will be glued
         * to the given other tetrahedron.  This
         * should be between 0 and 3 inclusive, where face \c i is
         * opposite vertex \c i of the tetrahedron.
         * @param you the tetrahedron (possibly this one) that will be
         * glued to the given face of this tetrahedron.
         * @param gluing a permutation describing the mapping of
         * vertices by which the two tetrahedra will be joined.  Each
         * vertex \c v of this tetrahedron that lies on the given face will
         * be identified with vertex <tt>gluing[v]</tt> of tetrahedron
         * <tt>you</tt>.  In addition, the face of <tt>you</tt> that
         * will be glued to the given face of this tetrahedron will be
         * face number <tt>gluing[myFace]</tt>.
         */
        void joinTo(int myFace, NTetrahedron* you, NPerm4 gluing);
        /**
         * Unglues the given face of this tetrahedron from whatever is
         * joined to it.  The other tetrahedron involved (possibly this
         * one) will be automatically updated.
         *
         * \pre The given face of this tetrahedron has some tetrahedron
         * (possibly this one) glued to it.
         *
         * @param myFace the face of this tetrahedron whose gluing we
         * will undo.  This should be between 0 and 3 inclusive, where
         * face \c i is opposite vertex \c i of the tetrahedron.
         * @return the ex-adjacent tetrahedron that was originally glued
         * to the given face of this tetrahedron.
         */
        NTetrahedron* unjoin(int myFace);
        /**
         * Undoes any face gluings involving this tetrahedron.
         * Any other tetrahedra involved will be automatically updated.
         */
        void isolate();

        /**
         * Returns the triangulation to which this tetrahedron belongs.
         *
         * @return the triangulation containing this tetrahedron.
         */
        REGINA_INLINE_REQUIRED
        NTriangulation* getTriangulation() const;

        /**
         * Returns the triangulation component to which this tetrahedron
         * belongs.
         *
         * As of Regina 4.90, if the skeletal information for the
         * triangulation has not been computed then this will be done
         * automatically.  There is no need for users to explicitly
         * recompute the skeleton themselves.
         *
         * @return the component containing this tetrahedron.
         */
        NComponent* getComponent() const;
        /**
         * Returns the vertex in the triangulation skeleton
         * corresponding to the given vertex of this tetrahedron.
         *
         * As of Regina 4.90, if the skeletal information for the
         * triangulation has not been computed then this will be done
         * automatically.  There is no need for users to explicitly
         * recompute the skeleton themselves.
         *
         * @param vertex the vertex of this tetrahedron to examine.
         * This should be between 0 and 3 inclusive.
         * @return the vertex of the skeleton corresponding to the
         * requested tetrahedron vertex.
         */
        REGINA_INLINE_REQUIRED
        NVertex* getVertex(int vertex) const;
        /**
         * Returns the edge in the triangulation skeleton
         * corresponding to the given edge of this tetrahedron.
         *
         * See NEdge::edgeNumber and NEdge::edgeVertex for
         * the conventions of how edges are numbered within a tetrahedron.
         *
         * As of Regina 4.90, if the skeletal information for the
         * triangulation has not been computed then this will be done
         * automatically.  There is no need for users to explicitly
         * recompute the skeleton themselves.
         *
         * @param edge the edge of this tetrahedron to examine.
         * This should be between 0 and 5 inclusive.
         * @return the edge of the skeleton corresponding to the
         * requested tetrahedron edge.
         */
        NEdge* getEdge(int edge) const;
        /**
         * Returns the triangle in the triangulation skeleton
         * corresponding to the given face of this tetrahedron.
         *
         * As of Regina 4.90, if the skeletal information for the
         * triangulation has not been computed then this will be done
         * automatically.  There is no need for users to explicitly
         * recompute the skeleton themselves.
         *
         * @param face the face of this tetrahedron to examine.
         * This should be between 0 and 3 inclusive, where face \c i
         * lies opposite vertex \c i.
         * @return the triangle of the skeleton corresponding to the
         * requested tetrahedron face.
         */
        NTriangle* getTriangle(int face) const;
        /**
         * A deprecated alias for getTriangle().
         *
         * This routine returns the triangle in the triangulation
         * skeleton corresponding to the given face of this tetrahedron.
         * See getTriangle() for further details.
         *
         * \deprecated This routine will be removed in a future version
         * of Regina.  Please use getTriangle() instead.
         *
         * @param face the face of this tetrahedron to examine.
         * This should be between 0 and 3 inclusive, where face \c i
         * lies opposite vertex \c i.
         * @return the triangle of the skeleton corresponding to the
         * requested tetrahedron face.
         */
        NTriangle* getFace(int face) const;
        /**
         * Returns a permutation that maps 0 to the given vertex of this
         * tetrahedron, and that maps (1,2,3) to the three remaining vertices
         * in the following "orientation-preserving" fashion.
         *
         * The images of (1,2,3) under this permutation imply an
         * orientation for the tetrahedron face opposite the given vertex.
         * These orientations will be consistent for all tetrahedra
         * containing the given vertex, if this is possible (i.e., if
         * the vertex link is orientable).
         *
         * Note that there are still arbitrary decisions to be made for
         * the images of (1,2,3), since there will always be three possible
         * mappings that yield the correct orientation.
         *
         * As of Regina 4.90, if the skeletal information for the
         * triangulation has not been computed then this will be done
         * automatically.  There is no need for users to explicitly
         * recompute the skeleton themselves.
         *
         * @param vertex the vertex of this tetrahedron to examine.
         * This should be between 0 and 3 inclusive.
         * @return a permutation that maps 0 to the given vertex of this
         * tetrahedron, with the properties outlined above.
         */
        REGINA_INLINE_REQUIRED
        NPerm4 getVertexMapping(int vertex) const;
        /**
         * Examines the given edge of this tetrahedron, and returns a
         * permutation that maps the "canonical" vertices (0,1) of the
         * corresponding edge of the triangulation to the matching vertices
         * of this tetrahedron.  This permutation also maps (2,3) to the
         * remaining tetrahedron vertices in an "orientation-preserving"
         * way, as described below.
         *
         * In detail:  Suppose several edges of several tetrahedra are
         * identified within the overall triangulation.  We call this a
         * single "edge of the triangulation", and arbitrarily
         * label its vertices (0,1).  This routine then maps the vertices
         * (0,1) of this edge of the triangulation to the individual
         * vertices of this tetrahedron that make up the given edge.
         *
         * Because we are passing the argument \a edge, we already know
         * \e which vertices of this tetrahedron are involved.  What this
         * routine tells us is the \a order in which they appear to form the
         * overall edge of the triangulation.
         *
         * As a consequence:  Consider some collection of tetrahedron edges
         * that are identified together as a single edge of the triangulation,
         * and choose some \a i from the set {0,1}.  Then the vertices
         * <tt>getEdgeMapping(...)[i]</tt> of the individual tetrahedra
         * are all identified together, since they all become the same
         * vertex of the same edge of the triangulation (assuming of
         * course that we pass the correct edge number in each case to
         * getEdgeMapping()).
         *
         * The images of 2 and 3 under the permutations that are returned
         * have the following properties.  In each tetrahedron, the images
         * of 2 and 3 under this map form a directed edge of the tetrahedron
         * (running from the image of vertex 2 to the image of vertex 3).
         * For any given edge of the triangulation, these corresponding
         * directed edges together form an ordered path within the
         * triangulation that circles the common edge of the triangulation
         * (like an edge link, except that it is not near to the edge and so
         * might intersect itself).  Furthermore, if we consider the individual
         * tetrahedra in the order in which they appear in the list
         * NEdge::getEmbeddings(), these corresponding directed edges
         * appear in order from the start of this path to the finish
         * (for internal edges this path is actually a cycle, and the
         * starting point is arbitrary).
         *
         * As of Regina 4.90, if the skeletal information for the
         * triangulation has not been computed then this will be done
         * automatically.  There is no need for users to explicitly
         * recompute the skeleton themselves.
         *
         * @param edge the edge of this tetrahedron to examine.
         * This should be between 0 and 5 inclusive.
         * @return a mapping from vertices (0,1) of the requested
         * triangulation edge to the vertices of this tetrahedron.
         */
        REGINA_INLINE_REQUIRED
        NPerm4 getEdgeMapping(int edge) const;
        /**
         * Examines the given face of this tetrahedron, and returns a
         * mapping from the "canonical" vertices of the corresponding
         * triangle of the triangulation to the matching vertices of this
         * tetrahedron.
         *
         * In detail:  Suppose two faces of two tetrahedra are identified
         * within the overall triangulation.  We call this a single
         * "triangle of the triangulation", and arbitrarily label its
         * vertices (0,1,2).  This routine then maps the vertices
         * (0,1,2) of this triangle of the triangulation to the individual
         * vertices of this tetrahedron that make up the given face.
         *
         * Because we are passing the argument \a face, we already know
         * \e which vertices of this tetrahedron are involved.  What this
         * routine tells us is the \a order in which they appear to form the
         * overall face of the triangulation.
         *
         * As a consequence:  Consider some pair of tetrahedron faces that are
         * identified together as a single triangle of the triangulation,
         * and choose some \a i from the set {0,1,2}.  Then the vertices
         * <tt>getTriangleMapping(...)[i]</tt> of the individual tetrahedra
         * are identified together, since they both become the same
         * vertex of the same triangle of the triangulation (assuming of
         * course that we pass the correct face number in each case to
         * getTriangleMapping()).
         *
         * As of Regina 4.90, if the skeletal information for the
         * triangulation has not been computed then this will be done
         * automatically.  There is no need for users to explicitly
         * recompute the skeleton themselves.
         *
         * @param face the face of this tetrahedron to examine.
         * This should be between 0 and 3 inclusive.
         * @return a mapping from vertices (0,1,2) of the corresponding
         * triangle to the vertices of this tetrahedron.
         */
        REGINA_INLINE_REQUIRED
        NPerm4 getTriangleMapping(int face) const;
        /**
         * A deprecated alias for getTriangleMapping().
         *
         * This routine examines the given face of this tetrahedron, and
         * returns a mapping from the "canonical" vertices of the corresponding
         * triangle of the triangulation to the matching vertices of this
         * tetrahedron.  See getTriangleMapping() for further details.
         *
         * \deprecated This routine will be removed in a future version
         * of Regina.  Please use getTriangleMapping() instead.
         *
         * @param face the face of this tetrahedron to examine.
         * This should be between 0 and 3 inclusive.
         * @return a mapping from vertices (0,1,2) of the corresponding
         * triangle to the vertices of this tetrahedron.
         */
        NPerm4 getFaceMapping(int face) const;
        /**
         * Returns the orientation of this tetrahedron in the
         * triangulation.
         *
         * The orientation of each tetrahedron is always +1 or -1.
         * In an orientable component of a triangulation,
         * adjacent tetrahedra have the same orientations if one could be
         * transposed onto the other without reflection, and they have
         * opposite orientations if a reflection would be required.
         * In a non-orientable component, orientations are still +1 and
         * -1 but no further guarantees can be made.
         *
         * As of Regina 4.90, if the skeletal information for the
         * triangulation has not been computed then this will be done
         * automatically.  There is no need for users to explicitly
         * recompute the skeleton themselves.
         *
         * @return +1 or -1 according to the orientation of this tetrahedron.
         */
        int orientation() const;

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
         * Creates a new tetrahedron with empty description and no
         * faces joined to anything.
         *
         * @param tri the triangulation to which the new tetrahedron belongs.
         */
        NTetrahedron(NTriangulation* tri);
        /**
         * Creates a new tetrahedron with the given description and
         * no faces joined to anything.
         *
         * @param desc the description to give the new tetrahedron.
         * @param tri the triangulation to which the new tetrahedron belongs.
         */
        NTetrahedron(const std::string& desc, NTriangulation* tri);

    friend class NTriangulation;
        /**< Allow access to private members. */
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "triangulation/ntriangulation.h"
namespace regina {

// Inline functions for NTetrahedron

inline const std::string& NTetrahedron::getDescription() const {
    return description_;
}

inline void NTetrahedron::setDescription(const std::string& desc) {
    NPacket::ChangeEventSpan span(tri_);
    description_ = desc;
}

inline unsigned long NTetrahedron::index() const {
    return markedIndex();
}

inline NTetrahedron* NTetrahedron::adjacentTetrahedron(int face) const {
    return tetrahedra_[face];
}

inline NTetrahedron* NTetrahedron::adjacentSimplex(int face) const {
    return tetrahedra_[face];
}

inline NTetrahedron* NTetrahedron::getAdjacentTetrahedron(int face) const {
    // Deprecated.
    return tetrahedra_[face];
}

inline int NTetrahedron::adjacentFace(int face) const {
    return tetrahedronPerm_[face][face];
}

inline int NTetrahedron::adjacentFacet(int facet) const {
    return tetrahedronPerm_[facet][facet];
}

inline int NTetrahedron::getAdjacentFace(int face) const {
    // Deprecated.
    return tetrahedronPerm_[face][face];
}

inline NPerm4 NTetrahedron::adjacentGluing(int face) const {
    return tetrahedronPerm_[face];
}

inline NPerm4 NTetrahedron::getAdjacentTetrahedronGluing(int face) const {
    // Deprecated!  Finally.
    return tetrahedronPerm_[face];
}

inline NTriangulation* NTetrahedron::getTriangulation() const {
    return tri_;
}

inline NComponent* NTetrahedron::getComponent() const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return component_;
}

inline NVertex* NTetrahedron::getVertex(int vertex) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return vertices_[vertex];
}

inline NEdge* NTetrahedron::getEdge(int edge) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return edges_[edge];
}

inline NTriangle* NTetrahedron::getTriangle(int face) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return triangles_[face];
}

inline NTriangle* NTetrahedron::getFace(int face) const {
    return getTriangle(face);
}

inline NPerm4 NTetrahedron::getVertexMapping(int vertex) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return vertexMapping_[vertex];
}

inline NPerm4 NTetrahedron::getEdgeMapping(int edge) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return edgeMapping_[edge];
}

inline NPerm4 NTetrahedron::getTriangleMapping(int face) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return triMapping_[face];
}

inline NPerm4 NTetrahedron::getFaceMapping(int face) const {
    return getTriangleMapping(face);
}

inline int NTetrahedron::orientation() const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return tetOrientation_;
}

inline void NTetrahedron::writeTextShort(std::ostream& out) const {
    out << "Tetrahedron";
    if (description_.length() > 0)
        out << ": " << description_;
}

} // namespace regina

#endif

