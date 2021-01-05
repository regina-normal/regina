
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

#include "link/link.h"
#include "progress/progresstracker.h"
#include <condition_variable>
#include <limits>
#include <mutex>
#include <queue>
#include <map>
#include <set>
#include <system_error>
#include <thread>

// The following code is written to support optional backtracing (i.e.,
// when a rewriting path is found for which action returns true, it outputs
// the sequence of links that it passed through to get there).
//
// HOWEVER: This code is not well optimised and it simply dumps the
// backtrace to std::cerr, and so it is currently disabled and not available
// at all through the API.  To turn on backtracing, simply change the
// typedef SigSet from SigGraph<false> to SigGraph<true>.

namespace regina {

namespace {
    template <bool threading> class SyncData;
    template <bool backtrace> class SigGraph;

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

    template <>
    class SigGraph<true> : private std::map<std::string, std::string> {
        public:
            using std::map<std::string, std::string>::iterator;

            auto insertStart(const std::string& sig) {
                return insert(std::make_pair(sig, std::string()));
            }

            auto insertDerived(const std::string& sig,
                    const std::string& from) {
                return insert(std::make_pair(sig, from));
            }

            static const std::string& sig(iterator it) {
                return it->first;
            }

            void backtrace(const std::string& sig) const {
                std::cerr << sig;
                for (auto it = find(sig); ! it->second.empty();
                        it = find(it->second))
                    std::cerr << " <- " << it->second;
                std::cerr << std::endl;
            }
    };

    template <>
    class SigGraph<false> : private std::set<std::string> {
        public:
            using std::set<std::string>::iterator;

            auto insertStart(const std::string& sig) {
                return insert(sig);
            }

            auto insertDerived(const std::string& sig, const std::string&) {
                return insert(sig);
            }

            static const std::string& sig(iterator it) {
                return *it;
            }

            void backtrace(const std::string&) const {
            }
    };

    template <bool threading>
    class LinkBFS : protected SyncData<threading> {
        private:
            // To switch on backtracing, change the following typedef to
            // SigGraph<true>.
            typedef SigGraph<false> SigSet;

            static bool lowerPriority(SigSet::iterator a, SigSet::iterator b) {
                // The function should compute whether a.size > b.size,
                // where "size" measures number of crossings.
                // We will use string length, which is fast to compute
                // and (for knots) should have the same effect.
                return SigSet::sig(a).length() > SigSet::sig(b).length();
            }

            const size_t maxCrossings_;
            std::function<bool(Link&)> action_;
            bool done_;

            SigSet sigs_;
            std::priority_queue<SigSet::iterator,
                std::vector<SigSet::iterator>,
                std::function<bool(SigSet::iterator, SigSet::iterator)>>
                process_;

        public:
            LinkBFS(size_t maxCrossings,
                const std::function<bool(Link&)>& action) :
                maxCrossings_(maxCrossings), action_(action), done_(false),
                process_(lowerPriority) {
            }

            bool seed(const Link& link);
            void processQueue(ProgressTrackerOpen* tracker);
            void processQueueParallel(unsigned nThreads,
                ProgressTrackerOpen* tracker);

            bool done() const;

            LinkBFS(const LinkBFS&) = delete;
            LinkBFS& operator = (const LinkBFS&) = delete;

        private:
            // candidate() may assume that alt will be deleted immediately
            // after returning.
            bool candidate(Link& alt, const std::string& from);
            void propagateFrom(const SigSet::iterator& it);
    };

    template <bool threading>
    inline bool LinkBFS<threading>::seed(const Link& link) {
        // We have to pass a copy of link to action_, since action_ is
        // allowed to change the link that is passed to it.
        // This is inefficient, but at least it only happens once.
        {
            Link copy(link, false);
            if (action_(copy))
                return (done_ = true);
            // We cannot use copy from here, since action_() might have
            // changed it.
        }
        process_.push(sigs_.insertStart(link.knotSig()).first);
        return false;
    }

