
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

/*! \file nfacepair.h
 *  \brief Deals with simple pairs of face numbers.
 */

#ifndef __NFACEPAIR_H
#ifndef __DOXYGEN
#define __NFACEPAIR_H
#endif

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Represents a pair of tetrahedron face numbers.
 *
 * Note that we are not storing face numbers in a triangulation
 * skeleton, but merely face numbers 0, 1, 2 and 3 in a single
 * tetrahedron.  Thus there are only six possible face pairs; this class
 * merely makes working with such pairs more convenient.
 *
 * As well as providing the six possible face pairs, there is also a
 * before-the-start and a past-the-end value; this (combined with the
 * increment operator) allows for iteration through face pairs.
 *
 * When iterating through face pairs, the ordering will be the
 * lexicographical ordering (0,1), (0,2), (0,3), (1,2), (1,3), (2,3).
 *
 * The before-the-start and past-the-end values are internally
 * represented as (0,0) and (3,3) respectively.
 */
class NFacePair {
    private:
        unsigned first;
            /**< The smaller of the two faces in this pair. */
        unsigned second;
            /**< The larger of the two faces in this pair. */

    public:
        /**
         * Creates a new face pair (0,1).
         */
        NFacePair();
        /**
         * Creates a new face pair from the two given face numbers.
         * The two given numbers need not be in any particular order.
         *
         * \pre The two given face numbers must be distinct integers
         * between 0 and 3 inclusive.
         *
         * @param newFirst the first face number in the new pair.
         * @param newSecond the second face number in the new pair.
         */
        NFacePair(int newFirst, int newSecond);
        /**
         * Creates a new face pair that is a clone of the given pair.
         *
         * @param cloneMe the face pair to clone.
         */
        NFacePair(const NFacePair& cloneMe);

        /**
         * Returns the smaller of the two face numbers in this pair.
         *
         * \pre This face pair is neither before-the-start or
         * past-the-end.
         *
         * @return the lower face number.
         */
        unsigned lower() const;
        /**
         * Returns the larger of the two face numbers in this pair.
         *
         * \pre This face pair is neither before-the-start or
         * past-the-end.
         *
         * @return the upper face number.
         */
        unsigned upper() const;

        /**
         * Determines if this face pair represents a before-the-start
         * value.
         *
         * @return \c true if and only if this face pair is
         * before-the-start.
         */
        bool isBeforeStart() const;
        /**
         * Determines if this face pair represents a past-the-end
         * value.
         *
         * @return \c true if and only if this face pair is
         * past-the-end.
         */
        bool isPastEnd() const;

        /**
         * Returns the complement of this face pair.  The complement
         * is the pair containing the two faces not included in this
         * face pair.
         *
         * \pre This face pair is neither before-the-start nor
         * past-the-end.
         *
         * @return the complement of this face pair.
         */
        NFacePair complement() const;

        /**
         * Sets this face pair to be a copy of the given pair.
         *
         * @param cloneMe the face pair to clone.
         * @return a reference to this face pair.
         */
        NFacePair& operator = (const NFacePair& cloneMe);
        /**
         * Determines if this and the given face pair are equal.
         *
         * @param other the pair to compare with this.
         * @return \c true if and only if this and the given pair are equal.
         */
        bool operator == (const NFacePair& other) const;
        /**
         * Determines if this is less than the given face pair.
         *
         * @param other the pair to compare with this.
         * @return \c true if and only if this is less than \a other.
         */
        bool operator < (const NFacePair& other) const;
        /**
         * Determines if this is greater than the given face pair.
         *
         * @param other the pair to compare with this.
         * @return \c true if and only if this is greater than \a other.
         */
        bool operator > (const NFacePair& other) const;
        /**
         * Determines if this is less than or equal to the given face pair.
         *
         * @param other the pair to compare with this.
         * @return \c true if and only if this is less than or
         * equal to \a other.
         */
        bool operator <= (const NFacePair& other) const;
        /**
         * Determines if this is greater than or equal to the given face pair.
         *
         * @param other the pair to compare with this.
         * @return \c true if and only if this is greater than or
         * equal to \a other.
         */
        bool operator >= (const NFacePair& other) const;
        /**
         * Increments this face pair.  It will be set to the following
         * face pair in the lexicographical ordering, or to a
         * past-the-end value if there are no more face pairs.
         *
         * \ifacespython This routine is called inc(), since Python does
         * not support the increment operator.
         */
        void operator ++ (int);
        /**
         * Decrements this face pair.  It will be set to the previous
         * face pair in the lexicographical ordering, or to a
         * before-the-start value if there are no previous face pairs.
         *
         * \ifacespython This routine is called dec(), since Python does
         * not support the decrement operator.
         */
        void operator -- (int);
};

/*@}*/

// Inline functions for NFacePair

inline NFacePair::NFacePair() : first(0), second(1) {
}

inline NFacePair::NFacePair(const NFacePair& cloneMe) :
        first(cloneMe.first), second(cloneMe.second) {
}

inline unsigned NFacePair::lower() const {
    return first;
}

inline unsigned NFacePair::upper() const {
    return second;
}

inline bool NFacePair::isBeforeStart() const {
    return (second <= 0);
}

inline bool NFacePair::isPastEnd() const {
    return (first >= 3);
}

inline NFacePair& NFacePair::operator = (const NFacePair& cloneMe) {
    first = cloneMe.first;
    second = cloneMe.second;
    return *this;
}

inline bool NFacePair::operator == (const NFacePair& other) const {
    return (first == other.first && second == other.second);
}

inline bool NFacePair::operator < (const NFacePair& other) const {
    if (first < other.first)
        return true;
    if (first > other.first)
        return false;
    return (second < other.second);
}

inline bool NFacePair::operator > (const NFacePair& other) const {
    return (other < *this);
}

inline bool NFacePair::operator <= (const NFacePair& other) const {
    if (first < other.first)
        return true;
    if (first > other.first)
        return false;
    return (second <= other.second);
}

inline bool NFacePair::operator >= (const NFacePair& other) const {
    return (other <= *this);
}

} // namespace regina

#endif

