
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "progress/progresstracker.h"
#include "triangulation/dim3.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <set>
#include <system_error>
#include <thread>
#include <boost/noncopyable.hpp>

namespace regina {

namespace {
    template <bool threading> class SyncData;

    template <>
    class SyncData<true> {
        protected:
            unsigned nRunning_;
            std::mutex mutex_;
            std::condition_variable cond_;
    };

    template <>
    class SyncData<false> {
    };

    template <bool threading>
    class TriBFS : protected SyncData<threading>, public boost::noncopyable {
        private:
            typedef std::set<std::string> SigSet;

            const size_t maxTet_;
            std::function<bool(Triangulation<3>&)> action_;
            bool done_;

            SigSet sigs_;
            std::queue<SigSet::iterator> process_;

        public:
            TriBFS(size_t maxTet,
                const std::function<bool(Triangulation<3>&)>& action) :
                maxTet_(maxTet), action_(action), done_(false) {
            }

            bool seed(const Triangulation<3>& tri);
            void processQueue(ProgressTrackerOpen* tracker);
            void processQueueParallel(unsigned nThreads,
                ProgressTrackerOpen* tracker);

            bool done() const;

        private:
            // candidate() may assume that alt will be deleted immediately
            // after returning.
            bool candidate(Triangulation<3>& alt);
            void propagateFrom(const SigSet::iterator& it);
    };

    template <bool threading>
    inline bool TriBFS<threading>::seed(const Triangulation<3>& tri) {
        // We have to pass a copy of tri to action_, since action_ is
        // allowed to change the triangulation that is passed to it.
        // This is inefficient, but at least it only happens once.
        {
            Triangulation<3> copy(tri, false);
            if (action_(copy))
                return (done_ = true);
            // We cannot use copy from here, since action_() might have
            // changed it.
        }
        process_.push(sigs_.insert(tri.isoSig()).first);
        return false;
    }

    template <bool threading>
    void TriBFS<threading>::propagateFrom(const SigSet::iterator& it) {
        // We can do all of this outside the mutex, since the C++ standard
        // requres that insertion into a std::set does not invalidate
        // iterators.

        Triangulation<3>* t = Triangulation<3>::fromIsoSig(*it);
        size_t i;
        for (i = 0; i < t->countEdges(); ++i)
            if (t->threeTwoMove(t->edge(i), true, false)) {
                Triangulation<3> alt(*t, false);
                alt.threeTwoMove(alt.edge(i), false, true);
                if (candidate(alt)) {
                    delete t;
                    return;
                }
                // We cannot use alt from here, since candidate() might
                // have changed it.
            }

        if (t->size() < maxTet_)
            for (i = 0; i < t->countTriangles(); ++i)
                if (t->twoThreeMove(t->triangle(i), true, false)) {
                    Triangulation<3> alt(*t, false);
                    alt.twoThreeMove(alt.triangle(i), false, true);
                    if (candidate(alt)) {
                        delete t;
                        return;
                    }
                    // We cannot use alt from here, since candidate() might
                    // have changed it.
                }

        delete t;
        return;
    }

    template <>
    void TriBFS<false>::processQueue(ProgressTrackerOpen* tracker) {
        SigSet::iterator next;
        while (! (done_ || process_.empty())) {
            if (tracker && tracker->isCancelled())
                break;

            next = process_.front();
            process_.pop();

            propagateFrom(next);

            if (tracker)
                tracker->incSteps();
        }
    }

    template <>
    void TriBFS<true>::processQueue(ProgressTrackerOpen* tracker) {
        SigSet::iterator next;

        std::unique_lock<std::mutex> lock(mutex_);

        while (true) {
            // Process the queue until either done_ is true, or there is
            // nothing left to process.
            while (! (done_ || process_.empty())) {
                if (tracker && tracker->isCancelled())
                    break;

                next = process_.front();
                process_.pop();

                lock.unlock();
                propagateFrom(next);
                lock.lock();

                if (tracker)
                    tracker->incSteps();
            }

            if (--nRunning_ == 0) {
                // Everybody has finished.
                // Wake up the other threads so they can exit also.
                cond_.notify_all();
                return;
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
                    return;
                else
                    ++nRunning_;
            }
        }
    }

