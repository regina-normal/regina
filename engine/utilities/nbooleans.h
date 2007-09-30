
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

/*! \file nbooleans.h
 *  \brief Provides various types that extend the standard boolean.
 */

#ifndef __NBOOLEANS_H
#ifndef __DOXYGEN
#define __NBOOLEANS_H
#endif

#include <iostream>

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * A three-way extension of the boolean type.
 * Three-way booleans can be true, false or unknown.
 */
class NTriBool {
    private:
        enum Code { codeTrue = 1, codeFalse = -1, codeUnknown = 0 };
            /**< Internal codes for the three possible values. */

        Code code;
            /**< The internal code for this particular three-way boolean. */

    public:
        static const NTriBool True;
            /**< A global instance of the true value. */
        static const NTriBool False;
            /**< A global instance of the false value. */
        static const NTriBool Unknown;
            /**< A global instance of the unknown value. */

    public:
        /**
         * Creates a new three-way boolean initialised to the unknown value.
         */
        NTriBool();
        /**
         * Creates a copy of the given three-way boolean.
         *
         * @param cloneMe the three-way boolean to clone.
         */
        NTriBool(const NTriBool& cloneMe);
        /**
         * Creates a new three-way boolean whose initial value is the
         * given standard boolean.
         *
         * @param value the value (true or false) to assign to the new
         * three-way boolean.
         */
        NTriBool(bool value);

        /**
         * Is the value of this three-way boolean equal to true?
         *
         * @return whether or not this object has the value of true.
         */
        bool isTrue() const;
        /**
         * Is the value of this three-way boolean equal to false?
         *
         * @return whether or not this object has the value of false.
         */
        bool isFalse() const;
        /**
         * Is the value of this three-way boolean equal to unknown?
         *
         * @return whether or not this object has the value of unknown.
         */
        bool isUnknown() const;
        /**
         * Does this three-way boolean take one of the explicit values
         * true or false?
         *
         * Note that this routine is the negation of isUnknown().
         *
         * @return whether or not this object has an explicit true or
         * false value.
         */
        bool isKnown() const;

        /**
         * Sets this three-way boolean to true.
         */
        void setTrue();
        /**
         * Sets this three-way boolean to false.
         */
        void setFalse();
        /**
         * Sets this three-way boolean to unknown.
         */
        void setUnknown();

