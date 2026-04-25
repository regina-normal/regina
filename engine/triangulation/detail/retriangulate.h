
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

#include <concepts>
#include <functional>
#include "utilities/typeutils.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

class ProgressTrackerOpen;

namespace detail {

/**
 * Provides domain-specific details for the retriangulation or link
 * rewriting process, specific to a particular triangulation or link class.
 *
 * Every class (e.g., `Triangulation<dim>` or `Link`) that uses
 * Regina's generic retriangulation / link rewriting machinery must provide
 * its own specialisation of RetriangulateParams.
 *
 * The specialisation must provide:
 *
 * - a type alias `Signature`, indicating the type used to store object
 *   signatures (e.g., isomorphism signatures of triangulations, or knot/link
 *   signatures);
 *
 * - a template function `static void propagateFrom(sig, max, options, action)`
 *   and accompanying type alias `PropagationOptions`, whose task is to identify
 *   objects that are "nearby" to an input object (e.g., via Pachner moves or
 *   Reidemeister moves), as described below;
 *
 * - a static constexpr member `const char* progressStage`, which
 *   returns the human-readable description of the processing stage that
 *   will be set up in the progress tracker;
 *
 * - a function `static Signature sig(const Object&)`, which returns the
 *   signature of a triangulation or link, used to identify the triangulation
 *   or link up to some appropriate notion of combinatorial equivalence;
 *
 * - a function `static Signature rigidSig(const Object&)`, which returns
 *   the same kind of signature, but without allowing reflection, reversal
 *   and/or rotation of link diagrams.  For triangulations, rigidity options
 *   are currently ignored and so rigidSig() should return the same signature
 *   as sig().
 *
 * The function `static void propagateFrom(sig, max, options, action)` takes
 * the following arguments:
 *
 * - \a sig is an object signature (e.g., the isomorphism signature of a
 *   triangulation or the knot signature of a link), typically passed as a
 *   `const Signature&`;
 * - \a max is the maximum size() of the "nearby" objects that we are allowed
 *   to consider, typically passed as a `size_t`;
 * - \a options controls which moves to "nearby" objects are allowed (as
 *   discussed further below), and is of type `PropagationOptions`;
 * - \a action is a template argument that adheres to the concept
 *   `TerminatingCallback<Link&&, const Signature&>`, and `propagateFrom()`
 *   should call this for each nearby object that it identifies (as discussed
 *   below).
 *
 * Your implementation of `propagateFrom()` should:
 *
 * - reconstruct a triangulation or link \a obj from \a sig;
 * - examine all possible moves from \a obj to "nearby" objects that do not
 *   exceed size \a max, using \a options to control which moves are allowed;
 * - for each such "nearby" object \a alt, call `action(std::move(alt), sig)`,
 *   noting that \a sig is the original signature passed to `propagateFrom()`,
 *   _not_ the signature of the newly-constructed nearby object;
 * - check the return value from `action()`, and if this ever returns `true`
 *   then stop trying moves, clean up and return immediately.
 *
 * Regarding the \a options argument:
 *
 * - this controls which moves are allowed (e.g., the link rewriting process
 *   uses \a options to indicate whether to allow virtual as well as classical
 *   moves);
 * - its type `PropagationOptions` must be simple enough that its values can be
 *   used as template arguments (e.g., integer or enum types are fine but
 *   complex class types are not), and it must be default constructible;
 * - if you do not need options then you should define `PropagationOptions`
 *   to be the empty type `NoPropagationOptions`, and your `propagateFrom()`
 *   should just ignore the \a options argument.
 *
 * Note that the retriangulation/rewriting machinery also requires \a Object
 * to have a copy constructor that takes a second boolean argument, indicating
 * whether computed properties should be cloned.  This (as well as the
 * requirements on `RetriangulateParams<Object>`) is enforced by the concept
 * `Retriangulable<Object>`.
 *
 * \apinotfinal
 *
 * \tparam Object the class that provides the retriangulation/rewriting
 * function, such as `Triangulation<dim>` or `Link`.
 */
template <typename Object>
struct RetriangulateParams;

/**
 * An empty struct type used to indicate that a particular type of object
 * (such as a triangulation or link type) has no options to control
 * propagation to "nearby" objects.
 *
 * This type is used with Regina's retriangulation / link rewriting machinery,
 * and would typically be used as the type
 * `RetriangulateParams<Object>::PropagationOptions` in the case where there
 * are no options available.
 *
 * See the RetriangulateParams documentation for further details.
 */
struct NoPropagationOptions {};

/**
 * A class that can be used with Regina's generic retriangulation / link
 * rewriting machinery.
 *
 * Examples of such classes are `Triangulation<3>`, `Triangulation<4>`, and
 * Link.
 *
 * Regarding semantics:
 *
 * - This concept enforces the requirements for the specialisation
 *   `RetriangulationParams<T>`.  See RetriangulationParams for a thorough
 *   discussion of the semantic requirements here.
 *
 * - This concept also requires a copy constructor with an extra boolean
 *   argument.  That boolean argument should indicate whether computed
 *   properties are to be cloned.
 *
 * \ingroup triangulation
 */
template <typename T>
concept Retriangulable =
    std::constructible_from<T, const T&, bool> &&
    requires(const T x, size_t max) {
        typename RetriangulateParams<T>;

        typename RetriangulateParams<T>::Signature;
        requires std::copyable<typename RetriangulateParams<T>::Signature>;

        typename RetriangulateParams<T>::PropagationOptions;
        requires std::default_initializable<
            typename RetriangulateParams<T>::PropagationOptions>;

        { RetriangulateParams<T>::progressStage } ->
            std::convertible_to<const char*>;
        { RetriangulateParams<T>::sig(x) } ->
            std::same_as<typename RetriangulateParams<T>::Signature>;
        { RetriangulateParams<T>::rigidSig(x) } ->
            std::same_as<typename RetriangulateParams<T>::Signature>;

        #if defined(__GNUC__) && ! defined(__clang__)
        // The constraint on propagateFrom() causes an internal compiler error
        // under gcc-13 and gcc-14 (this is fixed in gcc-15).  For now we only
        // enforce the constraint under clang, which handles it fine.
        #else
        RetriangulateParams<T>::propagateFrom(
            std::declval<typename RetriangulateParams<T>::Signature>(), max,
            typename RetriangulateParams<T>::PropagationOptions(),
            [](T&&, const typename RetriangulateParams<T>::Signature&) {
                return false;
            });
        #endif
    };

/**
 * Represents flags that control the behaviour of Regina's retriangulation /
 * link rewriting machinery, and that apply to all supported object types.
 *
 * These flags are intended to be used as template arguments.  As such,
 * they are not wrapped into the safer Flags type (since C++20 does not allow
 * non-type template arguments with private members).  Instead this is an
 * unscoped enumeration, and these integer constants should be combined
 * directly using the bitwise OR operator.
 *
 * \ingroup detail
 */
enum RetriangulationFlags {
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

