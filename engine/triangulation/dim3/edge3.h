
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
 * documentation for Face for a general overview of how this class works.
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
         * Be aware that, after constructing the edge link as the frontier of
         * a regular neighbourhood of the edge, making this \e normal might
         * require further normalisation steps (this will happen if the edge
         * appears more than once within the same triangle).
         *
         * Because of this, the resulting surface might not be recognised as a
         * \e thin edge link (i.e., an edge link where no normalisation is
         * required).  Indeed, in some pathological cases, the resulting
         * surface might even be empty (since there are scenarios in which
         * the edge link normalises away to nothing).
         *
         * @return the corresponding edge linking normal surface.
         */
        NormalSurface linkingSurface() const;

    protected:
        /**
         * Creates a new edge and marks it as belonging to the given
         * triangulation component.
         *
         * @param component the component of the underlying triangulation
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

inline NormalSurface Face<3, 1>::linkingSurface() const {
    return triangulation().linkingSurface(*this);
}

} // namespace regina

#endif

