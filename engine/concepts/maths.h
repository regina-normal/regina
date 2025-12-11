
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

/*! \file concepts/maths.h
 *  \brief Concepts related to "large" mathematical types, such as vectors.
 */

#ifndef __REGINA_CONCEPTS_MATHS_H
#ifndef __DOXYGEN
#define __REGINA_CONCEPTS_MATHS_H
#endif

#include <type_traits>
#include "concepts/core.h"
#include "maths/forward.h"

namespace regina {

/**
 * A mathematical vector type derived from (or the same as) `Vector<T>`,
 * where the element type \a T is one of Regina's arbitrary-precision integer
 * types (Integer or LargeInteger).
 *
 * \ingroup concepts
 */
template <typename T>
concept ArbitraryPrecisionIntegerVector =
    requires(T x) {
        []<typename U>(Vector<U>&){}(x); // derived from Vector<...>
    } &&
    ArbitraryPrecisionInteger<typename T::value_type>;

/**
 * A mathematical vector type derived from (or the same as) `Vector<T>`,
 * where the element type \a T is one of Regina's own integer types
 * (Integer, LargeInteger, or NativeInteger).
 *
 * \ingroup concepts
 */
template <typename T>
concept IntegerVector =
    requires(T x) {
        []<typename U>(Vector<U>&){}(x); // derived from Vector<...>
    } &&
    ReginaInteger<typename T::value_type>;

} // namespace regina

#endif

