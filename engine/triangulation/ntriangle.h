
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

/*! \file triangulation/ntriangle.h
 *  \brief Deals with triangles in a triangulation.
 */

#ifndef __NFACE_H
#ifndef __DOXYGEN
#define __NFACE_H
#endif

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
typedef Face<3, 1> NEdge;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Details how a triangle in a 3-manifold triangulation appears within each
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
class REGINA_API FaceEmbedding<3, 2> : public FaceEmbeddingBase<3, 2> {
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
         * @param tet the tetrahedron in which the underlying triangle
         * of the triangulation is contained.
         * @param tri the corresponding triangle number of \a tet.
         * This must be between 0 and 3 inclusive.
         */
        FaceEmbedding(NTetrahedron* tet, int tri);

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
        int getTriangle() const;
};

/**
 * A convenience typedef for FaceEmbedding<3, 2>.
 */
typedef FaceEmbedding<3, 2> NTriangleEmbedding;

/**
 * Represents a triangle in the skeleton of a 3-manifold triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * documentation for Face for a general overview of how this class works.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions",
 * offer significant extra functionality.
 */
template <>
class REGINA_API Face<3, 2> : public FaceBase<3, 2>, public Output<Face<3, 2>> {
    public:
        /**
         * The \e type of a triangle, which indicates how the vertices and
         * edges of the triangle are identified together.  Here the vertices
         * of a triangle are considered unlabelled (so a relabelling
         * will not change the triangle type).
         *
         * @see getType
         */
        enum Type {
            UNKNOWN_TYPE = 0,
                /**< Indicates that the triangle type has not yet been
                     determined. */
            TRIANGLE = 1,
                /**< Specifies a triangle with no identified vertices or
                     edges. */
            SCARF = 2,
                /**< Specifies a triangle with two identified vertices. */
            PARACHUTE = 3,
                /**< Specifies a triangle with three identified vertices. */
            CONE = 4,
                /**< Specifies a triangle with two edges identified to form a
                     cone. */
            MOBIUS = 5,
                /**< Specifies a triangle with two edges identified to form a
                     mobius band. */
            HORN = 6,
                /**< Specifies a triangle with two edges identified to form a
                     cone with all three vertices identified. */
            DUNCEHAT = 7,
                /**< Specifies a triangle with all three edges identified, some
                     via orientable and some via non-orientable gluings. */
            L31 = 8
                /**< Specifies a triangle with all three edges identified using
                     non-orientable gluings.  Note that this forms a spine for
                     the Lens space L(3,1). */
        };

        /**
         * An array that maps triangle numbers within a tetrahedron
         * (i.e., face numbers) to the canonical ordering of the individual
         * tetrahedron vertices that form each triangle.
         *
         * This means that the vertices of triangle \a i in a tetrahedron
         * are, in canonical order, <tt>ordering[i][0..2]</tt>.  As an
         * immediate consequence, we obtain <tt>ordering[i][3] == i</tt>.
         *
         * Regina defines canonical order to be \e increasing order.
         * That is, <tt>ordering[i][0] &lt; ... &lt; ordering[i][2]</tt>.
         *
         * This table does \e not describe the mapping from specific
         * triangles within a triangulation into individual tetrahedra (for
         * that, see NTetrahedron::getTriangleMapping() instead).  This table
         * merely provides a neat and consistent way of listing the
         * vertices of any given tetrahedron face.
         *
         * This lookup table replaces the deprecated routine
         * regina::faceOrdering().
         */
        static const NPerm4 ordering[4];

    private:
        NBoundaryComponent* boundaryComponent_;
            /**< The boundary component that this triangle is a part of,
                 or 0 if this triangle is internal. */
        Type type_;
            /**< Specifies the triangle type, or \a UNKNOWN_TYPE if the
                 type has not yet been determined. */
        int subtype_;
            /**< Specifies the vertex or edge that plays a special role
                 for the triangle type specified by \a type.  This is only
                 relevant for some triangle types. */

    public:
        /**
         * Determines if this triangle lies entirely on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this triangle lies on the boundary.
         */
        bool isBoundary() const;

        /**
         * Determines whether this triangle represents a dual edge in the
         * maximal forest that has been chosen for the dual 1-skeleton of the
         * triangulation.
         *
         * When the skeletal structure of a triangulation is first computed,
         * a maximal forest in the dual 1-skeleton of the triangulation is
         * also constructed.  Each dual edge in this maximal forest
         * represents a triangle of the (primal) triangulation.
         *
         * This maximal forest will remain fixed until the triangulation
         * changes, at which point it will be recomputed (as will all
         * other skeletal objects, such as connected components and so on).
         * There is no guarantee that, when it is recomputed, the
         * maximal forest will use the same dual edges as before.
         *
         * This routine identifies whether this triangle belongs to the
         * dual forest.  In this sense it performs a similar role to
         * Simplex::facetInMaximalForest(), but this routine is typically
         * easier to use.
         *
         * If the skeleton has already been computed, then this routine is
         * very fast (since it just returns a precomputed answer).
         *
         * @return \c true if and only if this triangle represents a
         * dual edge in the maximal forest.
         */
        bool inMaximalForest() const;

        /**
         * Returns a description of the triangle type.
         * This will be one of the eight shapes described by the Type
         * enumeration, indicating how the edges and vertices of the
         * triangle are identified.
         *
         * @return the type of this triangle.  This routine will never
         * return UNKNOWN_TYPE.
         */
        Type getType();

