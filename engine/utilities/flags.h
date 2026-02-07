
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

/*! \file utilities/flags.h
 *  \brief A template class for handling bitwise combinations of enum flags.
 */

#ifndef __REGINA_FLAGS_H
#ifndef __DOXYGEN
#define __REGINA_FLAGS_H
#endif

#include <concepts>
#include <type_traits>
#include "regina-core.h"

namespace regina {

/**
 * A class representing a bitwise combination of flags defined by an
 * enumeration type.
 *
 * The enumeration type is given in the template parameter \a T.
 * This class allows the user to form and test bitwise combinations of the
 * individual enum values, without losing type safety.
 *
 * There is usually no need for end users to refer to the type `Flags<T>`
 * explicitly by name.  If a function takes an argument of type `Flags<T>`,
 * then you can pass a single flag of type \a T, or a bitwise combination of
 * such flags `(flag1 | flag2)`, or empty braces `{}` to indicate no flags
 * at all.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \python Present only for some particular enumeration types \a T,
 * when explicitly noted in the corresponding enum documentation.
 * The Python name of the flags class will be the enumeration type name with
 * the prefix `Flags_`; for instance, the C++ type `Flags<NormalAlg>` will
 * appear in Python as `Flags_NormalAlg`.
 *
 * \tparam T the enumeration type holding the individual flags that can be
 * combined.  This may be a scoped or unscoped enumeration, based upon any
 * underlying native C++ integer type.
 *
 * \ingroup utilities
 */
template <typename T>
requires std::is_enum_v<T>
class Flags {
    public:
        /**
         * The underlying enumeration type used to represent individual flags.
         */
        using Enum = T;

        /**
         * The native C++ integer type used to store individual flags.
         */
        using BaseInt = std::underlying_type_t<T>;

    private:
        /**
         * The combination of flags that we are storing.
         */
        BaseInt value_;

    public:
        /**
         * Creates an empty flag set, with no flags set at all.
         */
        constexpr Flags() : value_(0) {
        }

        /**
         * Creates a flag set initialised to the given value.
         *
         * \param init the initial value of this flag set.
         */
        constexpr Flags(T init) : value_(static_cast<BaseInt>(init)) {
        }

        /**
         * Creates a clone of the given flag set.
         */
        constexpr Flags(const Flags<T>&) = default;

        /**
         * Returns the native C++ integer representation of this set.
         * This is suitable for file input and/or output.
         *
         * \warning This function should not be used widely, since it
         * effectively works around inbuilt type safety mechanisms.
         *
         * \return the native C++ integer value of this set.
         */
        constexpr BaseInt baseValue() const {
            return static_cast<BaseInt>(value_);
        }

        /**
         * Deprecated function that returns the integer representation of this
         * set.  This is suitable for file input and/or output.
         *
         * \deprecated Use baseValue() instead, which makes no assumptions
         * about the underlying native C++ integer type.  This routine should,
         * however, still be safe for all of the flag types currently used in
         * Regina, since they are all based on scoped enumerations with the
         * default underlying base type of \c int.
         *
         * \warning This function should not be used widely, since it
         * effectively works around inbuilt type safety mechanisms.
         *
         * \return the integer value of this set.
         */
        [[deprecated]] constexpr int intValue() const {
            return static_cast<int>(value_);
        }

        /**
         * Returns the set corresponding to the given native C++ integer value.
         * This is suitable for file input and/or output.
         *
         * \warning This function should not be used widely, since it
         * effectively works around inbuilt type safety mechanisms.
         *
         * \return the set corresponding to the given native C++ integer value.
         */
        constexpr static Flags<T> fromBase(BaseInt value) {
            return Flags<T>(value);
        }

        /**
         * Deprecated function that returns the set corresponding to the given
         * integer value.  This is suitable for file input and/or output.
         *
         * \deprecated Use fromBase() instead, which makes no assumptions
         * about the underlying native C++ integer type.  This routine should,
         * however, still be safe for all of the flag types currently used in
         * Regina, since they are all based on scoped enumerations with the
         * default underlying base type of \c int.
         *
         * \warning This function should not be used widely, since it
         * effectively works around inbuilt type safety mechanisms.
         *
         * \return the set corresponding to the given integer value.
         */
        [[deprecated]] constexpr static Flags<T> fromInt(int value) {
            return Flags<T>(value);
        }

        /**
         * Determines whether this flag set is non-empty.
         *
         * An empty flag set has no bits set at all.
         *
         * \return \c true if and only if this flag set is non-empty.
         */
        constexpr operator bool() const {
            return value_;
        }

