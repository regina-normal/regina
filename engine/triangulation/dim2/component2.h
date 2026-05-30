
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file triangulation/dim2/component2.h
 *  \brief Internal header for connected components of a 2-manifold
 *  triangulation.
 *
 *  This file is automatically included from triangulation/dim2.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation2.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation2.h.
#include "triangulation/dim2/triangulation2.h"

#ifndef __REGINA_COMPONENT2_H
#ifndef __DOXYGEN
#define __REGINA_COMPONENT2_H
#endif

#include <variant>

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

/**
 * Represents a connected component of a 2-manifold triangulation.
 *
 * This is a specialisation of the generic Component class template; see the
 * generic Component documentation for an overview of how the component
 * classes work.  In Python, you can read this generic documentation by
 * looking at a higher dimension: try `help(Component5)`.
 *
 * This 2-dimensional specialisation contains some extra functionality.
 * In particular, each 2-dimensional component also stores details on
 * lower-dimensional faces (i.e., vertices and edges).
 *
 * Components do not support value semantics: they cannot be copied, swapped,
 * or manually constructed.  Their location in memory defines them, and
 * they are often passed and compared by pointer.  End users are never
 * responsible for their memory management; this is all taken care of by
 * the Triangulation to which they belong.
 *
 * \pyclassname{Component2}
 *
 * \headerfile triangulation/dim2.h
 *
 * \ingroup dim2
 */
template <>
class Component<2> : public detail::ComponentBase<2> {
    public:
        /**
         * A dimension-specific alias for hasBoundaryFacets().
         *
         * See hasBoundaryFacets() for further information.
         */
        bool hasBoundaryEdges() const;

        /**
         * A dimension-specific alias for countBoundaryFacets().
         *
         * See countBoundaryFacets() for further information.
         */
        size_t countBoundaryEdges() const;

        /**
         * Determines if this component is closed.
         * This is the case if and only if it has no boundary.
         *
         * \return \c true if and only if this component is closed.
         */
        bool isClosed() const;

    private:
        /**
         * Default constructor.
         */
        Component() = default;

    friend class Triangulation<2>;
    friend class detail::TriangulationBase<2>;
};

// Inline functions for Component<2>

inline bool Component<2>::hasBoundaryEdges() const {
    return hasBoundaryFacets();
}

inline size_t Component<2>::countBoundaryEdges() const {
    return countBoundaryFacets();
}

inline bool Component<2>::isClosed() const {
    return (boundaryComponents().empty());
}

} // namespace regina

#endif

