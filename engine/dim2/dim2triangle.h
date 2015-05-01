
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

/*! \file dim2/dim2triangle.h
 *  \brief Deals with triangular faces in a 2-manifold triangulation.
 */

#ifndef __DIM2TRIANGLE_H
#ifndef __DOXYGEN
#define __DIM2TRIANGLE_H
#endif

#include "regina-core.h"
#include "generic/simplex.h"
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
class REGINA_API Dim2Triangle : public SimplexBase<2, true> {
    private:
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
        Dim2Component* component_;
            /**< The component to which this triangle belongs in the
                 triangulation. */

    public:
        /**
         * Destroys this triangle.
         */
        virtual ~Dim2Triangle();

        /**
         * A dimension-specific alias for adjacentSimplex().
         *
         * See adjacentSimplex() for further information.
         */
        Dim2Triangle* adjacentTriangle(int edge) const;
        /**
         * A dimension-specific alias for adjacentFacet().
         * This is to assist with writing dimension-agnostic code that
         * can be reused to work in different dimensions.
         *
         * See adjacentFacet() for further information.
         */
        int adjacentEdge(int edge) const;

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

inline Dim2Triangle* Dim2Triangle::adjacentTriangle(int edge) const {
    return adjacentSimplex(edge);
}

inline int Dim2Triangle::adjacentEdge(int edge) const {
    return adjacentFacet(edge);
}

inline Dim2Component* Dim2Triangle::getComponent() const {
    if (! getTriangulation()->calculatedSkeleton_)
        getTriangulation()->calculateSkeleton();
    return component_;
}

inline Dim2Vertex* Dim2Triangle::getVertex(int vertex) const {
    if (! getTriangulation()->calculatedSkeleton_)
        getTriangulation()->calculateSkeleton();
    return vertex_[vertex];
}

inline Dim2Edge* Dim2Triangle::getEdge(int edge) const {
    if (! getTriangulation()->calculatedSkeleton_)
        getTriangulation()->calculateSkeleton();
    return edge_[edge];
}

inline NPerm3 Dim2Triangle::getVertexMapping(int vertex) const {
    if (! getTriangulation()->calculatedSkeleton_)
        getTriangulation()->calculateSkeleton();
    return vertexMapping_[vertex];
}

inline NPerm3 Dim2Triangle::getEdgeMapping(int edge) const {
    if (! getTriangulation()->calculatedSkeleton_)
        getTriangulation()->calculateSkeleton();
    return edgeMapping_[edge];
}

inline int Dim2Triangle::orientation() const {
    if (! getTriangulation()->calculatedSkeleton_)
        getTriangulation()->calculateSkeleton();
    return orientation_;
}

inline Dim2Triangle::Dim2Triangle(Dim2Triangulation* tri) :
        SimplexBase<2, true>(tri) {
}

inline Dim2Triangle::Dim2Triangle(const std::string& desc,
        Dim2Triangulation* tri) : SimplexBase<2, true>(desc, tri) {
}

} // namespace regina

#endif

