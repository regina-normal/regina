
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

/*! \file triangulation/example.h
 *  \brief Offers some example higher-dimensional triangulations as
 *  starting points for testing code or getting used to Regina.
 */

#ifndef __EXAMPLE_H
#ifndef __DOXYGEN
#define __EXAMPLE_H
#endif

#include "regina-core.h"
#include "triangulation/forward.h"
#include "triangulation/detail/example.h"

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * This class offers static routines for constructing a variety of sample
 * <i>dim</i>-dimensional triangulations.  These triangulations may be useful
 * for testing new code, or for simply getting a feel for how Regina works.
 *
 * In higher dimensions, only a handful of triangulations are available
 * (in contrast to the 3-dimensional class Example<3>, which offers
 * many interesting examples).  The authors hope to expand this list
 * in future releases of Regina.
 *
 * The sample triangulations offered here may prove especially useful in
 * Regina's scripting interface, where working with pre-existing files
 * is more complicated than in the GUI.
 *
 * All of the methods in this class will assign an appropriate packet label
 * to the triangulation that they return.
 *
 * Note that each of these routines constructs a new triangulation from
 * scratch.  It is up to the caller of each routine to destroy the
 * triangulation that is returned.
 *
 * For Regina's \ref stddim "standard dimensions", this template is
 * specialised and offers many more example triangulations.  In order to
 * use these specialised classes, you will need to include the corresponding
 * headers (e.g., triangulation/example2.h for \a dim = 2, or
 * triangulation/example3.h for \a dim = 3).
 *
 * \ifacespython Python does not support templates.  Instead this class
 * can be used by appending the dimension as a suffix (e.g., Example2
 * and Example3 for dimensions 2 and 3).
 *
 * \tparam dim the dimension of the example triangulations to construct.
 * This must be between 2 and 15 inclusive.
 *
 * \headerfile triangulation/generic.h
 */
template <int dim>
class Example : public detail::ExampleBase<dim> {
    static_assert(! standardDim(dim),
        "The generic implementation of Example<dim> "
        "should not be used for Regina's standard dimensions.");

    /**
     * Explicitly disable the default constructor.
     */
    Example() = delete;
};

/*@}*/

} // namespace regina

#endif

