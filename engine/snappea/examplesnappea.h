
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file snappea/examplesnappea.h
 *  \brief Offers several ready-made example SnapPea triangulations.
 */

#ifndef __REGINA_EXAMPLESNAPPEA_H
#ifndef __DOXYGEN
#define __REGINA_EXAMPLESNAPPEA_H
#endif

#include "regina-core.h"

namespace regina {

class SnapPeaTriangulation;

/**
 * This class offers routines for constructing various example SnapPea
 * triangulations.  This is analagous to the (much richer) Example<3> class.
 *
 * \ingroup snappea
 */
class ExampleSnapPea {
    public:
        /**
         * Returns a new triangulation of the Gieseking manifold.
         *
         * @return the Gieseking manifold.
         */
        static SnapPeaTriangulation gieseking();

        /**
         * Returns a new triangulation of the figure eight knot complement.
         *
         * @return the figure eight knot complement.
         */
        static SnapPeaTriangulation figureEight();

        /**
         * Returns a new triangulation of the trefoil knot complement.
         *
         * @return the trefoil knot complement.
         */
        static SnapPeaTriangulation trefoil();

        /**
         * Returns a new triangulation of the Whitehead link complement.
         *
         * @return the Whitehead link complement.
         */
        static SnapPeaTriangulation whiteheadLink();

        /**
         * Returns a new triangulation of the census manifold \c x101.
         *
         * @return the census manifold \c x101.
         */
        static SnapPeaTriangulation x101();

        // Make this class non-constructible.
        ExampleSnapPea() = delete;
};

} // namespace regina

#endif

