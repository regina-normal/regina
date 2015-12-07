
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
#include "generic/simplex.h"
#include "maths/nperm4.h"
#include "utilities/nmarkedvector.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

class NTriangle;
class NEdge;
class NVertex;

template <int> class Component;
template <int> class Triangulation;
typedef Triangulation<3> NTriangulation;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Represents a tetrahedron within a 3-manifold triangulation.
 *
 * This is a specialisation of the generic Simplex class template; see the
 * generic Simplex documentation for an overview of how this class works.
 *
 * This 3-dimensional specialisation contains some extra functionality.
 * In particular, each tetrahedron stores additional details on how this
 * tetrahedron and its sub-faces integrate into the overall skeletal
 * structure of the triangulation.
 *
 * An implementation note: the NTriangulation class is responsible for
 * creating, maintaining and destroying this extra skeletal information.
 */
template <>
class REGINA_API Simplex<3> : public SimplexBase<3> {
    private:
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

    public:
        /**
         * A dimension-specific alias for adjacentSimplex().
         *
         * See adjacentSimplex() for further information.
         */
        Simplex* adjacentTetrahedron(int face) const;
        /**
         * A dimension-specific alias for adjacentFacet().
         *
         * See adjacentFacet() for further information.
         */
        int adjacentFace(int face) const;

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

        template <int subdim>
        REGINA_INLINE_REQUIRED
        NPerm4 getFaceMapping(int face) const;

    private:
        /**
         * Creates a new tetrahedron with empty description and no
         * faces joined to anything.
         *
         * @param tri the triangulation to which the new tetrahedron belongs.
         */
        Simplex(NTriangulation* tri);
        /**
         * Creates a new tetrahedron with the given description and
         * no edges joined to anything.
         *
         * @param desc the description to give the new tetrahedron.
         * @param tri the triangulation to which the new tetrahedron belongs.
         */
        Simplex(const std::string& desc, NTriangulation* tri);

    friend class Triangulation<3>;
    friend class TriangulationBase<3>;
        /**< Allow access to private members. */
};

/**
 * A convenience typedef for Simplex<3>.
 */
typedef Simplex<3> NTetrahedron;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "triangulation/ntriangulation.h"
namespace regina {

// Inline functions for Simplex<3>

inline Simplex<3>* Simplex<3>::adjacentTetrahedron(int face) const {
    return adjacentSimplex(face);
}

inline int Simplex<3>::adjacentFace(int face) const {
    return adjacentFacet(face);
}

inline NVertex* Simplex<3>::getVertex(int vertex) const {
    getTriangulation()->ensureSkeleton();
    return vertices_[vertex];
}

inline NEdge* Simplex<3>::getEdge(int edge) const {
    getTriangulation()->ensureSkeleton();
    return edges_[edge];
}

inline NTriangle* Simplex<3>::getTriangle(int face) const {
    getTriangulation()->ensureSkeleton();
    return triangles_[face];
}

template <>
inline NPerm4 Simplex<3>::getFaceMapping<0>(int vertex) const {
    getTriangulation()->ensureSkeleton();
    return vertexMapping_[vertex];
}

template <>
inline NPerm4 Simplex<3>::getFaceMapping<1>(int edge) const {
    getTriangulation()->ensureSkeleton();
    return edgeMapping_[edge];
}

template <>
inline NPerm4 Simplex<3>::getFaceMapping<2>(int triangle) const {
    getTriangulation()->ensureSkeleton();
    return triMapping_[triangle];
}

inline NPerm4 Simplex<3>::getVertexMapping(int vertex) const {
    return getFaceMapping<0>(vertex);
}

inline NPerm4 Simplex<3>::getEdgeMapping(int edge) const {
    return getFaceMapping<1>(edge);
}

inline NPerm4 Simplex<3>::getTriangleMapping(int triangle) const {
    return getFaceMapping<2>(triangle);
}

inline Simplex<3>::Simplex(NTriangulation* tri) : SimplexBase<3>(tri) {
}

inline Simplex<3>::Simplex(const std::string& desc, NTriangulation* tri) :
        SimplexBase<3>(desc, tri) {
}

} // namespace regina

#endif

