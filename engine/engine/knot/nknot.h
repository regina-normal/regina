
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nknot.h
 *  \brief Deals with knots and links in S^3.
 */

#ifndef __NKNOT_H
#ifndef __DOXYGEN
#define __NKNOT_H
#endif

#include <string>
#include <list>

namespace regina {

class NTriangulation;

/**
 * \addtogroup knot Knots and Links
 * Knots and links in the 3-sphere.
 * @{
 */

/**
 * Represents knots and links in S^3 and allows the construction of
 * triangulations from them.
 *
 * A knot/link can be presented in one of two ways:  Dowker notation
 * which works for knots only, or using an embedded of a sign planar
 * graph in a plane which works for both knots and links.
 *
 * Once a knot or link is created ideal and finite triangulations of
 * the complement can be constructed.
 *
 * \warning This class is incomplete and at the present time should not
 * be used!
 *
 * \ifacespython Will not be present until this class is functional.
 *
 * @author David Letscher
 */
class NKnot {
    private:
        unsigned numComponents;
            /**< Stores the number of components of the link. */
        unsigned numCrossings;
            /**< Stores the number of crossings in the knot/link. */
        std::list<std::list<int> > black, white;
            /**< Stores the internal representations of the black
             * and white regions. */
        bool valid;
            /**< Indicates if the input represents a valid knot or link. */

    public:
        /**
         * Creates the empty link.
         */
        NKnot();

        /**
         * Creates a knot using its Dowker notation or a spanning surface
         * representation.
         *
         * @param s a string storing the Dowker notation for the knot or
         * the spanning surface representation of a link.
         */
        NKnot(const std::string& s);

        /**
         * Returns true if a valid knot or link was created.
         *
         * @return the validity of the knot.
         */
        bool isValid() const;

        /**
         * Creates a triangulation of the knot complement.  Both ideal and finite
         * triangulations are possible.
         *
         * @param ideal is true if an ideal triangulation should be constructed
         * (default to /c true).
         *
         * @return the triangulation of the knot complement.
         */
        NTriangulation *getTriang(bool ideal=true);

    private:
};

/*@}*/

// Inline functions for NKnot

bool NKnot::isValid() const {
    return valid;
}

} // namespace regina

#endif

