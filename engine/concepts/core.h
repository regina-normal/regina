
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

/*! \file concepts/core.h
 *  \brief Concepts related to core types.
 */

#ifndef __REGINA_CONCEPTS_CORE_H
#ifndef __DOXYGEN
#define __REGINA_CONCEPTS_CORE_H
#endif

#include <concepts>
#include "utilities/intutils.h"

namespace regina {

/**
 * \defgroup concepts Concepts
 * C++ concepts for describing constraints on types.
 */

/**
 * One of the standard non-boolean C++ integer types, without making any
 * special accommodations for 128-bit integer compiler extensions.
 *
 * This concept is exactly like `std::integral` but with `bool` excluded.
 *
 * \ingroup concepts
 */
template <typename T>
concept StandardCppInteger = std::integral<T> && ! std::same_as<T, bool>;

/**
 * A native non-boolean C++ integer type, allowing for 128-bit integers also
 * if these are supported by the compiler.
 *
 * See the constant regina::is_cpp_integer_v for further details.
 *
 * \ingroup concepts
 */
template <typename T>
concept CppInteger = is_cpp_integer_v<T>;

/**
 * A signed native non-boolean C++ integer type, allowing for 128-bit integers
 * also if these are supported by the compiler.
 *
 * See the constant regina::is_signed_cpp_integer_v for further details.
 *
 * \ingroup concepts
 */
template <typename T>
concept SignedCppInteger = is_signed_cpp_integer_v<T>;

/**
 * An unsigned native non-boolean C++ integer type, allowing for 128-bit
 * integers also if these are supported by the compiler.
 *
 * See the constant regina::is_unsigned_cpp_integer_v for further details.
 *
 * \ingroup concepts
 */
template <typename T>
concept UnsignedCppInteger = is_unsigned_cpp_integer_v<T>;

} // namespace regina

#endif

