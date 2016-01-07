
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

/*! \file dim4/dim4triangle.h
 *  \brief Deals with triangles in a 4-manifold triangulation.
 */

#ifndef __DIM4TRIANGLE_H
#ifndef __DOXYGEN
#define __DIM4TRIANGLE_H
#endif

#include "regina-core.h"
#include "generic/face.h"
#include "maths/nperm5.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim4BoundaryComponent;

typedef Component<4> Dim4Component;
typedef Simplex<4> Dim4Pentachoron;
typedef Triangulation<4> Dim4Triangulation;
typedef Face<4, 0> Dim4Vertex;
typedef Face<4, 1> Dim4Edge;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * A convenience typedef for FaceEmbedding<4, 2>.
 */
typedef FaceEmbedding<4, 2> Dim4TriangleEmbedding;

/**
 * Represents a triangle in the skeleton of a 4-manifold triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * documentation for Face for a general overview of how this class works.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions"
 * offer significant extra functionality.
 */
template <>
class REGINA_API Face<4, 2> : public detail::FaceBase<4, 2>,
        public Output<Face<4, 2>> {
    public:
        /**
         * A table that maps vertices of a pentachoron to triangle numbers.
         *
         * Triangles in a pentachoron are numbered 0,...,9.  This table
         * converts vertices to triangle numbers; in particular, the triangle
         * spanned by vertices \a i, \a j and \a k of a pentachoron is triangle
         * number <tt>triangleNumber[i][j][k]</tt>.  Here \a i, \a j and \a k
         * must be distinct, must be between 0 and 4 inclusive, and may
         * be given in any order.  The resulting triangle number will be
         * between 0 and 9 inclusive.
         *
         * Note that triangle \i is always opposite edge \i in a pentachoron.
         *
         * This is analagous to the lookup table NEdge::edgeNumber
         * for 3-manifold triangulations (and its deprecated predecessor,
         * the old global array regina::edgeNumber).
         */
        static const int triangleNumber[5][5][5];

        /**
         * A table that maps triangles of a pentachoron to vertex numbers.
         *
         * Triangles in a pentachoron are numbered 0,...,9.  This table converts
         * triangle numbers to vertices; in particular, triangle \a i in a
         * pentachoron is spanned by vertices <tt>triangleVertex[i][0]</tt>,
         * <tt>triangleVertex[i][1]</tt> and <tt>triangleVertex[i][2]</tt>.
         * Here \a i must be between 0 and 9 inclusive; the resulting
         * vertex numbers will be between 0 and 4 inclusive.
         *
         * Note that triangle \i is always opposite edge \i in a pentachoron.
         * It is guaranteed that <tt>triangleVertex[i][0]</tt> will always
         * be smaller than <tt>triangleVertex[i][1]</tt>, which in turn will
         * always be smaller than <tt>triangleVertex[i][2]</tt>.
         *
         * This is analagous to the lookup table NEdge::edgeVertex
         * for 3-manifold triangulations (and its deprecated predecessors,
         * the old global arrays regina::edgeStart and regina::edgeEnd).
         */
        static const int triangleVertex[10][3];

    private:
        Dim4BoundaryComponent* boundaryComponent_;
            /**< The boundary component that this triangle is a part of,
                 or 0 if this triangle is internal. */

    public:
        /**
         * Returns the boundary component of the triangulation to which
         * this triangle belongs.
         *
         * @return the boundary component containing this triangle, or 0 if this
         * triangle does not lie entirely within the boundary of the
         * triangulation.
         */
        Dim4BoundaryComponent* getBoundaryComponent() const;

        /**
         * Returns the vertex of the 4-manifold triangulation corresponding
         * to the given vertex of this triangle.
         *
         * Note that vertex \a i of a triangle is opposite edge \a i of
         * the triangle.
         *
         * @param vertex the vertex of this triangle to examine.  This should
         * be 0, 1 or 2.
         * @return the corresponding vertex of the 4-manifold triangulation.
         */
        Dim4Vertex* getVertex(int vertex) const;

        /**
         * Returns the edge of the 4-manifold triangulation corresponding
         * to the given edge of this triangle.
         *
         * Note that edge \a i of a triangle is opposite vertex \a i of
         * the triangle.
         *
         * @param edge the edge of this triangle to examine.  This should
         * be 0, 1 or 2.
         * @return the corresponding edge of the 4-manifold triangulation.
         */
        Dim4Edge* getEdge(int edge) const;

        /**
         * Examines the given edge of this triangle, and returns a mapping
         * from the "canonical" vertices of the corresponding edge of
         * the triangulation to the vertices of this triangle.
         *
         * This routine behaves much the same way as
         * Dim4Pentachoron::getEdgeMapping(), except that it maps the
         * edge vertices into a triangle, not into a pentachoron.  See
         * Dim4Pentachoron::getEdgeMapping() for a more detailed
         * explanation of precisely what this mapping means.
         *
         * This routine differs from Dim4Pentachoron::getEdgeMapping()
         * in how it handles the images of 2, 3 and 4.  This routine
         * will always map 2 to the remaining vertex of this triangle (which
         * is equal to the argument \a edge), and will always map 3 and 4
         * to themselves.
         *
         * @param edge the edge of this triangle to examine.  This should be
         * 0, 1 or 2.
         * @return a mapping from vertices (0,1) of the requested edge
         * to the vertices of this triangle.
         */
        NPerm5 getEdgeMapping(int edge) const;

        /**
         * Determines if this triangle lies entirely on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this triangle lies on the boundary.
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
         * Creates a new triangle and marks it as belonging to the
         * given triangulation component.
         *
         * @param component the triangulation component to which this
         * triangle belongs.
         */
        Face(Dim4Component* component);

    friend class Triangulation<4>;
    friend class detail::TriangulationBase<4>;
};

/**
 * A convenience typedef for Face<4, 2>.
 */
typedef Face<4, 2> Dim4Triangle;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim4/dim4pentachoron.h"
namespace regina {

// Inline functions for Dim4Triangle

inline Face<4, 2>::Face(Dim4Component* component) :
        detail::FaceBase<4, 2>(component), boundaryComponent_(0) {
}

inline Dim4BoundaryComponent* Face<4, 2>::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline Dim4Vertex* Face<4, 2>::getVertex(int vertex) const {
    return front().pentachoron()->vertex(front().vertices()[vertex]);
}

inline bool Face<4, 2>::isBoundary() const {
    return (boundaryComponent_ != 0);
}

inline void Face<4, 2>::writeTextShort(std::ostream& out) const {
    out << (boundaryComponent_ ? "Boundary " : "Internal ")
        << "triangle of degree " << degree();
}

} // namespace regina

#endif

