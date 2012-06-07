
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/*! \file dim2/dim2triangle.h
 *  \brief Deals with triangular faces in a 2-manifold triangulation.
 */

#ifndef __DIM2TRIANGLE_H
#ifndef __DOXYGEN
#define __DIM2TRIANGLE_H
#endif

#include "regina-core.h"
#include "shareableobject.h"
#include "maths/nperm3.h"
#include "utilities/nmarkedvector.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim2Edge;
class Dim2Vertex;
class Dim2Component;
class Dim2Triangulation;

/**
 * \weakgroup dim2
 * @{
 */

/**
 * Represents a triangular face in a 2-manifold triangulation.
 *
 * With each triangle is stored various pieces of information
 * regarding the overall skeletal structure and component structure of
 * the triangulation.  This skeletal information will be allocated, calculated
 * and deallocated by the Dim2Triangulation object containing the
 * corresponding triangles.
 *
 * A triangle must always belong to a 2-manifold triangulation.  You can
 * construct new triangles using either Dim2Triangulation::newTriangle()
 * or Dim2Triangulation::newTriangle(const std::string&); these
 * routines will automatically add the new triangles to the triangulation.
 * You can destroy triangles by calling Dim2Trianguation::removeTriangle(),
 * Dim2Trianguation::removeTriangleAt() or
 * Dim2Triangulation::removeAllTriangles(); these routines will
 * automatically destroy the triangles as they are removed.
 */
class REGINA_API Dim2Triangle : public ShareableObject, public NMarkedElement {
    private:
        Dim2Triangle* adj_[3];
            /**< Stores the adjacent triangles glued to each edge of this
                 triangle.  Specifically, <tt>adj_[e]</tt>
                 represents the triangle joined to edge \c e
                 of this triangle, or is 0 if edge \c e lies on the
                 triangulation boundary.  Edges are numbered from 0 to 2
                 inclusive, where edge \c i is opposite vertex \c i. */
        NPerm3 adjPerm_[3];
            /**< Stores the corresponence between vertices of this
                 triangle and adjacent triangles.  If edge \c e is
                 joined to another triangle, <tt>adjPerm_[3]</tt>
                 represents the permutation \c p whereby vertex \c v of
                 this triangle is identified with vertex <tt>p[v]</tt> of
                 the adjacent triangle along edge \c e. */
        std::string desc_;
            /**< A text description of this triangle.
                 Descriptions are not mandatory and need not be unique. */

        Dim2Vertex* vertex_[3];
            /**< Vertices in the triangulation skeleton that are
                 vertices of this triangle. */
        Dim2Edge* edge_[3];
            /**< Edges in the triangulation skeleton that are
                 edges of this triangle. */

        NPerm3 vertexMapping_[3];
            /**< Maps 0 to each vertex of this triangle in turn whilst
                 mapping (1,2) in a suitably "orientation-preserving" way,
                 as described in getVertexMapping(). */
        NPerm3 edgeMapping_[3];
            /**< Maps (0,1) to the vertices of this triangle that form
                 each edge, as described in getEdgeMapping(). */
        int orientation_;
            /**< The orientation of this triangle in the triangulation.
                 This will either be 1 or -1. */
        Dim2Triangulation* tri_;
            /**< The triangulation to which this triangle belongs. */
        Dim2Component* component_;
            /**< The component to which this triangle belongs in the
                 triangulation. */

    public:
        /**
         * Destroys this triangle.
         */
        virtual ~Dim2Triangle();

        /**
         * Returns the text description associated with this
         * triangle.
         *
         * @return the description of this triangle.
         */
        const std::string& getDescription() const;

        /**
         * Sets the text description associated with this triangle.
         * Note that descriptions need not be unique, and may be empty.
         *
         * @param desc the new description to assign to this
         * triangle.
         */
        void setDescription(const std::string& desc);

