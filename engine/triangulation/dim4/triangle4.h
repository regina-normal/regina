
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

/*! \file triangulation/dim4/triangle4.h
 *  \brief Internal header for triangles in a 4-manifold triangulation.
 *
 *  This file is automatically included from triangulation/dim4.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation4.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation4.h.
#include "triangulation/dim4/triangulation4.h"

#ifndef __REGINA_TRIANGLE4_H
#ifndef __DOXYGEN
#define __REGINA_TRIANGLE4_H
#endif

namespace regina {

/**
 * Represents a triangle in the skeleton of a 4-manifold triangulation.
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
 * \headerfile triangulation/dim4.h
 *
 * \ingroup dim4
 */
template <>
class Face<4, 2> : public detail::FaceBase<4, 2> {
    public:
        /**
         * Returns the link of this triangle as a normal hypersurface.
         *
         * Constructing the link of a triangle begins with building the frontier
         * of a regular neighbourhood of the triangle.  If this is already a
         * normal hypersurface, then then link is called _thin_.  Otherwise
         * some basic normalisation steps are performed until the hypersurface
         * becomes normal; note that these normalisation steps could
         * change the topology of the hypersurface, and in some pathological
         * cases could even reduce it to the empty hypersurface.
         *
         * Although normalisation of arbitrary embedded 3-manifolds is messy,
         * for triangle links the process is thankfully simpler.  Essentially,
         * any changes will be limited to operations analagous to compressions
         * and boundary compressions along discs and 3-balls, as well as
         * removing trivial 4-sphere components.
         *
         * \return a pair (\a s, \a thin), where \a s is the triangle linking
         * normal hypersurface, and \a thin is \c true if and only if this link
         * is thin (i.e., no additional normalisation steps were required).
         */
        std::pair<NormalHypersurface, bool> linkingSurface() const;

    private:
        /**
         * Creates a new triangle and marks it as belonging to the
         * given triangulation component.
         *
         * \param component the triangulation component to which this
         * triangle belongs.
         */
        Face(Component<4>* component);

    friend class Triangulation<4>;
    friend class detail::TriangulationBase<4>;
};

// Inline functions for Triangle<4>

inline Face<4, 2>::Face(Component<4>* component) :
        detail::FaceBase<4, 2>(component) {
}

inline std::pair<NormalHypersurface, bool> Face<4, 2>::linkingSurface() const {
    return triangulation().linkingSurface(*this);
}

} // namespace regina

#endif

