
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

/*! \file nbooleans.h
 *  \brief Provides various types that extend the standard boolean.
 */

#ifndef __NBOOLEANS_H
#ifndef __DOXYGEN
#define __NBOOLEANS_H
#endif

class ostream;

/**
 * A set of booleans.  Note that there are only four possible such sets.
 * NBoolSet objects are small enough to pass about by value instead of
 * by reference.
 *
 * \ifacescorba Not present.
 */
class NBoolSet {
    private:
        char elements;
            /**< The first two bits of this character represent whether
             *   or not \c true or \c false belongs to this set. */

        static const char eltTrue = 1;
            /**< A character with only the \c true member bit set. */
        static const char eltFalse = 2;
            /**< A character with only the \c false member bit set. */

    public:
        static const NBoolSet sNone;
            /**< The empty set. */
        static const NBoolSet sTrue;
            /**< The set containing only \c true. */
        static const NBoolSet sFalse;
            /**< The set containing only \c false. */
        static const NBoolSet sBoth;
            /**< The set containing both \c true and \c false. */

    public:
        /**
         * Creates a new empty set.
         */
        NBoolSet();
        /**
         * Creates a set containing a single member as given.
         *
         * @param member the single element to include in this set.
         */
        NBoolSet(bool member);
        /**
         * Creates a set equal to the given set.
         *
         * @param cloneMe the set upon which we will base the new set.
         */
        NBoolSet(const NBoolSet& cloneMe);
        /**
         * Creates a set specifying whether \c true and/or \c false
         * should be a member.
         *
         * @param insertTrue should the new set include the element
         * <tt>true</tt>?
         * @param insertFalse should the new set include the element
         * <tt>false</tt>?
         */
        NBoolSet(bool insertTrue, bool insertFalse);

        /**
         * Determines if \c true is a member of this set.
         *
         * @return \c true if and only if \c true is a member of this
         * set.
         */
        bool hasTrue() const;
        /**
         * Determines if \c false is a member of this set.
         *
         * @return \c true if and only if \c false is a member of this
         * set.
         */
        bool hasFalse() const;
        /**
         * Determines if the given boolean is a member of this set.
         *
         * @param value the boolean to search for in this set.
         * @return \c true if and only if the given boolean is a member
         * of this set.
         */
        bool contains(bool value) const;

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
         */
        void empty();
        /**
         * Places both \c true and \c false into this set if they are
         * not already present.
         */
        void fill();

        /**
         * Determines if this set is equal to the given set.
         *
         * \ifaces This routine is called \a sameSet.
         *
         * @param other the set to compare with this.
         * @return \c true if and only if this and the given set are
         * equal.
         */
        bool operator == (const NBoolSet& other) const;
        /**
         * Determines if this set is not equal to the given set.
         *
         * \ifaces This routine is called \a notSameSet.
         *
         * @param other the set to compare with this.
         * @return \c true if and only if this and the given set are
         * not equal.
         */
        bool operator != (const NBoolSet& other) const;
        /**
         * Determines if this set is a proper subset of the given set.
         *
         * \ifaces This routine is called \a properSubsetOf.
         *
         * @param other the set to compare with this.
         * @return \c true if and only if this is a proper subset of the
         * given set.
         */
        bool operator < (const NBoolSet& other) const;
        /**
         * Determines if this set is a proper superset of the given set.
         *
         * \ifaces This routine is called \a properSupersetOf.
         *
         * @param other the set to compare with this.
         * @return \c true if and only if this is a proper superset of the
         * given set.
         */
        bool operator > (const NBoolSet& other) const;
        /**
         * Determines if this set is a subset of (possibly equal to)
         * the given set.
         *
         * \ifaces This routine is called \a subsetOf.
         *
         * @param other the set to compare with this.
         * @return \c true if and only if this is a subset of the
         * given set.
         */
        bool operator <= (const NBoolSet& other) const;
        /**
         * Determines if this set is a superset of (possibly equal to)
         * the given set.
         *
         * \ifaces This routine is called \a supersetOf.
         *
         * @param other the set to compare with this.
         * @return \c true if and only if this is a superset of the
         * given set.
         */
        bool operator >= (const NBoolSet& other) const;

