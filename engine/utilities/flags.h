
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

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
         * Determines whether this set is precisely equal to the given flag.
         *
         * @param rhs the flag to test this against.
         * @param \c true if and only if this and the given flag are identical.
         */
        inline bool operator == (T rhs) const {
            return (value_ == rhs);
        }

        /**
         * Determines whether this set is precisely equal to the given
         * flag set.
         *
         * @param rhs the flag set to test this against.
         * @param \c true if and only if this and the given flag set are
         * identical.
         */
        inline bool operator == (const Flags<T>& rhs) const {
            return (value_ == rhs.value_);
        }

        /**
         * Determines whether this set is not equal to the given flag.
         *
         * @param rhs the flag to test this against.
         * @param \c true if and only if this and the given flag are not
         * identical.
         */
        inline bool operator != (T rhs) const {
            return (value_ != rhs);
        }

        /**
         * Determines whether this set is not equal to the given flag set.
         *
         * @param rhs the flag to test this against.
         * @param \c true if and only if this and the given flag set are not
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
