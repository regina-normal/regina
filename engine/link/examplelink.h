
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

/*! \file link/examplelink.h
 *  \brief Offers several ready-made examples of knots and links.
 */

#ifndef __EXAMPLELINK_H
#ifndef __DOXYGEN
#define __EXAMPLELINK_H
#endif

#include "regina-core.h"

namespace regina {

class Link;

/**
 * \weakgroup link
 * @{
 */

/**
 * This class offers routines for constructing ready-made examples of
 * knots and links.  These examples may be useful for testing new
 * code, or for simply getting a feel for how Regina works.
 *
 * The sample links offered here may prove especially useful in
 * Regina's scripting interface, where working with pre-existing files
 * is more complicated than in the GUI.
 *
 * All of the methods in this class will assign an appropriate packet label
 * to the link that they return.
 *
 * Note that each of these routines constructs a new link from scratch.
 * It is up to the caller of each routine to destroy the link that is returned.
 */
class REGINA_API ExampleLink {
    public:
        /**
         * Returns a zero-crossing diagram of the unknot.
         *
         * @return a newly constructed link, which must be
         * destroyed by the caller of this routine.
         */
        static Link* unknot();

        /**
         * Returns Haken's Gordian unknot, a 141-crossing diagram of the
         * unknot that is difficult to untangle.
         *
         * @return a newly constructed link, which must be
         * destroyed by the caller of this routine.
         */
        static Link* gordian();

        /**
         * Returns a three-crossing diagram of the left-hand trefoil.
         *
         * @return a newly constructed link, which must be
         * destroyed by the caller of this routine.
         */
        static Link* trefoilLeft();

        /**
         * Returns a three-crossing diagram of the right-hand trefoil.
         * This returns the same knot as trefoil().
         *
         * @return a newly constructed link, which must be
         * destroyed by the caller of this routine.
         */
        static Link* trefoilRight();

        /**
         * Returns a three-crossing diagram of the right-hand trefoil.
         * This returns the same knot as trefoilRight().
         *
         * @return a newly constructed link, which must be
         * destroyed by the caller of this routine.
         */
        static Link* trefoil();

        /**
         * Returns a four-crossing diagram of the figure eight knot.
         *
         * @return a newly constructed link, which must be
         * destroyed by the caller of this routine.
         */
        static Link* figureEight();

        /**
         * Returns a two-crossing diagram of the Hopf link.
         *
         * @return a newly constructed link, which must be
         * destroyed by the caller of this routine.
         */
        static Link* hopf();

        /**
         * Returns a five-crossing diagram of the Whitehead link.
         *
         * @return a newly constructed link, which must be
         * destroyed by the caller of this routine.
         */
        static Link* whitehead();

        /**
         * Returns a six-crossing diagram of the Borromean rings.
         *
         * @return a newly constructed link, which must be
         * destroyed by the caller of this routine.
         */
        static Link* borromean();
};

/*@}*/

} // namespace regina

#endif