    template <>
    void TriBFS<true>::processQueueParallel(unsigned nThreads,
            ProgressTrackerOpen* tracker) {
        nRunning_ = nThreads;

        std::thread* t = new std::thread[nThreads];
        unsigned i;

        // In the std::thread constructor, passing this as a pointer is
        // essential - otherwise we may end up making copies of this instead.
        for (i = 0; i < nThreads; ++i)
            t[i] = std::thread(&TriBFS<true>::processQueue, this, tracker);
        for (i = 0; i < nThreads; ++i)
            t[i].join();

        delete[] t;
    }

    template <bool threading>
    inline bool TriBFS<threading>::done() const {
        return done_;
    }

    template <>
    bool TriBFS<true>::candidate(Triangulation<3>& alt) {
        const std::string sig = alt.isoSig();

        std::lock_guard<std::mutex> lock(mutex_);
        if (done_)
            return false;

        auto result = sigs_.insert(sig);
        if (result.second) {
            // We have not seen this triangulation before.
            if (process_.empty()) {
                process_.push(result.first);

                // Wake up any other threads that had previously emptied
                // the queue.
                cond_.notify_all();
            } else
                process_.push(result.first);

            if (action_(alt))
                return (done_ = true);
            // We cannot use alt from here, since action_() might have
            // changed it.
        }
        return false;
    }

    template <>
    bool TriBFS<false>::candidate(Triangulation<3>& alt) {
        const std::string sig = alt.isoSig();

        auto result = sigs_.insert(sig);
        if (result.second) {
            // We have not seen this triangulation before.
            process_.push(result.first);

            if (action_(alt))
                return (done_ = true);
            // We cannot use alt from here, since action_() might have
            // changed it.
        }
        return false;
    }

    bool enumerate(const Triangulation<3>& tri, int height, unsigned nThreads,
            ProgressTrackerOpen* tracker,
            const std::function<bool(Triangulation<3>&)>& action) {
        if (tracker)
            tracker->newStage("Exploring triangulations");

        if (height < 0) {
            if (tracker)
                tracker->setFinished();
            return false;
        }

        if (nThreads <= 1) {
            TriBFS<false> bfs(tri.size() + height, action);
            if (bfs.seed(tri)) {
                if (tracker)
                    tracker->setFinished();
                return true;
            }
            bfs.processQueue(tracker);
            if (tracker)
                tracker->setFinished();
            return bfs.done();
        } else {
            TriBFS<true> bfs(tri.size() + height, action);
            if (bfs.seed(tri)) {
                if (tracker)
                    tracker->setFinished();
                return true;
            }
            bfs.processQueueParallel(nThreads, tracker);
            if (tracker)
                tracker->setFinished();
            return bfs.done();
        }
    }

    bool simplifyFound(Triangulation<3>& alt, Triangulation<3>& original,
            size_t minTet) {
        if (alt.size() < minTet) {
            // Since we are allowed to change alt, we use swapContents(),
            // which avoids yet another round of remaking the tetrahedron
            // gluings.
            Packet::ChangeEventSpan span(&original);
            original.swapContents(alt);
            original.intelligentSimplify();
            return true;
        } else
            return false;
    }
}

bool Triangulation<3>::simplifyExhaustive(int height, unsigned nThreads,
        ProgressTrackerOpen* tracker) {
    return retriangulate(height, nThreads, tracker, &simplifyFound,
        std::ref(*this), size());
}

bool Triangulation<3>::retriangulateInternal(int height, unsigned nThreads,
        ProgressTrackerOpen* tracker,
        const std::function<bool(Triangulation<3>&)>& action) const {
    if (tracker) {
        try {
            std::thread(&enumerate, *this, height, nThreads, tracker, action)
                .detach();
            return true;
        } catch (const std::system_error& e) {
            return false;
        }
    } else
        return enumerate(*this, height, nThreads, tracker, action);
}

} // namespace regina

