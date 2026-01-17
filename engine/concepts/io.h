
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

/*! \file concepts/io.h
 *  \brief Concepts related to input and output.
 */

#ifndef __REGINA_CONCEPTS_IO_H
#ifndef __DOXYGEN
#define __REGINA_CONCEPTS_IO_H
#endif

#include <concepts>
#include <iostream>
#include <string>
#include "regina-core.h"

namespace regina {

/**
 * A type that can be written to an output stream.
 *
 * \ingroup concepts
 */
template <typename T>
concept Writeable =
    requires(const T x, std::ostream& out) {
        { out << x } -> std::same_as<std::ostream&>;
    };

/**
 * A class or struct type that supports string output through a member function
 * `str()`.
 *
 * Many of Regina's own classes provide such a function.
 *
 * \ingroup concepts
 */
template <typename T>
concept Stringifiable =
    requires(const T x) {
        { x.str() } -> std::same_as<std::string>;
    };

/**
 * A type that supports string conversion via `std::to_string()`.
 *
 * For example, the standard C++ native integer types support string conversion
 * in this way.
 *
 * \ingroup concepts
 */
template <typename T>
concept StandardStringifiable =
    requires(const T x) {
        { std::to_string(x) } -> std::same_as<std::string>;
    };

/**
 * A type that has member functions for tight encoding and decoding.
 *
 * \ingroup concepts
 */
template <typename T>
concept InherentlyTightEncodable =
    requires(const T x, std::ostream& out, std::istream& in) {
        { x.tightEncode(out) };
        { T::tightDecode(in) } -> std::same_as<T>;
    };

} // namespace regina

#endif

