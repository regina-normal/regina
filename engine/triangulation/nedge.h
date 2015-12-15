
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

/*! \file triangulation/nedge.h
 *  \brief Deals with edges in a triangulation.
 */

#ifndef __NEDGE_H
#ifndef __DOXYGEN
#define __NEDGE_H
#endif

#include <deque>
#include "regina-core.h"
#include "output.h"
#include "generic/face.h"
#include "maths/nperm4.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

class NBoundaryComponent;

template <int> class Component;
template <int> class Simplex;
template <int> class Triangulation;
typedef Component<3> NComponent;
typedef Simplex<3> NTetrahedron;
typedef Triangulation<3> NTriangulation;
typedef Face<3, 0> NVertex;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * <tt>edgeNumber[i][j]</tt> is the number of the edge linking vertices
 * <tt>i</tt> and <tt>j</tt> in a tetrahedron.  <tt>i</tt> and <tt>j</tt>
 * must be between 0 and 3 inclusive and may be given in any order.
 * The resulting edge number will be between 0 and 5 inclusive.
 *
 * Note that edge numbers of opposite edges will always add to 5.
 *
 * \deprecated This array has been replaced with the identical array
 * NEdge::edgeNumber.  Users are advised to switch to NEdge::edgeNumber
 * instead, since the old regina::edgeNumber will eventually be removed
 * in some future version of Regina.
 */
REGINA_API extern const int edgeNumber[4][4];

/**
 * <tt>edgeStart[k]</tt> is the vertex of a tetrahedron at which edge
 * <tt>k</tt> of the tetrahedron begins.  <tt>k</tt> must be between 0 and 5
 * inclusive.  The resulting vertex number will be between 0 and 3 inclusive.
 *
 * Note that edge numbers of opposite edges will always add to 5.
 * You are guaranteed that <tt>edgeStart[e]</tt> will always be smaller
 * than <tt>edgeEnd[e]</tt>.
 *
 * \deprecated This array has been superceded by NEdge::edgeVertex
 * (where <tt>edgeStart[i]</tt> is now <tt>NEdge::edgeVertex[i][0]</tt>).
 * Users are advised to switch to NEdge::edgeVertex instead, since the old
 * regina::edgeStart and regina::edgeEnd will eventually be removed in some
 * future version of Regina.
 */
REGINA_API extern const int edgeStart[6];

/**
 * <tt>edgeEnd[k]</tt> is the vertex of a tetrahedron
 * at which edge <tt>k</tt> of the tetrahedron ends.
 * <tt>k</tt> must be between 0 and 5 inclusive.
 * The resulting vertex number will be between 0 and 3 inclusive.
 *
 * Note that edge numbers of opposite edges will always add to 5.
 * You are guaranteed that <tt>edgeStart[e]</tt> will always be smaller
 * than <tt>edgeEnd[e]</tt>.
 *
 * \deprecated This array has been superceded by NEdge::edgeVertex
 * (where <tt>edgeEnd[i]</tt> is now <tt>NEdge::edgeVertex[i][1]</tt>).
 * Users are advised to switch to NEdge::edgeVertex instead, since the old
 * regina::edgeStart and regina::edgeEnd will eventually be removed in some
 * future version of Regina.
 */
REGINA_API extern const int edgeEnd[6];

/**
 * Details how an edge in a 3-manifold triangulation appears within each
 * tetrahedron.
 *
 * This is a specialisation of the generic FaceEmbedding class template;
 * see the documentation for FaceEmbedding (and also Face) for a general
 * overview of how these face-related classes work.
 *
 * This 3-dimensional specialisation of FaceEmbedding offers additional
 * dimension-specific aliases of some member functions.
 */
template <>
class REGINA_API FaceEmbedding<3, 1> : public FaceEmbeddingBase<3, 1> {
    public:
        /**
         * Default constructor.  This object is unusable until it has
         * some data assigned to it using <tt>operator =</tt>.
         *
         * \ifacespython Not present.
         */
        FaceEmbedding();

