
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

/*! \file triangulation/dim2/edge2.h
 *  \brief Internal header for edges in a 2-manifold triangulation.
 *
 *  This file is automatically included from triangulation/dim2.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation2.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation2.h.
#include "triangulation/dim2/triangulation2.h"

#ifndef __EDGE2_H
#ifndef __DOXYGEN
#define __EDGE2_H
#endif

namespace regina {

/**
 * \weakgroup dim2
 * @{
 */

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
    public:
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
        Face(Component<2>* component);

    friend class Triangulation<2>;
    friend class detail::TriangulationBase<2>;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim2EdgeEmbedding, you should
 * use either the new alias EdgeEmbedding<2>, or the full class name
 * FaceEmbedding<2, 1>.
 */
REGINA_DEPRECATED typedef FaceEmbedding<2, 1> Dim2EdgeEmbedding;

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim2Edge, you should use
 * either the new alias Edge<2>, or the full class name Face<2, 1>.
 */
REGINA_DEPRECATED typedef Face<2, 1> Dim2Edge;

/*@}*/

// Inline functions for Edge<2>

inline Face<2, 1>::Face(Component<2>* component) :
        detail::FaceBase<2, 1>(component) {
}

inline void Face<2, 1>::writeTextShort(std::ostream& out) const {
    out << (isBoundary() ? "Boundary " : "Internal ") << "edge";
}

} // namespace regina

#endif