        /**
         * Returns whether the given flag is set.
         *
         * This requires _all_ of the bits of the given flag to be set.
         * The test is equivalent to `(*this & flag) == flag`.
         *
         * \param flag the flag whose presence will be tested.
         * \return \c true if and only if all of the bits of the given
         * flag are set.
         */
        constexpr bool has(T flag) const {
            return (value_ & static_cast<BaseInt>(flag)) ==
                static_cast<BaseInt>(flag);
        }

        /**
         * Returns whether all of the flags in the given set are set.
         *
         * This requires _all_ of the bits of all of the flags in the
         * given set to be present in this set.
         * The test is equivalent to `(*this & rhs) == rhs`.
         *
         * \param rhs the set whose presence will be tested.
         * \return \c true if and only if all of the bits of the given
         * set are present in this set.
         */
        constexpr bool has(const Flags<T>& rhs) const {
            return (value_ & rhs.value_) == rhs.value_;
        }

        /**
         * Determines whether this set is precisely equal to the given flag.
         *
         * \param rhs the flag to test this against.
         * \return \c true if and only if this and the given flag are identical.
         */
        constexpr bool operator == (T rhs) const {
            return (value_ == static_cast<BaseInt>(rhs));
        }

        /**
         * Determines whether this set is precisely equal to the given
         * flag set.
         *
         * \return \c true if and only if this and the given flag set are
         * identical.
         */
        constexpr bool operator == (const Flags<T>&) const = default;

        /**
         * Sets this flag set to contain precisely the given flag only.
         *
         * \param rhs the new value of this flag set.
         * \return a reference to this flag set.
         */
        constexpr Flags<T>& operator = (T rhs) {
            value_ = static_cast<BaseInt>(rhs);
            return *this;
        }

        /**
         * Sets this flag set to contain a copy of the given flag set.
         *
         * \return a reference to this flag set.
         */
        constexpr Flags<T>& operator = (const Flags<T>&) = default;

        /**
         * Changes this flag set by taking a bitwise OR with the given
         * flag.
         *
         * \param rhs the flag to combine with this set.
         * \return a reference to this flag set.
         */
        constexpr Flags<T>& operator |= (T rhs) {
            value_ |= static_cast<BaseInt>(rhs);
            return *this;
        }

        /**
         * Changes this flag set by taking a bitwise OR with the given
         * flag set.
         *
         * \param rhs the flag set to combine with this set.
         * \return a reference to this flag set.
         */
        constexpr Flags<T>& operator |= (const Flags<T>& rhs) {
            value_ |= rhs.value_;
            return *this;
        }

        /**
         * Changes this flag set by taking a bitwise AND with the given
         * flag.
         *
         * \param rhs the flag to combine with this set.
         * \return a reference to this flag set.
         */
        constexpr Flags<T>& operator &= (T rhs) {
            value_ &= static_cast<BaseInt>(rhs);
            return *this;
        }

        /**
         * Changes this flag set by taking a bitwise AND with the given
         * flag set.
         *
         * \param rhs the flag set to combine with this set.
         * \return a reference to this flag set.
         */
        constexpr Flags<T>& operator &= (const Flags<T>& rhs) {
            value_ &= rhs.value_;
            return *this;
        }

        /**
         * Changes this flag set by taking a bitwise XOR with the given
         * flag.
         *
         * \param rhs the flag to combine with this set.
         * \return a reference to this flag set.
         */
        constexpr Flags<T>& operator ^= (T rhs) {
            value_ ^= static_cast<BaseInt>(rhs);
            return *this;
        }

        /**
         * Changes this flag set by taking a bitwise XOR with the given
         * flag set.
         *
         * \param rhs the flag set to combine with this set.
         * \return a reference to this flag set.
         */
        constexpr Flags<T>& operator ^= (const Flags<T>& rhs) {
            value_ ^= rhs.value_;
            return *this;
        }

        /**
         * Returns the bitwise OR of this set and the given flag.
         * This flag set is not changed.
         *
         * \param rhs the flag to combine with this set.
         * \return the combination of this set and the given flag.
         */
        constexpr Flags<T> operator | (T rhs) const {
            return Flags(value_ | static_cast<BaseInt>(rhs));
        }

        /**
         * Returns the bitwise OR of this and the given flag set.
         * This flag set is not changed.
         *
         * \param rhs the flag set to combine with this set.
         * \return the combination of this and the given flag set.
         */
        constexpr Flags<T> operator | (const Flags<T>& rhs) const {
            return Flags(value_ | rhs.value_);
        }