        /**
         * Determines whether this and the given three-way boolean are equal.
         * This routine makes a straightforward comparison of states.
         * That is, true is equal to true, false is equal to false and
         * unknown is equal to unknown.
         *
         * @param other the three-way boolean to compare with this.
         * @return \c true if and only if this and the given value are
         * equal.
         */
        bool operator == (const NTriBool& other) const;
        /**
         * Determines whether this and the given standard boolean are equal.
         * This routine makes a straightforward comparison of states.
         * That is, true is equal to true and false is equal to false.
         * Unknown is equal to neither true nor false.
         *
         * \ifacespython Not present, to avoid accidental integer
         * conversions in Python.
         *
         * @param other the standard boolean to compare with this.
         * @return \c true if and only if this and the given value are
         * equal.
         */
        bool operator == (bool other) const;
        /**
         * Determines whether this and the given three-way boolean are
         * different.
         * This routine makes a straightforward comparison of states.
         * That is, true is equal to true, false is equal to false and
         * unknown is equal to unknown.
         *
         * @param other the three-way boolean to compare with this.
         * @return \c true if and only if this and the given value are
         * different.
         */
        bool operator != (const NTriBool& other) const;
        /**
         * Determines whether this and the given standard boolean are
         * different.
         * This routine makes a straightforward comparison of states.
         * That is, true is equal to true and false is equal to false.
         * Unknown is equal to neither true nor false.
         *
         * \ifacespython Not present, to avoid accidental integer
         * conversions in Python.
         *
         * @param other the standard boolean to compare with this.
         * @return \c true if and only if this and the given value are
         * different.
         */
        bool operator != (bool other) const;
        /**
         * Sets this three-way boolean to be identical to the given
         * three-way boolean.
         *
         * @param cloneMe the value to assign to this object.
         * @return a reference to this object.
         */
        NTriBool& operator = (const NTriBool& cloneMe);
        /**
         * Sets this three-way boolean to be identical to the given
         * standard boolean.
         *
         * \ifacespython Not present, to avoid accidental integer
         * conversions in Python.
         *
         * @param cloneMe the value to assign to this object.
         * @return a reference to this object.
         */
        NTriBool& operator = (bool cloneMe);
        /**
         * Sets this to be the disjunction of this and the given value.
         * Unknown is handled in the intuitive way.  For instance,
         * (unknown or true) is true, and (unknown or false) is unknown.
         *
         * @param other the value to combine with this value.
         * @return a reference to this object.
         */
        NTriBool& operator |= (const NTriBool& other);
        /**
         * Sets this to be the disjunction of this and the given value.
         * Unknown is handled in the intuitive way.  For instance,
         * (unknown or true) is true, and (unknown or false) is unknown.
         *
         * \ifacespython Not present, to avoid accidental integer
         * conversions in Python.
         *
         * @param other the value to combine with this value.
         * @return a reference to this object.
         */
        NTriBool& operator |= (bool other);
        /**
         * Sets this to be the conjunction of this and the given value.
         * Unknown is handled in the intuitive way.  For instance,
         * (unknown and true) is unknown, and (unknown and false) is false.
         *
         * @param other the value to combine with this value.
         * @return a reference to this object.
         */
        NTriBool& operator &= (const NTriBool& other);
        /**
         * Sets this to be the conjunction of this and the given value.
         * Unknown is handled in the intuitive way.  For instance,
         * (unknown and true) is unknown, and (unknown and false) is false.
         *
         * \ifacespython Not present, to avoid accidental integer
         * conversions in Python.
         *
         * @param other the value to combine with this value.
         * @return a reference to this object.
         */
        NTriBool& operator &= (bool other);
        /**
         * Returns the disjunction of this and the given value.
         * Unknown is handled in the intuitive way.  For instance,
         * (unknown or true) is true, and (unknown or false) is unknown.
         *
         * @param other the value to combine with this value.
         * @return the disjunction of this and the given value.
         */
        NTriBool operator | (const NTriBool& other) const;
        /**
         * Returns the disjunction of this and the given value.
         * Unknown is handled in the intuitive way.  For instance,
         * (unknown or true) is true, and (unknown or false) is unknown.
         *
         * \ifacespython Not present, to avoid accidental integer
         * conversions in Python.
         *
         * @param other the value to combine with this value.
         * @return the disjunction of this and the given value.
         */
        NTriBool operator | (bool other) const;
        /**
         * Returns the conjunction of this and the given value.
         * Unknown is handled in the intuitive way.  For instance,
         * (unknown and true) is unknown, and (unknown and false) is false.
         *
         * @param other the value to combine with this value.
         * @return the conjunction of this and the given value.
         */
        NTriBool operator & (const NTriBool& other) const;
        /**
         * Returns the conjunction of this and the given value.
         * Unknown is handled in the intuitive way.  For instance,
         * (unknown and true) is unknown, and (unknown and false) is false.
         *
         * \ifacespython Not present, to avoid accidental integer
         * conversions in Python.
         *
         * @param other the value to combine with this value.
         * @return the conjunction of this and the given value.
         */
        NTriBool operator & (bool other) const;
        /**
         * Returns the negation of this value.
         * The negation of unknown is unknown.
         *
         * @return the negation of this value.
         */
        NTriBool operator ~ () const;

    private:
        /**
         * Creates a new three-way boolean corresponding to the given internal
         * code.
         *
         * @param newCode the internal code representing the new three-way
         * boolean.
         */
        explicit NTriBool(Code newCode);
        /**
         * A private routine that should never be used.
         * This routine is defined so that illegal integer operations
         * don't automatically convert to boolean operations.
         */
        NTriBool(int);
        /**
         * A private routine that should never be used.
         * This routine is defined so that illegal integer operations
         * don't automatically convert to boolean operations.
         */
        bool operator == (int) const;
        /**
         * A private routine that should never be used.
         * This routine is defined so that illegal integer operations
         * don't automatically convert to boolean operations.
         */
        bool operator != (int) const;
        /**
         * A private routine that should never be used.
         * This routine is defined so that illegal integer operations
         * don't automatically convert to boolean operations.
         */
        NTriBool& operator = (int);
        /**
         * A private routine that should never be used.
         * This routine is defined so that illegal integer operations
         * don't automatically convert to boolean operations.
         */
        NTriBool& operator |= (int);
        /**
         * A private routine that should never be used.
         * This routine is defined so that illegal integer operations
         * don't automatically convert to boolean operations.
         */
        NTriBool& operator &= (int);
        /**
         * A private routine that should never be used.
         * This routine is defined so that illegal integer operations
         * don't automatically convert to boolean operations.
         */
        NTriBool operator | (int) const;
        /**
         * A private routine that should never be used.
         * This routine is defined so that illegal integer operations
         * don't automatically convert to boolean operations.
         */
        NTriBool operator & (int) const;
};

