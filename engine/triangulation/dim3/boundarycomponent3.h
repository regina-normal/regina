
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

/*! \file triangulation/dim3/boundarycomponent3.h
 *  \brief Internal header for components of the boundary of a
 *  3-manifold triangulation.
 *
 *  This file is automatically included from triangulation/dim3.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation3.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation3.h.
#include "triangulation/dim3/triangulation3.h"

#ifndef __BOUNDARYCOMPONENT3_H
#ifndef __DOXYGEN
#define __BOUNDARYCOMPONENT3_H
#endif

namespace regina {

/**
 * \weakgroup dim3
 * @{
 */

/**
 * A component of the boundary of a 3-manifold triangulation.
 *
 * This is a specialisation of the generic BoundaryComponent class template;
 * see the BoundaryComponent documentation for an overview of how this class
 * works.
 *
 * This 3-dimensional specialisation contains some extra functionality.
 * In particular, it allows you to query the Euler characteristic and
 * orientability of a boundary component, without the heavyweight process
 * of triangulating the boundary component first.
 *
 * \headerfile triangulation/dim3.h
 */
template <>
class BoundaryComponent<3>: public detail::BoundaryComponentBase<3> {
    public:
        /**
         * Returns the Euler characteristic of this boundary component.
         * If this boundary component is ideal, the Euler characteristic
         * of the link of the corresponding ideal vertex is returned.
         *
         * @return the Euler characteristic.
         */
        long eulerChar() const;

    private:
        /**
         * Default constructor.
         */
        BoundaryComponent() = default;

    friend class Triangulation<3>;
    friend class detail::TriangulationBase<3>;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef NBoundaryComponent, you should use
 * the real class name BoundaryComponent<3>.
 */
REGINA_DEPRECATED typedef BoundaryComponent<3> NBoundaryComponent;

/*@}*/

// Inline functions for BoundaryComponent<3>

inline long BoundaryComponent<3>::eulerChar() const {
    return (isIdeal() ?
        vertex(0)->linkEulerChar() :
        long(countVertices()) - long(countEdges()) + long(countTriangles()));
}

} // namespace regina

#endif

