
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file triangulation/facepair.h
 *  \brief Deals with simple pairs of face numbers.
 */

#ifndef __REGINA_FACEPAIR_H
#ifndef __DOXYGEN
#define __REGINA_FACEPAIR_H
#endif

#include "regina-core.h"
#include <cstdint>
#include <iostream>

namespace regina {

/**
 * Represents a pair of tetrahedron face numbers.
 *
 * Note that we are not storing triangle numbers in a triangulation
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
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \ingroup triangulation
 */
class FacePair {
    private:
        uint8_t code_;
            /**< An internal code, whose lowest two bits represent the
                 smaller index face, and whose next two bits represent
                 the higher index face.  Specifically:
                 (0,0) (0,1) (0,2) (0,3) (1,2) (1,3) (2,3) (3,3) ->
                  0,    1,    2,    3,    6,    7,    11,   15. */

    public:
        /**
         * Creates a new face pair (0,1).
         */
        FacePair();
        /**
         * Creates a new face pair from the two given face numbers.
         * The two given numbers need not be in any particular order.
         *
         * \pre The two given face numbers must be distinct integers
         * between 0 and 3 inclusive.
         *
         * \param first the first face number in the new pair.
         * \param second the second face number in the new pair.
         */
        FacePair(int first, int second);
        /**
         * Creates a new face pair that is a clone of the given pair.
         */
        FacePair(const FacePair&) = default;

        /**
         * Returns the smaller of the two face numbers in this pair.
         *
         * \pre This face pair is neither before-the-start or
         * past-the-end.
         *
         * \return the lower face number.
         */
        int lower() const;
        /**
         * Returns the larger of the two face numbers in this pair.
         *
         * \pre This face pair is neither before-the-start or
         * past-the-end.
         *
         * \return the upper face number.
         */
        int upper() const;

        /**
         * Determines if this face pair represents a before-the-start
         * value.
         *
         * \return \c true if and only if this face pair is
         * before-the-start.
         */
        bool isBeforeStart() const;
        /**
         * Determines if this face pair represents a past-the-end
         * value.
         *
         * \return \c true if and only if this face pair is
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
         * \return the complement of this face pair.
         */
        FacePair complement() const;

        /**
         * Sets this face pair to be a copy of the given pair.
         *
         * \param cloneMe the face pair to clone.
         * \return a reference to this face pair.
         */
        FacePair& operator = (const FacePair& cloneMe) = default;
        /**
         * Determines if this and the given face pair are equal.
         *
         * \param other the pair to compare with this.
         * \return \c true if and only if this and the given pair are equal.
         */
        bool operator == (const FacePair& other) const;
        /**
         * Determines if this and the given face pair are not equal.
         *
         * \param other the pair to compare with this.
         * \return \c true if and only if this and the given pair are not equal.
         */
        bool operator != (const FacePair& other) const;
        /**
         * Determines if this is less than the given face pair.
         *
         * \param other the pair to compare with this.
         * \return \c true if and only if this is less than \a other.
         */
        bool operator < (const FacePair& other) const;
        /**
         * Determines if this is greater than the given face pair.
         *
         * \param other the pair to compare with this.
         * \return \c true if and only if this is greater than \a other.
         */
        bool operator > (const FacePair& other) const;
        /**
         * Determines if this is less than or equal to the given face pair.
         *
         * \param other the pair to compare with this.
         * \return \c true if and only if this is less than or
         * equal to \a other.
         */
        bool operator <= (const FacePair& other) const;
        /**
         * Determines if this is greater than or equal to the given face pair.
         *
         * \param other the pair to compare with this.
         * \return \c true if and only if this is greater than or
         * equal to \a other.
         */
        bool operator >= (const FacePair& other) const;
        /**
         * Increments this face pair.  It will be set to the following
         * face pair in the lexicographical ordering, or to a
         * past-the-end value if there are no more face pairs.
         *
         * This is a preincrement operator: the object will be changed,
         * and then a reference to it will be returned.
         *
         * \pre This face pair is not currently past-the-end.
         *
         * \nopython The postincrement operator is available in Python
         * under the name inc().
         *
         * \return a reference to this object.
         */
        FacePair& operator ++ ();
        /**
         * Increments this face pair.  It will be set to the following
         * face pair in the lexicographical ordering, or to a
         * past-the-end value if there are no more face pairs.
         *
         * This is a postincrement operator: the object will be changed,
         * but a copy of the original reference will be returned.
         *
         * \pre This face pair is not currently past-the-end.
         *
         * \python This routine is available under the name inc().
         *
         * \return a copy of this object before the change took place.
         */
        FacePair operator ++ (int);
        /**
         * Decrements this face pair.  It will be set to the previous
         * face pair in the lexicographical ordering, or to a
         * before-the-start value if there are no previous face pairs.
         *
         * This is a predecrement operator: the object will be changed,
         * and then a reference to it will be returned.
         *
         * \pre This face pair is not currently before-the-start.
         *
         * \nopython The postdecrement operator is available in Python
         * under the name dec().
         *
         * \return a reference to this object.
         */
        FacePair& operator -- ();
        /**
         * Decrements this face pair.  It will be set to the previous
         * face pair in the lexicographical ordering, or to a
         * before-the-start value if there are no previous face pairs.
         *
         * This is a postdecrement operator: the object will be changed,
         * but a copy of the original reference will be returned.
         *
         * \pre This face pair is not currently before-the-start.
         *
         * \python This routine is available under the name dec().
         *
         * \return a copy of this object before the change took place.
         */
        FacePair operator -- (int);

