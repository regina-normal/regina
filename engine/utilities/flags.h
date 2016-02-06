
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file utilities/flags.h
 *  \brief A template class for handling bitwise combinations of enum flags.
 */

#ifndef __FLAGS_H
#ifndef __DOXYGEN
#define __FLAGS_H
#endif

namespace regina {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * A class representing a bitwise combination of flags defined by an
 * enumeration type.
 *
 * The enumeration type is given in the template parameter \a T.
 * This class allows the user to form and test bitwise combinations of the
 * individual enum values, without losing type safety.
 *
 * \ifacespython Present only for some particular enumeration types \a T,
 * when explicitly noted in the corresponding enum documentation.
 */
template <typename T>
class Flags {
    public:
        /**
         * The underlying enumeration type.
         */
        typedef T Enum;

    private:
        /**
         * The combination of flags that we are storing.
         */
        int value_;

    public:
        /**
         * Creates an empty flag set, with no flags set at all.
         */
        inline Flags() : value_(0) {
        }

        /**
         * Creates a flag set initialised to the given value.
         *
         * @param init the initial value of this flag set.
         */
        inline Flags(T init) : value_(init) {
        }

        /**
         * Creates a clone of the given flag set.
         *
         * @param init the flag set to clone.
         */
        inline Flags(const Flags<T>& init) : value_(init.value_) {
        }

        /**
         * Returns the integer representation of this set.
         * This is suitable for file input and/or output.
         *
         * \warning This function should not be used widely, since it
         * effectively works around inbuilt type safety mechanisms.
         *
         * @return the integer value of this set.
         */
        int intValue() const {
            return static_cast<int>(value_);
        }

        /**
         * Returns the set corresponding to the given integer value.
         * This is suitable for file input and/or output.
         *
         * \warning This function should not be used widely, since it
         * effectively works around inbuilt type safety mechanisms.
         *
         * @return the set corresponding to the given integer value.
         */
        inline static Flags<T> fromInt(int value) {
            return Flags<T>(value);
        }

        /**
         * Returns whether the given flag is set.
         *
         * This requires \e all of the bits of the given flag to be set.
         * The test is equivalent to <tt>(*this & flag) == flag</tt>.
         *
         * @param flag the flag whose presence will be tested.
         * @return \c true if and only if all of the bits of the given
         * flag are set.
         */
        bool has(T flag) const {
            return (value_ & flag) == flag;
        }

        /**
         * Returns whether all of the flags in the given set are set.
         *
         * This requires \e all of the bits of all of the flags in the
         * given set to be present in this set.
         * The test is equivalent to <tt>(*this & rhs) == rhs</tt>.
         *
         * @param rhs the set whose presence will be tested.
         * @return \c true if and only if all of the bits of the given
         * set are present in this set.
         */
        bool has(const Flags<T>& rhs) const {
            return (value_ & rhs.value_) == rhs.value_;
        }

        /**
         * Determines whether this set is precisely equal to the given flag.
         *
         * @param rhs the flag to test this against.
         * @return \c true if and only if this and the given flag are identical.
         */
        inline bool operator == (T rhs) const {
            return (value_ == rhs);
        }

        /**
         * Determines whether this set is precisely equal to the given
         * flag set.
         *
         * @param rhs the flag set to test this against.
         * @return \c true if and only if this and the given flag set are
         * identical.
         */
        inline bool operator == (const Flags<T>& rhs) const {
            return (value_ == rhs.value_);
        }

        /**
         * Determines whether this set is not equal to the given flag.
         *
         * @param rhs the flag to test this against.
         * @return \c true if and only if this and the given flag are not
         * identical.
         */
        inline bool operator != (T rhs) const {
            return (value_ != rhs);
        }

        /**
         * Determines whether this set is not equal to the given flag set.
         *
         * @param rhs the flag to test this against.
         * @return \c true if and only if this and the given flag set are not
         * identical.
         */
        inline bool operator != (const Flags<T>& rhs) const {
            return (value_ != rhs.value_);
        }

        /**
         * Sets this flag set to contain precisely the given flag only.
         *
         * @param rhs the new value of this flag set.
         * @return a reference to this flag set.
         */
        inline Flags<T>& operator = (T rhs) {
            value_ = rhs;
            return *this;
        }

        /**
         * Sets this flag set to contain a copy of the given flag set.
         *
         * @param rhs the new value of this flag set.
         * @return a reference to this flag set.
         */
        inline Flags<T>& operator = (const Flags<T>& rhs) {
            value_ = rhs.value_;
            return *this;
        }

        /**
         * Changes this flag set by taking a bitwise OR with the given
         * flag.
         *
         * @param rhs the flag to combine with this set.
         * @return a reference to this flag set.
         */
        inline Flags<T>& operator |= (T rhs) {
            value_ |= rhs;
            return *this;
        }

        /**
         * Changes this flag set by taking a bitwise OR with the given
         * flag set.
         *
         * @param rhs the flag set to combine with this set.
         * @return a reference to this flag set.
         */
        inline Flags<T>& operator |= (const Flags<T>& rhs) {
            value_ |= rhs.value_;
            return *this;
        }

        /**
         * Changes this flag set by taking a bitwise AND with the given
         * flag.
         *
         * @param rhs the flag to combine with this set.
         * @return a reference to this flag set.
         */
        inline Flags<T>& operator &= (T rhs) {
            value_ &= rhs;
            return *this;
        }

        /**
         * Changes this flag set by taking a bitwise AND with the given
         * flag set.
         *
         * @param rhs the flag set to combine with this set.
         * @return a reference to this flag set.
         */
        inline Flags<T>& operator &= (const Flags<T>& rhs) {
            value_ &= rhs.value_;
            return *this;
        }

