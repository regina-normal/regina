
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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
        unsigned maxNonZero;
            /**< The non-zero coordinate cap for this constraint. */

    public:
        /**
         * Creates a new constraint with an empty set of coordinate
         * positions and a non-zero coordinate cap of zero.
         */
        NCompConstraint();
        /**
         * Creates a new constraint with an empty set of coordinate
         * positions and the given non-zero coordinate cap.
         *
         * @param newMaxNonZero the maximum number of coordinate positions
         * from the associated set for which some vector may have
         * non-zero entries without breaking this constraint.
         */
        NCompConstraint(unsigned newMaxNonZero);

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

        /**
         * Returns the non-zero coordinate cap for this constraint.
         *
         * @return the maximum number of coordinate positions
         * from the associated set for which some vector may have
         * non-zero entries without breaking this constraint.
         */
        unsigned getMaxNonZero() const;
        /**
         * Sets the non-zero coordinate cap for this constraint.
         *
         * @param newMaxNonZero the maximum number of coordinate positions
         * from the associated set for which some vector may have
         * non-zero entries without breaking this constraint.
         */
        void setMaxNonZero(unsigned newMaxNonZero);

        /**
         * Determines whether the given pair of vectors satisfies this
         * constraint.
         *
         * @param first the first of the two vectors to examine.
         * @param second the second of the two vectors to examine.
         * @return \c true if the given pair of vectors satisfies this
         * constraint, or \c false otherwise.
         */
        template <class T>
        bool isSatisfied(const NVector<T>& first,
            const NVector<T>& second) const;
        /**
         * Determines whether the given vector satisfies this constraint.
         *
         * Although constraints are defined in terms of pairs of
         * vectors, a single vector can also be said to satisfy a constraint
         * if the number of non-zero coordinates amongst the given set of
         * coordinate positions is at most the given maximum.
         *
         * Examining whether a single vector <i>v</i> satisfies a constraint
         * is equivalent to deciding whether the pair (<i>v</i>, <i>v</i>)
         * or equivalently the pair (<i>v</i>, 0) satisfies the constraint.
         *
         * @param v the vector to examine.
         * @return \c true if the given vector satisfies this constraint,
         * or \c false otherwise.
         */
        template <class T>
        bool isSatisfied(const NVector<T>& v) const;
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

        /**
         * Determines whether the given pair of vectors satisfies every
         * constraint in this set.
         *
         * @param first the first of the two vectors to examine.
         * @param second the second of the two vectors to examine.
         * @return \c true if the given pair of vectors satisfies every
         * constraint in this set, or \c false if some constraint is not
         * satisfied.
         */
        template <class T>
        bool isSatisfied(const NVector<T>& first,
            const NVector<T>& second) const;
        /**
         * Determines whether the given vector satisfies every
         * constraint in this set.
         *
         * Although constraints are defined in terms of pairs of
         * vectors, a single vector can also be said to satisfy a constraint
         * if the number of non-zero coordinates amongst the given set of
         * coordinate positions is at most the given maximum.
         *
         * Examining whether a single vector <i>v</i> satisfies a constraint
         * is equivalent to deciding whether the pair (<i>v</i>, <i>v</i>)
         * or equivalently the pair (<i>v</i>, 0) satisfies the constraint.
         *
         * @param v the vector to examine.
         * @return \c true if the given vector satisfies every
         * constraint in this set, or \c false if some constraint is not
         * satisfied.
         */
        template <class T>
        bool isSatisfied(const NVector<T>& v) const;
};

/*@}*/

// Inline functions for NCompConstraint

inline NCompConstraint::NCompConstraint() : maxNonZero(0) {
}
inline NCompConstraint::NCompConstraint(unsigned newMaxNonZero) :
        maxNonZero(newMaxNonZero) {
}

inline const std::set<unsigned>& NCompConstraint::getCoordinates() const {
    return coordinates;
}
inline std::set<unsigned>& NCompConstraint::getCoordinates() {
    return coordinates;
}

inline unsigned NCompConstraint::getMaxNonZero() const {
    return maxNonZero;
}
inline void NCompConstraint::setMaxNonZero(unsigned newMaxNonZero) {
    maxNonZero = newMaxNonZero;
}

// Inline functions for NCompConstraintSet

inline NCompConstraintSet::NCompConstraintSet() {
}
inline NCompConstraintSet::~NCompConstraintSet() {
    for_each(begin(), end(), FuncDelete<NCompConstraint>());
}

} // namespace regina

// Template definitions

#include "enumerate/ncompconstraint.tcc"

#endif

