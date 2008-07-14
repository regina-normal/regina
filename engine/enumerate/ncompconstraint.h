
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file ncompconstraint.h
 *  \brief Deals with compatibility constraints in polytope vertex enumeration.
 */

#ifndef __NCOMPCONSTRAINT_H
#ifndef __DOXYGEN
#define __NCOMPCONSTRAINT_H
#endif

#include <algorithm>
#include <deque>
#include <set>
#include "maths/nvector.h"
#include "utilities/memutils.h"

namespace regina {

/**
 * \weakgroup enumerate
 * @{
 */

/**
 * Represents an individual compatibility constraint for use with
 * polytope vertex enumeration.
 *
 * Some vertex enumerations search only for vectors representing \e valid
 * vertices, under varying definitions of valid.  In this case, two valid
 * vectors are said to be \e compatible if any convex combination of the two
 * vectors is also valid.
 *
 * A compatibility constraint represents a necessary condition for two
 * valid vectors to be compatible.  This class stores a single constraint
 * stating that, for a given set of coordinate positions, all but at most
 * a given maximum number of these positions must be set to zero in
 * both vectors.  This maximum number will be referred to as the
 * <i>non-zero coordinate cap</i>.
 *
 * Thus, for a pair of vectors to satisfy this constraint, the number of
 * coordinate positions in the given set for which at least one of the two
 * vectors has a non-zero entry must be at most the non-zero coordinate cap.
 *
 * \ifacespython Not present.
 */
class NCompConstraint {
    private:
        std::set<unsigned> coordinates;
            /**< The set of coordinate positions under consideration. */

    public:
        /**
         * Creates a new constraint with an empty set of coordinate
         * positions and a non-zero coordinate cap of zero.
         */
        NCompConstraint();

        /**
         * Returns the set of coordinate positions under consideration
         * by this constraint.
         *
         * @return the associated set of coordinate positions.
         */
        const std::set<unsigned>& getCoordinates() const;
        /**
         * Returns the set of coordinate positions under consideration
         * by this constraint.
         *
         * This non-const member function may be used to modify the
         * associated set of coordinate positions.
         *
         * @return the associated set of coordinate positions.
         */
        std::set<unsigned>& getCoordinates();
};

/**
 * Represents a set of compatibility constraints for use with
 * polytope vertex enumeration.
 *
 * A compatibility constraint set represents a set of necessary and
 * sufficient conditions for two valid vectors to be compatible.  See
 * class NCompConstraint for details on the concept of compatibility.
 *
 * \ifacespython Not present.
 */
class NCompConstraintSet : public std::deque<NCompConstraint*> {
    public:
        /**
         * Creates a new empty constraint set.
         */
        NCompConstraintSet();
        /**
         * Destroys this constraint set.  All individual constraints
         * belonging to this set will be deallocated.
         */
        ~NCompConstraintSet();
};

/*@}*/

// Inline functions for NCompConstraint

inline NCompConstraint::NCompConstraint() {
}

inline const std::set<unsigned>& NCompConstraint::getCoordinates() const {
    return coordinates;
}
inline std::set<unsigned>& NCompConstraint::getCoordinates() {
    return coordinates;
}

// Inline functions for NCompConstraintSet

inline NCompConstraintSet::NCompConstraintSet() {
}
inline NCompConstraintSet::~NCompConstraintSet() {
    for_each(begin(), end(), FuncDelete<NCompConstraint>());
}

} // namespace regina

#endif