/**
 * Writes the given three-way boolean to the given output stream.
 * The value will be written in the form
 * <tt>true</tt>, <tt>false</tt> or <tt>unknown</tt>.
 *
 * @param out the output stream to which to write.
 * @param set the three-way boolean to write.
 * @return a reference to \a out.
 */
std::ostream& operator << (std::ostream& out, const NTriBool& set);

/**
 * An exception thrown when an illegal integer conversion is attempted
 * with NTriBool.
 */
class NTriBool_Illegal_Integer_Conversion {
};

/**
 * A set of booleans.  Note that there are only four possible such sets.
 * NBoolSet objects are small enough to pass about by value instead of
 * by reference.
 */
class NBoolSet {
    private:
        unsigned char elements;
            /**< The first two bits of this character represent whether
                 or not \c true or \c false belongs to this set. */

        static const unsigned char eltTrue;
            /**< A character with only the \c true member bit set. */
        static const unsigned char eltFalse;
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
         * @param other the set to compare with this.
         * @return \c true if and only if this and the given set are
         * equal.
         */
        bool operator == (const NBoolSet& other) const;
        /**
         * Determines if this set is not equal to the given set.
         *
         * @param other the set to compare with this.
         * @return \c true if and only if this and the given set are
         * not equal.
         */
        bool operator != (const NBoolSet& other) const;
        /**
         * Determines if this set is a proper subset of the given set.
         *
         * @param other the set to compare with this.
         * @return \c true if and only if this is a proper subset of the
         * given set.
         */
        bool operator < (const NBoolSet& other) const;
        /**
         * Determines if this set is a proper superset of the given set.
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
         * @param other the set to compare with this.
         * @return \c true if and only if this is a subset of the
         * given set.
         */
        bool operator <= (const NBoolSet& other) const;
        /**
         * Determines if this set is a superset of (possibly equal to)
         * the given set.
         *
         * @param other the set to compare with this.
         * @return \c true if and only if this is a superset of the
         * given set.
         */
        bool operator >= (const NBoolSet& other) const;

        /**
         * Sets this set to be identical to the given set.
         *
         * @param cloneMe the set whose value this set will take.
         * @return a reference to this set.
         */
        NBoolSet& operator = (const NBoolSet& cloneMe);
        /**
         * Sets this set to the single member set containing the given
         * element.
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
        unsigned char getByteCode() const;
        /**
         * Sets this boolean set to that represented by the given byte
         * code.  See getByteCode() for more information on byte codes.
         *
         * \pre \a code is 0, 1, 2 or 3.
         *
         * @param code the byte code that will determine the new value
         * of this set.
         */
        void setByteCode(unsigned char code);
        /**
         * Creates a boolean set from the given byte code.
         * See getByteCode() for more information on byte codes.
         *
         * \pre \a code is 0, 1, 2 or 3.
         *
         * @param code the byte code from which the new set will be
         * created.
         */
        static NBoolSet fromByteCode(unsigned char code);

