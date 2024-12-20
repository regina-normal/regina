
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#ifndef __REGINA_FACE3_H
#ifndef __DOXYGEN
#define __REGINA_FACE3_H
#endif

namespace regina {

/**
 * Represents a triangle in the skeleton of a 3-manifold triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * generic documentation for Face for a general overview of how the face
 * classes work.  In Python, you can read this generic documentation by
 * looking at faces in a higher dimension: try `help(Triangle5)`.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions"
 * offer significant extra functionality.
 *
 * Triangles do not support value semantics: they cannot be copied, swapped,
 * or manually constructed.  Their location in memory defines them, and
 * they are often passed and compared by pointer.  End users are never
 * responsible for their memory management; this is all taken care of by
 * the Triangulation to which they belong.
 *
 * \headerfile triangulation/dim3.h
 *
 * \ingroup dim3
 */
template <>
class Face<3, 2> : public detail::FaceBase<3, 2> {
    public:
        /**
         * Deprecated alias for the combinatorial type of a triangle, which
         * indicates how the vertices and edges of the triangle are identified
         * together.
         *
         * \deprecated This enumeration is now used for triangles within
         * triangulations of all dimensions ≥ 3, and so it has been renamed
         * to the global type TriangleType.
         */
        using Type [[deprecated]] = TriangleType;

        /**
         * A deprecated constant indicating how the vertices and edges of a
         * triangle are identified together.
         *
         * \deprecated This is now an alias for the scoped enumeration constant
         * TriangleType::Unknown.
         */
        [[deprecated]] inline static constexpr TriangleType UNKNOWN_TYPE =
            TriangleType::Unknown;

        /**
         * A deprecated constant indicating how the vertices and edges of a
         * triangle are identified together.
         *
         * \deprecated This is now an alias for the scoped enumeration constant
         * TriangleType::Triangle.
         */
        [[deprecated]] inline static constexpr TriangleType TRIANGLE =
            TriangleType::Triangle;

        /**
         * A deprecated constant indicating how the vertices and edges of a
         * triangle are identified together.
         *
         * \deprecated This is now an alias for the scoped enumeration constant
         * TriangleType::Scarf.
         */
        [[deprecated]] inline static constexpr TriangleType SCARF =
            TriangleType::Scarf;

        /**
         * A deprecated constant indicating how the vertices and edges of a
         * triangle are identified together.
         *
         * \deprecated This is now an alias for the scoped enumeration constant
         * TriangleType::Parachute.
         */
        [[deprecated]] inline static constexpr TriangleType PARACHUTE =
            TriangleType::Parachute;

        /**
         * A deprecated constant indicating how the vertices and edges of a
         * triangle are identified together.
         *
         * \deprecated This is now an alias for the scoped enumeration constant
         * TriangleType::Cone.
         */
        [[deprecated]] inline static constexpr TriangleType CONE =
            TriangleType::Cone;

        /**
         * A deprecated constant indicating how the vertices and edges of a
         * triangle are identified together.
         *
         * \deprecated This is now an alias for the scoped enumeration constant
         * TriangleType::Mobius.
         */
        [[deprecated]] inline static constexpr TriangleType MOBIUS =
            TriangleType::Mobius;

        /**
         * A deprecated constant indicating how the vertices and edges of a
         * triangle are identified together.
         *
         * \deprecated This is now an alias for the scoped enumeration constant
         * TriangleType::Horn.
         */
        [[deprecated]] inline static constexpr TriangleType HORN =
            TriangleType::Horn;

        /**
         * A deprecated constant indicating how the vertices and edges of a
         * triangle are identified together.
         *
         * \deprecated This is now an alias for the scoped enumeration constant
         * TriangleType::DunceHat.
         */
        [[deprecated]] inline static constexpr TriangleType DUNCEHAT =
            TriangleType::DunceHat;

        /**
         * A deprecated constant indicating how the vertices and edges of a
         * triangle are identified together.
         *
         * \deprecated This is now an alias for the scoped enumeration constant
         * TriangleType::L31.
         */
        [[deprecated]] inline static constexpr TriangleType L31 =
            TriangleType::L31;

