
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

/*! \file triangulation/detail/retriangulate.h
 *  \brief Traits classes and implementation details for retriangulation or
 *  link rewriting functions.
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
 * Represents different options that can be used internally with a
 * retriangulation or link rewriting function.
 *
 * These options are intended to be used as template arguments.  As such,
 * they are not wrapped into the safer Flags type (since C++20 does not allow
 * non-type template arguments with private members).  Instead this is an
 * unscoped enumeration, and these integer constants should be combined
 * directly using the bitwise OR operator.
 *
 * \ingroup detail
 */
enum {
    /**
     * An empty flag, indicating that we should use default behaviour.
     */
    RetriangulateDefault = 0x0000,

    /**
     * Indicates that the retriangulation/rewrite action should _not_ be
     * protected by a mutex.  Instead the user is responsible for managing
     * locks to prevent race conditions when running in multithreaded mode.
     */
    RetriangulateNoLocks = 0x0001,
};

/**
 * Declares the internal type used to store a callable action that is passed
 * to a retriangulation or link rewriting function.
 *
 * A retriangulation or link rewriting function can work with arbitrary
 * callable objects.  However, the _implementations_ of such functions are
 * long and should not be dragged into the main headers.  The core purpose
 * of this class is therefore to coalesce the arbitrary action types
 * down to just _two_ fixed types (depending on whether the action includes a
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
 * \tparam flags controls how the retriangulation/rewriting process is managed;
 * see the RetriangulationOptions enum for what can be included here.
 * \tparam PropagationOptions Any options needed to specify how objects should
 * be propagated to produce "nearby" objects.  The domain-specific propagation
 * function `RetriangulationParams<Object>::propagateFrom<Retriangulator>`
 * can access this parameter via the type `Retriangulator::PropagationOptions`.
 *
 * \param obj the object being retriangulated or rewritten.
 * \param bool rigid \c true if link diagrams should never be reflected,
 * rotated and/or reversed.  This argument is currently ignored when working
 * with triangulations.
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
template <class Object, bool withSig, int flags = RetriangulateDefault,
    typename PropagationOptions = void>
bool retriangulateInternal(const Object& obj, bool rigid, int height,
        int nThreads, ProgressTrackerOpen* tracker,
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

/**
 * The common implementation of all exhaustive simplification functions.
 *
 * This routine performs exactly the task described by
 * Link::simplifyExhaustive() or Triangluation<dim>::simplifyExhaustive()
 * (for those dimensions where it is defined), with the following differences:
 *
 * - This routine assumes any preconditions have already been checked, and
 *   exceptions thrown if they failed.
 *
 * See Triangulation<dim>::simplifyExhaustive() or Link::simplifyExhaustive()
 * for full details on what this routine actually does.
 *
 * \tparam Object the class providing the exhaustive simplification function,
 * such as regina::Triangulation<dim> or regina::Link.
 * \tparam PropagationOptions Any options needed to specify how objects should
 * be propagated to produce "nearby" objects.  This will be passed through to
 * retriangulateInternal(), and is ultimately used by the domain-specific
 * function `RetriangulationParams<Object>::propagateFrom<Retriangulator>`.
 *
 * \param obj the object being simplified.
 * \param height the maximum number of top-dimensional simplices or crossings
 * to allow beyond the initial number in \a obj, or a negative number if
 * this should not be bounded.
 * \param threads the number of threads to use.  If this is 1 or smaller then
 * the routine will run single-threaded.
 * \param tracker a progress tracker through which progress will be reported,
 * or \c null if no progress reporting is required.
 * \return \c true if and only if the given object was simplified.
 *
 * \ingroup detail
 */
template <class Object, typename PropagationOptions = void>
bool simplifyExhaustiveInternal(Object& obj, int height,
        int threads, ProgressTrackerOpen* tracker) {
    // Make a place for the callback to put a simplified object, if it finds
    // one.  Afterwards we will move this into obj, since the change to obj
    // must happen on the calling thread.  The upshot is that we end up moving
    // the result twice (not once), but moves are cheap and thread safety
    // matters.
    std::unique_ptr<Object> simplified;

    size_t initSize = obj.size();
    if (retriangulateInternal<Object, false, RetriangulateDefault,
            PropagationOptions>(obj, true /* rigid */, height, threads, tracker,
            [&simplified, initSize](Object&& alt) {
                if (alt.size() < initSize) {
                    simplified.reset(new Object(std::move(alt)));
                    return true;
                } else
                    return false;
            })) {
        typename Object::PacketChangeGroup span(obj);
        obj = std::move(*simplified);
        obj.simplify();
        return true;
    } else
        return false;
}

