
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file utilities/boolset.h
 *  \brief Provides various types that extend the standard boolean.
 */

#ifndef __REGINA_BOOLSET_H
#ifndef __DOXYGEN
#define __REGINA_BOOLSET_H
#endif

#include <iostream>
#include <string>
#include "regina-core.h"

namespace regina {

/**
 * A set of booleans.  Note that there are only four possible such sets.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \ingroup utilities
 */
class BoolSet {
    private:
        unsigned char elements;
            /**< The first two bits of this character represent whether
                 or not \c true or \c false belongs to this set. */

        static constexpr unsigned char eltTrue = 1;
            /**< A character with only the \c true member bit set. */
        static constexpr unsigned char eltFalse = 2;
            /**< A character with only the \c false member bit set. */
        static constexpr char stringCodes[4][3] = { "--", "T-", "-F", "TF" };
            /**< The string codes for all four boolean sets. */

    public:
        /**
         * Creates a new empty set.
         */
        constexpr BoolSet();
        /**
         * Creates a set containing a single boolean, which is passed as
         * an argument.
         *
         * \param member the single element to include in this set.
         */
        constexpr BoolSet(bool member);
        /**
         * Creates a clone of the given set.
         */
        constexpr BoolSet(const BoolSet&) = default;
        /**
         * Creates a set specifying whether \c true and/or \c false
         * should be a member.
         *
         * \param insertTrue should the new set include the element `true`?
         * \param insertFalse should the new set include the element `false`?
         */
        constexpr BoolSet(bool insertTrue, bool insertFalse);

        /**
         * Determines if \c true is a member of this set.
         *
         * \return \c true if and only if \c true is a member of this
         * set.
         */
        constexpr bool hasTrue() const;
        /**
         * Determines if \c false is a member of this set.
         *
         * \return \c true if and only if \c false is a member of this
         * set.
         */
        constexpr bool hasFalse() const;
        /**
         * Determines if the given boolean is a member of this set.
         *
         * \param value the boolean to search for in this set.
         * \return \c true if and only if the given boolean is a member
         * of this set.
         */
        constexpr bool contains(bool value) const;
        /**
         * Determines whether this is the full set, containing both
         * \c true and \c false.
         *
         * \return \c true if and only if this is the full set.
         */
        constexpr bool full() const;

        /**
         * Inserts \c true into this set if it is not already present.
         */
        void insertTrue();
        /**
         * Inserts \c false into this set if it is not already present.
         */
        void insertFalse();
        /**
         * Removes \c true from this set if it is present.
         */
        void removeTrue();
        /**
         * Removes \c false from this set if it is present.
         */
        void removeFalse();
        /**
         * Removes all elements from this set.
         *
         * In Regina 7.3 and earlier, this routine was called empty().
         * It has been renamed to clear(), _without_ a deprecated alias, to
         * avoid confusion with the more common pattern where empty() queries
         * whether a container holds any elements at all.
         */
        void clear();
        /**
         * Places both \c true and \c false into this set if they are
         * not already present.
         */
        void fill();

        /**
         * Determines if this set is equal to the given set.
         *
         * \return \c true if and only if this and the given set are
         * equal.
         */
        constexpr bool operator == (const BoolSet&) const = default;
        /**
         * Compares two sets under the subset relation.
         *
         * This generates all of the usual comparison operators, including
         * `<`, `<=`, `>`, and `>=`.
         *
         * \python This spaceship operator `x <=> y` is not available, but the
         * other comparison operators that it generates _are_ available.
         *
         * \param rhs the set to compare with this.
         * \return The result of the subset comparison between this and the
         * given set.
         */
        constexpr std::partial_ordering operator <=> (BoolSet rhs) const;

