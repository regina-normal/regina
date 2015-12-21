
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

/*! \file dim4/dim4tetrahedron.h
 *  \brief Deals with tetrahedra in the 3-skeleton of a 4-manifold
 *  triangulation.
 */

#ifndef __DIM4TETRAHEDRON_H
#ifndef __DOXYGEN
#define __DIM4TETRAHEDRON_H
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
typedef Face<4, 2> Dim4Triangle;
typedef Face<4, 1> Dim4Edge;
typedef Face<4, 0> Dim4Vertex;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * A convenience typedef for FaceEmbedding<4, 3>.
 */
typedef FaceEmbedding<4, 3> Dim4TetrahedronEmbedding;

namespace detail {

/**
 * Helper class that specifies how tetrahedra are numbered within a pentachoron.
 *
 * See the general FaceNumbering<dim, subdim> template class notes for
 * further details.
 */
template <>
class FaceNumbering<4, 3> {
    private:
        static const NPerm5 ordering_[5];
            /**< An array that hard-codes the results of ordering(). */

    public:
        /**
         * Given a tetrahedron number within a pentachoron, returns the
         * corresponding canonical ordering of the pentachoron vertices.
         *
         * If this canonical ordering is \a c, then <tt>c[0,...,3]</tt> will be
         * the vertices of the given tetrahedron in increasing numerical order.
         * That is, <tt>c[0]</tt> &lt; ... &lt; <tt>c[3]</tt>.
         *
         * Note that this is \e not the same permutation as returned by
         * Dim4Pentachoron::getTetrahedronMapping():
         *
         * - ordering() is a static function, which returns the same
         *   permutation for the same tetrahedron number, regardless of which
         *   pentachoron we are looking at.  The images of 0,...,3 will always
         *   appear in increasing order.
         *
         * - getTetrahedronMapping() examines the underlying tetrahedron \a T
         *   of the triangulation, and chooses the images of 0,...,3 to map to
         *   the same respective vertices of \a T for all appearances of
         *   \a T in different pentachora.
         *
         * @param tetrahedron identifies which tetrahedron of a pentachoron to
         * query.  This must be between 0 and 4 inclusive.
         * @return the corresponding canonical ordering of the
         * pentachoron vertices.
         */
        static NPerm5 ordering(unsigned tetrahedron);
        /**
         * Identifies which tetrahedron number in a pentachoron is represented
         * by the first four elements of the given permutation.
         *
         * In other words, this routine identifies which tetrahedron number in
         * a pentachoron spans vertices <tt>vertices[0,...,3]</tt>.
         *
         * @param vertices a permutation whose first four elements
         * represent some vertex numbers in a pentachoron.
         * @return the corresponding tetrahedron number in a pentachoron.
         * This will be between 0 and 4 inclusive.
         */
        static unsigned faceNumber(NPerm5 vertices);
        /**
         * Tests whether the given tetrahedron in a pentachoron contains the
         * given vertex of the pentachoron.
         *
         * @param tetrahedron a tetrahedron number in a pentachoron; this must
         * be between 0 and 4 inclusive.
         * @param vertex a vertex number in a pentachoron; this must be
         * between 0 and 4 inclusive.
         * @return \c true if and only if the given tetrahedron contains the
         * given vertex.
         */
        static bool containsVertex(unsigned tetrahedron, unsigned vertex);
};

} // namespace detail

/**
 * Represents a tetrahedron in the skeleton of a 4-dimensional triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * documentation for Face for a general overview of how this class works.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions",
 * offer significant extra functionality.
 */
