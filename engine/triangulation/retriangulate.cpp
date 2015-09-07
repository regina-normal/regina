
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
#include <queue>
#include <set>

namespace regina {

namespace {
    typedef std::set<std::string> SigSet;

    struct TriBFS {
        SigSet sigs;
        std::queue<SigSet::iterator> process;
        size_t maxTet;
        bool (*action)(const std::string& isoSig, void*);
        void* arg;
        bool done;
    };

    struct TriSimplify {
        size_t minTet;
        std::string soln;
    };

    void workQueue(TriBFS& bfs) {
        SigSet::iterator next, altPos;
        std::string sig;
        size_t i;
        while (true) {
            if (bfs.done || bfs.process.empty())
                return;
            next = bfs.process.front();
            bfs.process.pop();

            if ((*bfs.action)(*next, bfs.arg)) {
                bfs.done = true;
                return;
            }

            NTriangulation* t = NTriangulation::fromIsoSig(*next);
            for (i = 0; i < t->getNumberOfEdges(); ++i)
                if (t->threeTwoMove(t->getEdge(i), true, false)) {
                    NTriangulation alt(*t);
                    alt.threeTwoMove(alt.getEdge(i), false, true);
                    sig = alt.isoSig();

                    altPos = bfs.sigs.find(sig);
                    if (altPos == bfs.sigs.end()) {
                        altPos = bfs.sigs.insert(sig).first;
                        bfs.process.push(altPos);
                    }
                }

            if (t->getNumberOfTetrahedra() < bfs.maxTet)
                for (i = 0; i < t->getNumberOfFaces(); ++i)
                    if (t->twoThreeMove(t->getFace(i), true, false)) {
                        NTriangulation alt(*t);
                        alt.twoThreeMove(alt.getFace(i), false, true);
                        sig = alt.isoSig();

                        altPos = bfs.sigs.find(sig);
                        if (altPos == bfs.sigs.end()) {
                            altPos = bfs.sigs.insert(sig).first;
                            bfs.process.push(altPos);
                        }
                    }

            delete t;
        }
    }

    bool simplifyFound(const std::string& sig, void* arg) {
        std::cerr << "Found: " << sig << std::endl;
        if (NTriangulation::isoSigComponentSize(sig) <
                static_cast<TriSimplify*>(arg)->minTet) {
            static_cast<TriSimplify*>(arg)->soln = sig;
            return true;
        } else
            return false;
    }
}

bool NTriangulation::simplifyExhaustive(int height, unsigned nThreads) {
    TriSimplify simp;
    simp.minTet = getNumberOfTetrahedra();

    retriangulate(height, &simplifyFound, &simp, nThreads);

    if (! simp.soln.empty()) {
        NTriangulation* tmp = NTriangulation::fromIsoSig(simp.soln);
        tmp->swapContents(*this);
        delete tmp;

        intelligentSimplify();
        return true;
    } else
        return false;
}

bool NTriangulation::retriangulate(int height,
        bool (*action)(const std::string& isoSig, void*),
        void* arg, unsigned nThreads) const {
    if (height < 0)
        return false;

    TriBFS bfs;
    bfs.maxTet = getNumberOfTetrahedra() + height;
    bfs.action = action;
    bfs.arg = arg;
    bfs.done = false;

    SigSet::iterator it = bfs.sigs.insert(isoSig()).first;
    bfs.process.push(it);

    // TODO: Multithreading!
    workQueue(bfs);

    return bfs.done;
}

} // namespace regina

