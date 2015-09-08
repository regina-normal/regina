
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include "triangulation/ntriangulation.h"
#include "utilities/nthread.h"
#include <queue>
#include <set>
#ifdef HAS_CXX11_STDTHREAD
#include <thread>
#endif

namespace regina {

namespace {
    template <bool threading>
    class TriBFS {
        private:
            typedef std::set<std::string> SigSet;

            const size_t maxTet_;
            bool (*const action_)(const NTriangulation&, void*);
            void *const arg_;
            bool done_;

            SigSet sigs_;
            std::queue<SigSet::iterator> process_;

            Mutex<threading> mutex_;

        public:
            TriBFS(size_t maxTet,
                bool (*const action)(const NTriangulation& isoSig, void*),
                void *const arg) :
                maxTet_(maxTet), action_(action), arg_(arg), done_(false) {
            }

            bool seed(const NTriangulation& tri);
            void run();

            bool done() const;

        private:
            bool found(const NTriangulation& alt);
    };

    template <bool threading>
    inline bool TriBFS<threading>::seed(const NTriangulation& tri) {
        if ((*action_)(tri, arg_))
            return (done_ = true);

        process_.push(sigs_.insert(tri.isoSig()).first);
        return false;
    }

    template <bool threading>
    void TriBFS<threading>::run() {
        SigSet::iterator next;
        size_t i;
        while (true) {
            {
                typename Mutex<threading>::MutexLock lock(mutex_);
                if (done_ || process_.empty())
                    return;
                next = process_.front();
                process_.pop();
            }

            // We can work with next outside the mutex, since the C++ standard
            // requres that insertion into a std::set does not invalidate
            // iterators.

            NTriangulation* t = NTriangulation::fromIsoSig(*next);
            for (i = 0; i < t->getNumberOfEdges(); ++i)
                if (t->threeTwoMove(t->getEdge(i), true, false)) {
                    NTriangulation alt(*t);
                    alt.threeTwoMove(alt.getEdge(i), false, true);
                    if (found(alt))
                        return;
                }

            if (t->getNumberOfTetrahedra() < maxTet_)
                for (i = 0; i < t->getNumberOfFaces(); ++i)
                    if (t->twoThreeMove(t->getFace(i), true, false)) {
                        NTriangulation alt(*t);
                        alt.twoThreeMove(alt.getFace(i), false, true);
                        if (found(alt))
                            return;
                    }

            delete t;
        }
    }

    template <bool threading>
    inline bool TriBFS<threading>::done() const {
        return done_;
    }

    template <bool threading>
    bool TriBFS<threading>::found(const NTriangulation& alt) {
        const std::string sig = alt.isoSig();

        typename Mutex<threading>::MutexLock lock(mutex_);
        if (done_)
            return false;

        if (sigs_.find(sig) == sigs_.end()) {
            if ((*action_)(alt, arg_))
                return (done_ = true);

            process_.push(sigs_.insert(sig).first);
        }
        return false;
    }

    struct TriSimplify {
        NTriangulation& original;
        size_t minTet;

        TriSimplify(NTriangulation& original_) :
                original(original_),
                minTet(original_.getNumberOfTetrahedra()) {
        }

        static bool found(const NTriangulation& alt, void* simp);
    };

    bool TriSimplify::found(const NTriangulation& alt, void* simp) {
        std::cout << "Found: " << alt.isoSig() << std::endl; // TODO: Remove
        if (alt.getNumberOfTetrahedra() <
                static_cast<TriSimplify*>(simp)->minTet) {
            NTriangulation& t(static_cast<TriSimplify*>(simp)->original);
            // TODO: t.cloneFrom(alt);
            t.removeAllTetrahedra();
            t.insertTriangulation(alt);
            t.intelligentSimplify();
            return true;
        } else
            return false;
    }
}

bool NTriangulation::simplifyExhaustive(int height, unsigned nThreads) {
    TriSimplify simp(*this);
    // TODO: Template, callable object.
    return retriangulate(height, &TriSimplify::found, &simp, nThreads);
}

bool NTriangulation::retriangulate(int height,
        bool (*action)(const NTriangulation&, void*),
        void* arg, unsigned nThreads) const {
    if (height < 0)
        return false;

#ifdef HAS_CXX11_STDTHREAD
    if (nThreads <= 1) {
#endif
        TriBFS<false> bfs(getNumberOfTetrahedra() + height, action, arg);
        if (bfs.seed(*this))
            return true;
        bfs.run();
        return bfs.done();
#ifdef HAS_CXX11_STDTHREAD
    } else {
        TriBFS<true> bfs(getNumberOfTetrahedra() + height, action, arg);
        if (bfs.seed(*this))
            return true;

        std::thread* t = new std::thread[nThreads];
        unsigned i;
        // In the std::thread constructor, the pointer to bfs is essential -
        // otherwise we may end up making copies of bfs instead.
        for (i = 0; i < nThreads; ++i)
            t[i] = std::thread(&TriBFS<true>::run, &bfs);
        for (i = 0; i < nThreads; ++i)
            t[i].join();

        return bfs.done();
    }
#endif
}

} // namespace regina