        /**
         * Sets this set to be identical to the given set.
         *
         * \ifaces This routine is called \a setTo.
         *
         * @param cloneMe the set whose value this set will take.
         * @return a reference to this set.
         */
        NBoolSet& operator = (const NBoolSet& cloneMe);
        /**
         * Sets this set to the single member set containing the given
         * element.
         *
         * \ifaces This routine is called \a setTo.
         *
         * @param member the single element to include in this set.
         * @return a reference to this set.
         */
        NBoolSet& operator = (bool member);
        /**
         * Sets this set to be the union of this and the given set.
         * The result is a set containing precisely the elements that
         * belong to either of the original sets.
         * Note that this set will be modified.
         *
         * \ifaces Not present.
         *
         * @param other the set to union with this set.
         * @return a reference to this set.
         */
        NBoolSet& operator |= (const NBoolSet& other);
        /**
         * Sets this set to be the intersection of this and the given set.
         * The result is a set containing precisely the elements that
         * belong to both original sets.
         * Note that this set will be modified.
         *
         * \ifaces Not present.
         *
         * @param other the set to intersect with this set.
         * @return a reference to this set.
         */
        NBoolSet& operator &= (const NBoolSet& other);
        /**
         * Sets this set to be the symmetric difference of this and the
         * given set.
         * The result is a set containing precisely the elements that
         * belong to one but not both of the original sets.
         * Note that this set will be modified.
         *
         * \ifaces Not present.
         *
         * @param other the set whose symmetric difference with this set
         * is to be found.
         * @return a reference to this set.
         */
        NBoolSet& operator ^= (const NBoolSet& other);

        /**
         * Returns the union of this set with the given set.
         * The result is a set containing precisely the elements that
         * belong to either of the original sets.
         * This set is not changed.
         *
         * \ifaces This routine is called \a formUnion.
         *
         * @param other the set to union with this set.
         * @return the union of this and the given set.
         */
        NBoolSet operator | (const NBoolSet& other) const;
        /**
         * Returns the intersection of this set with the given set.
         * The result is a set containing precisely the elements that
         * belong to both original sets.
         * This set is not changed.
         *
         * \ifaces This routine is called \a formIntersection.
         *
         * @param other the set to intersect with this set.
         * @return the intersection of this and the given set.
         */
        NBoolSet operator & (const NBoolSet& other) const;
        /**
         * Returns the symmetric difference of this set and the given set.
         * The result is a set containing precisely the elements that
         * belong to one but not both of the original sets.
         * This set is not changed.
         *
         * \ifaces This routine is called \a formSymmDiff.
         *
         * @param other the set whose symmetric difference with this set
         * is to be found.
         * @return the symmetric difference of this and the given set.
         */
        NBoolSet operator ^ (const NBoolSet& other) const;
        /**
         * Returns the complement of this set.
         * The result is a set containing precisely the elements that
         * this set does not contain.
         * This set is not changed.
         *
         * \ifaces This routine is called \a formComplement.
         *
         * @return the complement of this set.
         */
        NBoolSet operator ~ () const;

		/**
		 * Returns the byte code representing this boolean set.
		 * The byte code is sufficient to reconstruct the set
		 * and is thus a useful means for passing boolean sets to and
		 * from the engine.
		 *
		 * The lowest order bit of the byte code is 1 if and only if
		 * \c true is in the set.  The next lowest order bit is 1 if and
		 * only if \c false is in the set.  All other bits are 0.
		 * Thus sets NBoolSet::sNone, NBoolSet::sTrue, NBoolSet::sFalse
		 * and NBoolSet::sBoth have byte codes 0, 1, 2 and 3 respectively.
		 *
		 * @return the byte code representing this set.
		 */
		char getByteCode() const;
		/**
		 * Sets this boolean set to that represented by the given byte
		 * code.  See getByteCode() for more information on byte codes.
		 *
		 * \pre \a code is 0, 1, 2 or 3.
		 *
		 * @param code the byte code that will determine the new value
		 * of this set.
		 */
		void setByteCode(char code);
		/**
		 * Creates a boolean set from the given byte code.
		 * See getByteCode() for more information on byte codes.
		 *
		 * \pre \a code is 0, 1, 2 or 3.
		 *
		 * @param code the byte code from which the new set will be
		 * created.
		 */
		static NBoolSet fromByteCode(char code);

