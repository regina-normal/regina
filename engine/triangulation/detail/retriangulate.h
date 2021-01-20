
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

/*! \file triangulation/detail/retriangulate.h
 *  \brief Traits classes for analysing actions that are passed to
 *  retriangulation functions.
 */

#ifndef __RETRIANGULATE_H_DETAIL
#ifndef __DOXYGEN
#define __RETRIANGULATE_H_DETAIL
#endif

#include <functional>
#include <string>
#include "regina-core.h"

namespace regina {
namespace detail {

/**
 * A traits class that deduces the type of the first argument for a
 * callable object.  It can (amongst other things) work with
 * function pointers, member functions, and lambdas.
 *
 * This struct provides a single member typedef, named \a type,
 * which is the type of the first argument to \a Action.
 *
 * If \a Action is a member function, then \a type will still be the
 * first "real" argument type, not a pointer to the underlying class
 * (in other words, it ignores the underlying object pointer \c this
 * which is implicitly passed to every non-static member function).
 *
 * \tparam Action the type of a callable object that takes at least
 * one argument.
 */
template <typename Action>
struct FirstArg;

#ifndef __DOXYGEN

// Generic implementation which works for lambdas and classes with a
// bracket operator:
template <typename Action>
struct FirstArg : public FirstArg<decltype(&Action::operator())> {
};

// Implementation for global functions:
template <typename ReturnType, typename... Args>
struct FirstArg<ReturnType(*)(Args...)> {
    typedef typename std::tuple_element<0, std::tuple<Args...>>::type type;
};

// Implementation for member functions (and this is also where the
// lambda implementation ultimately falls through to):
template <typename Class, typename ReturnType, typename... Args>
struct FirstArg<ReturnType(Class::*)(Args...) const> {
    typedef typename std::tuple_element<0, std::tuple<Args...>>::type type;
};

#endif // __DOXYGEN

/**
 * Declares the internal type used to store a callable action that is
 * passed to a retriangulation function.  This internal type is included
 * here as a member typedef, but you can also access it directly through
 * the simpler type alias RetriangulateActionFunc<Object, sigOnly>.
 *
 * A retriangulation function can work with arbitrary callable objects.
 * However, the \e implementation of retriangulation is long and should
 * not be dragged into the main triangulation headers.  The main purpose
 * of this class is therefore to coalesce the arbitrary action types
 * down to just \e two fixed types (depending on whether the action takes
 * a triangulation or an isomorphism signature as its first argument).
 * This means that the retriangulation code can be templated on a single
 * boolean parameter, and so we can instatiate it completely in Regina's
 * library and keep the implementation details out of the main headers.
 *
 * The current implementation packages the action up as a std::function
 * object with a single argument (i.e., any additional arguments to the
 * retriangulation action are now bound in the std::function object).
 * This implementation is subject to change in future versions of Regina.
 *
 * This struct provides a single member typedef, named \a type,
 * which is the internal type used to store the callable action.
 *
 * \tparam Object the class providing the retriangulation function, such
 * as regina::Triangulation<dim>.
 * \tparam sigOnly \c true if we are storing an action that takes an
 * isomorphism signature as its first argument, or \c false if we are
 * storing an action that takes a full \a Object as its first argument.
 */
template <class Object, bool sigOnly>
struct RetriangulateActionFuncDetail;

#ifndef __DOXYGEN

template <class Object>
struct RetriangulateActionFuncDetail<Object, true> {
    typedef std::function<bool(const std::string&)> type;
};

template <class Object>
struct RetriangulateActionFuncDetail<Object, false> {
    typedef std::function<bool(Object&)> type;
};

#endif // __DOXYGEN

/**
 * The internal type used to store a callable action that is passed to a
 * retriangulation function.  See RetriangulateActionFuncDetail for details.
 */
template <class Object, bool sigOnly>
using RetriangulateActionFunc =
    typename RetriangulateActionFuncDetail<Object, sigOnly>::type;

/**
 * A traits class that analyses callable objects that are passed to
 * retriangulation functions.
 *
 * Recall that such a callable object must take either a triangulation
 * or an isomorphism signature as its first argument.
 *
 * This struct provides a boolean compile-time constant \a valid, which is
 * \c true if and only if the first argument to \a Action is acceptable
 * as outlined above (i.e., a reference to the underlying \a Object class
 * for actions that take a triangulation, or a const string reference
 * for actions that take an isomorphism signature).
 *
 * If \c valid is \c true, then this struct also provides a boolean
 * compile-time constant \c sigOnly, which is \c false if the action takes
 * a triangulation, or \c true if the action takes an isomorphism signature.
 *
 * \tparam Object the class providing the retriangulation function, such
 * as regina::Triangulation<dim>.
 * \tparam Action the type of a callable object that is passed to the
 * retriangulation function.
 * \tparam FirstArg the type of the first argument to \a Action; you should
 * not specify this directly, but instead allow the compiler to deduce it.
 */
template <class Object, typename Action,
        typename FirstArg = typename FirstArg<Action>::type>
struct RetriangulateActionTraits;

#ifndef __DOXYGEN

template <class Object, typename Action, typename FirstArg>
struct RetriangulateActionTraits {
    static constexpr bool valid = false;
};

template <class Object, typename Action>
struct RetriangulateActionTraits<Object, Action, Object&> {
    static constexpr bool valid = true;
    static constexpr bool sigOnly = false;
};

template <class Object, typename Action>
struct RetriangulateActionTraits<Object, Action, const Object&> {
    static constexpr bool valid = true;
    static constexpr bool sigOnly = false;
};

template <class Object, typename Action>
struct RetriangulateActionTraits<Object, Action, const std::string&> {
    static constexpr bool valid = true;
    static constexpr bool sigOnly = true;
};

#endif // __DOXYGEN

} } // namespace regina::detail

#endif

