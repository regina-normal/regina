
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

#include "regina-core.h"
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
 * A convenience typedef for FaceEmbedding<3, 1>.
 */
typedef FaceEmbedding<3, 1> NEdgeEmbedding;

/**
 * Represents an edge in the skeleton of a 3-manifold triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * documentation for Face for a general overview of how this class works.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions"
 * offer significant extra functionality.
 */
template <>
class REGINA_API Face<3, 1> : public detail::FaceBase<3, 1>,
        public Output<Face<3, 1>> {
    public:
        /**
         * Deprecated table that maps vertices of a tetrahedron to edge numbers.
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
         * \deprecated This static array is deprecated, and will eventually be
         * removed from Regina.  Instead of accessing <tt>edgeNumber[i][j]</tt>,
         * you should instead call <tt>faceNumber(p)</tt>, where \a p is
         * a permutation that maps 0,1 to \a i,\a j in some order.
         */
        static const int (*const edgeNumber)[4];

        /**
         * Deprecated table that maps edges of a tetrahedron to vertex numbers.
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
         * \deprecated This static array is deprecated, and will eventually be
         * removed from Regina.  Instead of accessing <tt>edgeVertex[i][j]</tt>,
         * you should call <tt>ordering(i)[j]</tt> instead.
         */
        static const int (*const edgeVertex)[2];

    private:
        NBoundaryComponent* boundaryComponent_;
            /**< The boundary component that this edge is a part of,
                 or 0 if this edge is internal. */

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
    friend class detail::TriangulationBase<3>;
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

// Inline functions for NEdge

inline Face<3, 1>::Face(NComponent* component) :
        detail::FaceBase<3, 1>(component),
        boundaryComponent_(0) {
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

inline void Face<3, 1>::writeTextShort(std::ostream& out) const {
    out << (isBoundary() ? "Boundary " : "Internal ")
        << "edge of degree " << degree();
}

} // namespace regina

#endif

