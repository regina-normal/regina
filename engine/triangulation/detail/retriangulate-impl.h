
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

/**
 *! \file triangulation/detail/retriangulate-impl.h
 *  \brief Full implementation details for the retriangulation and
 *  link rewriting functions.
 *
 *  This file is _not_ included from triangulation.h or link.h, and it is not
 *  shipped with Regina's development headers.  The routines it contains are
 *  explicitly instantiated in Regina's calculation engine for all dimensions.
 *
 *  The reason for "quarantining" this file is so that the helper function and
 *  classes it defines are not inadvertently made accessible to end users.
 */

#ifndef __REGINA_RETRIANGULATE_IMPL_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_RETRIANGULATE_IMPL_H_DETAIL
#endif

#include <condition_variable>
#include <limits>
#include <map>
#include <mutex>
#include <queue>
#include <set>
#include <system_error>
#include <thread>
#include "progress/progresstracker.h"
#include "triangulation/detail/retriangulate.h"

namespace regina::detail {

#ifndef __DOXYGEN

/**
 * Provides domain-specific details for the retriangulation or link
 * rewriting process, specific to a particular triangulation or link class.
 *
 * Every class (e.g., regina::Triangulation<dim> or regina::Link) that uses this
 * retriangulation code must provide its own specialisation of
 * RetriangulateParams.
 *
 * The specialisation should provide:
 *
 * - a template function
 *   `static void propagateFrom<T>(sig, max, retriangulator)`,
 *   as described below;
 *
 * - a static constexpr member `const char* progressStage`, which
 *   returns the human-readable description of the processing stage that
 *   will be set up in the progress tracker;
 *
 * - a function `static std::string sig(const Object&)`, which
 *   returns the text signature that is used to identify a triangulation
 *   or link up to the appropriate notion of combinatorial equivalence.
 *
 * The function `static void propagateFrom<T>(sig, max, retriangulator)`
 * takes the following arguments:
 *
 * - \a sig is the isomorphism signature of a triangulation or the knot
 *   signature of a link (typically passed as a const std::string&);
 * - \a max is the maximum size() of a triangulation or link that we are
 *   allowed to consider (typically passed as a \c size_t);
 * - \a retriangulator is the instance of the Retriangulator class that
 *   is managing the overall retriangulation/rewriting process (which must be
 *   passed as a pointer to type \a T).
 *
 * The template parameter \a T is the type of the managing Retriangulator class,
 * with all of the Retriangulator template parameters filled in.
 *
 * The function should reconstruct a triangulation or link \a obj from \a sig;
 * examine all possible moves from \a obj that do not exceed size \a max; and
 * for each resulting triangulation/link \a alt, it should call
 * `retriangulator->candidate(std::move(alt), sig)`.
 *
 * The function should also check the return value each time it calls
 * `retriangulator->candidate(...)`.  If the \a candidate
 * function ever returns \c true then it should not try any further moves, but
 * instead should clean up and return immediately.
 *
 * \apinotfinal
 *
 * \pre The class \a Object provides a two-argument copy constructor,
 * where the second argument is a boolean indicating whether computed
 * properties should be cloned.  All of Regina's triangulation and link
 * classes have such a constructor.
 *
 * \tparam Object the class that provides the retriangulation/rewriting
 * function, such as regina::Triangulation<dim> or regina::Link.
 */
template <class Object>
struct RetriangulateParams;

/**
 * A helper class that manages multithreading for the main Retriangulator class.
 * A single instance of this class is shared by all threads.
 *
 * \tparam threading \c true if and only if multithreading is enabled.
 */
template <bool threading>
class RetriangulateThreadSync;

template <>
class RetriangulateThreadSync<true> {
    protected:
        unsigned nRunning_;
        std::mutex mutex_;
        std::condition_variable cond_;
        bool done_;

    public:
        class Lock : public std::unique_lock<std::mutex> {
            public:
                Lock(RetriangulateThreadSync& self) :
                        std::unique_lock<std::mutex>(self.mutex_) {
                }
        };

    public:
        RetriangulateThreadSync() : done_(false) {}