        /**
         * Returns the adjacent triangle glued to the given edge of this
         * triangle, or 0 if the given edge is on the boundary of the
         * 2-manifold triangulation.
         *
         * @param edge the edge of this triangle to examine.  This
         * should be between 0 and 2 inclusive, where edge \c i is
         * opposite vertex \c i of the triangle.
         * @return the adjacent triangle glued to the given edge, or 0
         * if the given edge lies on the boundary.
         */
        Dim2Triangle* adjacentTriangle(int edge) const;
        /**
         * A dimension-agnostic alias for adjacentTriangle().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         * 
         * Here "simplex" refers to a top-dimensional simplex (which for
         * 2-manifold triangulations means a triangle).
         * 
         * See adjacentTriangle() for further information.
         */
        Dim2Triangle* adjacentSimplex(int edge) const;
        /**
         * Returns a permutation describing the correspondence between
         * vertices of this triangle and vertices of the adjacent
         * triangle glued to the given edge of this triangle.
         *
         * If we call this permutation \c p, then for each vertex \c v of this
         * triangle, <tt>p[v]</tt> will be the vertex of the adjacent
         * triangle that is identified with \c v according to the gluing
         * along the given edge of this triangle.
         *
         * \pre The given edge of this triangle has some triangle
         * (possibly this one) glued to it.
         *
         * @param edge the edge of this triangle whose gluing we
         * will examine.  This should be between 0 and 2 inclusive, where
         * edge \c i is opposite vertex \c i of the triangle.
         * @return a permutation mapping the vertices of this
         * triangle to the vertices of the triangle adjacent along
         * the given edge.
         */
        NPerm3 adjacentGluing(int edge) const;
        /**
         * Examines the triangle glued to the given edge of this
         * triangle, and returns the corresponding edge of that
         * triangle.  That is, the returned edge of the adjacent
         * triangle is glued to the given edge of this triangle.
         *
         * \pre The given edge of this triangle has some triangle
         * (possibly this one) glued to it.
         *
         * @param edge the edge of this triangle whose gluing we
         * will examine.  This should be between 0 and 2 inclusive, where
         * edge \c i is opposite vertex \c i of the triangle.
         * @return the edge of the triangle adjacent along the given
         * edge that is in fact glued to the given edge of this triangle.
         */
        int adjacentEdge(int edge) const;
        /**
         * A dimension-agnostic alias for adjacentEdge().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         * 
         * Here "facet" refers to a facet of a top-dimensional simplex
         * (which for 2-manifold triangulations means an edge of a triangle).
         * 
         * See adjacentEdge() for further information.
         */
        int adjacentFacet(int facet) const;
        /**
         * Determines if this triangle has any edges that are boundary edges.
         *
         * @return \c true if and only if this triangle has any boundary edges.
         */
        bool hasBoundary() const;

        /**
         * Joins the given edge of this triangle to another triangle.
         * The other triangle involved will be automatically updated.
         *
         * \pre This and the given triangle do not belong to
         * different triangulations.
         * \pre The given edge of this triangle is not currently glued
         * to anything.
         * \pre The edge of the other triangle that will be glued to the
         * given edge of this triangle is not currently glued to anything.
         * \pre If the other triangle involved is this triangle, we are
         * not attempting to glue an edge to itself.
         *
         * @param myEdge the edge of this triangle that will be glued to
         * the given other triangle.  This should be between 0 and 2
         * inclusive, where edge \c i is opposite vertex \c i of the triangle.
         * @param you the triangle (possibly this one) that will be
         * glued to the given edge of this triangle.
         * @param gluing a permutation describing the mapping of
         * vertices by which the two triangles will be joined.  Each
         * vertex \c v of this triangle that lies on the given edge will
         * be identified with vertex <tt>gluing[v]</tt> of triangle
         * <tt>you</tt>.  In addition, the edge of <tt>you</tt> that
         * will be glued to the given edge of this triangle will be
         * edge number <tt>gluing[myEdge]</tt>.
         */
        void joinTo(int myEdge, Dim2Triangle* you, NPerm3 gluing);
        /**
         * Unglues the given edge of this triangle from whatever is
         * joined to it.  The other triangle involved (possibly this
         * one) will be automatically updated.
         *
         * \pre The given edge of this triangle has some triangle
         * (possibly this one) glued to it.
         *
         * @param myEdge the edge of this triangle whose gluing we
         * will undo.  This should be between 0 and 2 inclusive, where
         * edge \c i is opposite vertex \c i of the triangle.
         * @return the ex-adjacent triangle that was originally glued to the
         * given edge of this triangle.
         */
        Dim2Triangle* unjoin(int myEdge);
        /**
         * Undoes any edge gluings involving this triangle.
         * Any other triangles involved will be automatically updated.
         */
        void isolate();

        /**
         * Returns the triangulation to which this triangle belongs.
         *
         * @return the triangulation containing this triangle.
         */
        Dim2Triangulation* getTriangulation() const;

