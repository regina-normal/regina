
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

#include <functional>
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

/**
 * Implements a compile-time \c for loop over a range of integers.
 *
 * This function will call \a action for each integer \a i in the range
 * <i>from</i>, ..., (<i>to</i>-1) inclusive.
 *
 * The action should be a templated callable object (e.g., a generic lambda)
 * that takes a single argument whose type depends on the value of \a i.
 * Any return value will be ignored.  For each integer \a i, the argument will
 * be of type <tt>std::integral_constant<int, i></tt>, which means that \a i
 * is accessible as a compile-time constant.
 *
 * If \a from is not less than \a to, then this routine safely does nothing.
 *
 * @param action the body of the \c for loop; that is, the action to
 * perform for each integer \a i.  See above for the interface that
 * \a action should adhere to.
 */
template <int from, int to, class Action>
constexpr void for_constexpr(Action&& action) {
    if constexpr (from < to) {
        action(std::integral_constant<int, from>());
        for_constexpr<from + 1, to>(std::forward<Action>(action));
    }
}

#ifndef __DOXYGEN

namespace detail {

/**
 * Implementation details for safe_tuple_element.  This allows us to provide
 * separate specialisations for the index-in-range and index-out-of-range
 * cases, without ever instantiating an invalid std::tuple_element.
 *
 * See safe_tuple_element below for details.
 */
template <int pos, typename tuple, typename out_of_range,
    bool pos_in_range = (pos >= 0 && pos < std::tuple_size<tuple>::value)>
struct safe_tuple_element_impl;

template <int pos, typename tuple, typename out_of_range>
struct safe_tuple_element_impl<pos, tuple, out_of_range, true> {
    using type = typename std::tuple_element<pos, tuple>::type;
};

template <int pos, typename tuple, typename out_of_range>
struct safe_tuple_element_impl<pos, tuple, out_of_range, false> {
    using type = out_of_range;
};

} // namespace detail

#endif

/**
 * An alternative to std::tuple_element that gracefully handles an
 * out-of-range index.
 *
 * If \a pos is a valid index into the tuple type \a tuple, then this
 * type alias is identical to std::tuple_element<pos, tuple>::type.  Otherwise
 * this type alias is identical to the argument \a out_of_range.
 *
 * Note that you should not append ::type when using safe_tuple_element
 * (i.e., this is really a drop-in replacement for the C++17 type alias
 * std::tuple_element_t, and not the C++11 structure std::tuple_element).
 *
 * \tparam pos an index, which may take any integer value.
 * \tparam tuple a std::tuple type (which is allowed to include \c const and/or
 * \c volatile modifiers).
 * \tparam out_of_range the type to use if \a pos is not a valid index
 * into \a tuple.
 */
template <int pos, typename tuple, typename out_of_range = void>
using safe_tuple_element = typename regina::detail::safe_tuple_element_impl<
    pos, tuple, out_of_range>::type;

/**
 * A traits class that deduces the type of the argument in a given position
 * for a callable object.  It can (amongst other things) work with
 * function pointers, function references, member function pointers,
 * std::function wrappers, and lambdas.
 *
 * This struct provides a single member type alias, named \a type, which is
 * the type of the argument to \a Action that appears in position \a pos.
 *
 * If \a Action is a member function, then arguments will still be numbered
 * according to the "real" arguments (i.e., the numbering ignores the
 * object pointer \c this which is implicitly passed to every non-static
 * member function).
 *
 * If \a Action does not take enough arguments for the given position \a pos,
 * then \a type will be \c void.
 *
 * \tparam Action the type of a callable object that takes at least
 * one argument.
 * \tparam pos the index of the argument being requested.  Positions are
 * numbered from 0 upwards.
 *
 * \ingroup detail
 */
template <typename Action, int pos>
struct CallableArg;

#ifndef __DOXYGEN

// Generic implementation which works for lambdas and classes with a
// bracket operator.  For lambdas, this then falls through (via inheritance)
// to the case for member function pointers, implemented separately below.
template <typename Action, int pos>
struct CallableArg : public CallableArg<decltype(&Action::operator()), pos> {
};

// Implementation for global function pointers and references:
template <typename ReturnType, typename... Args, int pos>
struct CallableArg<ReturnType(*)(Args...), pos> {
    using type = safe_tuple_element<pos, std::tuple<Args...>>;
};
template <typename ReturnType, typename... Args, int pos>
struct CallableArg<ReturnType(&)(Args...), pos> {
    using type = safe_tuple_element<pos, std::tuple<Args...>>;
};


// Implementation for member function pointers:
template <typename Class, typename ReturnType, typename... Args, int pos>
struct CallableArg<ReturnType(Class::*)(Args...) const, pos> {
    using type = safe_tuple_element<pos, std::tuple<Args...>>;
};

// Implementation for std::function objects:
template <typename ReturnType, typename... Args, int pos>
struct CallableArg<std::function<ReturnType(Args...)>, pos> {
    using type = safe_tuple_element<pos, std::tuple<Args...>>;
};
template <typename ReturnType, typename... Args, int pos>
struct CallableArg<std::function<ReturnType(Args...)>&, pos> {
    using type = safe_tuple_element<pos, std::tuple<Args...>>;
};
template <typename ReturnType, typename... Args, int pos>
struct CallableArg<const std::function<ReturnType(Args...)>&, pos> {
    using type = safe_tuple_element<pos, std::tuple<Args...>>;
};

#endif // __DOXYGEN

} // namespace regina

#endif

