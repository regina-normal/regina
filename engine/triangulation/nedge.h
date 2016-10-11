
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

/*! \file triangulation/nedge.h
 *  \brief Internal header for edges in a 3-manifold triangulation.
 *
 *  This file is automatically included from ntriangulation.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include ntriangulation.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within ntriangulation.h.
#include "triangulation/ntriangulation.h"

#ifndef __NEDGE_H
#ifndef __DOXYGEN
#define __NEDGE_H
#endif

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
        NBoundaryComponent* boundaryComponent() const;

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

// Inline functions for NEdge

inline Face<3, 1>::Face(NComponent* component) :
        detail::FaceBase<3, 1>(component),
        boundaryComponent_(0) {
}

inline NBoundaryComponent* Face<3, 1>::boundaryComponent() const {
    return boundaryComponent_;
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

