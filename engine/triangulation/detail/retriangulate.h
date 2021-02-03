
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
 *  retriangulation or link rewriting functions.
 */

#ifndef __RETRIANGULATE_H_DETAIL
#ifndef __DOXYGEN
#define __RETRIANGULATE_H_DETAIL
#endif

#include <functional>
#include <string>
#include "regina-core.h"

namespace regina::detail {

/**
 * A traits class that deduces the type of the argument in a given position
 * for a callable object.  It can (amongst other things) work with
 * function pointers, member functions, and lambdas.
 *
 * This struct provides a single member typedef, named \a type, which is
 * the type of the argument to \a Action that appears in position \a pos.
 *
 * If \a Action is a member function, then arguments will still be numbered
 * according to the "real" arguments (i.e., the numbering ignores the
 * object pointer \c this which is implicitly passed to every non-static
 * member function).
 *
 * If \a Action does not take enough arguments for the given \a pos,
 * then this will almost certainly generate an error deep with in the
 * standard library (most likely with std::tuple_element).
 *
 * \tparam Action the type of a callable object that takes at least
 * one argument.
 * \tparam pos the index of the argument being requested.  Positions are
 * numbered from 0 upwards.
 */
template <typename Action, int pos>
struct CallableArg;

#ifndef __DOXYGEN

// Generic implementation which works for lambdas and classes with a
// bracket operator:
template <typename Action, int pos>
struct CallableArg : public CallableArg<decltype(&Action::operator()), pos> {
};

// Implementation for global functions:
template <typename ReturnType, typename... Args, int pos>
struct CallableArg<ReturnType(*)(Args...), pos> {
    typedef typename std::tuple_element<pos, std::tuple<Args...>>::type type;
};

// Implementation for member functions (and this is also where the
// lambda implementation ultimately falls through to):
template <typename Class, typename ReturnType, typename... Args, int pos>
struct CallableArg<ReturnType(Class::*)(Args...) const, pos> {
    typedef typename std::tuple_element<pos, std::tuple<Args...>>::type type;
};

#endif // __DOXYGEN

/**
 * Declares the internal type used to store a callable action that is passed
 * to a retriangulation or link rewriting function.  This internal type is
 * included here as a member typedef, but you can also access it directly
 * through the simpler type alias RetriangulateActionFunc<Object, withSig>.
 *
 * A retriangulation or link rewriting function can work with arbitrary
 * callable objects.  However, the \e implementations of such functions are
 * long and should not be dragged into the main headers.  The core purpose
 * of this class is therefore to coalesce the arbitrary action types
 * down to just \e two fixed types (depending on whether the action includes a
 * text signature (e.g., an isomorphism signature) in its initial argument(s)).
 * This means that the retriangulation or rewriting code can be templated on
 * a single boolean parameter, and so we can instatiate it completely in
 * Regina's library and keep the implementation details out of the main headers.
 *
 * The current implementation packages the action up as a std::function object
 * with either a single argument (a triangulation/link) or a pair of arguments
 * (a text signature and a triangulation/link).  Any additional arguments to the
 * retriangulation/rewriting action will be bound in the std::function object).
 * This implementation is subject to change in future versions of Regina.
 *
 * This struct provides a single member typedef, named \a type,
 * which is the internal type used to store the callable action.
 *
 * \tparam Object the class providing the retriangulation/rewriting function,
 * such as regina::Triangulation<dim> or regina::Link.
 * \tparam withSig \c true if we are storing an action that includes both a
 * text signature and a triangulation in its initial argument(s),
 * or \c false if we are storing an action whose argument list begins with
 * just a triangulation/link.
 */
template <class Object, bool withSig>
struct RetriangulateActionFuncDetail;

#ifndef __DOXYGEN

template <class Object>
struct RetriangulateActionFuncDetail<Object, true> {
    typedef std::function<bool(const std::string&, Object&)> type;
};

template <class Object>
struct RetriangulateActionFuncDetail<Object, false> {
    typedef std::function<bool(Object&)> type;
};

#endif // __DOXYGEN

/**
 * The internal type used to store a callable action that is passed to a
 * retriangulation or link rewriting function.
 * See RetriangulateActionFuncDetail for details.
 */
template <class Object, bool withSig>
using RetriangulateActionFunc =
    typename RetriangulateActionFuncDetail<Object, withSig>::type;

/**
 * A traits class that analyses callable objects that are passed to
 * retriangulation or link rewriting functions.
 *
 * Recall that the initial arguments for such a callable object must be either
 * (a) a single triangulation/link, or (b) a text signature (e.g., an
 * isomorphism signature) followed by a triangulation/link.
 *
 * This struct provides a boolean compile-time constant \a valid, which is
 * \c true if and only if the initial arguemnt(s) to \a Action are acceptable
 * as outlined above (i.e., a reference to the underlying \a Object class
 * for actions that take a triangulation/link, or a const string reference and
 * an \a Object reference for actions that take a text signature also).
 *
 * If \a valid is \c true, then this struct also provides a boolean
 * compile-time constant \a withSig, which is \c true if and only if the action
 * takes both a text signature and a triangulation/link.
 * If \a valid is \c false then the boolean constant \a withSig will still
 * be present, but its value is not defined.
 *
 * Finally, if \a valid is \c true, then this struct provides a static
 * function convert() that takes a callable object and all of its later
 * optional arguments (i.e., excluding the initial triangulation/link
 * and possibly the text signature before it), and returns a callable object
 * of type RetriangulateActionFunc<withSig> where these later optional
 * arguments are bound.  All arguments to convert() will be moved/copied
 * using std::forward().
 * If \a valid is \c false then the function \a convert will still be
 * declared but not defined, and it will have a \c void return type.
 *
 * \tparam Object the class providing the retriangulation or link rewriting
 * function, such as regina::Triangulation<dim> or regina::Link.
 * \tparam Action the type of a callable object that is passed to the
 * retriangulation/rewriting function.
 * \tparam FirstArg the type of the first argument to \a Action; you should
 * not specify this directly, but instead allow the compiler to deduce it.
 */
template <class Object, typename Action,
        typename FirstArg = typename CallableArg<Action, 0>::type>
struct RetriangulateActionTraits;

#ifndef __DOXYGEN

template <class Object, typename Action, typename FirstArg>
struct RetriangulateActionTraits {
    static constexpr bool valid = false;
    static constexpr bool withSig = false;