        /**
         * Changes this flag set by taking a bitwise XOR with the given
         * flag.
         *
         * @param rhs the flag to combine with this set.
         * @return a reference to this flag set.
         */
        inline Flags<T>& operator ^= (T rhs) {
            value_ ^= rhs;
            return *this;
        }

        /**
         * Changes this flag set by taking a bitwise XOR with the given
         * flag set.
         *
         * @param rhs the flag set to combine with this set.
         * @return a reference to this flag set.
         */
        inline Flags<T>& operator ^= (const Flags<T>& rhs) {
            value_ ^= rhs.value_;
            return *this;
        }

        /**
         * Returns the bitwise OR of this set and the given flag.
         * This flag set is not changed.
         *
         * @param rhs the flag to combine with this set.
         * @return the combination of this set and the given flag.
         */
        inline Flags<T> operator | (T rhs) const {
            return Flags(value_ | rhs);
        }

        /**
         * Returns the bitwise OR of this and the given flag set.
         * This flag set is not changed.
         *
         * @param rhs the flag set to combine with this set.
         * @return the combination of this and the given flag set.
         */
        inline Flags<T> operator | (const Flags<T>& rhs) const {
            return Flags(value_ | rhs.value_);
        }

        /**
         * Returns the bitwise AND of this set and the given flag.
         * This flag set is not changed.
         *
         * @param rhs the flag to combine with this set.
         * @return the combination of this set and the given flag.
         */
        inline Flags<T> operator & (T rhs) const {
            return Flags(value_ & rhs);
        }

        /**
         * Returns the bitwise AND of this and the given flag set.
         * This flag set is not changed.
         *
         * @param rhs the flag set to combine with this set.
         * @return the combination of this and the given flag set.
         */
        inline Flags<T> operator & (const Flags<T>& rhs) const {
            return Flags(value_ & rhs.value_);
        }

        /**
         * Returns the bitwise XOR of this set and the given flag.
         * This flag set is not changed.
         *
         * @param rhs the flag to combine with this set.
         * @return the combination of this set and the given flag.
         */
        inline Flags<T> operator ^ (T rhs) const {
            return Flags(value_ ^ rhs);
        }

        /**
         * Returns the bitwise XOR of this and the given flag set.
         * This flag set is not changed.
         *
         * @param rhs the flag set to combine with this set.
         * @return the combination of this and the given flag set.
         */
        inline Flags<T> operator ^ (const Flags<T>& rhs) const {
            return Flags(value_ ^ rhs.value_);
        }

        /**
         * Clears all bits from this set that appear in the given flag.
         *
         * @param rhs the flag to clear from this set.
         */
        inline void clear(T rhs) {
            value_ |= rhs;
            value_ ^= rhs;
        }

        /**
         * Clears all bits from this set that appear in the given set.
         *
         * @param rhs identifies the bits to clear from this set.
         */
        inline void clear(const Flags<T>& rhs) {
            value_ |= rhs.value_;
            value_ ^= rhs.value_;
        }

        /**
         * Adjust this set so that exactly one and only one of the two
         * given flags are included.
         *
         * If neither flag is present or both flags are present, this
         * set will be adjusted so that \a default_ is present and \a other
         * is not.
         *
         * \pre Both \a default_ and \a other are each single-bit flags.
         *
         * \ifacespython Not present, even for flag types that are
         * exposed to Python.
         *
         * @param default_ the flag that will be set if any adjustments
         * need to be made.
         * @param other the flag that will be cleared if any adjustments
         * need to be made.
         */
        inline void ensureOne(T default_, T other) {
            if (! ((value_ & default_) || (value_ & other)))
                value_ |= default_;
            else if ((value_ & default_) && (value_ & other))
                value_ ^= other;
        }

        /**
         * Adjust this set so that exactly one and only one of the three
         * given flags are included.
         *
         * If neither flag is present, then \a default_ will be used.
         * If multiple flags are present, then the flag that appears
         * \e earlier in the argument list for this routine will be used.
         *
         * \pre Each of the given flags is single-bit.
         *
         * \ifacespython Not present, even for flag types that are
         * exposed to Python.
         *
         * @param default_ the highest-priority flag.
         * @param second the second-highest-priority flag.
         * @param last the lowest-priority flag.
         */
        inline void ensureOne(T default_, T second, T last) {
            // Cast to int, because (T | T) is overloaded to return Flags<T>.
            if (! (value_ & (static_cast<int>(default_) | second | last)))
                value_ = default_;
            else if (value_ & default_)
                clear(second | last);
            else if (value_ & second)
                clear(last);
        }

        /**
         * Adjust this set so that exactly one and only one of the four
         * given flags are included.
         *
         * If neither flag is present, then \a default_ will be used.
         * If multiple flags are present, then the flag that appears
         * \e earlier in the argument list for this routine will be used.
         *
         * \pre Each of the given flags is single-bit.
         *
         * \ifacespython Not present, even for flag types that are
         * exposed to Python.
         *
         * @param default_ the highest-priority flag.
         * @param second the second-highest-priority flag.
         * @param third the third-highest-priority flag.
         * @param last the lowest-priority flag.
         */
        inline void ensureOne(T default_, T second, T third, T last) {
            // Cast to int, because (T | T) is overloaded to return Flags<T>.
            if (! (value_ & (static_cast<int>(default_) | second | third | last)))
                value_ = default_;
            else if (value_ & default_)
                clear(second | third | last);
            else if (value_ & second)
                clear(third | last);
            else if (value_ & third)
                clear(last);
        }

    private:
        /**
         * Constructs a new flag set with the given internal value.
         *
         * @param init the new internal value.
         */
        inline Flags(int init) : value_(init) {
        }
};

/*@}*/

} // namespace regina

#endif