        /**
         * Sets this set to be identical to the given set.
         *
         * \return a reference to this set.
         */
        BoolSet& operator = (const BoolSet&) = default;
        /**
         * Sets this set to the single member set containing the given
         * element.
         *
         * \param member the single element to include in this set.
         * \return a reference to this set.
         */
        BoolSet& operator = (bool member);
        /**
         * Sets this set to be the union of this and the given set.
         * The result is a set containing precisely the elements that
         * belong to either of the original sets.
         * Note that this set will be modified.
         *
         * \param other the set to union with this set.
         * \return a reference to this set.
         */
        BoolSet& operator |= (BoolSet other);
        /**
         * Sets this set to be the intersection of this and the given set.
         * The result is a set containing precisely the elements that
         * belong to both original sets.
         * Note that this set will be modified.
         *
         * \param other the set to intersect with this set.
         * \return a reference to this set.
         */
        BoolSet& operator &= (BoolSet other);
        /**
         * Sets this set to be the symmetric difference of this and the
         * given set.
         * The result is a set containing precisely the elements that
         * belong to one but not both of the original sets.
         * Note that this set will be modified.
         *
         * \param other the set whose symmetric difference with this set
         * is to be found.
         * \return a reference to this set.
         */
        BoolSet& operator ^= (BoolSet other);

        /**
         * Returns the union of this set with the given set.
         * The result is a set containing precisely the elements that
         * belong to either of the original sets.
         * This set is not changed.
         *
         * \param other the set to union with this set.
         * \return the union of this and the given set.
         */
        constexpr BoolSet operator | (BoolSet other) const;
        /**
         * Returns the intersection of this set with the given set.
         * The result is a set containing precisely the elements that
         * belong to both original sets.
         * This set is not changed.
         *
         * \param other the set to intersect with this set.
         * \return the intersection of this and the given set.
         */
        constexpr BoolSet operator & (BoolSet other) const;
        /**
         * Returns the symmetric difference of this set and the given set.
         * The result is a set containing precisely the elements that
         * belong to one but not both of the original sets.
         * This set is not changed.
         *
         * \param other the set whose symmetric difference with this set
         * is to be found.
         * \return the symmetric difference of this and the given set.
         */
        constexpr BoolSet operator ^ (BoolSet other) const;
        /**
         * Returns the complement of this set.
         * The result is a set containing precisely the elements that
         * this set does not contain.
         * This set is not changed.
         *
         * \return the complement of this set.
         */
        constexpr BoolSet operator ~ () const;

        /**
         * Returns the byte code representing this boolean set.
         * The byte code is sufficient to reconstruct the set
         * and is thus a useful means for passing boolean sets to and
         * from the engine.
         *
         * The lowest order bit of the byte code is 1 if and only if
         * \c true is in the set.  The next lowest order bit is 1 if and
         * only if \c false is in the set.  All other bits are 0.
         * Therefore sets {}, {true}, {false} and {true, false} have
         * byte codes 0, 1, 2 and 3 respectively.
         *
         * \return the byte code representing this set.
         */
        constexpr unsigned char byteCode() const;
        /**
         * Sets this to be the boolean set represented by the given byte code.
         * See byteCode() for more information on byte codes.
         *
         * If \a code is not a value byte code, then this routine will
         * do nothing and return \c false.
         *
         * \param code the byte code that will determine the new value
         * of this set.
         * \return \c true if and only if \c code is a valid byte code.
         */
        bool setByteCode(unsigned char code);
        /**
         * Creates a boolean set from the given byte code.
         * See byteCode() for more information on byte codes.
         *
         * \pre \a code is 0, 1, 2 or 3.
         *
         * \param code the byte code from which the new set will be
         * created.
         */
        constexpr static BoolSet fromByteCode(unsigned char code);

        /**
         * Returns the string code representing this boolean set.
         * String codes are a more human-readable alternative to byte codes;
         * in particular, they are used in XML data files.
         *
         * Every string code contains precisely two characters (plus a
         * terminating null).
         * Sets {}, {true}, {false} and {true, false} have string codes
         * `--`, `T-`, `-F` and `TF` respectively.
         *
         * \return the two-character string code representing this set.
         */
        const char* stringCode() const;
        /**
         * Sets this to be the boolean set represented by the given string code.
         * See stringCode() for more information on string codes.
         *
         * If \a code is not a value string code, then this routine will
         * do nothing and return \c false.
         *
         * \param code the string code that will determine the new value
         * of this set.
         * \return \c true if and only if \c code is a valid string code.
         */
        bool setStringCode(const std::string& code);