    template <bool threading>
    void LinkBFS<threading>::propagateFrom(const SigSet::iterator& it) {
        // We can do all of this outside the mutex, since the C++ standard
        // requres that insertion into a std::set does not invalidate
        // iterators.

        Link* t = Link::fromKnotSig(SigSet::sig(it));
        if (t->size() == 0) {
            // We have a zero-crossing unknot.
            // There is only one move we can perform on this.
            Link alt(*t, false);
            alt.r1(regina::StrandRef(), 0, 1 /* sign */, false, true);
            candidate(alt, SigSet::sig(it));
            delete t;
            return;
        }

        // From here we assume >= 1 crossing.
        size_t i;
        int strand, side, sign;

        for (i = 0; i < t->size(); ++i)
            if (t->r1(t->crossing(i), true, false)) {
                Link alt(*t, false);
                alt.r1(alt.crossing(i), false, true);
                if (candidate(alt, SigSet::sig(it))) {
                    delete t;
                    return;
                }
                // We cannot use alt from here, since candidate() might
                // have changed it.
            }

        for (i = 0; i < t->size(); ++i)
            if (t->r2(t->crossing(i), true, false)) {
                Link alt(*t, false);
                alt.r2(alt.crossing(i), false, true);
                if (candidate(alt, SigSet::sig(it))) {
                    delete t;
                    return;
                }
                // We cannot use alt from here, since candidate() might
                // have changed it.
            }

        for (i = 0; i < t->size(); ++i)
            for (side = 0; side < 2; ++side)
                if (t->r3(t->crossing(i), side, true, false)) {
                    Link alt(*t, false);
                    alt.r3(alt.crossing(i), side, false, true);
                    if (candidate(alt, SigSet::sig(it))) {
                        delete t;
                        return;
                    }
                    // We cannot use alt from here, since candidate() might
                    // have changed it.
                }

        // R1 twist moves on arcs are always valid.
        if (t->size() < maxCrossings_)
            for (i = 0; i < t->size(); ++i)
                for (strand = 0; strand < 2; ++strand)
                    for (side = 0; side < 2; ++side)
                        for (sign = -1; sign <= 1; sign += 2) {
                            Link alt(*t, false);
                            alt.r1(alt.crossing(i)->strand(strand),
                                side, sign, false, true);
                            if (candidate(alt, SigSet::sig(it))) {
                                delete t;
                                return;
                            }
                            // We cannot use alt from here, since
                            // candidate() might have changed it.
                        }

        if (t->size() + 1 < maxCrossings_) {
            StrandRef upperArc, lowerArc;
            int upperSide, lowerSide;
            for (i = 0; i < t->size(); ++i)
                for (strand = 0; strand < 2; ++strand) {
                    upperArc = t->crossing(i)->strand(strand);
                    for (upperSide = 0; upperSide < 2; ++upperSide) {
                        // Walk around the 2-cell containing upperArc.
                        // This code follows the (better documented)
                        // code in reidemeister.cpp for testing r2 validity.
                        //
                        // We walk around the 2-cell from upper, ensuring that
                        // we always turn left.
                        //
                        // At each stage we consider an edge of this 2-cell:
                        //
                        // - ref points to the strand of the crossing at the
                        //   beginning of the edge, with respect to the
                        //   direction in which we are walking around the cell;
                        // - lowerArc points to the strand of the crossing at
                        //   the beginning of the edge, with respect to the
                        //   orientation of the link.
                        // - forward indicates whether these two directions
                        //   are the same.
                        //
                        // Note that we don't actually set lowerArc until we
                        // get near the end of the while loop.
                        //
                        StrandRef ref = upperArc;
                        bool forward;
                        if (upperSide == 0) {
                            forward = true;
                        } else {
                            // Since we are traversing the arc backwards,
                            // we need to jump to the other endpoint.
                            ref = ref.next();
                            forward = false;
                        }

                        while (true) {
                            // Move to the next edge around this 2-cell.
                            if (forward) {
                                ref = ref.next();
                                ref.jump();

                                // forward remains true for (sign, strand):
                                // +, 0
                                // -, 1
                                if (ref.crossing()->sign() > 0)
                                    forward = (0 == ref.strand());
                                else
                                    forward = (0 != ref.strand());
                            } else {
                                ref = ref.prev();
                                ref.jump();

                                // forward becomes true for (sign, strand):
                                // -, 0
                                // +, 1
                                if (ref.crossing()->sign() > 0)
                                    forward = (0 != ref.strand());
                                else
                                    forward = (0 == ref.strand());
                            }

                            lowerArc = (forward ? ref : ref.prev());

                            // By planarity, the 2-cell can meet one side of
                            // an arc, but never both.
                            if (lowerArc == upperArc) {
                                // We completed the cycle.
                                break;
                            }

                            // Try arc as the lower strand.
                            // Make sure we're on the correct side.
                            lowerSide = (forward ? 0 : 1);

                            Link alt(*t, false);
                            alt.r2(alt.translate(upperArc), upperSide,
                                alt.translate(lowerArc), lowerSide,
                                false, true);
                            if (candidate(alt, SigSet::sig(it))) {
                                delete t;
                                return;
                            }
                            // We cannot use alt from here, since
                            // candidate() might have changed it.
                        }
                    }
                }
        }

        delete t;
        return;
    }

