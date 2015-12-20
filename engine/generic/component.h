
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

#ifndef __COMPONENT_H
#ifndef __DOXYGEN
#define __COMPONENT_H
#endif

/*! \file generic/component.h
 *  \brief Deals with connected components of triangulations.
 */

#include "generic/detail/component.h"

namespace regina {

/**
 * \weakgroup generic
 * @{
 */

/**
 * A connected component of a <i>dim</i>-manifold triangulation.
 *
 * Components are highly temporary: whenever a triangulation changes, all
 * of its component objects will be deleted and new ones will be created
 * in their place.
 *
 * Component objects are all created, managed and destroyed by the
 * class Triangulation<dim>.  See the Triangulation notes for further
 * information on working with <i>dim</i>-dimensional triangulations.
 *
 * For Regina's \ref stddim "standard dimensions", this template is specialised
 * and offers more functionality.  In order to use these specialised classes,
 * you will need to include the corresponding headers (e.g.,
 * dim2/dim2component.h for \a dim = 2, or triangulation/ncomponent.h for
 * \a dim = 3).  For convenience, there are typedefs available for these
 * specialised classes (such as Dim2Component and NComponent respectively).
 *
 * \ifacespython Python does not support templates.  For standard dimensions
 * this class can be used by appending the dimension as a suffix
 * (e.g., Component2 and Component3 for dimensions 2 and 3).
 * The typedefs mentioned above (e.g., Dim2Component and NComponent)
 * are also available.  Higher-dimensional classes are not available
 * in Python for the time being.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 */
template <int dim>
class Component : public detail::ComponentBase<dim> {
    static_assert(! standardDim(dim),
        "The generic implementation of Component<dim> "
        "should not be used for Regina's standard dimensions.");

    private:
        /**
         * Default constructor.
         *
         * Marks the component as orientable, with no boundary facets.
         */
        Component();

    friend class detail::TriangulationBase<dim>;
};

// Note that some of our component classes are specialised elsewhere.
// Do not explicitly drag in the specialised headers for now.
template <> class Component<2>;
template <> class Component<3>;
template <> class Component<4>;

/*@}*/

// Inline functions for Component

template <int dim>
inline Component<dim>::Component() : detail::ComponentBase<dim>() {
}

} // namespace regina

#endif