    /**
     * Indicates that, if a progress tracker is used, it should _not_ be marked
     * as finished once the retriangulation process is completed.  Instead the
     * caller is responsible for calling `ProgressTracker::setFinished()`
     * themselves at an appropriate time.
     */
    RetriangulateNotFinished = 0x0002
};

/**
 * The type used to hold options that control propagation to "nearby" objects
 * in Regina's retriangulation / link rewriting machinery.  Unlike
 * RetriangulationFlags, these options are specific to a particular \a Object
 * type.
 *
 * The main reason for this type alias is to simplify the code that actually
 * works with propagation options.
 *
 * \ingroup detail
 */
template <Retriangulable Object>
using PropagationOptions = RetriangulateParams<Object>::PropagationOptions;

/**
 * Declares the internal type used to store a callable action that is passed
 * to a retriangulation or link rewriting function.
 *
 * A retriangulation or link rewriting function can work with arbitrary
 * callable types.  However, the _implementations_ of such functions are
 * long and should not be dragged into the main headers.  The core purpose
 * of this class is therefore to coalesce the arbitrary action types
 * down to just _two_ fixed types (depending on whether the action includes a
 * signature (e.g., an isomorphism signature) in its initial argument(s)).
 * This means that the retriangulation or rewriting code can be templated on
 * a single boolean parameter, and so we can instatiate it completely in
 * Regina's library and keep the implementation details out of the main headers.
 *
 * The current implementation packages the action up as a std::function object
 * with either a single argument (a triangulation/link) or a pair of arguments
 * (a signature and a triangulation/link).  Any additional arguments to the
 * retriangulation/rewriting action will be bound in the std::function object).
 * This implementation is subject to change in future versions of Regina.
 *
 * \tparam Object the class providing the retriangulation/rewriting function,
 * such as regina::Triangulation<dim> or regina::Link.
 * \tparam withSig \c true if we are storing an action that includes both a
 * signature and a triangulation/link in its initial argument(s), or \c false
 * if we are storing an action whose argument list begins with just a
 * triangulation/link.
 *
 * \ingroup detail
 */
template <Retriangulable Object, bool withSig>
using RetriangulateActionFunc = std::conditional_t<withSig,
    std::function<bool(
        const typename RetriangulateParams<Object>::Signature&, Object&&)>,
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
 * signature and a triangulation/link in its initial argument(s), or \c false
 * if we are storing an action whose argument list begins with just a
 * triangulation/link.
 * \tparam flags controls how the overall retriangulation/rewriting process is
 * managed; see the RetriangulationFlags enum for what can be included here.
 * \tparam options any options specific to the Object type that control
 * propagation to "nearby" objects.
 *
 * \param obj the object being retriangulated or rewritten.
 * \param rigid \c true if link diagrams should never be reflected,
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
template <Retriangulable Object, bool withSig, int flags = RetriangulateDefault,
    PropagationOptions<Object> options = {}>
bool retriangulateInternal(const Object& obj, bool rigid, int height,
        int nThreads, ProgressTrackerOpen* tracker,
        RetriangulateActionFunc<Object, withSig>&& action);

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
 * \tparam options any options specific to the Object type that control
 * propagation to "nearby" objects.
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
template <Retriangulable Object, PropagationOptions<Object> options = {}>
bool simplifyExhaustiveInternal(Object& obj, int height,
        int threads, ProgressTrackerOpen* tracker) {
    // Make a place for the callback to put a simplified object, if it finds
    // one.  Afterwards we will move this into obj, since the change to obj
    // must happen on the calling thread.  The upshot is that we end up moving
    // the result twice (not once), but moves are cheap and thread safety
    // matters.
    std::unique_ptr<Object> simplified;

    size_t initSize = obj.size();
    if (retriangulateInternal<Object, false, RetriangulateDefault, options>(
            obj, true /* rigid */, height, threads, tracker,
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
 * \tparam options any options specific to the Object type that control
 * propagation to "nearby" objects.
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
template <Retriangulable Object, PropagationOptions<Object> options = {}>
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
    if (retriangulateInternal<Object, false,
            RetriangulateNoLocks | RetriangulateNotFinished, options>(
            obj, true /* rigid */, height, threads, tracker,
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
        if (! improved) {
            if (tracker)
                tracker->setFinished();
            return false;
        }
    } else {
        // We exhausted the entire flip graph (up to the given height) and did
        // not find any improvement.
        if (tracker)
            tracker->setFinished();
        return false;
    }

    // We improved the treewidth, and so we will definitely be changing obj.
    // If max #attempts was limited, then we will be finishing soon anyway,
    // so see how much further we can reduce it now.
    if (maxAttempts >= 0) {
        while (true) {
            attempts = 0;
            init = curr;

            if (retriangulateInternal<Object, false,
                    RetriangulateNoLocks | RetriangulateNotFinished, options>(
                    *improved, true /* rigid */, height,
                    threads, tracker, [&improved, &attempts, &mutex_, &curr,
                    init, maxAttempts](Object&& alt) {
                        size_t w = TreeDecomposition(alt).width();

                        std::unique_lock lock(mutex_);
                        ++attempts;
                        if (w < init) {
                            // Note: we are explicitly allowed to change the
                            // object that we are retriangulating/rewriting.
                            *improved = std::move(alt);
                            curr = w;
                            return true;
                        } else if (maxAttempts >= 0 &&
                                attempts >= maxAttempts) {
                            return true;
                        } else
                            return false;
                    })) {
                // We explicitly asked the search to stop.
                // Either we improved the treewidth (in which case we loop
                // around and try again), or we exhausted our budgeted number
                // of attempts (in which case stop and return what we've got).
                if (curr == init)
                    break;
            } else {
                // We exhausted the entire flip graph (up to the given height)
                // and did not find any improvement.
                break;
            }
        }
    }

    // Return with whatever improvement we managed to make.
    obj = std::move(*improved);
    if (tracker)
        tracker->setFinished();
    return true;
}

} } // namespace regina::detail

#endif

