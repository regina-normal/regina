
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

#ifndef __COMPONENT_H
#ifndef __DOXYGEN
#define __COMPONENT_H
#endif

/*! \file triangulation/generic/component.h
 *  \brief Internal header for connected components of triangulations of
 *  arbitrary dimension.
 *
 *  This file is automatically included from triangulation/generic.h;
 *  there is no need for end users to include this header explicitly.
 */

#include "triangulation/detail/component.h"

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
 * you will need to include the corresponding triangulation headers (e.g.,
 * triangulation/dim2.h for \a dim = 2, or triangulation/dim3.h
 * for \a dim = 3).
 *
 * \ifacespython Python does not support templates.  Instead
 * this class can be used by appending the dimension as a suffix
 * (e.g., Component2 and Component3 for dimensions 2 and 3).
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \headerfile triangulation/generic.h
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
        Component() = default;

    friend class detail::TriangulationBase<dim>;
};

/*@}*/

} // namespace regina

#endif

