
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

/*! \file nmiscutils.h
 *  \brief Provides small miscellaneous utility classes.
 */

#ifndef __NMISCUTILS_H
#ifndef __DOXYGEN
#define __NMISCUTILS_H
#endif

/**
 * An ordered pair of elements of two given (possibly different) types.
 *
 * \pre Types S and T both provide copy constructors.
 *
 * \ifaces Not present.
 */
template <class S, class T>
struct NOrderedPair {
    S first;
        /**< First element of the ordered pair. */
    T second;
        /**< Second element of the ordered pair. */

    /**
     * Creates a new ordered pair whose elements are built from default
     * constructors.
     *
     * \pre Types S and T both provide default constructors.
     */
    NOrderedPair() {
    }
    /**
     * Creates a new ordered pair containing the given values.
     *
     * @param newFirst the value to use for the first element of the
     * ordered pair.
     * @param newSecond the value to use for the second element of the
     * ordered pair.
     */
    NOrderedPair(const S& newFirst, const T& newSecond) :
            first(newFirst), second(newSecond) {
    }
    /**
     * Creates a new ordered pair containing the same values as the
     * given ordered pair.
     *
     * @param cloneMe the ordered pair to clone.
     */
    NOrderedPair(const NOrderedPair& cloneMe) : first(cloneMe.first),
            second(cloneMe.second) {
    }
    /**
     * Assigns the values of the elements in the given ordered pair to
     * the elements of this ordered pair.
     *
     * \pre Types S and T both provide assignment operators.
     *
     * @param other the pairs whose value will be assigned to this pair.
     */
    NOrderedPair<S,T>& operator = (const NOrderedPair<S,T>& other) {
        first = other.first;
        second = other.second;
        return *this;
    }
    /**
     * Determines if this pair is strictly less than the given pair
     * according to lexicographical ordering.  The first elements of the
     * pairs are compared, and if these are equal the second elements
     * are compared instead.
     *
     * \pre Types S and T both provide operators <tt>\<</tt>.
     *
     * @param other the pair with which this will be compared.
     * @return \c true if and only if this pair is strictly less than
     * the given pair.
     */
    bool operator < (const NOrderedPair<S,T>& other) {
        if (first < other.first)
            return true;
        if (other.first < first)
            return false;
        return (second < other.second);
    }
};

#endif

