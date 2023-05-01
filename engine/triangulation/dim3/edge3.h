
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

/*! \file triangulation/dim3/edge3.h
 *  \brief Internal header for edges in a 3-manifold triangulation.
 *
 *  This file is automatically included from triangulation/dim3.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation3.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation3.h.
#include "triangulation/dim3/triangulation3.h"

#ifndef __REGINA_EDGE3_H
#ifndef __DOXYGEN
#define __REGINA_EDGE3_H
#endif

namespace regina {

/**
 * Represents an edge in the skeleton of a 3-manifold triangulation.
 *
 * This is a specialisation of the generic Face class template; see the
 * generic documentation for Face for a general overview of how the face
 * classes work.  In Python, you can read this generic documentation by
 * looking at faces in a higher dimension: try `help(Edge5)`.
 *
 * These specialisations for Regina's \ref stddim "standard dimensions"
 * offer significant extra functionality.
 *
 * Edges do not support value semantics: they cannot be copied, swapped,
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
class Face<3, 1> : public detail::FaceBase<3, 1> {
    public:
        /**
         * Returns the link of this edge as a normal surface.
         *
         * Constructing the link of a edge begins with building the frontier
         * of a regular neighbourhood of the edge.  If this is already a
         * normal surface, then then link is called _thin_.  Otherwise
         * the usual normalisation steps are performed until the surface
         * becomes normal; note that these normalisation steps could
         * change the topology of the surface, and in some pathological
         * cases could even reduce it to the empty surface.
         *
         * \return a pair (\a s, \a thin), where \a s is the edge linking
         * normal surface, and \a thin is \c true if and only if this link
         * is thin (i.e., no additional normalisation steps were required).
         */
        std::pair<NormalSurface, bool> linkingSurface() const;

    protected:
        /**
         * Creates a new edge and marks it as belonging to the given
         * triangulation component.
         *
         * \param component the component of the underlying triangulation
         * to which the new edge belongs.
         */
        Face(Component<3>* component);

    friend class Triangulation<3>;
    friend class detail::TriangulationBase<3>;
};

// Inline functions for Edge<3>

inline Face<3, 1>::Face(Component<3>* component) :
        detail::FaceBase<3, 1>(component) {
}

inline std::pair<NormalSurface, bool> Face<3, 1>::linkingSurface() const {
    return triangulation().linkingSurface(*this);
}

} // namespace regina

#endif

