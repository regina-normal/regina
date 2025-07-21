
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/*! \file utilities/typeutils.h
 *  \brief Provides helper classes for use with template metaprogramming
 *  and type analysis.  The need for these will likely diminish as
 *  Regina switches to use more modern C++ standards.
 */

#ifndef __REGINA_TYPEUTILS_H
#ifndef __DOXYGEN
#define __REGINA_TYPEUTILS_H
#endif

#include <functional>
#include <typeinfo>
#include <stdexcept>
#include <variant>
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
 * be of type `std::integral_constant<int, i>`, which means that \a i
 * is accessible as a compile-time constant.
 *
 * If \a from is not less than \a to, then this routine safely does nothing.
 *
 * \param action the body of the \c for loop; that is, the action to
 * perform for each integer \a i.  See above for the interface that
 * \a action should adhere to.
 *
 * \ingroup utilities
 */
template <int from, int to, class Action>
constexpr void for_constexpr(Action&& action) {
    if constexpr (from < to) {
        action(std::integral_constant<int, from>());
        for_constexpr<from + 1, to>(std::forward<Action>(action));
    }
}

/**
 * Implements a compile-time \c for loop over a given set of integers.
 *
 * This function will call \a action for each integer \a i in the sequence
 * \a values.
 *
 * The action should be a templated callable object (e.g., a generic lambda)
 * that takes a single argument whose type depends on the value of \a i.
 * Any return value will be ignored.  For each integer \a i, the argument will
 * be of type `std::integral_constant<int, i>`, which means that \a i
 * is accessible as a compile-time constant.
 *
 * There are two versions of foreach_constexpr(): one that takes the list of
 * values as individual template parameters (simpler for ad-hoc invocations),
 * and one that takes the list of values as a std::integer_sequence (which
 * allows you to reuse the list of values via a type alias).
 *
 * \param action the body of the \c for loop; that is, the action to
 * perform for each integer \a i.  See above for the interface that
 * \a action should adhere to.
 *
 * \ingroup utilities
 */
template <int... values, class Action>
constexpr void foreach_constexpr(Action&& action) {
    (action(std::integral_constant<int, values>()), ...);
}

/**
 * Implements a compile-time \c for loop over a given set of integers.
 *
 * This function will call \a action for each integer \a i in the integer
 * sequence \a values.
 *
 * The action should be a templated callable object (e.g., a generic lambda)
 * that takes a single argument whose type depends on the value of \a i.
 * Any return value will be ignored.  For each integer \a i, the argument will
 * be of type `std::integral_constant<int, i>`, which means that \a i
 * is accessible as a compile-time constant.
 *
 * There are two versions of foreach_constexpr(): one that takes the list of
 * values as individual template parameters (simpler for ad-hoc invocations),
 * and one that takes the list of values as a std::integer_sequence (which
 * allows you to reuse the list of values via a type alias).
 *
 * \param action the body of the \c for loop; that is, the action to
 * perform for each integer \a i.  See above for the interface that
 * \a action should adhere to.
 *
 * \ingroup utilities
 */
template <int... values, class Action>
constexpr void foreach_constexpr(std::integer_sequence<int, values...>,
        Action&& action) {
    (action(std::integral_constant<int, values>()), ...);
}

/**
 * Implements a compile-time selection, where the runtime argument must belong
 * to a compile-time range of integers, and the value of the argument
 * determines what is returned.
 *
 * The action should be a templated callable object (e.g., a generic lambda)
 * that takes a single argument.  If \a value is equal to the integer \a i,
 * for some \a i in the range <i>from</i>, ..., (<i>to</i>-1) inclusive,
 * then this function will return `action(i)`.  The argument \a i
 * will be passed using the type `std::integral_constant<int, i>`,
 * which means that the value of \a i will be accessible to \a action as a
 * compile-time constant.
 *
 * \exception std::runtime_error The given runtime value is not within the
 * range <i>from</i>, ..., (<i>to</i>-1).
 *
 * \tparam Return the type to be returned from this function.
 * Typically this will be the same as the return type from \a action,
 * but it may differ (particuarly if the return type of \a action
 * depends upon its integer argument).
 *
 * \param value the runtime value that determines the selection; that is, the
 * argument that will be passed to the given action as a compile-time constant.
 * \param action the action to perform for whichever integer \a i matches
 * the given runtime value.  See above for the interface that \a action
 * should adhere to.
 * \return the value returned from \a action.
 *
 * \ingroup utilities
 */