        /**
         * Return the triangle vertex or triangle edge that plays a special role
         * for the triangle type of this triangle.  Note that this routine is
         * only relevant for some triangle types.  The triangle type is
         * returned by getType().
         *
         * @return The vertex or edge that plays a special role (this
         * will be 0, 1 or 2), or -1 if this triangle type has no special
         * vertex or edge.
         */
        int getSubtype();

        /**
         * Determines whether this triangle is wrapped up to form a Mobius band.
         *
         * Note that several different triangle types (as returned by
         * getType()) can produce this result.
         * Note also that a triangle can be both a Mobius band \a and a cone.
         *
         * @return \c true if and only if this triangle is a Mobius band.
         */
        bool isMobiusBand();

        /**
         * Determines whether this triangle is wrapped up to form a cone.
         *
         * Note that several different triangle types (as returned by
         * getType()) can produce this result.
         * Note also that a triangle can be both a Mobius band \a and a cone.
         *
         * @return \c true if and only if this triangle is a cone.
         */
        bool isCone();

        /**
         * Returns the boundary component of the triangulation to which
         * this triangle belongs.
         *
         * @return the boundary component containing this triangle, or 0 if this
         * triangle does not lie entirely within the boundary of the
         * triangulation.
         */
        NBoundaryComponent* getBoundaryComponent() const;

        /**
         * Returns the vertex of the triangulation that corresponds
         * to the given vertex of this triangle.
         *
         * Note that vertex \a i of a triangle is opposite edge \a i of the
         * triangle.
         *
         * @param vertex the vertex of this triangle to examine.  This should
         * be 0, 1 or 2.
         * @return the corresponding vertex of the triangulation.
         */
        NVertex* getVertex(int vertex) const;
        /**
         * Returns the edge of the triangulation that corresponds
         * to the given edge of this triangle.
         *
         * Note that edge \a i of a triangle is opposite vertex \a i of the
         * triangle.
         *
         * @param edge the edge of this triangle to examine.  This should be
         * 0, 1 or 2.
         * @return the corresponding edge of the triangulation.
         */
        NEdge* getEdge(int edge) const;
        /**
         * Examines the given edge of this triangle, and returns a mapping
         * from the "canonical" vertices of the corresponding edge of
         * the triangulation to the vertices of this triangle.
         *
         * This routine behaves much the same as
         * NTetrahedron::getEdgeMapping(), except that it maps the edge
         * vertices into a triangle, not into a tetrahedron.  See
         * NTetrahedron::getEdgeMapping() for a more detailed
         * explanation of precisely what this mapping means.
         *
         * This routine differs from NTetrahedron::getEdgeMapping() in
         * how it handles the images of 2 and 3.  This routine will
         * always map 2 to the remaining vertex of this triangle (which is
         * equal to the argument \a edge), and will always map 3 to itself.
         *
         * @param edge the edge of this triangle to examine.  This should be
         * 0, 1 or 2.
         * @return a mapping from vertices (0,1) of the requested edge to
         * the vertices of this triangle.
         */
        NPerm4 getEdgeMapping(int edge) const;

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
         * @param myComponent the triangulation component to which this
         * triangle belongs.
         */
        Face(NComponent* component);

    friend class Triangulation<3>;
        /**< Allow access to private members. */
};

/**
 * A convenience typedef for Face<3, 2>.
 */
typedef Face<3, 2> NTriangle;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "triangulation/ntetrahedron.h"
namespace regina {

// Inline functions for NTriangleEmbedding

inline FaceEmbedding<3, 2>::FaceEmbedding() :
        FaceEmbeddingBase<3, 2>() {
}

inline FaceEmbedding<3, 2>::FaceEmbedding(NTetrahedron* tet, int tri) :
        FaceEmbeddingBase<3, 2>(tet, tri) {
}

inline FaceEmbedding<3, 2>::FaceEmbedding(
        const NTriangleEmbedding& cloneMe) :
        FaceEmbeddingBase<3, 2>(cloneMe) {
}

inline NTetrahedron* FaceEmbedding<3, 2>::getTetrahedron() const {
    return getSimplex();
}

inline int FaceEmbedding<3, 2>::getTriangle() const {
    return getFace();
}

// Inline functions for NTriangle

inline Face<3, 2>::Face(NComponent* component) :
        FaceBase<3, 2>(component),
        boundaryComponent_(0), type_(UNKNOWN_TYPE) {
}

inline NBoundaryComponent* Face<3, 2>::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline NVertex* Face<3, 2>::getVertex(int vertex) const {
    return front().getTetrahedron()->getVertex(front().getVertices()[vertex]);
}

inline bool Face<3, 2>::isBoundary() const {
    return (boundaryComponent_ != 0);
}

inline bool Face<3, 2>::inMaximalForest() const {
    return front().getTetrahedron()->facetInMaximalForest(
        front().getTriangle());
}

inline int Face<3, 2>::getSubtype() {
    getType();
    return subtype_;
}

inline bool Face<3, 2>::isMobiusBand() {
    getType();
    return (type_ == L31 || type_ == DUNCEHAT || type_ == MOBIUS);
}

inline bool Face<3, 2>::isCone() {
    getType();
    return (type_ == DUNCEHAT || type_ == CONE || type_ == HORN);
}

inline void Face<3, 2>::writeTextShort(std::ostream& out) const {
    out << (isBoundary() ? "Boundary " : "Internal ") << "triangle";
}

} // namespace regina

#endif

