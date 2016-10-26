
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

/*! \file triangulation/dim4/tetrahedron4.h
 *  \brief Internal header for tetrahedra in a 4-manifold triangulation.
 *
 *  This file is automatically included from triangulation/dim4.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation4.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation4.h.
#include "triangulation/dim4/triangulation4.h"

#ifndef __TETRAHEDRON4_H
#ifndef __DOXYGEN
#define __TETRAHEDRON4_H
#endif

namespace regina {

class Dim4BoundaryComponent;

typedef Component<4> Dim4Component;

/**
 * \weakgroup dim4
 * @{
 */

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
         * Determines if this tetrahedron lies entirely on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this tetrahedron lies on the boundary.
         */
        bool isBoundary() const;

        /**
         * Returns the boundary component of the triangulation to which
         * this tetrahedron belongs.
         *
         * @return the boundary component containing this tetrahedron, or 0
         * if this tetrahedron does not lie entirely within the boundary of
         * the triangulation.
         */
        Dim4BoundaryComponent* boundaryComponent() const;

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
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4TetrahedronEmbedding, you should
 * use either the new alias TetrahedronEmbedding<4>, or the full class name
 * FaceEmbedding<4, 3>.
 */
REGINA_DEPRECATED typedef FaceEmbedding<4, 3> Dim4TetrahedronEmbedding;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4Tetrahedron, you should use
 * either the new alias Tetrahedron<4>, or the full class name Face<4, 3>.
 */
REGINA_DEPRECATED typedef Face<4, 3> Dim4Tetrahedron;

/*@}*/

// Inline functions for Tetrahedron<4>

inline Face<4, 3>::Face(Dim4Component* component) :
        FaceBase<4, 3>(component), boundaryComponent_(0) {
}

inline Dim4BoundaryComponent* Face<4, 3>::boundaryComponent() const {
    return boundaryComponent_;
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