        /**
         * Identifies the tetrahedron edge that is common to both faces
         * in this pair.
         *
         * This will be a tetrahedron edge number, between 0 and 5 inclusive.
         * Note that edges commonEdge() and oppositeEdge() will be opposite
         * edges in the tetrahedron.
         *
         * \pre This face pair is neither before-the-start nor
         * past-the-end.
         *
         * \return the edge that belongs to both faces in this pair.
         */
        int commonEdge() const;
        /**
         * Identifies the tetrahedron edge that belongs to neither of
         * the two faces in this pair.
         *
         * This will be a tetrahedron edge number, between 0 and 5 inclusive.
         * Note that edges commonEdge() and oppositeEdge() will be opposite
         * edges in the tetrahedron.
         *
         * \pre This face pair is neither before-the-start nor
         * past-the-end.
         *
         * \return the edge that does not belong to either of these two faces.
         */
        int oppositeEdge() const;

    private:
        /**
         * Creates a new face pair with the given internal code.
         */
        FacePair(uint8_t code);
};

/**
 * Writes the given face pair to the given output stream.
 *
 * \param out the output stream to which to write.
 * \param pair the face pair to write.
 * \return a reference to \a out.
 *
 * \ingroup triangulation
 */
std::ostream& operator << (std::ostream& out, const FacePair& pair);

// Inline functions for FacePair

inline FacePair::FacePair() : code_(1) {
}

inline FacePair::FacePair(int first, int second) :
        code_(static_cast<uint8_t>(first < second ?
              ((first << 2) | second) :
              ((second << 2) | first))) {
}

inline FacePair::FacePair(uint8_t code) : code_(code) {
}

inline int FacePair::lower() const {
    return (code_ >> 2) & 3;
}

inline int FacePair::upper() const {
    return (code_ & 3);
}

inline bool FacePair::isBeforeStart() const {
    return (code_ <= 0);
}

inline bool FacePair::isPastEnd() const {
    return (code_ >= 15);
}

inline FacePair FacePair::complement() const {
    // Codes: 1 <-> 11, 2 <-> 7, 3 <-> 6.
    switch (code_) {
        case 1: return FacePair(11);
        case 11: return FacePair(1);
        default: return FacePair(9 - code_);
    }
}

inline bool FacePair::operator == (const FacePair& other) const {
    return (code_ == other.code_);
}

inline bool FacePair::operator != (const FacePair& other) const {
    return (code_ != other.code_);
}

inline bool FacePair::operator < (const FacePair& other) const {
    return (code_ < other.code_);
}

inline bool FacePair::operator > (const FacePair& other) const {
    return (code_ > other.code_);
}

inline bool FacePair::operator <= (const FacePair& other) const {
    return (code_ <= other.code_);
}

inline bool FacePair::operator >= (const FacePair& other) const {
    return (code_ >= other.code_);
}

inline FacePair FacePair::operator ++ (int) {
    FacePair ans = *this;
    ++(*this);
    return ans;
}

inline FacePair FacePair::operator -- (int) {
    FacePair ans = *this;
    --(*this);
    return ans;
}

inline std::ostream& operator << (std::ostream& out, const FacePair& pair) {
    return out << '{' << pair.lower() << ',' << pair.upper() << '}';
}

inline int FacePair::commonEdge() const {
    // Code 1, 2, 3, 6, 7, 11 -> edge 5, 4, 3, 2, 1, 0.
    if (code_ < 6)
        return 6 - code_;
    if (code_ < 11)
        return 8 - code_;
    return 0;
}

inline int FacePair::oppositeEdge() const {
    // Code 1, 2, 3, 6, 7, 11 -> edge 0, 1, 2, 3, 4, 5.
    if (code_ < 6)
        return code_ - 1;
    if (code_ < 11)
        return code_ - 3;
    return 5;
}

} // namespace regina

#endif