    template <>
    void LinkBFS<false>::processQueue(ProgressTrackerOpen* tracker) {
        SigSet::iterator next;
        while (! (done_ || process_.empty())) {
            if (tracker && tracker->isCancelled())
                break;

            next = process_.top();
            process_.pop();

            propagateFrom(next);

            if (tracker)
                tracker->incSteps();
        }
    }

    template <>
    void LinkBFS<true>::processQueue(ProgressTrackerOpen* tracker) {
        SigSet::iterator next;

        std::unique_lock<std::mutex> lock(mutex_);

        while (true) {
            // Process the queue until either done_ is true, or there is
            // nothing left to process.
            while (! (done_ || process_.empty())) {
                if (tracker && tracker->isCancelled())
                    break;

                next = process_.top();
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
    void LinkBFS<true>::processQueueParallel(unsigned nThreads,
            ProgressTrackerOpen* tracker) {
        nRunning_ = nThreads;

        std::thread* t = new std::thread[nThreads];
        unsigned i;

        // In the std::thread constructor, passing this as a pointer is
        // essential - otherwise we may end up making copies of this instead.
        for (i = 0; i < nThreads; ++i)
            t[i] = std::thread(&LinkBFS<true>::processQueue, this, tracker);
        for (i = 0; i < nThreads; ++i)
            t[i].join();

        delete[] t;
    }

    template <bool threading>
    inline bool LinkBFS<threading>::done() const {
        return done_;
    }

    template <>
    bool LinkBFS<true>::candidate(Link& alt, const std::string& from) {
        const std::string sig = alt.knotSig();

        std::lock_guard<std::mutex> lock(mutex_);
        if (done_)
            return false;

        auto result = sigs_.insertDerived(sig, from);
        if (result.second) {
            // We have not seen this diagram before.
            if (process_.empty()) {
                process_.push(result.first);

                // Wake up any other threads that had previously emptied
                // the queue.
                cond_.notify_all();
            } else
                process_.push(result.first);

            if (action_(alt)) {
                sigs_.backtrace(sig);
                return (done_ = true);
            }
            // We cannot use alt from here, since action_() might have
            // changed it.
        }
        return false;
    }

    template <>
    bool LinkBFS<false>::candidate(Link& alt, const std::string& from) {
        const std::string sig = alt.knotSig();

        auto result = sigs_.insertDerived(sig, from);
        if (result.second) {
            // We have not seen this diagram before.
            process_.push(result.first);

            if (action_(alt)) {
                sigs_.backtrace(sig);
                return (done_ = true);
            }
            // We cannot use alt from here, since action_() might have
            // changed it.
        }
        return false;
    }

    bool enumerate(const Link& link, int height, unsigned nThreads,
            ProgressTrackerOpen* tracker,
            const std::function<bool(Link&)>& action) {
        if (tracker)
            tracker->newStage("Exploring diagrams");

        if (link.countComponents() != 1) {
            if (tracker)
                tracker->setFinished();
            return false;
        }

        if (nThreads <= 1) {
            LinkBFS<false> bfs((height >= 0 ? link.size() + height :
                std::numeric_limits<std::size_t>::max()), action);
            if (bfs.seed(link)) {
                if (tracker)
                    tracker->setFinished();
                return true;
            }
            bfs.processQueue(tracker);
            if (tracker)
                tracker->setFinished();
            return bfs.done();
        } else {
            LinkBFS<true> bfs((height >= 0 ? link.size() + height :
                std::numeric_limits<std::size_t>::max()), action);
            if (bfs.seed(link)) {
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

    bool simplifyFound(Link& alt, Link& original, size_t minCrossings) {
        if (alt.size() < minCrossings) {
            // Since we are allowed to change alt, we use swapContents(),
            // which avoids yet another round of rewiring the crossings.
            Packet::ChangeEventSpan span(&original);
            original.swapContents(alt);
            original.intelligentSimplify();
            return true;
        } else
            return false;
    }
}

bool Link::simplifyExhaustive(int height, unsigned nThreads,
        ProgressTrackerOpen* tracker) {
    return rewrite(height, nThreads, tracker, &simplifyFound,
        std::ref(*this), size());
}

bool Link::rewriteInternal(int height, unsigned nThreads,
        ProgressTrackerOpen* tracker,
        const std::function<bool(Link&)>& action) const {
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