template <int from, int to, typename Return, class Action>
constexpr Return select_constexpr(int value, Action&& action) {
    if constexpr (from < to) {
        if (value == from)
            return action(std::integral_constant<int, from>());
        else
            return select_constexpr<from + 1, to, Return, Action>(value,
                std::forward<Action>(action));
    } else
        throw std::runtime_error("select_constexpr(): value out of range");
}

#ifndef __DOXYGEN

namespace detail {

/**
 * Implementation details for select_constexpr_as_variant.
 * These declarations are used to pack together the correct std::variant
 * return type.
 */
template <int from, class Action, int... arg /* 0,...,(to-from-1) */>
auto seqToVariantHelper(std::integer_sequence<int, arg...>) ->
    std::variant<decltype(std::declval<Action>()(
        std::integral_constant<int, arg + from>()))...>;

template <int from, int to, class Action>
using SeqToVariant = decltype(seqToVariantHelper<from, Action>(
    std::make_integer_sequence<int, to - from>()));

} // namespace detail

#endif

/**
 * A variant of select_constexpr() where the return type is a variant, built
 * from the return types for all integers in the given compile-time range.
 *
 * See select_constexpr() for an overview of how Regina's compile-time
 * selection function works.  This routine behaves exactly the same as
 * select_constexpr(), except that you do not need to explicitly give
 * the return type.  Instead, the return type will be
 * `std::variant<R(from), R(from+1), ..., R(to-1)>`, where each
 * `R(i)` denotes the type returned by the corresponding call to
 * `action(i)`.
 *
 * This is useful when the return _type_ from \a action (not just the
 * return value) depends on \a i.  An example of this is
 * `Triangulation::face(subdim, index)`, whose return type
 * would normally be `Face<subdim>*`, except for the fact that
 * \a subdim is not known until runtime.  Therefore this function needs
 * to return a std::variant, and so select_constexpr_as_variant()
 * can be used for its internal implementation.
 *
 * See select_constexpr() for further details.
 *
 * \pre All of the possible return types `R(from)`,
 * `R(from+1)`, ..., `R(to-1)` are different.
 *
 * \exception std::runtime_error The given runtime value is not within the
 * range <i>from</i>, ..., (<i>to</i>-1).
 *
 * \param value the runtime value that determines the selection; that is, the
 * argument that will be passed to the given action as a compile-time constant.
 * \param action the action to perform for whichever integer \a i matches
 * the given runtime value.  See above for the interface that \a action
 * should adhere to.
 * \return the value returned from \a action, given as a variant that
 * encapsulates all (\a to - \a from) possible return types.
 *
 * \ingroup utilities
 */
template <int from, int to, class Action>
constexpr auto select_constexpr_as_variant(int value, Action&& action) {
    return select_constexpr<from, to,
        regina::detail::SeqToVariant<from, to, Action>, Action>(
        value, std::forward<Action>(action));
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
    bool pos_in_range = (pos >= 0 && pos < std::tuple_size_v<tuple>)>
struct safe_tuple_element_impl;

template <int pos, typename tuple, typename out_of_range>
struct safe_tuple_element_impl<pos, tuple, out_of_range, true> {
    using type = std::tuple_element_t<pos, tuple>;
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
 * Note that you should not append `::type` when using safe_tuple_element
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

/**
 * Returns the preferred Python display name for the given C++ type.
 *
 * The Python bindings have an internal mechanism for converting _any_
 * C++ type into a suitable display name.  However, for some of Regina's
 * classes the results are not ideal.
 *
 * For example, the 3-D triangulation class may be displayed as
 * `regina::Triangulation<3>` instead of its "real" Python name
 * `regina.Triangulation3`.  (This kind of problem most commonly
 * appears in docstrings, where function signatures are generated as each
 * function is bound, which may happen before all of the types in the
 * argument/return list have been bound.)
 *
 * The purpose of this function is to override this default typename
 * conversion mechanism.  If the C++ type referred to by \a t has a
 * known Python name that should always be used, this function will
 * return it.  Otherwise this function returns \c nullptr, indicating
 * that the default conversion mechanism should be used.
 *
 * \nopython
 *
 * \param t an object that references the C++ type whose display name we
 * wish to obtain.
 * \return the preferred display name for this type in Python, or \c nullptr
 * if the default C++-to-Python name conversion mechanism should be used.
 */
const char* pythonTypename(const std::type_info* t);

} // namespace regina

#endif