        /**
         * Returns the 2-manifold triangulation component to which this
         * triangle belongs.
         *
         * @return the component containing this triangle.
         */
        Dim2Component* getComponent() const;
        /**
         * Returns the vertex in the 2-manifold triangulation skeleton
         * corresponding to the given vertex of this triangle.
         *
         * @param vertex the vertex of this triangle to examine.
         * This should be between 0 and 2 inclusive.
         * @return the vertex of the skeleton corresponding to the
         * requested triangle vertex.
         */
        Dim2Vertex* getVertex(int vertex) const;
        /**
         * Returns the edge in the 2-manifold triangulation skeleton
         * corresponding to the given edge of this triangle.  Edge \c i
         * of a triangle is always opposite vertex \c i of that triangle.
         *
         * @param edge the edge of this triangle to examine.
         * This should be between 0 and 2 inclusive.
         * @return the edge of the skeleton corresponding to the
         * requested triangle edge.
         */
        Dim2Edge* getEdge(int edge) const;
        /**
         * Returns a permutation that maps 0 to the given vertex of this
         * triangle, and that maps (1,2) to the two remaining vertices
         * in the following "orientation-preserving" fashion.
         *
         * The images of 1 and 2 under the permutations that are returned
         * have the following properties.  In each triangle, the images
         * of 1 and 2 under this map form a directed edge of the triangle
         * (running from the image of vertex 1 to the image of vertex 2).
         * For any given vertex of the triangulation, these corresponding
         * directed edges together form an ordered path within the
         * triangulation that circles the common vertex of the triangulation
         * (like a vertex link, except that it is not near to the vertex
         * and so might intersect itself).  Furthermore, if we consider the
         * individual triangles in the order in which they appear in the list
         * Dim2Vertex::getEmbeddings(), these corresponding directed edges
         * appear in order from the start of this path to the finish
         * (for internal vertices this path is actually a cycle, and the
         * starting point is arbitrary).
         *
         * @param vertex the vertex of this triangle to examine.
         * This should be between 0 and 2 inclusive.
         * @return a permutation that maps 0 to the given vertex of this
         * triangle, with the properties outlined above.
         */
        NPerm3 getVertexMapping(int vertex) const;
        /**
         * Examines the given edge of this triangle, and returns a mapping from
         * the "canonical" vertices of the corresponding edge of the
         * triangulation to the matching vertices of this triangle.
         *
         * In detail:  Suppose two edges of two triangles are
         * identified within the overall 2-manifold triangulation.  We call
         * this a single "edge of the triangulation", and arbitrarily
         * label its vertices (0,1).  This routine then maps the vertices
         * (0,1) of this edge of the triangulation to the individual
         * vertices of this triangle that make up the given edge.
         *
         * Because we are passing the argument \a edge, we already know
         * \e which vertices of this triangle are involved.  What this
         * routine tells us is the \a order in which they appear to form the
         * overall edge of the triangulation.
         *
         * As a consequence:  Consider two triangle edges that are
         * identified together as a single edge of the triangulation,
         * and choose some \a i from the set {0,1}.  Then the vertices
         * <tt>getEdgeMapping(...)[i]</tt> of the individual triangles
         * are identified together, since they both become the same
         * vertex of the same edge of the triangulation (assuming of
         * course that we pass the correct edge number in each case to
         * getEdgeMapping()).
         *
         * @param edge the edge of this triangle to examine.
         * This should be between 0 and 2 inclusive.
         * @return a mapping from vertices (0,1) of the requested
         * edge to the corresponding vertices of this triangle.
         */
        NPerm3 getEdgeMapping(int edge) const;
        /**
         * Returns the orientation of this triangle in the 2-manifold
         * triangulation.
         *
         * The orientation of each triangle is always +1 or -1.
         * In an orientable component of a triangulation,
         * adjacent triangles have the same orientations if one could be
         * transposed onto the other without reflection, and they have
         * opposite orientations if a reflection would be required.
         * In a non-orientable component, orientations are still +1 and
         * -1 but no further guarantees can be made.
         *
         * @return +1 or -1 according to the orientation of this triangle.
         */
        int orientation() const;

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new triangle with empty description and no
         * edges joined to anything.
         *
         * @param tri the triangulation to which the new triangle belongs.
         */
        Dim2Triangle(Dim2Triangulation* tri);
        /**
         * Creates a new triangle with the given description and
         * no edges joined to anything.
         *
         * @param desc the description to give the new triangle.
         * @param tri the triangulation to which the new triangle belongs.
         */
        Dim2Triangle(const std::string& desc, Dim2Triangulation* tri);

    friend class Dim2Triangulation;
        /**< Allow access to private members. */
};

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim2/dim2triangulation.h"
namespace regina {

// Inline functions for Dim2Triangle

inline Dim2Triangle::~Dim2Triangle() {
}

inline const std::string& Dim2Triangle::getDescription() const {
    return desc_;
}

inline void Dim2Triangle::setDescription(const std::string& desc) {
    desc_ = desc;
}

inline Dim2Triangle* Dim2Triangle::adjacentTriangle(int edge) const {
    return adj_[edge];
}

inline Dim2Triangle* Dim2Triangle::adjacentSimplex(int edge) const {
    return adj_[edge];
}

inline NPerm3 Dim2Triangle::adjacentGluing(int edge) const {
    return adjPerm_[edge];
}

inline int Dim2Triangle::adjacentEdge(int edge) const {
    return adjPerm_[edge][edge];
}

inline int Dim2Triangle::adjacentFacet(int facet) const {
    return adjPerm_[facet][facet];
}

inline Dim2Triangulation* Dim2Triangle::getTriangulation() const {
    return tri_;
}

inline Dim2Component* Dim2Triangle::getComponent() const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return component_;
}

inline Dim2Vertex* Dim2Triangle::getVertex(int vertex) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return vertex_[vertex];
}

inline Dim2Edge* Dim2Triangle::getEdge(int edge) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return edge_[edge];
}

inline NPerm3 Dim2Triangle::getVertexMapping(int vertex) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return vertexMapping_[vertex];
}

inline NPerm3 Dim2Triangle::getEdgeMapping(int edge) const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return edgeMapping_[edge];
}

inline int Dim2Triangle::orientation() const {
    if (! tri_->calculatedSkeleton_)
        tri_->calculateSkeleton();
    return orientation_;
}

inline void Dim2Triangle::writeTextShort(std::ostream& out) const {
    out << "Triangle";
    if (desc_.length() > 0)
        out << " " << desc_;
}

} // namespace regina

#endif