/**
 * The common implementation of all exhaustive treewidth improvement functions,
 * which aim to rewrite/retriangulate the given link diagram or triangulation
 * to become one with a smaller-width greedy tree decomposition.
 *
 * This routine performs exactly the task described by
 * Link::improveTreewidth() or Triangluation<dim>::improveTreewidth()
 * (for those dimensions where it is defined), with the following differences:
 *
 * - This routine assumes any preconditions have already been checked, and
 *   exceptions thrown if they failed.
 *
 * See Triangulation<dim>::improveTreewidth() or Link::improveTreewidth()
 * for full details on what this routine actually does.
 *
 * \tparam Object the class providing the exhaustive treewidth improvement
 * function, such as regina::Triangulation<dim> or regina::Link.
 * \tparam PropagationOptions Any options needed to specify how objects should
 * be propagated to produce "nearby" objects.  This will be passed through to
 * retriangulateInternal(), and is ultimately used by the domain-specific
 * function `RetriangulationParams<Object>::propagateFrom<Retriangulator>`.
 *
 * \param obj the object whose greedy tree decomposition we hope to improve.
 * \param maxAttempts the maximum number of combinatorially distinct objects
 * to examine before we give up and return \c false, or a negative number if
 * this should not be bounded.
 * \param height the maximum number of additional top-dimensional simplices or
 * crossings to allow, or a negative number if this should not be bounded.
 * \param threads the number of threads to use.  If this is 1 or smaller then
 * the routine will run single-threaded.
 * \param tracker a progress tracker through which progress will be reported,
 * or \c null if no progress reporting is required.
 * \return \c true if and only if an object with a smaller-width greedy
 * tree decomposition was found.
 *
 * \ingroup detail
 */
template <class Object, typename PropagationOptions = void>
bool improveTreewidthInternal(Object& obj, ssize_t maxAttempts, int height,
        int threads, ProgressTrackerOpen* tracker) {
    // Make a place for the callback to put an improved object, if it finds
    // one.  Afterwards we will move this into obj, since the change to obj
    // must happen on the calling thread.  The upshot is that we end up moving
    // the result twice (not once), but moves are cheap and thread safety
    // matters.
    std::unique_ptr<Object> improved;
    size_t attempts = 0;

    size_t init = TreeDecomposition(obj).width();
    size_t curr = init;

    // Since computing tree decompositions is non-trivial, we will run our
    // action outside the usual retriangulate/rewrite locks and instead manage
    // the locks ourselves.
    // This mutex protects the variables: improved, attempts, curr.
    std::mutex mutex_;

    // Make a first attempt to reduce treewidth.
    if (retriangulateInternal<Object, false, RetriangulateNoLocks,
            PropagationOptions>(obj, true /* rigid */, height, threads, tracker,
            [&improved, &attempts, &mutex_, &curr, init,
            maxAttempts](Object&& alt) {
                size_t w = TreeDecomposition(alt).width();

                std::unique_lock lock(mutex_);
                ++attempts;
                if (w < init) {
                    improved.reset(new Object(std::move(alt)));
                    curr = w;
                    return true;
                } else if (maxAttempts >= 0 && attempts >= maxAttempts) {
                    return true;
                } else
                    return false;
            })) {
        // We explicitly asked the search to stop.
        // Either we improved the treewidth (in which case we fall through and
        // continue searching below), or we exhausted our budgeted number of
        // attempts (in which case we return now).
        if (! improved)
            return false;
    } else {
        // We exhausted the entire flip graph (up to the given height) and did
        // not find any improvement.
        return false;
    }

    // We improved the treewidth, and so we will definitely be changing obj.
    // See how much further we can reduce it now.
    while (true) {
        attempts = 0;
        init = curr;

        if (retriangulateInternal<Object, false, RetriangulateNoLocks,
                PropagationOptions>(*improved, true /* rigid */, height,
                threads, tracker, [&improved, &attempts, &mutex_, &curr,
                init, maxAttempts](Object&& alt) {
                    size_t w = TreeDecomposition(alt).width();

                    std::unique_lock lock(mutex_);
                    ++attempts;
                    if (w < init) {
                        // Note: we are explicitly allowed to change the object
                        // that we are retriangulating/rewriting.
                        *improved = std::move(alt);
                        curr = w;
                        return true;
                    } else if (maxAttempts >= 0 && attempts >= maxAttempts) {
                        return true;
                    } else
                        return false;
                })) {
            // We explicitly asked the search to stop.
            // Either we improved the treewidth (in which case we loop around
            // and try again), or we exhausted our budgeted number of attempts
            // (in which case we finish and return what we've got).
            if (curr == init)
                break;
        } else {
            // We exhausted the entire flip graph (up to the given height) and
            // did not find any improvement.
            break;
        }
    }

    // Return with whatever improvement we managed to make.
    obj = std::move(*improved);
    return true;
}

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