    friend std::ostream& operator << (std::ostream& out, const NBoolSet& set);
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
std::ostream& operator << (std::ostream& out, const NBoolSet& set);

/*@}*/

// Inline functions for NTriBool

inline NTriBool::NTriBool() : code(codeUnknown) {
}

inline NTriBool::NTriBool(const NTriBool& cloneMe) : code(cloneMe.code) {
}

inline NTriBool::NTriBool(bool value) : code(value ? codeTrue : codeFalse) {
}

inline bool NTriBool::isTrue() const {
    return (code == codeTrue);
}

inline bool NTriBool::isFalse() const {
    return (code == codeFalse);
}

inline bool NTriBool::isUnknown() const {
    return (code == codeUnknown);
}

inline bool NTriBool::isKnown() const {
    return (code != codeUnknown);
}

inline void NTriBool::setTrue() {
    code = codeTrue;
}

inline void NTriBool::setFalse() {
    code = codeFalse;
}

inline void NTriBool::setUnknown() {
    code = codeUnknown;
}

inline bool NTriBool::operator == (const NTriBool& other) const {
    return (code == other.code);
}

inline bool NTriBool::operator == (bool other) const {
    return (other && (code == codeTrue)) || ((! other) && (code == codeFalse));
}

inline bool NTriBool::operator != (const NTriBool& other) const {
    return (code != other.code);
}

inline bool NTriBool::operator != (bool other) const {
    return (other && (code != codeTrue)) || ((! other) && (code != codeFalse));
}

inline NTriBool& NTriBool::operator = (const NTriBool& other) {
    code = other.code;
    return *this;
}

inline NTriBool& NTriBool::operator = (bool other) {
    code = (other ? codeTrue : codeFalse);
    return *this;
}

inline NTriBool& NTriBool::operator |= (const NTriBool& other) {
    if (other.code == codeTrue)
        code = codeTrue;
    if (other.code == codeUnknown && code == codeFalse)
        code = codeUnknown;
    return *this;
}

inline NTriBool& NTriBool::operator |= (bool other) {
    if (other)
        code = codeTrue;
    return *this;
}

inline NTriBool& NTriBool::operator &= (const NTriBool& other) {
    if (other.code == codeFalse)
        code = codeFalse;
    if (other.code == codeUnknown && code == codeTrue)
        code = codeUnknown;
    return *this;
}

inline NTriBool& NTriBool::operator &= (bool other) {
    if (! other)
        code = codeFalse;
    return *this;
}

inline NTriBool NTriBool::operator | (const NTriBool& other) const {
    if (code == codeTrue || other.code == codeTrue)
        return True;
    if (code == codeFalse && other.code == codeFalse)
        return False;
    return Unknown;
}

inline NTriBool NTriBool::operator | (bool other) const {
    return (other ? True : *this);
}

inline NTriBool NTriBool::operator & (const NTriBool& other) const {
    if (code == codeTrue && other.code == codeTrue)
        return True;
    if (code == codeFalse || other.code == codeFalse)
        return False;
    return Unknown;
}

inline NTriBool NTriBool::operator & (bool other) const {
    return (other ? *this : False);
}

inline NTriBool NTriBool::operator ~ () const {
    switch (code) {
        case codeTrue : return False;
        case codeFalse : return True;
        default : return Unknown;
    }
}

inline NTriBool::NTriBool(Code newCode) :
        code((newCode == codeTrue || newCode == codeFalse) ? newCode :
            codeUnknown) {
}

inline NTriBool::NTriBool(int) {
    throw NTriBool_Illegal_Integer_Conversion();
}

inline bool NTriBool::operator == (int) const {
    throw NTriBool_Illegal_Integer_Conversion();
}

inline bool NTriBool::operator != (int) const {
    throw NTriBool_Illegal_Integer_Conversion();
}

inline NTriBool& NTriBool::operator = (int) {
    throw NTriBool_Illegal_Integer_Conversion();
}

inline NTriBool& NTriBool::operator |= (int) {
    throw NTriBool_Illegal_Integer_Conversion();
}

inline NTriBool& NTriBool::operator &= (int) {
    throw NTriBool_Illegal_Integer_Conversion();
}

inline NTriBool NTriBool::operator | (int) const {
    throw NTriBool_Illegal_Integer_Conversion();
}

inline NTriBool NTriBool::operator & (int) const {
    throw NTriBool_Illegal_Integer_Conversion();
}

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

inline unsigned char NBoolSet::getByteCode() const {
    return elements;
}
inline void NBoolSet::setByteCode(unsigned char code) {
    elements = code;
}
inline NBoolSet NBoolSet::fromByteCode(unsigned char code) {
    return NBoolSet(code & eltTrue, code & eltFalse);
}

} // namespace regina

#endif