template <>
class REGINA_API Face<4, 3> : public detail::FaceBase<4, 3>,
        public Output<Face<4, 3>> {
    private:
        Dim4BoundaryComponent* boundaryComponent_;
            /**< The boundary component that this tetrahedron is a part of,
                 or 0 if this tetrahedron is internal. */

    public:
        /**
         * Returns the boundary component of the triangulation to which
         * this tetrahedron belongs.
         *
         * @return the boundary component containing this tetrahedron, or 0
         * if this tetrahedron does not lie entirely within the boundary of
         * the triangulation.
         */
        Dim4BoundaryComponent* getBoundaryComponent() const;

        /**
         * Returns the vertex of the 4-manifold triangulation corresponding
         * to the given vertex of this tetrahedron.
         *
         * @param vertex the vertex of this tetrahedron to examine.  This
         * should be between 0 and 3 inclusive.
         * @return the corresponding vertex of the 4-manifold triangulation.
         */
        Dim4Vertex* getVertex(int vertex) const;

        /**
         * Returns the edge of the 4-manifold triangulation corresponding
         * to the given edge of this tetrahedron.
         *
         * Edges of a tetrahedron are numbered from 0 to 5, as described
         * by the arrays NEdge::edgeNumber and NEdge::edgeVertex.
         * Edge \a i of a tetrahedron is always opposite edge \a 5-i.
         *
         * @param edge the edge of this tetrahedron to examine.  This should
         * be between 0 and 5 inclusive.
         * @return the corresponding edge of the 4-manifold triangulation.
         */
        Dim4Edge* getEdge(int edge) const;

        /**
         * Returns the triangle of the 4-manifold triangulation corresponding
         * to the given triangular face of this tetrahedron.
         *
         * Note that triangle \a i of a tetrahedron is opposite vertex \a i of
         * the tetrahedron.
         *
         * @param tri the triangle of this tetrahedron to examine.  This
         * should be between 0 and 3 inclusive.
         * @return the corresponding triangle of the 4-manifold triangulation.
         */
        Dim4Triangle* getTriangle(int tri) const;

        /**
         * Examines the given edge of this tetrahedron, and returns a mapping
         * from the "canonical" vertices of the corresponding edge of
         * the triangulation to the vertices of this tetrahedron.
         *
         * This routine behaves much the same way as
         * Dim4Pentachoron::getEdgeMapping(), except that it maps the
         * edge vertices into a tetrahedron, not into a pentachoron.  See
         * Dim4Pentachoron::getEdgeMapping() for a more detailed
         * explanation of precisely what this mapping means.
         *
         * This routine differs from Dim4Pentachoron::getEdgeMapping()
         * in how it handles the images of 2, 3 and 4.  This routine
         * will always map 2 and 3 to the remaining vertices of this
         * tetrahedron (in arbitrary order), and will always map 4 to itself.
         *
         * @param edge the edge of this tetrahedron to examine.  This should
         * be between 0 and 5 inclusive.
         * @return a mapping from vertices (0,1) of the requested edge
         * to the vertices of this tetrahedron.
         */
        NPerm5 getEdgeMapping(int edge) const;

        /**
         * Examines the given triangle of this tetrahedron, and returns a
         * mapping from the "canonical" vertices of the corresponding triangle
         * of the triangulation to the vertices of this tetrahedron.
         *
         * This routine behaves much the same way as
         * Dim4Pentachoron::getTriangleMapping(), except that it maps the
         * triangle vertices into a tetrahedron, not into a pentachoron.  See
         * Dim4Pentachoron::getTriangleMapping() for a more detailed
         * explanation of precisely what this mapping means.
         *
         * This routine differs from Dim4Pentachoron::getTriangleMapping()
         * in how it handles the images of 3 and 4.  This routine
         * will always map 3 to the remaining vertex of this tetrahedron (which
         * is equal to the argument \a tri), and will always map 4 to itself.
         *
         * @param tri the triangle of this tetrahedron to examine.  This should
         * be between 0 and 3 inclusive.
         * @return a mapping from vertices (0,1,2) of the requested triangle
         * to the vertices of this tetrahedron.
         */
        NPerm5 getTriangleMapping(int tri) const;

        /**
         * Determines if this tetrahedron lies entirely on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this tetrahedron lies on the boundary.
         */
        bool isBoundary() const;

        /**
         * Determines whether this tetrahedron represents a dual edge in the
         * maximal forest that has been chosen for the dual 1-skeleton of the
         * triangulation.
         *
         * When the skeletal structure of a triangulation is first computed,
         * a maximal forest in the dual 1-skeleton of the triangulation is
         * also constructed.  Each dual edge in this maximal forest
         * represents a tetrahedron of the (primal) triangulation.
         *
         * This maximal forest will remain fixed until the triangulation
         * changes, at which point it will be recomputed (as will all
         * other skeletal objects, such as connected components and so on).
         * There is no guarantee that, when it is recomputed, the
         * maximal forest will use the same dual edges as before.
         *
         * This routine identifies whether this tetrahedron belongs to the
         * dual forest.  In this sense it performs a similar role to
         * Simplex::facetInMaximalForest(), but this routine is typically
         * easier to use.
         *
         * If the skeleton has already been computed, then this routine is
         * very fast (since it just returns a precomputed answer).
         *
         * @return \c true if and only if this tetrahedron represents a
         * dual edge in the maximal forest.
         */
        bool inMaximalForest() const;

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
         * Creates a new tetrahedron and marks it as belonging to the
         * given triangulation component.
         *
         * @param component the triangulation component to which this
         * tetrahedron belongs.
         */
        Face(Dim4Component* component);

    friend class Triangulation<4>;
    friend class detail::TriangulationBase<4>;
};

/**
 * A convenience typedef for Face<4, 3>.
 */
typedef Face<4, 3> Dim4Tetrahedron;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim4/dim4pentachoron.h"
namespace regina {

// Inline functions for FaceNumbering

namespace detail {

inline NPerm5 FaceNumbering<4, 3>::ordering(unsigned tetrahedron) {
    return ordering_[tetrahedron];
}

inline unsigned FaceNumbering<4, 3>::faceNumber(NPerm5 vertices) {
    return vertices[4];
}

inline bool FaceNumbering<4, 3>::containsVertex(unsigned tetrahedron,
        unsigned vertex) {
    return (tetrahedron != vertex);
}

} // namespace detail

// Inline functions for Dim4Tetrahedron

inline Face<4, 3>::Face(Dim4Component* component) :
        FaceBase<4, 3>(component), boundaryComponent_(0) {
}

inline Dim4BoundaryComponent* Face<4, 3>::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline Dim4Vertex* Face<4, 3>::getVertex(int vertex) const {
    return front().pentachoron()->vertex(front().vertices()[vertex]);
}

inline bool Face<4, 3>::isBoundary() const {
    return (boundaryComponent_ != 0);
}

inline bool Face<4, 3>::inMaximalForest() const {
    return front().pentachoron()->facetInMaximalForest(front().tetrahedron());
}

inline void Face<4, 3>::writeTextShort(std::ostream& out) const {
    out << (boundaryComponent_ ? "Boundary " : "Internal ") << "tetrahedron";
}

} // namespace regina

#endif

