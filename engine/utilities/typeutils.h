
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file utilities/typeutils.h
 *  \brief Provides helper template classes for use with template
 *  metaprogramming.  The need for these will likely diminish as
 *  Regina switches to use more modern C++ standards.
 */

#ifndef __REGINA_TYPEUTILS_H
#ifndef __DOXYGEN
#define __REGINA_TYPEUTILS_H
#endif

#include "regina-core.h"

namespace regina {

/**
 * A struct that holds either a single value of type \a T or nothing at all,
 * depending on whether the given compile-time condition holds.
 *
 * If \a condition is \c true, then this struct holds a single data member
 * \a value of type \a T, which will be initialised on construction with
 * \a defaultValue.
 *
 * The advantage to using this class over std::conditional is that it
 * supports assigning a default value on construction.
 *
 * \tparam condition \c true if the data member should be included, or
 * \c false if this struct should be empty.
 * \tparam T the data type to store.
 * \tparam defaultValue the value to assign the data member upon construction.
 *
 * \ingroup utilities
 */
template <bool condition, typename T, T defaultValue>
struct EnableIf {
    T value { defaultValue };
        /**< The data member to store if \a condition is \c true. */
};

#ifndef __DOXYGEN
template <typename T, T defaultValue>
struct EnableIf<false, T, defaultValue> {
};
#endif // __DOXYGEN

} // namespace regina

#endif