    template <typename... Args>
    static void convert(Action&&, Args&&...);
};

template <class Object, typename Action>
struct RetriangulateActionTraits<Object, Action, Object&> {
    static constexpr bool valid = true;
    static constexpr bool withSig = false;

    template <typename... Args>
    static RetriangulateActionFunc<Object, withSig> convert(
            Action&& action, Args&&... args) {
        return std::bind(std::forward<Action>(action),
            std::placeholders::_1, std::forward<Args>(args)...);
    }
};

template <class Object, typename Action>
struct RetriangulateActionTraits<Object, Action, const Object&> {
    static constexpr bool valid = true;
    static constexpr bool withSig = false;

    template <typename... Args>
    static RetriangulateActionFunc<Object, withSig> convert(
            Action&& action, Args&&... args) {
        return std::bind(std::forward<Action>(action),
            std::placeholders::_1, std::forward<Args>(args)...);
    }
};

template <class Object, typename Action>
struct RetriangulateActionTraits<Object, Action, const std::string&> {
    typedef typename CallableArg<Action, 1>::type SecondArg;
    static constexpr bool valid =
        std::is_same<SecondArg, Object&>::value ||
        std::is_same<SecondArg, const Object&>::value;
    static constexpr bool withSig = true;

    template <typename... Args>
    static RetriangulateActionFunc<Object, withSig> convert(
            Action&& action, Args&&... args) {
        return std::bind(std::forward<Action>(action),
            std::placeholders::_1, std::placeholders::_2,
            std::forward<Args>(args)...);
    }
};

#endif // __DOXYGEN

} // namespace regina::detail

#endif

