
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file triangulation/dim3/triangle3.h
 *  \brief Internal header for triangles in a 3-manifold triangulation.
 *
 *  This file is automatically included from triangulation/dim3.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation3.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation3.h.
#include "triangulation/dim3/triangulation3.h"

#ifndef __FACE3_H
#ifndef __DOXYGEN
#define __FACE3_H
#endif

namespace regina {

/**
 * \weakgroup dim3
 * @{
 */

/**
 * Represents a triangle in the skeleton of a 3-manifold triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * documentation for Face for a general overview of how this class works.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions"
 * offer significant extra functionality.
 *
 * \headerfile triangulation/dim3.h
 */
template <>
class REGINA_API Face<3, 2> : public detail::FaceBase<3, 2> {
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
        Type type_;
            /**< Specifies the triangle type, or \a UNKNOWN_TYPE if the
                 type has not yet been determined. */
        int subtype_;
            /**< Specifies the vertex or edge that plays a special role
                 for the triangle type specified by \a type.  This is only
                 relevant for some triangle types. */

    public:
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

    private:
        /**
         * Creates a new triangle and marks it as belonging to the
         * given triangulation component.
         *
         * @param myComponent the triangulation component to which this
         * triangle belongs.
         */
        Face(Component<3>* component);

    friend class Triangulation<3>;
    friend class detail::TriangulationBase<3>;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef NTriangleEmbedding, you should
 * use either the new alias TriangleEmbedding<3>, or the full class name
 * FaceEmbedding<3, 2>.
 */
REGINA_DEPRECATED typedef FaceEmbedding<3, 2> NTriangleEmbedding;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef NTriangle, you should use
 * either the new alias Triangle<3>, or the full class name Face<3, 2>.
 */
REGINA_DEPRECATED typedef Face<3, 2> NTriangle;

/*@}*/

// Inline functions for Triangle<3>

inline Face<3, 2>::Face(Component<3>* component) :
        FaceBase<3, 2>(component), type_(UNKNOWN_TYPE) {
}

inline int Face<3, 2>::subtype() {
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

} // namespace regina

#endif