        // Make this class non-copyable.
        RetriangulateThreadSync(const RetriangulateThreadSync&) = delete;
        RetriangulateThreadSync& operator = (
            const RetriangulateThreadSync&) = delete;

        /**
         * \pre The calling thread currently holds the lock on \a mutex.
         */
        bool done() const {
            return done_;
        }

        /**
         * \pre The calling thread currently holds the lock on \a mutex.
         */
        void setDone() {
            done_ = true;
        }

        template <class RetriangulatorType>
        void startThreads(unsigned nThreads, ProgressTrackerOpen* tracker) {
            nRunning_ = nThreads;

            auto* t = new std::thread[nThreads];
            unsigned i;

            // In the std::thread constructor, we _must_ pass \c this as a
            // pointer - otherwise we may end up making deep copies instead.
            for (i = 0; i < nThreads; ++i)
                t[i] = std::thread(&RetriangulatorType::processQueue,
                    static_cast<RetriangulatorType*>(this), tracker);
            for (i = 0; i < nThreads; ++i)
                t[i].join();

            delete[] t;
        }

        /**
         * \pre The calling thread currently holds the lock on \a mutex.
         */
        void wakeAllThreads() {
            cond_.notify_all();
        }

        /**
         * \pre The calling thread currently holds the lock on \a mutex,
         * via the Lock object which is being passed.  The lock will
         * still be held on return, but it may be temporarily released
         * if the thread needs to wait on other threads to complete some tasks.
         *
         * If this function returns \c true, it means it decremented
         * \a nRunning_, and it should not do any further work.
         *
         * If this function returns \c false, it means that some other thread
         * has given this thread more work to do.  In this case \a nRunning_
         * is unaltered, and this thread should continue processing the
         * work queue.
         */
        bool notifyFinished(Lock& lock) {
            if (--nRunning_ == 0) {
                // Everybody has finished.
                // Wake up the other threads so they can exit also.
                cond_.notify_all();
                return true;
            } else {
                // We have finished, but somebody else is still running.
                // It is possible (but not certain) that the queue will be
                // refilled from another thread and we will need to resume
                // processing.
                cond_.wait(lock);

                // We woke up for one of two reasons:
                // 1) nRunning = 0, which means we are done;
                // 2) nRunning > 0, and somebody pushed something new
                // onto the queue.
                if (nRunning_ == 0 || done_)
                    return true;
                else {
                    ++nRunning_;
                    return false;
                }
            }
        }
};

template <>
class RetriangulateThreadSync<false> {
    protected:
        bool done_;

    public:
        RetriangulateThreadSync() : done_(false) {}

        bool done() const {
            return done_;
        }

        void setDone() {
            done_ = true;
        }

        class Lock {
            public:
                Lock(RetriangulateThreadSync&) {}
                void lock() {}
                void unlock() {}
        };

        template <class RetriangulatorType>
        void startThreads(unsigned, ProgressTrackerOpen* tracker) {
            static_cast<RetriangulatorType*>(this)->processQueue(tracker);
        }

        void wakeAllThreads() {
        }

        bool notifyFinished(Lock&) {
            return true;
        }
};

/**
 * A helper class that manages optional backtracing. This means that,
 * when a retriangulation/rewriting path is found for which the
 * user-suppled action returns \c true, Regina can output the series of
 * triangulations or links that it passed through to get there.
 *
 * The backtracing is not well optimised, and moreover it simply dumps the
 * backtrace to std::cerr.
 *
 * Therefore, this backtracing code is disabled at the source code level
 * and not available at all through the API. To turn it on, you will
 * need to edit this source file and change the type alias
 * Retriangulator::SigSet from RetriangulateSigGraph<false> to
 * RetriangulateSigGraph<true>.
 *
 * \tparam backtrace \c true if and only if backtracing is enabled.
 */
template <bool backtrace>
class RetriangulateSigGraph;

/**
 * This keeps a map whose keys are all signatures seen so far,
 * and whose corresponding values are the signatures from which they
 * were derived.  Seed signatures (the starting points of the search)
 * have associated values of the empty string.
 */
template <>
class RetriangulateSigGraph<true> : private std::map<std::string, std::string> {
    public:
        using std::map<std::string, std::string>::iterator;

