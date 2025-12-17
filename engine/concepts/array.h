
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

/*! \file concepts/array.h
 *  \brief Concepts related to the standard C++ array type.
 */

#ifndef __REGINA_CONCEPTS_ARRAY_H
#ifndef __DOXYGEN
#define __REGINA_CONCEPTS_ARRAY_H
#endif

#include <array>
#include <concepts>
#include "regina-core.h"

namespace regina {

/**
 * A type of the form `std::array<Element, n>` for some integer \a n.
 *
 * \ingroup concepts
 */
template <typename T, typename Element>
concept ArrayOf =
    requires(T x) { { std::array(x) } -> std::same_as<T>; } &&
    std::same_as<typename T::value_type, Element>;

/**
 * A type of the form `const std::array<Element, n>&` for some integer \a n.
 *
 * This concept could (for example) be used to constrain the type of an array
 * class constant.
 *
 * \ingroup concepts
 */
template <typename T, typename Element>
concept ConstRefArrayOf =
    std::is_reference_v<T> &&
    std::is_const_v<std::remove_reference_t<T>> &&
    ArrayOf<std::remove_const_t<std::remove_reference_t<T>>, Element>;

} // namespace regina

#endif