        /**
         * Creates a new object containing the given data.
         *
         * @param tet the tetrahedron in which the underlying edge
         * of the triangulation is contained.
         * @param edge the corresponding edge number of \a tet.
         * This must be between 0 and 5 inclusive.
         */
        FaceEmbedding(NTetrahedron* tet, int edge);

        /**
         * Creates a new copy of the given object.
         *
         * @param cloneMe the object to copy.
         */
        FaceEmbedding(const FaceEmbedding& cloneMe);

        /**
         * A dimension-specific alias for getSimplex().
         *
         * See getSimplex() for further information.
         */
        REGINA_INLINE_REQUIRED
        NTetrahedron* getTetrahedron() const;

        /**
         * A dimension-specific alias for getFace().
         *
         * See getFace() for further information.
         */
        int getEdge() const;
};

/**
 * A convenience typedef for FaceEmbedding<3, 1>.
 */
typedef FaceEmbedding<3, 1> NEdgeEmbedding;

/**
 * Represents an edge in the skeleton of a 3-manifold triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * documentation for Face for a general overview of how this class works.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions",
 * offer significant extra functionality.
 */
template <>
class REGINA_API Face<3, 1> : public FaceBase<3, 1>, public Output<Face<3, 1>> {
    public:
        /**
         * A table that maps vertices of a tetrahedron to edge numbers.
         *
         * Edges in a tetrahedron are numbered 0,...,5.  This table
         * converts vertices to edge numbers; in particular, the edge
         * joining vertices \a i and \a j of a tetrahedron is edge
         * number <tt>edgeNumber[i][j]</tt>.  Here \a i and \a j must be
         * distinct, must be between 0 and 3 inclusive, and may be given
         * in any order.  The resulting edge number will be between 0 and 5
         * inclusive.
         *
         * Note that edge \a i is always opposite edge \a 5-i in a
         * tetrahedron.
         *
         * For reference, Regina assigns edge numbers in lexicographical
         * order.  That is, edge 0 joins vertices 0 and 1, edge 1 joins
         * vertices 0 and 2, edge 2 joins vertices 0 and 3, and so on.
         *
         * This is identical to the old regina::edgeNumber global array.
         * Users are advised to use this NEdge::edgeNumber array instead,
         * since the global regina::edgeNumber is deprecated and will
         * eventually be removed in some future version of Regina.
         */
        static const int edgeNumber[4][4];

        /**
         * A table that maps edges of a tetrahedron to vertex numbers.
         *
         * Edges in a tetrahedron are numbered 0,...,5.  This table
         * converts edge numbers to vertices; in particular, edge \a i
         * in a tetrahedron joins vertices <tt>edgeVertex[i][0]</tt> and
         * <tt>edgeVertex[i][1]</tt>.  Here \a i must be bewteen 0 and 5
         * inclusive; the resulting vertex numbers will be between 0 and 3
         * inclusive.
         *
         * Note that edge \a i is always opposite edge \a 5-i in a tetrahedron.
         * It is guaranteed that <tt>edgeVertex[i][0]</tt> will always
         * be smaller than <tt>edgeVertex[i][1]</tt>.
         *
         * This is a combination of the old regina::edgeStart and
         * regina::edgeEnd global arrays (where
         * <tt>edgeVertex[i][0] == edgeStart[i]</tt> and
         * <tt>edgeVertex[i][1] == edgeEnd[i]</tt>).  Users are advised
         * to use this NEdge::edgeVertex array instead, since the global
         * regina::edgeStart and regina::edgeEnd arrays are deprecated
         * and will eventually be removed in some future version of Regina.
         */
        static const int edgeVertex[6][2];

    private:
        /**
         * An array that hard-codes the results of ordering().
         *
         * See ordering() for further details.
         */
        static const NPerm4 ordering_[6];

        NBoundaryComponent* boundaryComponent_;
            /**< The boundary component that this edge is a part of,
                 or 0 if this edge is internal. */
        bool valid_;
            /**< Is this edge valid? */