        auto insertStart(const std::string& sig) {
            return insert(std::make_pair(sig, std::string()));
        }

        auto insertDerived(const std::string& sig, const std::string& from) {
            return insert(std::make_pair(sig, from));
        }

        static const std::string& sigAt(iterator it) {
            return it->first;
        }

        void backtrace(const std::string& sig) const {
            std::cerr << sig;
            for (auto it = find(sig); ! it->second.empty();
                    it = find(it->second)) {
                std::cerr << " <- " << it->second;
            }
            std::cerr << std::endl;
        }
};

template <>
class RetriangulateSigGraph<false> : private std::set<std::string> {
    public:
        using std::set<std::string>::iterator;

        auto insertStart(const std::string& sig) {
            return insert(sig);
        }

        auto insertDerived(const std::string& sig, const std::string&) {
            return insert(sig);
        }

        static const std::string& sigAt(iterator it) {
            return *it;
        }

        void backtrace(const std::string& sig) const {
        }
};

/**
 * Provides the main implementation of the retriangulation and link
 * rewriting code.
 *
 * \tparam Object the class that provides the retriangulation/rewriting
 * function, such as regina::Triangulation<dim> or regina::Link.
 * \tparam threading \c true if and only if multithreading is enabled.
 * \tparam withSig \c true if the action that was passed takes a
 * text signature as its initial argument (in addition to the
 * triangulation/link which is passed in all cases).
 */
template <class Object, bool threading, bool withSig>
class Retriangulator : public RetriangulateThreadSync<threading> {
    private:
        // To switch on backtracing, just change the following type alias to
        // RetriangulateSigGraph<true>.
        using SigSet = RetriangulateSigGraph<false>;

        const size_t maxSize_;
        RetriangulateActionFunc<Object, withSig> action_;

        SigSet sigs_;
        std::priority_queue<SigSet::iterator,
            std::vector<SigSet::iterator>,
            std::function<bool(SigSet::iterator, SigSet::iterator)>>
            process_;

    public:
        /**
         * The priority function used to choose which triangulation or
         * link to process next.
         */
        static bool lowerPriority(SigSet::iterator a, SigSet::iterator b) {
            // The function should compute whether a.size > b.size,
            // where "size" measures number of top-dimensional simplices
            // for triangulations or number of crossings for link diagrams.
            //
            // Here we use the string length, which is fast to compute,
            // and which should have the same effect for knots (i.e.,
            // single-component links) and also for triangulations
            // with the same number of boundary facets (which is
            // preserved under Pachner moves).
            return SigSet::sigAt(a).length() > SigSet::sigAt(b).length();
        }

    public:
        Retriangulator(size_t maxSize,
                RetriangulateActionFunc<Object, withSig>&& action) :
            maxSize_(maxSize), action_(action), process_(lowerPriority) {
        }

        // Make this class non-copyable.
        Retriangulator(const Retriangulator&) = delete;
        Retriangulator& operator = (const Retriangulator&) = delete;

        /**
         * Sets up the initial triangulation/link from which our search will
         * start.  This should be called before any multithreading begins.
         */
        bool seed(const Object& obj);

        /**
         * The main function for each worker thread.
         * This should only be called by startThreads(), and this should
         * happen after seed() has been called.
         */
        void processQueue(ProgressTrackerOpen* tracker);

