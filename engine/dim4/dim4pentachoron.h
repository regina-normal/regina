
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/*! \file dim4/dim4pentachoron.h
 *  \brief Deals with 4-dimensional simplices in a 4-manifold triangulation.
 */

#ifndef __DIM4PENTACHORON_H
#ifndef __DOXYGEN
#define __DIM4PENTACHORON_H
#endif

#include "regina-core.h"
#include "output.h"
#include "maths/nperm5.h"
#include "utilities/nmarkedvector.h"
#include <boost/noncopyable.hpp>
// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim4Tetrahedron;
class Dim4Triangle;
class Dim4Edge;
class Dim4Vertex;
class Dim4Component;
class Dim4Triangulation;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Represents a pentachoron (a 4-dimensional simplex) in a
 * 4-manifold triangulation.
 *
 * With each pentachoron is stored various pieces of information
 * regarding the overall skeletal structure and component structure of
 * the triangulation.  This skeletal information will be allocated, calculated
 * and deallocated by the Dim4Triangulation object containing the
 * corresponding pentachora.
 *
 * A pentachoron must always belong to a 4-manifold triangulation.  You can
 * construct new pentachora using either Dim4Triangulation::newPentachoron()
 * or Dim4Triangulation::newPentachoron(const std::string&); these
 * routines will automatically add the new pentachora to the triangulation.
 * You can destroy pentachora by calling Dim4Trianguation::removePentachoron(),
 * Dim4Trianguation::removePentachoronAt() or
 * Dim4Triangulation::removeAllPentachora(); these routines will
 * automatically destroy the pentachora as they are removed.
 */
