
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
#include "generic/simplex.h"
#include "utilities/nmarkedvector.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim4Tetrahedron;
class Dim4Triangle;
class Dim4Edge;
class Dim4Vertex;

template <int> class Component;
template <int> class Triangulation;
typedef Triangulation<4> Dim4Triangulation;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Represents a pentachoron (a 4-dimensional simplex) within a
 * 4-manifold triangulation.
 *
 * This is a specialisation of the generic Simplex class template; see the
 * generic Simplex documentation for an overview of how this class works.
 *
 * This 4-dimensional specialisation contains some extra functionality.
 * In particular, each pentachoron stores additional details on how this
 * pentachoron and its sub-faces integrate into the overall skeletal
 * structure of the triangulation.
 *
 * An implementation note: the Dim4Triangulation class is responsible for
 * creating, maintaining and destroying this extra skeletal information.
 */
template <>
class REGINA_API Simplex<4> : public SimplexBase<4> {
    private:
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

    public:
        /**
         * A dimension-specific alias for adjacentSimplex().
         *
         * See adjacentSimplex() for further information.
         */
        Simplex* adjacentPentachoron(int facet) const;

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

    private:
        /**
         * Creates a new pentachoron with empty description and no
         * facets joined to anything.
         *
         * @param tri the triangulation to which the new pentachoron belongs.
         */
        Simplex(Dim4Triangulation* tri);
        /**
         * Creates a new pentachoron with the given description and
         * no facets joined to anything.
         *
         * @param desc the description to give the new pentachoron.
         * @param tri the triangulation to which the new pentachoron belongs.
         */
        Simplex(const std::string& desc, Dim4Triangulation* tri);

    friend class Triangulation<4>;
    friend class TriangulationBase<4>;
        /**< Allow access to private members. */
};

/**
 * A convenience typedef for Simplex<4>.
 */
typedef Simplex<4> Dim4Pentachoron;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim4/dim4triangulation.h"
namespace regina {

// Inline functions for Dim4Pentachoron

inline Dim4Pentachoron* Dim4Pentachoron::adjacentPentachoron(int facet) const {
    return adjacentSimplex(facet);
}

inline Dim4Vertex* Dim4Pentachoron::getVertex(int vertex) const {
    getTriangulation()->ensureSkeleton();
    return vertex_[vertex];
}

inline Dim4Edge* Dim4Pentachoron::getEdge(int edge) const {
    getTriangulation()->ensureSkeleton();
    return edge_[edge];
}

inline Dim4Triangle* Dim4Pentachoron::getTriangle(int triang) const {
    getTriangulation()->ensureSkeleton();
    return triangle_[triang];
}

inline Dim4Tetrahedron* Dim4Pentachoron::getTetrahedron(int tet) const {
    getTriangulation()->ensureSkeleton();
    return tet_[tet];
}

inline NPerm5 Dim4Pentachoron::getVertexMapping(int vertex) const {
    getTriangulation()->ensureSkeleton();
    return vertexMapping_[vertex];
}

inline NPerm5 Dim4Pentachoron::getEdgeMapping(int edge) const {
    getTriangulation()->ensureSkeleton();
    return edgeMapping_[edge];
}

inline NPerm5 Dim4Pentachoron::getTriangleMapping(int triang) const {
    getTriangulation()->ensureSkeleton();
    return triangleMapping_[triang];
}

inline NPerm5 Dim4Pentachoron::getTetrahedronMapping(int tet) const {
    getTriangulation()->ensureSkeleton();
    return tetMapping_[tet];
}

inline Simplex<4>::Simplex(Dim4Triangulation* tri) : SimplexBase<4>(tri) {
}

inline Simplex<4>::Simplex(const std::string& desc, Dim4Triangulation* tri) :
        SimplexBase<4>(desc, tri) {
}

} // namespace regina

#endif

