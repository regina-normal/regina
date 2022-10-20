
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

/*! \file link/examplelink.h
 *  \brief Offers several ready-made examples of knots and links.
 */

#ifndef __REGINA_EXAMPLELINK_H
#ifndef __DOXYGEN
#define __REGINA_EXAMPLELINK_H
#endif

#include "regina-core.h"

namespace regina {

class Link;

/**
 * This class offers routines for constructing ready-made examples of
 * knots and links.  These examples may be useful for testing new
 * code, or for simply getting a feel for how Regina works.
 *
 * The sample links offered here may prove especially useful in
 * Regina's scripting interface, where working with pre-existing files
 * is more complicated than in the GUI.
 *
 * \ingroup link
 */
class ExampleLink {
    public:
        /**
         * Returns a zero-crossing diagram of the unknot.
         *
         * @return the unknot.
         */
        static Link unknot();

        /**
         * Returns the monster unknot, a 10-crossing diagram of the
         * unknot that is difficult to untangle.
         *
         * @return the monster unknot.
         */
        static Link monster();

        /**
         * Returns Haken's Gordian unknot, a 141-crossing diagram of the
         * unknot that is difficult to untangle.
         *
         * @return the Gordian unknot.
         */
        static Link gordian();

        /**
         * Returns a three-crossing diagram of the left-hand trefoil.
         *
         * @return the left-hand trefoil.
         */
        static Link trefoilLeft();

        /**
         * Returns a three-crossing diagram of the right-hand trefoil.
         * This returns the same knot as trefoil().
         *
         * @return the right-hand trefoil.
         */
        static Link trefoilRight();

        /**
         * Returns a three-crossing diagram of the right-hand trefoil.
         * This returns the same knot as trefoilRight().
         *
         * @return the right-hand trefoil.
         */
        static Link trefoil();

        /**
         * Returns a four-crossing diagram of the figure eight knot.
         *
         * @return the figure eight knot.
         */
        static Link figureEight();

        /**
         * Returns a two-crossing diagram of the Hopf link.
         * This is the variant in which both crossings are positive.
         *
         * @return the Hopf link.
         */
        static Link hopf();

        /**
         * Returns a five-crossing diagram of the Whitehead link.
         *
         * @return the Whitehead link.
         */
        static Link whitehead();

        /**
         * Returns a six-crossing diagram of the Borromean rings.
         *
         * @return the Borromean rings.
         */
        static Link borromean();

        /**
         * Returns the 11-crossing Conway knot.
         *
         * This is the reflection of \a K11n34 in the Knot Atlas, and is
         * a mutant of the Kinoshita-Terasaka knot.
         *
         * @return the Conway knot.
         */
        static Link conway();

        /**
         * Returns the 11-crossing Kinoshita-Terasaka knot.
         *
         * This is the reflection of \a K11n42 in the Knot Atlas, and is
         * a mutant of the Conway knot.  It has trivial Alexander polynomial.
         *
         * @return the kinoshita-Terasaka knot.
         */
        static Link kinoshitaTerasaka();

        /**
         * Returns the (\a p,\a q) torus link.
         *
         * The parameters \a p and \a q must be non-negative, but they do
         * not need to be coprime.
         *
         * All of the crossings in the resulting link will be positive.
         *
         * \param p the first parameter of the torus link; this must be
         * strictly non-negative.
         * \param q the second parameter of the torus link; this must
         * also be strictly non-negative.
         * @return the (\a p, \a q) torus link.
         */
        static Link torus(int p, int q);

        /**
         * Returns a 48-crossing potential counterexample to the
         * slice-ribbon conjecture, as described by Gompf, Scharlemann
         * and Thompson.
         *
         * Specifically, this knot is Figure 2 from their paper
         * "Fibered knots and potential counterexamples to the property
         * 2R and slice-ribbon conjectures", arXiv:1103.1601.
         *
         * @return the Gompf-Scharlemann-Thompson knot.
         */
        static Link gst();

        // Make this class non-constructible.
        ExampleLink() = delete;
};

} // namespace regina

#endif

