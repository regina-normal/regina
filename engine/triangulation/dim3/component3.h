
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

/*! \file triangulation/dim3/component3.h
 *  \brief Internal header for connected components of a 3-manifold
 *  triangulation.
 *
 *  This file is automatically included from triangulation/dim3.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation3.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation3.h.
#include "triangulation/dim3/triangulation3.h"

#ifndef __REGINA_COMPONENT3_H
#ifndef __DOXYGEN
#define __REGINA_COMPONENT3_H
#endif

#include <variant>

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

/**
 * Represents a connected component of a 3-manifold triangulation.
 *
 * This is a specialisation of the generic Component class template; see the
 * generic Component documentation for an overview of how the component
 * classes work.  In Python, you can read this generic documentation by
 * looking at a higher dimension: try `help(Component5)`.
 *
 * This 3-dimensional specialisation contains some extra functionality.
 * In particular, each 3-dimensional component also stores details on
 * lower-dimensional faces (i.e., vertices, edges and triangles).
 *
 * Components do not support value semantics: they cannot be copied, swapped,
 * or manually constructed.  Their location in memory defines them, and
 * they are often passed and compared by pointer.  End users are never
 * responsible for their memory management; this is all taken care of by
 * the Triangulation to which they belong.
 *
 * \pyclassname{Component3}
 *
 * \headerfile triangulation/dim3.h
 *
 * \ingroup dim3
 */
template <>
class Component<3> : public detail::ComponentBase<3> {
    private:
        bool ideal_;
            /**< Is the component ideal? */

    public:
        /**
         * A dimension-specific alias for hasBoundaryFacets().
         *
         * See hasBoundaryFacets() for further information.
         */
        bool hasBoundaryTriangles() const;

        /**
         * A dimension-specific alias for countBoundaryFacets().
         *
         * See countBoundaryFacets() for further information.
         */
        size_t countBoundaryTriangles() const;

        /**
         * Determines if this component is ideal.
         * This is the case if and only if it contains an ideal vertex
         * as described by Vertex<3>::isIdeal().
         *
         * \return \c true if and only if this component is ideal.
         */
        bool isIdeal() const;

        /**
         * Determines if this component is closed.
         * This is the case if and only if it has no boundary.
         * Note that ideal components are not closed.
         *
         * \return \c true if and only if this component is closed.
         */
        bool isClosed() const;

    private:
        /**
         * Default constructor.
         *
         * Marks the component as non-ideal.
         */
        Component();

    friend class Triangulation<3>;
    friend class detail::TriangulationBase<3>;
};

// Inline functions for Component<3>

inline Component<3>::Component() : detail::ComponentBase<3>(), ideal_(false) {
}

inline bool Component<3>::hasBoundaryTriangles() const {
    return hasBoundaryFacets();
}

inline size_t Component<3>::countBoundaryTriangles() const {
    return countBoundaryFacets();
}

inline bool Component<3>::isIdeal() const {
    return ideal_;
}

inline bool Component<3>::isClosed() const {
    return (boundaryComponents().empty());
}

} // namespace regina

#endif

