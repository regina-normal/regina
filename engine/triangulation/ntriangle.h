
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
 * A convenience typedef for FaceEmbedding<3, 2>.
 */
typedef FaceEmbedding<3, 2> NTriangleEmbedding;

/**
 * Represents a triangle in the skeleton of a 3-manifold triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * documentation for Face for a general overview of how this class works.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions"
 * offer significant extra functionality.
 */
template <>
class REGINA_API Face<3, 2> : public detail::FaceBase<3, 2>,
        public Output<Face<3, 2>> {
    public:
        /**
         * The \e type of a triangle, which indicates how the vertices and
         * edges of the triangle are identified together.  Here the vertices
         * of a triangle are considered unlabelled (so a relabelling
         * will not change the triangle type).
         *
         * @see type
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
        Type type();

        /**
         * Deprecated routine that returns a description of the triangle type.
         *
         * \deprecated This routine has been renamed to type().
         * See the type() documentation for further details.
         */
        REGINA_DEPRECATED Type getType();

        /**
         * Return the triangle vertex or triangle edge that plays a special role
         * for the triangle type of this triangle.  Note that this routine is
         * only relevant for some triangle types.  The triangle type is
         * returned by type().
         *
         * @return The vertex or edge that plays a special role (this
         * will be 0, 1 or 2), or -1 if this triangle type has no special
         * vertex or edge.
         */
        int subtype();

        /**
         * Deprecated routine that returns the triangle vertex or triangle edge
         * that plays a special role for the triangle type of this triangle.
         *
         * \deprecated This routine has been renamed to subtype().
         * See the subtype() documentation for further details.
         */
        REGINA_DEPRECATED int getSubtype();

        /**
         * Determines whether this triangle is wrapped up to form a Mobius band.
         *
         * Note that several different triangle types (as returned by
         * type()) can produce this result.
         * Note also that a triangle can be both a Mobius band \a and a cone.
         *
         * @return \c true if and only if this triangle is a Mobius band.
         */
        bool isMobiusBand();

        /**
         * Determines whether this triangle is wrapped up to form a cone.
         *
         * Note that several different triangle types (as returned by
         * type()) can produce this result.
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
        NBoundaryComponent* boundaryComponent() const;
        /**
         * Deprecated routine that returns the boundary component of the
         * triangulation to which this triangle belongs.
         *
         * \deprecated This routine has been renamed as boundaryComponent().
         * See the boundaryComponent() documentation for further details.
         */
        REGINA_DEPRECATED NBoundaryComponent* getBoundaryComponent() const;

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
    friend class detail::TriangulationBase<3>;
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

// Inline functions for NTriangle

inline Face<3, 2>::Face(NComponent* component) :
        FaceBase<3, 2>(component),
        boundaryComponent_(0), type_(UNKNOWN_TYPE) {
}

inline NBoundaryComponent* Face<3, 2>::boundaryComponent() const {
    return boundaryComponent_;
}

inline NBoundaryComponent* Face<3, 2>::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline bool Face<3, 2>::isBoundary() const {
    return (boundaryComponent_ != 0);
}

inline bool Face<3, 2>::inMaximalForest() const {
    return front().tetrahedron()->facetInMaximalForest(
        front().triangle());
}

inline NTriangle::Type Face<3, 2>::getType() {
    return type();
}

inline int Face<3, 2>::subtype() {
    type();
    return subtype_;
}

inline int Face<3, 2>::getSubtype() {
    type();
    return subtype_;
}

inline bool Face<3, 2>::isMobiusBand() {
    type();
    return (type_ == L31 || type_ == DUNCEHAT || type_ == MOBIUS);
}

inline bool Face<3, 2>::isCone() {
    type();
    return (type_ == DUNCEHAT || type_ == CONE || type_ == HORN);
}

inline void Face<3, 2>::writeTextShort(std::ostream& out) const {
    out << (isBoundary() ? "Boundary " : "Internal ") << "triangle";
}

} // namespace regina

#endif