class REGINA_API Dim4Pentachoron :
        public Output<Dim4Pentachoron>,
        public boost::noncopyable,
        public NMarkedElement {
    private:
        Dim4Pentachoron* adj_[5];
            /**< Stores the adjacent pentachora glued to each facet of this
                 pentachoron.  Specifically, <tt>adj_[f]</tt>
                 represents the pentachoron joined to facet \c f
                 of this pentachoron, or is 0 if facet \c f lies on the
                 triangulation boundary.  Facets are numbered from 0 to 4
                 inclusive, where facet \c i is opposite vertex \c i. */
        NPerm5 adjPerm_[5];
            /**< Stores the corresponence between vertices of this
                 pentachoron and adjacent pentachora.  If facet \c f is
                 joined to another pentachoron, <tt>adjPerm_[f]</tt>
                 represents the permutation \c p whereby vertex \c v of
                 this pentachoron is identified with vertex <tt>p[v]</tt> of
                 the adjacent pentachoron along facet \c f. */
        std::string desc_;
            /**< A text description of this pentachoron.
                 Descriptions are not mandatory and need not be unique. */

        Dim4Vertex* vertex_[5];
            /**< Vertices in the triangulation skeleton that are
                 vertices of this pentachoron. */
        Dim4Edge* edge_[10];
            /**< Edges in the triangulation skeleton that are
                 edges of this pentachoron. */
        Dim4Triangle* triangle_[10];
            /**< Triangles in the triangulation skeleton that are
                 triangles of this pentachoron. */
        Dim4Tetrahedron* tet_[5];
            /**< Tetrahedra in the triangulation skeleton that are
                 facets of this pentachoron. */

        NPerm5 vertexMapping_[5];
            /**< Maps 0 to each vertex of this pentachoron in turn whilst
                 mapping (1,2,3,4) in a suitably "orientation-preserving" way,
                 as described in getVertexMapping(). */
        NPerm5 edgeMapping_[10];
            /**< Maps (0,1) to the vertices of this pentachoron that form
                 each edge whilst mapping (2,3,4) in a suitably "orientation-
                 preserving" way, as described in getEdgeMapping(). */
        NPerm5 triangleMapping_[10];
            /**< Maps (0,1,2) to the vertices of this pentachoron that form
                 each triangle whilst mapping (3,4) in a suitably
                 "orientation-preserving" way, as described in
                 getTriangleMapping(). */
        NPerm5 tetMapping_[5];
            /**< Maps (0,1,2,3) to the vertices of this pentachoron that form
                 each 3-dimensional facet, as described in
                 getTetrahedronMapping(). */
        int orientation_;
            /**< The orientation of this pentachoron in the triangulation.
                 This will either be 1 or -1. */
        Dim4Triangulation* tri_;
            /**< The triangulation to which this pentachoron belongs. */
        Dim4Component* component_;
            /**< The component to which this pentachoron belongs in the
                 triangulation. */

    public:
        /**
         * Returns the text description associated with this
         * pentachoron.
         *
         * @return the description of this pentachoron.
         */
        const std::string& getDescription() const;

        /**
         * Sets the text description associated with this pentachoron.
         * Note that descriptions need not be unique, and may be empty.
         *
         * @param desc the new description to assign to this
         * pentachoron.
         */
        void setDescription(const std::string& desc);

        /**
         * Returns the index of this pentachoron in the underlying
         * triangulation.  This is identical to calling
         * <tt>getTriangulation()->pentachoronIndex(this)</tt>.
         *
         * Note that pentachoron indexing may change when a triangle is
         * added or removed from the underlying triangulation.
         *
         * @return the index of this pentachoron.
         */
        unsigned long index() const;

        /**
         * Returns the adjacent pentachoron glued to the given facet of this
         * pentachoron, or 0 if the given facet is on the boundary of the
         * 4-manifold triangulation.
         *
         * @param facet the facet of this pentachoron to examine.  This
         * should be between 0 and 4 inclusive, where facet \c i is
         * opposite vertex \c i of the pentachoron.
         * @return the adjacent pentachoron glued to the given facet, or 0
         * if the given facet lies on the boundary.
         */
        Dim4Pentachoron* adjacentPentachoron(int facet) const;
        /**
         * A dimension-agnostic alias for adjacentPentachoron().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         * 
         * Here "simplex" refers to a top-dimensional simplex (which for
         * 4-manifold triangulations means a pentachoron).
         * 
         * See adjacentPentachoron() for further information.
         */
        Dim4Pentachoron* adjacentSimplex(int facet) const;
        /**
         * Returns a permutation describing the correspondence between
         * vertices of this pentachoron and vertices of the adjacent
         * pentachoron glued to the given facet of this pentachoron.
         *
         * If we call this permutation \c p, then for each vertex \c v of this
         * pentachoron, <tt>p[v]</tt> will be the vertex of the adjacent
         * pentachoron that is identified with \c v according to the gluing
         * along the given facet of this pentachoron.
         *
         * \pre The given facet of this pentachoron has some pentachoron
         * (possibly this one) glued to it.
         *
         * @param facet the facet of this pentachoron whose gluing we
         * will examine.  This should be between 0 and 4 inclusive, where
         * facet \c i is opposite vertex \c i of the pentachoron.
         * @return a permutation mapping the vertices of this
         * pentachoron to the vertices of the pentachoron adjacent along
         * the given facet.
         */
        NPerm5 adjacentGluing(int facet) const;
        /**
         * Examines the pentachoron glued to the given facet of this
         * pentachoron, and returns the corresponding facet of that
         * pentachoron.  That is, the returned facet of the adjacent
         * pentachoron is glued to the given facet of this pentachoron.
         *
         * \pre The given facet of this pentachoron has some pentachoron
         * (possibly this one) glued to it.
         *
         * @param facet the facet of this pentachoron whose gluing we
         * will examine.  This should be between 0 and 4 inclusive, where
         * facet \c i is opposite vertex \c i of the pentachoron.
         * @return the facet of the pentachoron adjacent along the given
         * facet that is in fact glued to the given facet of this pentachoron.
         */
        int adjacentFacet(int facet) const;
        /**
         * Determines if this pentachoron has any facets that are
         * boundary facets.
         *
         * @return \c true if and only if this pentachoron has any
         * boundary facets.
         */
        bool hasBoundary() const;

        /**
         * Joins the given facet of this pentachoron to another pentachoron.
         * The other pentachoron involved will be automatically updated.
         *
         * \pre This and the given pentachoron do not belong to
         * different triangulations.
         * \pre The given facet of this pentachoron is not currently glued
         * to anything.
         * \pre The facet of the other pentachoron that will be glued to the
         * given facet of this pentachoron is not currently glued to anything.
         * \pre If the other pentachoron involved is this pentachoron, we are
         * not attempting to glue a facet to itself.
         *
         * @param myFacet the facet of this pentachoron that will be glued to
         * the given other pentachoron.  This should be between 0 and 4
         * inclusive, where facet \c i is opposite vertex \c i of the
         * pentachoron.
         * @param you the pentachoron (possibly this one) that will be
         * glued to the given facet of this pentachoron.
         * @param gluing a permutation describing the mapping of
         * vertices by which the two pentachora will be joined.  Each
         * vertex \c v of this pentachoron that lies on the given facet will
         * be identified with vertex <tt>gluing[v]</tt> of pentachoron
         * <tt>you</tt>.  In addition, the facet of <tt>you</tt> that
         * will be glued to the given facet of this pentachoron will be
         * facet number <tt>gluing[myFacet]</tt>.
         */
        void joinTo(int myFacet, Dim4Pentachoron* you, NPerm5 gluing);
        /**
         * Unglues the given facet of this pentachoron from whatever is
         * joined to it.  The other pentachoron involved (possibly this
         * one) will be automatically updated.
         *
         * \pre The given facet of this pentachoron has some pentachoron
         * (possibly this one) glued to it.
         *
         * @param myFacet the facet of this pentachoron whose gluing we
         * will undo.  This should be between 0 and 4 inclusive, where
         * facet \c i is opposite vertex \c i of the pentachoron.
         * @return the ex-adjacent pentachoron that was originally glued to the
         * given facet of this pentachoron.
         */
        Dim4Pentachoron* unjoin(int myFacet);
        /**
         * Undoes any facet gluings involving this pentachoron.
         * Any other pentachora involved will be automatically updated.
         */
        void isolate();

        /**
         * Returns the triangulation to which this pentachoron belongs.
         *
         * @return the triangulation containing this pentachoron.
         */
        Dim4Triangulation* getTriangulation() const;

        /**
         * Returns the 4-manifold triangulation component to which this
         * pentachoron belongs.
         *
         * @return the component containing this pentachoron.
         */
        Dim4Component* getComponent() const;
        /**
         * Returns the vertex in the 4-manifold triangulation skeleton
         * corresponding to the given vertex of this pentachoron.
         *
         * @param vertex the vertex of this pentachoron to examine.
         * This should be between 0 and 4 inclusive.
         * @return the vertex of the skeleton corresponding to the
         * requested pentachoron vertex.
         */
        Dim4Vertex* getVertex(int vertex) const;
        /**
         * Returns the edge in the 4-manifold triangulation skeleton
         * corresponding to the given edge of this pentachoron.
         *
         * See Dim4Edge::edgeNumber and Dim4Edge::edgeVertex for
         * the conventions of how edges are numbered within a pentachoron.
         *
         * @param edge the edge of this pentachoron to examine.
         * This should be between 0 and 9 inclusive.  Note that edge \c i
         * lies opposite triangle \c i.
         * @return the edge of the skeleton corresponding to the
         * requested pentachoron edge.
         */
        Dim4Edge* getEdge(int edge) const;
        /**
         * Returns the triangle in the 4-manifold triangulation skeleton
         * corresponding to the given triangle of this pentachoron.
         *
         * See Dim4Edge::triangleNumber and Dim4Edge::triangleVertex for
         * the conventions of how triangles are numbered within a pentachoron.
         *
         * @param triang the triangle of this pentachoron to examine.
         * This should be between 0 and 9 inclusive.  Note that triangle \c i
         * lies opposite edge \c i.
         * @return the triangle of the skeleton corresponding to the
         * requested pentachoron triangle.
         */
        Dim4Triangle* getTriangle(int triang) const;
        /**
         * Returns the tetrahedron in the 4-manifold triangulation skeleton
         * corresponding to the given facet of this pentachoron.
         *
         * @param tet the tetrahedral facet of this pentachoron to examine.
         * This should be between 0 and 4 inclusive, where facet \c i
         * lies opposite vertex \c i.
         * @return the tetrahedron of the skeleton corresponding to the
         * requested pentachoron facet.
         */
        Dim4Tetrahedron* getTetrahedron(int tet) const;
        /**
         * Returns a permutation that maps 0 to the given vertex of this
         * pentachoron, and that maps (1,2,3,4) to the four remaining vertices
         * in the following "orientation-preserving" fashion.
         *
         * The images of (1,2,3,4) under this permutation imply an
         * orientation for the pentachoron facet opposite the given vertex.
         * These orientations will be consistent for all pentachora
         * containing the given vertex, if this is possible (i.e., if
         * the vertex link is an orientable 3-manifold).
         *
         * Note that there are still arbitrary decisions to be made for
         * the images of (1,2,3,4), since there will always be 12 possible
         * mappings that yield the correct orientation.
         *
         * @param vertex the vertex of this pentachoron to examine.
         * This should be between 0 and 4 inclusive.
         * @return a permutation that maps 0 to the given vertex of this
         * pentachoron, with the properties outlined above.
         */
        NPerm5 getVertexMapping(int vertex) const;
        /**
         * Examines the given edge of this pentachoron, and returns a
         * permutation that maps the "canonical" vertices (0,1) of the
         * corresponding edge of the triangulation to the matching vertices
         * of this pentachoron.  This permutation also maps (2,3,4) to
         * the remaining pentachoron vertices in an "orientation-preserving"
         * way, as described below.
         *
         * In detail:  Suppose several edges of several pentachora are
         * identified within the overall 4-manifold triangulation.  We
         * call this a single "edge of the triangulation", and arbitrarily
         * label its vertices (0,1).  This routine then maps the vertices
         * (0,1) of this edge of the triangulation to the individual
         * vertices of this pentachoron that make up the given edge.
         *
         * Because we are passing the argument \a edge, we already know
         * \e which vertices of this pentachoron are involved.  What this
         * routine tells us is the \a order in which they appear to form the
         * overall edge of the triangulation.
         *
         * As a consequence:  Consider some collection of pentachoron edges
         * that are identified together as a single edge of the triangulation,
         * and choose some \a i from the set {0,1}.  Then the vertices
         * <tt>getEdgeMapping(...)[i]</tt> of the individual pentachora
         * are all identified together, since they all become the same
         * vertex of the same edge of the triangulation (assuming of
         * course that we pass the correct edge number in each case to
         * getEdgeMapping()).
         *
         * The images of (2,3,4) under the returned permutation imply an
         * orientation for the pentachoron triangle opposite the given edge.
         * These orientations will be consistent for all pentachora
         * containing the given edge, if this is possible (i.e., if the edge
         * link is orientable, which is true for any valid triangulation).
         * Note that there are still arbitrary decisions to be made for
         * the images of (2,3,4), since there will always be three possible
         * mappings that yield the correct orientation.
         *
         * @param edge the edge of this pentachoron to examine.
         * This should be between 0 and 9 inclusive.
         * @return a mapping from vertices (0,1) of the requested
         * triangulation edge to the vertices of this pentachoron.
         */
        NPerm5 getEdgeMapping(int edge) const;
        /**
         * Examines the given triangle of this pentachoron, and returns a
         * permutation that maps the "canonical" vertices (0,1,2) of the
         * corresponding triangle of the triangulation to the matching vertices
         * of this pentachoron.  This permutation also maps (3,4) to the
         * remaining pentachoron vertices in an "orientation-preserving"
         * way, as described below.
         *
         * In detail:  Suppose several triangles of several pentachora are
         * identified within the overall 4-manifold triangulation.  We
         * call this a single "triangle of the triangulation", and arbitrarily
         * label its vertices (0,1,2).  This routine then maps the vertices
         * (0,1,2) of this triangle of the triangulation to the individual
         * vertices of this pentachoron that make up the given triangle.
         *
         * Because we are passing the argument \a triang, we already know
         * \e which vertices of this pentachoron are involved.  What this
         * routine tells us is the \a order in which they appear to form the
         * overall triangle of the triangulation.
         *
         * As a consequence:  Consider some collection of pentachoron triangles
         * that are identified together as a single triangle of the
         * triangulation, and choose some \a i from the set {0,1,2}.  Then the
         * vertices <tt>getTriangleMapping(...)[i]</tt> of the individual
         * pentachora are all identified together, since they all become the
         * same vertex of the same triangle of the triangulation (assuming of
         * course that we pass the correct triangle number in each case to
         * getTriangleMapping()).
         *
         * The images of 3 and 4 under the permutations that are returned
         * have the following properties.  In each pentachoron, the images
         * of 3 and 4 under this map form a directed edge of the pentachoron
         * (running from the image of vertex 3 to the image of vertex 4).
         * For any given triangle of the triangulation, these corresponding
         * directed edges together form an ordered path within the triangulation
         * that circles the common triangle of the triangulation (like
         * a triangle link, except that it is not near to the triangle and so
         * might intersect itself).  Furthermore, if we consider the individual
         * pentachora in the order in which they appear in the list
         * Dim4Triangle::getEmbeddings(), these corresponding directed edges
         * appear in order from the start of this path to the finish
         * (for internal triangles this path is actually a cycle, and the
         * starting point is arbitrary).
         *
         * @param triang the triangle of this pentachoron to examine.
         * This should be between 0 and 9 inclusive.
         * @return a mapping from vertices (0,1,2) of the requested triangle
         * to the vertices of this pentachoron.
         */
        NPerm5 getTriangleMapping(int triang) const;
        /**
         * Examines the given tetrahedral facet of this pentachoron, and
         * returns a mapping from the "canonical" vertices of the corresponding
         * tetrahedron of the triangulation to the matching vertices of this
         * pentachoron.
         *
         * In detail:  Suppose two tetrahedral facets of two pentachora are
         * identified within the overall 4-manifold triangulation.  We call
         * this a single "tetrahedron of the triangulation", and arbitrarily
         * label its vertices (0,1,2,3).  This routine then maps the vertices
         * (0,1,2,3) of this tetrahedron of the triangulation to the individual
         * vertices of this pentachoron that make up the given tetrahedron.
         *
         * Because we are passing the argument \a tet, we already know
         * \e which vertices of this pentachoron are involved.  What this
         * routine tells us is the \a order in which they appear to form the
         * overall tetrahedron of the triangulation.
         *
         * As a consequence:  Consider two pentachoron facets that are
         * identified together as a single tetrahedron of the triangulation,
         * and choose some \a i from the set {0,1,2,3}.  Then the vertices
         * <tt>getTetrahedronMapping(...)[i]</tt> of the individual pentachora
         * are all identified together, since they both become the same
         * vertex of the same tetrahedron of the triangulation (assuming of
         * course that we pass the correct facet number in each case to
         * getTetrahedronMapping()).
         *
         * @param tet the tetrahedral facet of this pentachoron to examine.
         * This should be between 0 and 4 inclusive.
         * @return a mapping from vertices (0,1,2,3) of the requested
         * tetrahedron to the vertices of this pentachoron.
         */
        NPerm5 getTetrahedronMapping(int tet) const;
        /**
         * Returns the orientation of this pentachoron in the 4-manifold
         * triangulation.
         *
         * The orientation of each pentachoron is always +1 or -1.
         * In an orientable component of a triangulation,
         * adjacent pentachora have the same orientations if one could be
         * transposed onto the other without reflection, and they have
         * opposite orientations if a reflection would be required.
         * In a non-orientable component, orientations are still +1 and
         * -1 but no further guarantees can be made.
         *
         * @return +1 or -1 according to the orientation of this pentachoron.
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
         * Creates a new pentachoron with empty description and no
         * facets joined to anything.
         *
         * @param tri the triangulation to which the new pentachoron belongs.
         */
        Dim4Pentachoron(Dim4Triangulation* tri);
        /**
         * Creates a new pentachoron with the given description and
         * no facets joined to anything.
         *
         * @param desc the description to give the new pentachoron.
         * @param tri the triangulation to which the new pentachoron belongs.
         */
        Dim4Pentachoron(const std::string& desc, Dim4Triangulation* tri);

    friend class Dim4Triangulation;
        /**< Allow access to private members. */
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim4/dim4triangulation.h"
namespace regina {

// Inline functions for Dim4Pentachoron

inline const std::string& Dim4Pentachoron::getDescription() const {
    return desc_;
}

inline void Dim4Pentachoron::setDescription(const std::string& desc) {
    desc_ = desc;
}

inline unsigned long Dim4Pentachoron::index() const {
    return markedIndex();
}

inline Dim4Pentachoron* Dim4Pentachoron::adjacentPentachoron(int facet) const {
    return adj_[facet];
}

inline Dim4Pentachoron* Dim4Pentachoron::adjacentSimplex(int facet) const {
    return adj_[facet];
}

inline NPerm5 Dim4Pentachoron::adjacentGluing(int facet) const {
    return adjPerm_[facet];
}

inline int Dim4Pentachoron::adjacentFacet(int facet) const {
    return adjPerm_[facet][facet];
}

inline Dim4Triangulation* Dim4Pentachoron::getTriangulation() const {
    return tri_;
}

inline Dim4Component* Dim4Pentachoron::getComponent() const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return component_;
}

inline Dim4Vertex* Dim4Pentachoron::getVertex(int vertex) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return vertex_[vertex];
}

inline Dim4Edge* Dim4Pentachoron::getEdge(int edge) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return edge_[edge];
}

inline Dim4Triangle* Dim4Pentachoron::getTriangle(int triang) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return triangle_[triang];
}

inline Dim4Tetrahedron* Dim4Pentachoron::getTetrahedron(int tet) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return tet_[tet];
}

inline NPerm5 Dim4Pentachoron::getVertexMapping(int vertex) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return vertexMapping_[vertex];
}

inline NPerm5 Dim4Pentachoron::getEdgeMapping(int edge) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return edgeMapping_[edge];
}

inline NPerm5 Dim4Pentachoron::getTriangleMapping(int triang) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return triangleMapping_[triang];
}

inline NPerm5 Dim4Pentachoron::getTetrahedronMapping(int tet) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return tetMapping_[tet];
}

inline int Dim4Pentachoron::orientation() const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return orientation_;
}

inline void Dim4Pentachoron::writeTextShort(std::ostream& out) const {
    out << "Pentachoron";
    if (desc_.length() > 0)
        out << ": " << desc_;
}

} // namespace regina

#endif

