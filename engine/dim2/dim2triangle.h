
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

template <int> class Component;
template <int> class Triangulation;
typedef Triangulation<2> Dim2Triangulation;

/**
 * \weakgroup dim2
 * @{
 */

/**
 * Represents a triangle within a 2-manifold triangulation.
 *
 * This is a specialisation of the generic Simplex class template; see the
 * Simplex documentation for an overview of how this class works.
 *
 * This 2-dimensional specialisation contains some extra functionality.
 * In particular, each triangle stores additional details on how this
 * triangle and its sub-faces integrate into the overall skeletal structure
 * of the triangulation.
 *
 * An implementation note: the Dim2Triangulation class is responsible for
 * creating, maintaining and destroying this extra skeletal information.
 */
template <>
class REGINA_API Simplex<2> : public SimplexBase<2> {
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

    public:
        /**
         * A dimension-specific alias for adjacentSimplex().
         *
         * See adjacentSimplex() for further information.
         */
        Simplex* adjacentTriangle(int edge) const;
        /**
         * A dimension-specific alias for adjacentFacet().
         *
         * See adjacentFacet() for further information.
         */
        int adjacentEdge(int edge) const;

        /**
         * Returns the vertex in the 2-manifold triangulation skeleton
         * corresponding to the given vertex of this triangle.
         *
         * @param vertex the vertex of this triangle to examine.
         * This should be between 0 and 2 inclusive.
         * @return the vertex of the skeleton corresponding to the
         * requested triangle vertex.
         */
        REGINA_INLINE_REQUIRED
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
        REGINA_INLINE_REQUIRED
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
        REGINA_INLINE_REQUIRED
        NPerm3 getEdgeMapping(int edge) const;

    private:
        /**
         * Creates a new triangle with empty description and no
         * edges joined to anything.
         *
         * @param tri the triangulation to which the new triangle belongs.
         */
        Simplex(Dim2Triangulation* tri);
        /**
         * Creates a new triangle with the given description and
         * no edges joined to anything.
         *
         * @param desc the description to give the new triangle.
         * @param tri the triangulation to which the new triangle belongs.
         */
        Simplex(const std::string& desc, Dim2Triangulation* tri);

    friend class Triangulation<2>;
    friend class TriangulationBase<2>;
        /**< Allow access to private members. */
};

/**
 * A convenience typedef for Simplex<2>.
 */
typedef Simplex<2> Dim2Triangle;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim2/dim2triangulation.h"
namespace regina {

// Inline functions for Simplex<2>

inline Simplex<2>* Simplex<2>::adjacentTriangle(int edge) const {
    return adjacentSimplex(edge);
}

inline int Simplex<2>::adjacentEdge(int edge) const {
    return adjacentFacet(edge);
}

inline Dim2Vertex* Simplex<2>::getVertex(int vertex) const {
    getTriangulation()->ensureSkeleton();
    return vertex_[vertex];
}

inline Dim2Edge* Simplex<2>::getEdge(int edge) const {
    getTriangulation()->ensureSkeleton();
    return edge_[edge];
}

inline NPerm3 Simplex<2>::getVertexMapping(int vertex) const {
    getTriangulation()->ensureSkeleton();
    return vertexMapping_[vertex];
}

inline NPerm3 Simplex<2>::getEdgeMapping(int edge) const {
    getTriangulation()->ensureSkeleton();
    return edgeMapping_[edge];
}

inline Simplex<2>::Simplex(Dim2Triangulation* tri) : SimplexBase<2>(tri) {
}

inline Simplex<2>::Simplex(const std::string& desc,
        Dim2Triangulation* tri) : SimplexBase<2>(desc, tri) {
}

} // namespace regina

#endif