    friend ostream& operator << (ostream& out, const NBoolSet& set);
};

/**
 * Writes the given boolean set to the given output stream.
 * The set will be written in the form
 * <tt>{ true, false }</tt>, <tt>{ true }</tt>,
 * <tt>{ false }</tt> or <tt>{ }</tt>.
 *
 * @param out the output stream to which to write.
 * @param set the boolean set to write.
 * @return a reference to \a out.
 */
ostream& operator << (ostream& out, const NBoolSet& set);

// Inline functions for NBoolSet

inline NBoolSet::NBoolSet() : elements(0) {
}
inline NBoolSet::NBoolSet(bool member) :
        elements(member ? eltTrue : eltFalse) {
}
inline NBoolSet::NBoolSet(const NBoolSet& cloneMe) :
        elements(cloneMe.elements) {
}
inline NBoolSet::NBoolSet(bool insertTrue, bool insertFalse) : elements(0) {
    if (insertTrue)
        elements |= eltTrue;
    if (insertFalse)
        elements |= eltFalse;
}

inline bool NBoolSet::hasTrue() const {
    return (elements & eltTrue);
}
inline bool NBoolSet::hasFalse() const {
    return (elements & eltFalse);
}
inline bool NBoolSet::contains(bool value) const {
    return (elements & (value ? eltTrue : eltFalse));
}

inline void NBoolSet::insertTrue() {
    elements |= eltTrue;
}
inline void NBoolSet::insertFalse() {
    elements |= eltFalse;
}
inline void NBoolSet::removeTrue() {
    elements &= eltFalse;
}
inline void NBoolSet::removeFalse() {
    elements &= eltTrue;
}
inline void NBoolSet::empty() {
    elements = 0;
}
inline void NBoolSet::fill() {
    elements = eltTrue | eltFalse;
}

inline bool NBoolSet::operator == (const NBoolSet& other) const {
    return (elements == other.elements);
}
inline bool NBoolSet::operator != (const NBoolSet& other) const {
    return (elements != other.elements);
}
inline bool NBoolSet::operator < (const NBoolSet& other) const {
    return ((elements & other.elements) == elements) &&
        (elements != other.elements);
}
inline bool NBoolSet::operator > (const NBoolSet& other) const {
    return ((elements & other.elements) == other.elements) &&
        (elements != other.elements);
}
inline bool NBoolSet::operator <= (const NBoolSet& other) const {
    return ((elements & other.elements) == elements);
}
inline bool NBoolSet::operator >= (const NBoolSet& other) const {
    return ((elements & other.elements) == other.elements);
}

inline NBoolSet& NBoolSet::operator = (const NBoolSet& cloneMe) {
    elements = cloneMe.elements;
    return *this;
}
inline NBoolSet& NBoolSet::operator = (bool member) {
    elements = (member ? eltTrue : eltFalse);
    return *this;
}
inline NBoolSet& NBoolSet::operator |= (const NBoolSet& other) {
    elements |= other.elements;
    return *this;
}
inline NBoolSet& NBoolSet::operator &= (const NBoolSet& other) {
    elements &= other.elements;
    return *this;
}
inline NBoolSet& NBoolSet::operator ^= (const NBoolSet& other) {
    elements ^= other.elements;
    return *this;
}

inline NBoolSet NBoolSet::operator | (const NBoolSet& other) const {
    NBoolSet ans;
    ans.elements = elements | other.elements;
    return ans;
}
inline NBoolSet NBoolSet::operator & (const NBoolSet& other) const {
    NBoolSet ans;
    ans.elements = elements & other.elements;
    return ans;
}
inline NBoolSet NBoolSet::operator ^ (const NBoolSet& other) const {
    NBoolSet ans;
    ans.elements = elements ^ other.elements;
    return ans;
}
inline NBoolSet NBoolSet::operator ~ () const {
    return NBoolSet(! hasTrue(), ! hasFalse());
}

inline char NBoolSet::getByteCode() const {
	return elements;
}
inline void NBoolSet::setByteCode(char code) {
	elements = code;
}
inline NBoolSet NBoolSet::fromByteCode(char code) {
	return NBoolSet(code & eltTrue, code & eltFalse);
}

#endif

