
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

/*! \file dim2/dim2edge.h
 *  \brief Deals with edges in the 1-skeleton of a 2-manifold triangulation.
 */

#ifndef __DIM2EDGE_H
#ifndef __DOXYGEN
#define __DIM2EDGE_H
#endif

#include "regina-core.h"
#include "generic/face.h"
#include "maths/nperm3.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim2BoundaryComponent;

template <int> class Component;
template <int> class Simplex;
template <int> class Triangulation;
typedef Component<2> Dim2Component;
typedef Simplex<2> Dim2Triangle;
typedef Triangulation<2> Dim2Triangulation;
typedef Face<2, 0> Dim2Vertex;

/**
 * \weakgroup dim2
 * @{
 */

/**
 * A convenience typedef for FaceEmbedding<2, 1>.
 */
typedef FaceEmbedding<2, 1> Dim2EdgeEmbedding;

/**
 * Represents an edge in the skeleton of a 2-manifold triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * documentation for Face for a general overview of how this class works.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions"
 * offer significant extra functionality.
 */
template <>
class REGINA_API Face<2, 1> : public detail::FaceBase<2, 1>,
        public Output<Face<2, 1>> {
    private:
        Dim2BoundaryComponent* boundaryComponent_;
            /**< The boundary component that this edge is a part of,
                 or 0 if this edge is internal. */

    public:
        /**
         * Returns the boundary component of the triangulation to which
         * this edge belongs.
         *
         * @return the boundary component containing this edge, or 0
         * if this edge does not lie entirely within the boundary of
         * the triangulation.
         */
        Dim2BoundaryComponent* boundaryComponent() const;
        /**
         * Deprecated routine that returns the boundary component of the
         * triangulation to which this edge belongs.
         *
         * \deprecated This routine has been renamed as boundaryComponent().
         * See the boundaryComponent() documentation for further details.
         */
        REGINA_DEPRECATED Dim2BoundaryComponent* getBoundaryComponent() const;

        /**
         * Determines if this edge lies entirely on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this edge lies on the boundary.
         */
        bool isBoundary() const;

        /**
         * Determines whether this edge represents a dual edge in the
         * maximal forest that has been chosen for the dual 1-skeleton of the
         * triangulation.
         *
         * When the skeletal structure of a triangulation is first computed,
         * a maximal forest in the dual 1-skeleton of the triangulation is
         * also constructed.  Each dual edge in this maximal forest
         * represents a (transverse) edge in the primal skeleton of the
         * triangulation.
         *
         * This maximal forest will remain fixed until the triangulation
         * changes, at which point it will be recomputed (as will all
         * other skeletal objects, such as connected components and so on).
         * There is no guarantee that, when it is recomputed, the
         * maximal forest will use the same dual edges as before.
         *
         * This routine identifies whether this edge corresponds to a
         * member of this dual forest.  In this sense it performs a similar
         * role to Simplex::facetInMaximalForest(), but this routine is
         * typically easier to use.
         *
         * If the skeleton has already been computed, then this routine is
         * very fast (since it just returns a precomputed answer).
         *
         * @return \c true if and only if this edge represents a
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
         * Creates a new edge and marks it as belonging to the
         * given triangulation component.
         *
         * @param component the triangulation component to which this
         * edge belongs.
         */
        Face(Dim2Component* component);

    friend class Triangulation<2>;
    friend class detail::TriangulationBase<2>;
};

/**
 * A convenience typedef for Face<2, 1>.
 */
typedef Face<2, 1> Dim2Edge;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim2/dim2triangle.h"
namespace regina {

// Inline functions for Dim2Edge

inline Face<2, 1>::Face(Dim2Component* component) :
        detail::FaceBase<2, 1>(component), boundaryComponent_(0) {
}

inline Dim2BoundaryComponent* Face<2, 1>::boundaryComponent() const {
    return boundaryComponent_;
}

inline Dim2BoundaryComponent* Face<2, 1>::getBoundaryComponent() const {
    return boundaryComponent_;
}

inline bool Face<2, 1>::isBoundary() const {
    return (boundaryComponent_ != 0);
}

inline bool Face<2, 1>::inMaximalForest() const {
    return front().triangle()->facetInMaximalForest(front().edge());
}

inline void Face<2, 1>::writeTextShort(std::ostream& out) const {
    out << (boundaryComponent_ ? "Boundary " : "Internal ") << "edge";
}

} // namespace regina

#endif

