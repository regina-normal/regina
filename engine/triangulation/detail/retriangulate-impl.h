
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

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina::detail {

#ifndef __DOXYGEN

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

        template <typename Action, typename... Args>
        requires std::invocable<Action, Args...>
        void startThreads(int nThreads, Action&& action, Args&&... args) {
            nRunning_ = nThreads;

            FixedArray<std::thread> t(nThreads);

            for (int i = 0; i < nThreads; ++i)
                t[i] = std::thread(std::forward<Action>(action),
                    std::forward<Args>(args)...);
            for (int i = 0; i < nThreads; ++i)
                t[i].join();
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

        template <typename Action, typename... Args>
        requires std::invocable<Action, Args...>
        void startThreads(int, Action&& action, Args&&... args) {
            std::invoke(std::forward<Action>(action),
                std::forward<Args>(args)...);
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
 * Retriangulator::SigSet from RetriangulateSigGraph<..., false> to
 * RetriangulateSigGraph<..., true>.
 *
 * \tparam backtrace \c true if and only if backtracing is enabled.
 */
template <typename Signature, bool backtrace>
class RetriangulateSigGraph;

/**
 * This keeps a map whose keys are all signatures seen so far,
 * and whose corresponding values are the signatures from which they
 * were derived.  Seed signatures (the starting points of the search)
 * have associated values of the empty signature.
 */
template <typename Signature>
class RetriangulateSigGraph<Signature, true> :
        private std::map<Signature, Signature> {
    public:
        using iterator = std::map<Signature, Signature>::iterator;

        auto insertStart(const Signature& sig) {
            return std::map<Signature, Signature>::emplace(sig, Signature());
        }

        auto insertDerived(const Signature& sig, const Signature& from) {
            return std::map<Signature, Signature>::emplace(sig, from);
        }

        static const Signature& sigAt(iterator it) {
            return it->first;
        }

        void backtrace(const Signature& sig) const {
            std::cerr << sig;
            for (auto it = find(sig); ! it->second.empty();
                    it = find(it->second)) {
                std::cerr << " <- " << it->second;
            }
            std::cerr << std::endl;
        }
};

template <typename Signature>
class RetriangulateSigGraph<Signature, false> : private std::set<Signature> {
    public:
        using iterator = std::set<Signature>::iterator;

        auto insertStart(const Signature& sig) {
            return std::set<Signature>::emplace(sig);
        }

        auto insertDerived(const Signature& sig, const Signature&) {
            return std::set<Signature>::emplace(sig);
        }

        static const Signature& sigAt(iterator it) {
            return *it;
        }

        void backtrace(const Signature&) const {
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
 * \tparam flags controls how the overall retriangulation/rewriting process is
 * managed; see the RetriangulationFlags enum for what can be included here.
 * \tparam options_ any options specific to the Object type that control
 * propagation to "nearby" objects.
 */
template <Retriangulable Object, bool threading, bool withSig, int flags,
    PropagationOptions<Object> options_>
class Retriangulator : public RetriangulateThreadSync<threading> {
    public:
        static constexpr PropagationOptions<Object> options = options_;

    private:
        // To switch on backtracing, just change the following type alias to
        // RetriangulateSigGraph<..., true>.
        using SigSet = RetriangulateSigGraph<
            typename RetriangulateParams<Object>::Signature, false>;

        const bool rigid_;
        const size_t maxSize_;
        RetriangulateActionFunc<Object, withSig> action_;
        ProgressTrackerOpen* tracker_;

        SigSet sigs_;
        std::priority_queue<typename SigSet::iterator,
            std::vector<typename SigSet::iterator>,
            std::function<bool(
                typename SigSet::iterator, typename SigSet::iterator)>>
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
            // Here we use the signature length, which is fast to compute,
            // and which should have the same effect for knots (i.e.,
            // single-component links) and also for triangulations
            // with the same number of boundary facets (which is
            // preserved under Pachner moves).
            return SigSet::sigAt(a).size() > SigSet::sigAt(b).size();
        }

    public:
        Retriangulator(bool rigid, size_t maxSize,
                RetriangulateActionFunc<Object, withSig>&& action,
                ProgressTrackerOpen* tracker) :
            rigid_(rigid), maxSize_(maxSize), action_(action),
            tracker_(tracker), process_(lowerPriority) {
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
        bool candidate(Object&& alt,
            const typename RetriangulateParams<Object>::Signature& derivedFrom);
};

template <Retriangulable Object, bool threading, bool withSig, int flags,
    PropagationOptions<Object> options_>
inline bool Retriangulator<Object, threading, withSig, flags, options_>::seed(
        const Object& obj) {
    // We have to pass a *copy* of obj to action_, since action_ is
    // allowed to change the object that is passed to it.
    // This is inefficient, but at least it only happens once.
    const typename RetriangulateParams<Object>::Signature sig = (rigid_ ?
        RetriangulateParams<Object>::rigidSig(obj) :
        RetriangulateParams<Object>::sig(obj));
    {
        // Note: for triangulations, this constructor clones locks also.
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

template <Retriangulable Object, bool threading, bool withSig, int flags,
    PropagationOptions<Object> options_>
void Retriangulator<Object, threading, withSig, flags, options_>::processQueue(
        ProgressTrackerOpen* tracker) {
    using Signature = typename RetriangulateParams<Object>::Signature;

    typename RetriangulateThreadSync<threading>::Lock lock(*this);

    while (true) {
        // Process the queue until either done_ is true, or there is
        // nothing left to process.
        while (! (RetriangulateThreadSync<threading>::done() ||
                process_.empty())) {
            if (tracker && tracker->isCancelled())
                break;

            auto next = process_.top();
            process_.pop();

            // We can do our expensive propagation outside the mutex,
            // since the C++ standard requires that insertion into a
            // std::set or std::map does not invalidate iterators.
            lock.unlock();
            RetriangulateParams<Object>::propagateFrom(
                SigSet::sigAt(next), maxSize_, options_,
                [this](Object&& alt, const Signature& derivedFrom) {
                    return candidate(std::move(alt), derivedFrom);
                });
            lock.lock();
        }

        // It looks like we're finished.  Tell the other threads we're done,
        // and see if one of them gives us more work to do.
        if (RetriangulateThreadSync<threading>::notifyFinished(lock))
            return;
    }
}

template <Retriangulable Object, bool threading, bool withSig, int flags,
    PropagationOptions<Object> options_>
bool Retriangulator<Object, threading, withSig, flags, options_>::candidate(
        Object&& alt,
        const typename RetriangulateParams<Object>::Signature& derivedFrom) {
    typename RetriangulateParams<Object>::Signature sig = (rigid_ ?
        RetriangulateParams<Object>::rigidSig(alt) :
        RetriangulateParams<Object>::sig(alt));

    typename RetriangulateThreadSync<threading>::Lock lock(*this);

    if (RetriangulateThreadSync<threading>::done())
        return false; // return value irrelevant - the queues are shutting down

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

        if (tracker_)
            if (! tracker_->incSteps())
                return true; // stop propagating (the operation was cancelled)

        if constexpr (flags & RetriangulateNoLocks) {
            bool shouldStop;
            lock.unlock();
            if constexpr (withSig)
                shouldStop = action_(sig, std::move(alt));
            else
                shouldStop = action_(std::move(alt));
            lock.lock();
            if (shouldStop) {
                sigs_.backtrace(sig);
                RetriangulateThreadSync<threading>::setDone();
                return true; // stop propagating (action says we are done)
            }
        } else {
            if constexpr (withSig) {
                if (action_(sig, std::move(alt))) {
                    sigs_.backtrace(sig);
                    RetriangulateThreadSync<threading>::setDone();
                    return true; // stop propagating (action says we are done)
                }
            } else {
                if (action_(std::move(alt))) {
                    sigs_.backtrace(sig);
                    RetriangulateThreadSync<threading>::setDone();
                    return true; // stop propagating (action says we are done)
                }
            }
        }
    }
    return false; // keep propagating
}

template <Retriangulable Object, bool threading, bool withSig, int flags,
    PropagationOptions<Object> options>
bool enumerateDetail(const Object& obj, bool rigid, int height, int nThreads,
        ProgressTrackerOpen* tracker,
        RetriangulateActionFunc<Object, withSig>&& action) {
    if (tracker)
        tracker->newStage(RetriangulateParams<Object>::progressStage);

    if (obj.isEmpty()) {
        // There are no moves possible on empty links or empty triangulations.
        // However, we should still visit the original (empty) object.
        bool result;
        if constexpr (withSig) {
            // There is only one empty link diagram and/or triangulation, so
            // we can ignore the value of rigid_ here.
            result = action(obj.sig(), Object(obj));
        } else {
            result = action(Object(obj));
        }
        if constexpr (! (flags & RetriangulateNotFinished)) {
            if (tracker)
                tracker->setFinished();
        }
        return result;
    }

    // The flag RetriangulateNotFinished is of no use to us inside the
    // Retriangulator class.
    using T = Retriangulator<Object, threading, withSig,
        (flags & ~RetriangulateNotFinished), options>;

    T bfs(rigid, (height >= 0 ? obj.size() + height :
        std::numeric_limits<std::size_t>::max()), std::move(action), tracker);
    if (bfs.seed(obj)) {
        if constexpr (! (flags & RetriangulateNotFinished)) {
            if (tracker)
                tracker->setFinished();
        }
        return true;
    }
    bfs.startThreads(nThreads, &T::processQueue, std::addressof(bfs), tracker);

    if constexpr (! (flags & RetriangulateNotFinished)) {
        if (tracker)
            tracker->setFinished();
    }
    return bfs.done();
}

template <Retriangulable Object, bool withSig, int flags,
    PropagationOptions<Object> options>
bool retriangulateInternal(const Object& obj, bool rigid, int height,
        int nThreads, ProgressTrackerOpen* tracker,
        RetriangulateActionFunc<Object, withSig>&& action) {
    if (nThreads <= 1) {
        return enumerateDetail<Object, false, withSig, flags, options>(
            obj, rigid, height, nThreads, tracker, std::move(action));
    } else {
        return enumerateDetail<Object, true, withSig, flags, options>(
            obj, rigid, height, nThreads, tracker, std::move(action));
    }
}

#endif // __DOXYGEN

} // namespace regina::detail

#endif