    private:
        TriangleType type_;
            /**< Specifies the triangle type, or \a TriangleType::Unknown if the
                 type has not yet been determined. */
        int subtype_;
            /**< Specifies the vertex or edge that plays a special role
                 for the triangle type specified by \a type.  This is only
                 relevant for some triangle types. */

    public:
        /**
         * Deprecated function that returns the combinatorial type of this
         * triangle.
         *
         * \deprecated This has been renamed to triangleType(), and is now
         * available for triangulations of all dimensions ≥ 3.
         * See triangleType() for further information.
         *
         * \return the combinatorial type of this triangle, which will never
         * be TriangleType::Unknown.
         */
        [[deprecated]] TriangleType type();

        /**
         * Deprecated function that returns the vertex or edge number in
         * this triangle that plays a special role for this triangle's
         * combinatorial type.
         *
         * \deprecated This has been renamed to triangleSubtype(), and is now
         * available for triangulations of all dimensions ≥ 3.
         * See triangleSubtype() for further information.
         *
         * \return The vertex or edge number (0, 1 or 2) that plays a special
         * role, or -1 if this triangle's combinatorial type has no special
         * vertex or edge.
         */
        [[deprecated]] int subtype();

        /**
         * Deprecated function that determines whether this triangle is
         * wrapped up to form a Möbius band, possibly with or without
         * additional identifications between its vertices and/or edges.
         *
         * \deprecated This has been renamed to formsMobiusBand(), and is now
         * available for triangulations of all dimensions ≥ 3.
         * See formsMobiusBand() for further information.
         *
         * \return \c true if and only if this triangle forms a Möbius band.
         */
        [[deprecated]] bool isMobiusBand();

        /**
         * Deprecated function that determines whether this triangle is
         * wrapped up to form a cone, possibly with or without additional
         * identifications between its vertices and/or edges.
         *
         * \deprecated This has been renamed to formsCone(), and is now
         * available for triangulations of all dimensions ≥ 3.
         * See formsCone() for further information.
         *
         * \return \c true if and only if this triangle forms a cone.
         */
        [[deprecated]] bool isCone();

        /**
         * Returns the link of this triangle as a normal surface.
         *
         * Constructing the link of a triangle begins with building the frontier
         * of a regular neighbourhood of the triangle.  If this is already a
         * normal surface, then then link is called _thin_.  Otherwise
         * the usual normalisation steps are performed until the surface
         * becomes normal; note that these normalisation steps could
         * change the topology of the surface, and in some pathological
         * cases could even reduce it to the empty surface.
         *
         * \return a pair (\a s, \a thin), where \a s is the triangle linking
         * normal surface, and \a thin is \c true if and only if this link
         * is thin (i.e., no additional normalisation steps were required).
         *
         * \return the corresponding triangle linking normal surface.
         */
        std::pair<NormalSurface, bool> linkingSurface() const;

    private:
        /**
         * Creates a new triangle and marks it as belonging to the
         * given triangulation component.
         *
         * \param component the triangulation component to which this
         * triangle belongs.
         */
        Face(Component<3>* component);

    friend class Triangulation<3>;
    friend class detail::TriangulationBase<3>;
};

// Inline functions for Triangle<3>

inline Face<3, 2>::Face(Component<3>* component) :
        FaceBase<3, 2>(component), type_(TriangleType::Unknown) {
}

inline TriangleType Face<3, 2>::type() {
    return triangleType();
}

inline int Face<3, 2>::subtype() {
    return triangleSubtype();
}

inline bool Face<3, 2>::isMobiusBand() {
    return formsMobiusBand();
}

inline bool Face<3, 2>::isCone() {
    return formsCone();
}

inline std::pair<NormalSurface, bool> Face<3, 2>::linkingSurface() const {
    return triangulation().linkingSurface(*this);
}

} // namespace regina

#endif

