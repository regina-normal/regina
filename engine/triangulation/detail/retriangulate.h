
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#ifndef __REGINA_RETRIANGULATE_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_RETRIANGULATE_H_DETAIL
#endif

#include <functional>
#include <string>
#include "utilities/typeutils.h"

namespace regina {

class ProgressTrackerOpen;

namespace detail {

/**
 * Declares the internal type used to store a callable action that is passed
 * to a retriangulation or link rewriting function.
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
 * \tparam Object the class providing the retriangulation/rewriting function,
 * such as regina::Triangulation<dim> or regina::Link.
 * \tparam withSig \c true if we are storing an action that includes both a
 * text signature and a triangulation in its initial argument(s),
 * or \c false if we are storing an action whose argument list begins with
 * just a triangulation/link.
 *
 * \ingroup detail
 */
template <class Object, bool withSig>
using RetriangulateActionFunc = std::conditional_t<withSig,
    std::function<bool(const std::string&, Object&&)>,
    std::function<bool(Object&&)>>;

/**
 * The common implementation of all retriangulation and link rewriting
 * functions.
 *
 * This routine performs exactly the task described by Link::rewrite() or
 * Triangluation<dim>::retriangulate() (for those dimensions where it is
 * defined), with the following differences:
 *
 * - This routine assumes any preconditions have already been checked, and
 *   exceptions thrown if they failed;
 *
 * - The action passed to this routine uses a much less templated type, which
 *   means the implementation can be kept out of the public API headers.
 *
 * See Triangulation<dim>::retriangulate() or Link::rewrite() for full
 * details on what this routine actually does.
 *
 * \tparam Object the class providing the retriangulation/rewriting function,
 * such as regina::Triangulation<dim> or regina::Link.
 * \tparam withSig \c true if we are storing an action that includes both a
 * text signature and a triangulation in its initial argument(s),
 * or \c false if we are storing an action whose argument list begins with
 * just a triangulation/link.
 *
 * \param obj the object being retriangulated or rewritten.
 * \param height the maximum number of top-dimensional simplices or crossings
 * to allow beyond the initial number in \a obj, or a negative number if
 * this should not be bounded.
 * \param nThreads the number of threads to use.  If this is 1 or smaller then
 * the routine will run single-threaded.
 * \param tracker a progress tracker through which progress will be reported,
 * or \c null if no progress reporting is required.
 * \param action a function to call for each triangulation that is found.
 * \return \c true if some call to \a action returned \c true (thereby
 * terminating the search early), or \c false if the search ran to completion.
 *
 * \ingroup detail
 */
template <class Object, bool withSig>
bool retriangulateInternal(const Object& obj, int height, unsigned nThreads,
        ProgressTrackerOpen* tracker,
        RetriangulateActionFunc<Object, withSig>&& action);

/**
 * A traits class that analyses callable objects that are passed to
 * retriangulation or link rewriting functions.
 *
 * Recall that the initial arguments for such a callable object must be either
 * (a) a single triangulation/link, or (b) a text signature (e.g., an
 * isomorphism signature) followed by a triangulation/link.  The callable
 * object may take its triangulation/link by value, const reference or
 * rvalue reference; however, if it takes a signature also then this must be
 * by (const std::string&).
 *
 * This struct provides a boolean compile-time constant \a valid, which is
 * \c true if and only if the initial arguemnt(s) to \a Action are acceptable
 * as outlined above (i.e., an argument of the underlying \a Object class
 * for actions that take a triangulation/link, or a const string reference and
 * an \a Object for actions that take a text signature also).
 *
 * If \a valid is \c true, then this struct also provides a boolean
 * compile-time constant \a withSig, which is \c true if and only if the action
 * takes both a text signature and a triangulation/link.
 * If \a valid is \c false then the boolean constant \a withSig will still
 * be present, but its value is not defined.
 *
 * \tparam Object the class providing the retriangulation or link rewriting
 * function, such as regina::Triangulation<dim> or regina::Link.
 * \tparam Action the type of a callable object that is passed to the
 * retriangulation/rewriting function.
 * \tparam FirstArg the type of the first argument to \a Action; you should
 * not specify this directly, but instead allow the compiler to deduce it.
 *
 * \ingroup detail
 */
template <class Object, typename Action,
        typename FirstArg = typename CallableArg<Action, 0>::type>
struct RetriangulateActionTraits;

#ifndef __DOXYGEN

template <class Object, typename Action, typename FirstArg>
struct RetriangulateActionTraits {
    static constexpr bool valid = false;
    static constexpr bool withSig = false;
};

template <class Object, typename Action>
struct RetriangulateActionTraits<Object, Action, Object> {
    static constexpr bool valid = true;
    static constexpr bool withSig = false;
};

template <class Object, typename Action>
struct RetriangulateActionTraits<Object, Action, Object&&> {
    static constexpr bool valid = true;
    static constexpr bool withSig = false;
};

template <class Object, typename Action>
struct RetriangulateActionTraits<Object, Action, const Object&> {
    static constexpr bool valid = true;
    static constexpr bool withSig = false;
};

template <class Object, typename Action>
struct RetriangulateActionTraits<Object, Action, const std::string&> {
    using SecondArg = typename CallableArg<Action, 1>::type;
    static constexpr bool valid =
        std::is_same_v<SecondArg, Object> ||
        std::is_same_v<SecondArg, Object&&> ||
        std::is_same_v<SecondArg, const Object&>;
    static constexpr bool withSig = true;
};

#endif // __DOXYGEN

} } // namespace regina::detail

#endif