        /**
         * This function is called from the RetriangulateParams class, whose
         * implementation is specific to \a Object (i.e., the underlying
         * triangulation class).
         *
         * The argument \a derivedFrom is to support optional backtracing.
         * This should be the signature of the previously-found object from
         * which a single move (e.g., a Pachner or Reidemeister move)
         * produced \a alt.
         */
        bool candidate(Object&& alt, const std::string& derivedFrom);
};

template <class Object, bool threading, bool withSig>
inline bool Retriangulator<Object, threading, withSig>::seed(
        const Object& obj) {
    // We have to pass a *copy* of obj to action_, since action_ is
    // allowed to change the object that is passed to it.
    // This is inefficient, but at least it only happens once.
    const std::string sig = RetriangulateParams<Object>::sig(obj);
    {
        Object copy(obj, false);
        if constexpr (withSig) {
            if (action_(sig, std::move(copy))) {
                sigs_.backtrace(sig);
                RetriangulateThreadSync<threading>::setDone();
                return true;
            }
        } else {
            if (action_(std::move(copy))) {
                sigs_.backtrace(sig);
                RetriangulateThreadSync<threading>::setDone();
                return true;
            }
        }
    }
    process_.push(sigs_.insertStart(sig).first);
    return false;
}

template <class Object, bool threading, bool withSig>
void Retriangulator<Object, threading, withSig>::processQueue(
        ProgressTrackerOpen* tracker) {
    SigSet::iterator next;

    typename RetriangulateThreadSync<threading>::Lock lock(*this);

    while (true) {
        // Process the queue until either done_ is true, or there is
        // nothing left to process.
        while (! (RetriangulateThreadSync<threading>::done() ||
                process_.empty())) {
            if (tracker && tracker->isCancelled())
                break;

            next = process_.top();
            process_.pop();

            // We can do our expensive propagation outside the mutex,
            // since the C++ standard requires that insertion into a
            // std::set or std::map does not invalidate iterators.
            lock.unlock();
            RetriangulateParams<Object>::template propagateFrom<Retriangulator>(
                SigSet::sigAt(next), maxSize_, this);
            lock.lock();

            if (tracker)
                tracker->incSteps();
        }

        // It looks like we're finished.  Tell the other threads we're done,
        // and see if one of them gives us more work to do.
        if (RetriangulateThreadSync<threading>::notifyFinished(lock))
            return;
    }
}

template <class Object, bool threading, bool withSig>
bool Retriangulator<Object, threading, withSig>::candidate(
        Object&& alt, const std::string& derivedFrom) {
    const std::string sig = RetriangulateParams<Object>::sig(alt);

    typename RetriangulateThreadSync<threading>::Lock lock(*this);

    if (RetriangulateThreadSync<threading>::done())
        return false;

    auto result = sigs_.insertDerived(sig, derivedFrom);
    if (result.second) {
        // We have not seen this triangulation before.
        if (process_.empty()) {
            process_.push(result.first);

            // Wake up any other threads that had previously emptied
            // the queue.
            RetriangulateThreadSync<threading>::wakeAllThreads();
        } else
            process_.push(result.first);

        if constexpr (withSig) {
            if (action_(sig, std::move(alt))) {
                sigs_.backtrace(sig);
                RetriangulateThreadSync<threading>::setDone();
                return true;
            }
        } else {
            if (action_(std::move(alt))) {
                sigs_.backtrace(sig);
                RetriangulateThreadSync<threading>::setDone();
                return true;
            }
        }
    }
    return false;
}

template <class Object, bool threading, bool withSig>
bool enumerateDetail(const Object& obj, int height, unsigned nThreads,
        ProgressTrackerOpen* tracker,
        RetriangulateActionFunc<Object, withSig>&& action) {
    if (tracker)
        tracker->newStage(RetriangulateParams<Object>::progressStage);

    if (obj.isEmpty()) {
        // There are no moves possible on empty links or empty triangulations.
        if (tracker)
            tracker->setFinished();
        return false;
    }

    using T = Retriangulator<Object, threading, withSig>;

    T bfs((height >= 0 ? obj.size() + height :
        std::numeric_limits<std::size_t>::max()), std::move(action));
    if (bfs.seed(obj)) {
        if (tracker)
            tracker->setFinished();
        return true;
    }
    bfs.template startThreads<T>(nThreads, tracker);

    if (tracker)
        tracker->setFinished();
    return bfs.done();
}

template <class Object, bool withSig>
bool retriangulateInternal(const Object& obj, int height, unsigned nThreads,
        ProgressTrackerOpen* tracker,
        RetriangulateActionFunc<Object, withSig>&& action) {
    if (nThreads <= 1) {
        return enumerateDetail<Object, false, withSig>(
            obj, height, nThreads, tracker, std::move(action));
    } else {
        return enumerateDetail<Object, true, withSig>(
            obj, height, nThreads, tracker, std::move(action));
    }
}

#endif // __DOXYGEN

} // namespace regina::detail

#endif