        /**
         * Returns the bitwise AND of this set and the given flag.
         * This flag set is not changed.
         *
         * \param rhs the flag to combine with this set.
         * \return the combination of this set and the given flag.
         */
        constexpr Flags<T> operator & (T rhs) const {
            return Flags(value_ & static_cast<BaseInt>(rhs));
        }

        /**
         * Returns the bitwise AND of this and the given flag set.
         * This flag set is not changed.
         *
         * \param rhs the flag set to combine with this set.
         * \return the combination of this and the given flag set.
         */
        constexpr Flags<T> operator & (const Flags<T>& rhs) const {
            return Flags(value_ & rhs.value_);
        }

        /**
         * Returns the bitwise XOR of this set and the given flag.
         * This flag set is not changed.
         *
         * \param rhs the flag to combine with this set.
         * \return the combination of this set and the given flag.
         */
        constexpr Flags<T> operator ^ (T rhs) const {
            return Flags(value_ ^ static_cast<BaseInt>(rhs));
        }

        /**
         * Returns the bitwise XOR of this and the given flag set.
         * This flag set is not changed.
         *
         * \param rhs the flag set to combine with this set.
         * \return the combination of this and the given flag set.
         */
        constexpr Flags<T> operator ^ (const Flags<T>& rhs) const {
            return Flags(value_ ^ rhs.value_);
        }

        /**
         * Clears all bits from this set that appear in the given flag.
         *
         * \param rhs the flag to clear from this set.
         */
        constexpr void clear(T rhs) {
            value_ |= static_cast<BaseInt>(rhs);
            value_ ^= static_cast<BaseInt>(rhs);
        }

        /**
         * Clears all bits from this set that appear in the given set.
         *
         * \param rhs identifies the bits to clear from this set.
         */
        constexpr void clear(const Flags<T>& rhs) {
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
         * \param default_ the flag that will be set if any adjustments
         * need to be made.
         * \param other the flag that will be cleared if any adjustments
         * need to be made.
         */
        constexpr void ensureOne(T default_, T other) {
            if (! ((value_ & static_cast<BaseInt>(default_)) ||
                    (value_ & static_cast<BaseInt>(other))))
                value_ |= static_cast<BaseInt>(default_);
            else if ((value_ & static_cast<BaseInt>(default_)) &&
                    (value_ & static_cast<BaseInt>(other)))
                value_ ^= static_cast<BaseInt>(other);
        }

        /**
         * Adjust this set so that exactly one and only one of the three
         * given flags are included.
         *
         * If neither flag is present, then \a default_ will be used.
         * If multiple flags are present, then the flag that appears
         * _earlier_ in the argument list for this routine will be used.
         *
         * \pre Each of the given flags is single-bit.
         *
         * \param default_ the highest-priority flag.
         * \param second the second-highest-priority flag.
         * \param last the lowest-priority flag.
         */
        constexpr void ensureOne(T default_, T second, T last) {
            // Cast to BaseInt, since (T | T) is overloaded to return Flags<T>.
            if (! (value_ & (static_cast<BaseInt>(default_) |
                    static_cast<BaseInt>(second) | static_cast<BaseInt>(last))))
                value_ |= static_cast<BaseInt>(default_);
            else if (value_ & static_cast<BaseInt>(default_))
                clear(second | last);
            else if (value_ & static_cast<BaseInt>(second))
                clear(last);
        }

        /**
         * Adjust this set so that exactly one and only one of the four
         * given flags are included.
         *
         * If neither flag is present, then \a default_ will be used.
         * If multiple flags are present, then the flag that appears
         * _earlier_ in the argument list for this routine will be used.
         *
         * \pre Each of the given flags is single-bit.
         *
         * \param default_ the highest-priority flag.
         * \param second the second-highest-priority flag.
         * \param third the third-highest-priority flag.
         * \param last the lowest-priority flag.
         */
        constexpr void ensureOne(T default_, T second, T third, T last) {
            // Cast to BaseInt, since (T | T) is overloaded to return Flags<T>.
            if (! (value_ & (static_cast<BaseInt>(default_) |
                    static_cast<BaseInt>(second) | static_cast<BaseInt>(third) |
                    static_cast<BaseInt>(last))))
                value_ |= static_cast<BaseInt>(default_);
            else if (value_ & static_cast<BaseInt>(default_))
                clear(second | third | last);
            else if (value_ & static_cast<BaseInt>(second))
                clear(third | last);
            else if (value_ & static_cast<BaseInt>(third))
                clear(last);
        }

    private:
        /**
         * Constructs a new flag set with the given internal value.
         *
         * \param init the new internal value.
         */
        constexpr Flags(BaseInt init) : value_(init) {
        }
};

} // namespace regina

#endif