    public:
        /**
         * Returns the boundary component of the triangulation to which
         * this edge belongs.
         *
         * @return the boundary component containing this edge, or 0 if this
         * edge does not lie entirely within the boundary of the triangulation.
         */
        NBoundaryComponent* getBoundaryComponent() const;

        /**
         * Returns the vertex of the triangulation that corresponds
         * to the given vertex of this edge.
         *
         * @param vertex the vertex of this edge to examine.  This should
         * be 0 or 1.
         * @return the corresponding vertex of the triangulation.
         */
        NVertex* getVertex(int vertex) const;

        /**
         * Determines if this edge lies entirely on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this edge lies on the boundary.
         */
        bool isBoundary() const;

        /**
         * Determines if this edge is valid.
         * An edge is valid if and only if it is not glued to itself
         * in reverse.
         *
         * @return \c true if and only if this edge is valid.
         */
        bool isValid() const;

        /**
         * Given an edge number within a tetrahedron, returns the canonical
         * ordering of those tetrahedron vertices that make up the given edge.
         *
         * This means that the vertices of edge \a i in a tetrahedron are,
         * in canonical order, <tt>ordering[i][0,1]</tt>.
         *
         * Regina defines canonical order to be \e increasing order.
         * That is, <tt>ordering[i][0] &lt; ordering[i][1]</tt>.
         *
         * The remaining images <tt>ordering[i][2,3]</tt> are chosen to make
         * each permutation even.
         *
         * This routine does \e not describe the mapping from edges
         * of the triangulation into individual tetrahedron; for that, see
         * the routine NTetrahedron::getEdgeMapping().  Instead, this routine
         * just provides a neat and consistent way of listing the vertices of
         * any given edge of any given tetrahedron.
         *
         * @param edge identifies which edge of a tetrahedron to query.
         * This must be between 0 and 5 inclusive.
         * @return the canonical ordering of the tetrahedron vertices that
         * make up the given tetrahedron edge.
         */
        static NPerm4 ordering(unsigned edge);

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
         * Creates a new edge and marks it as belonging to the
         * given triangulation component.
         *
         * @param myComponent the triangulation component to which this
         * edge belongs.
         */
        Face(NComponent* component);

    friend class Triangulation<3>;
        /**< Allow access to private members. */
};

/**
 * A convenience typedef for Face<3, 1>.
 */
typedef Face<3, 1> NEdge;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "triangulation/ntetrahedron.h"
namespace regina {

// Inline functions for NEdgeEmbedding

inline FaceEmbedding<3, 1>::FaceEmbedding() :
        FaceEmbeddingBase<3, 1>() {
}

inline FaceEmbedding<3, 1>::FaceEmbedding(NTetrahedron* tet, int edge) :
        FaceEmbeddingBase<3, 1>(tet, edge) {
}

inline FaceEmbedding<3, 1>::FaceEmbedding(const NEdgeEmbedding& cloneMe) :
        FaceEmbeddingBase<3, 1>(cloneMe) {
}

inline NTetrahedron* FaceEmbedding<3, 1>::getTetrahedron() const {
    return getSimplex();
}

inline int FaceEmbedding<3, 1>::getEdge() const {
    return getFace();
}

// Inline functions for NEdge

inline Face<3, 1>::Face(NComponent* component) :
        FaceBase<3, 1>(component),
        boundaryComponent_(0), valid_(true) {
}

inline NBoundaryComponent* Face<3, 1>::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline NVertex* Face<3, 1>::getVertex(int vertex) const {
    return front().getTetrahedron()->getVertex(front().getVertices()[vertex]);
}

inline bool Face<3, 1>::isBoundary() const {
    return (boundaryComponent_ != 0);
}

inline bool Face<3, 1>::isValid() const {
    return valid_;
}

inline NPerm4 Face<3, 1>::ordering(unsigned edge) {
    return ordering_[edge];
}

inline void Face<3, 1>::writeTextShort(std::ostream& out) const {
    out << (isBoundary() ? "Boundary " : "Internal ")
        << "edge of degree " << getDegree();
}

} // namespace regina

#endif

