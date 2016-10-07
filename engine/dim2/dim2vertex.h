
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

/*! \file dim2/dim2vertex.h
 *  \brief Deals with vertices in a 2-manifold triangulation.
 */

#ifndef __DIM2VERTEX_H
#ifndef __DOXYGEN
#define __DIM2VERTEX_H
#endif

#include "regina-core.h"
#include "generic/face.h"
// NOTE: More #includes follow after the class declarations.

namespace regina {

class Dim2BoundaryComponent;

template <int> class Component;
template <int> class Simplex;
template <int> class Triangulation;
typedef Component<2> Dim2Component;
typedef Simplex<2> Dim2Triangle;
typedef Triangulation<2> Dim2Triangulation;

/**
 * \weakgroup dim2
 * @{
 */

/**
 * A convenience typedef for FaceEmbedding<2, 0>.
 */
typedef FaceEmbedding<2, 0> Dim2VertexEmbedding;

/**
 * Represents a vertex in the skeleton of a 2-manifold triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * documentation for Face for a general overview of how this class works.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions"
 * offer significant extra functionality.
 */
template <>
class REGINA_API Face<2, 0> : public detail::FaceBase<2, 0>,
        public Output<Face<2, 0>> {
    private:
        Dim2BoundaryComponent* boundaryComponent_;
            /**< The boundary component that this vertex is a part of,
                 or 0 if this vertex is internal. */

    public:
        /**
         * Returns the boundary component of the triangulation to which
         * this vertex belongs.
         *
         * @return the boundary component containing this vertex,
         * or 0 if this vertex is not on the boundary of the triangulation.
         */
        Dim2BoundaryComponent* boundaryComponent() const;

        /**
         * Determines if this vertex lies on the boundary of the
         * triangulation.
         *
         * @return \c true if and only if this vertex lies on the boundary.
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
         * Creates a new vertex and marks it as belonging to the
         * given triangulation component.
         *
         * @param component the triangulation component to which this
         * vertex belongs.
         */
        Face(Dim2Component* component);

    friend class Triangulation<2>;
    friend class detail::TriangulationBase<2>;
};

/**
 * A convenience typedef for Face<2, 0>.
 */
typedef Face<2, 0> Dim2Vertex;

/*@}*/

} // namespace regina
// Some more headers that are required for inline functions:
#include "dim2/dim2triangle.h"
namespace regina {

// Inline functions for Dim2Vertex

inline Face<2, 0>::Face(Dim2Component* component) :
        detail::FaceBase<2, 0>(component), boundaryComponent_(0) {
}

inline Dim2BoundaryComponent* Face<2, 0>::boundaryComponent() const {
    return boundaryComponent_;
}

inline bool Face<2, 0>::isBoundary() const {
    return (boundaryComponent_ != 0);
}

inline void Face<2, 0>::writeTextShort(std::ostream& out) const {
    out << (boundaryComponent_ ? "Boundary " : "Internal ")
        << "vertex of degree " << degree();
}

} // namespace regina

#endif