    friend std::ostream& operator << (std::ostream& out, BoolSet set);
};

/**
 * Writes the given boolean set to the given output stream.
 * The set will be written in the form
 * `{ true, false }`, `{ true }`, `{ false }` or `{ }`.
 *
 * \param out the output stream to which to write.
 * \param set the boolean set to write.
 * \return a reference to \a out.
 *
 * \ingroup utilities
 */
std::ostream& operator << (std::ostream& out, BoolSet set);

// Inline functions for BoolSet

inline constexpr BoolSet::BoolSet() : elements(0) {
}
inline constexpr BoolSet::BoolSet(bool member) :
        elements(member ? eltTrue : eltFalse) {
}
inline constexpr BoolSet::BoolSet(bool insertTrue, bool insertFalse) :
        elements(0) {
    if (insertTrue)
        elements = static_cast<unsigned char>(elements | eltTrue);
    if (insertFalse)
        elements = static_cast<unsigned char>(elements | eltFalse);
}

inline constexpr bool BoolSet::hasTrue() const {
    return (elements & eltTrue);
}
inline constexpr bool BoolSet::hasFalse() const {
    return (elements & eltFalse);
}
inline constexpr bool BoolSet::contains(bool value) const {
    return (elements & (value ? eltTrue : eltFalse));
}
inline constexpr bool BoolSet::full() const {
    return (elements == (eltTrue | eltFalse));
}

inline void BoolSet::insertTrue() {
    elements = static_cast<unsigned char>(elements | eltTrue);
}
inline void BoolSet::insertFalse() {
    elements = static_cast<unsigned char>(elements | eltFalse);
}
inline void BoolSet::removeTrue() {
    elements = static_cast<unsigned char>(elements & eltFalse);
}
inline void BoolSet::removeFalse() {
    elements = static_cast<unsigned char>(elements & eltTrue);
}
inline void BoolSet::clear() {
    elements = 0;
}
inline void BoolSet::fill() {
    elements = static_cast<unsigned char>(eltTrue | eltFalse);
}

inline constexpr std::partial_ordering BoolSet::operator <=> (BoolSet rhs)
        const {
    if (elements == rhs.elements)
        return std::partial_ordering::equivalent;
    else if ((elements & rhs.elements) == elements)
        return std::partial_ordering::less;
    else if ((elements & rhs.elements) == rhs.elements)
        return std::partial_ordering::greater;
    else
        return std::partial_ordering::unordered;
}

inline BoolSet& BoolSet::operator = (bool member) {
    elements = (member ? eltTrue : eltFalse);
    return *this;
}
inline BoolSet& BoolSet::operator |= (BoolSet other) {
    elements = static_cast<unsigned char>(elements | other.elements);
    return *this;
}
inline BoolSet& BoolSet::operator &= (BoolSet other) {
    elements = static_cast<unsigned char>(elements & other.elements);
    return *this;
}
inline BoolSet& BoolSet::operator ^= (BoolSet other) {
    elements = static_cast<unsigned char>(elements ^ other.elements);
    return *this;
}

inline constexpr BoolSet BoolSet::operator | (BoolSet other) const {
    BoolSet ans;
    ans.elements = static_cast<unsigned char>(elements | other.elements);
    return ans;
}
inline constexpr BoolSet BoolSet::operator & (BoolSet other) const {
    BoolSet ans;
    ans.elements = static_cast<unsigned char>(elements & other.elements);
    return ans;
}
inline constexpr BoolSet BoolSet::operator ^ (BoolSet other) const {
    BoolSet ans;
    ans.elements = static_cast<unsigned char>(elements ^ other.elements);
    return ans;
}
inline constexpr BoolSet BoolSet::operator ~ () const {
    return BoolSet(! hasTrue(), ! hasFalse());
}

inline constexpr unsigned char BoolSet::byteCode() const {
    return elements;
}
inline bool BoolSet::setByteCode(unsigned char code) {
    if (code < 4) {
        elements = code;
        return true;
    } else
        return false;
}
inline constexpr BoolSet BoolSet::fromByteCode(unsigned char code) {
    return BoolSet(code & eltTrue, code & eltFalse);
}
inline const char* BoolSet::stringCode() const {
    return stringCodes[elements];
}

} // namespace regina

#endif

