
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#include "exhaustive.h"
#include "census/ngluingpermsearcher.h"
#include "packet/ncontainer.h"
#include "triangulation/ntriangulation.h"

// When we run tests over an entire census, do we use a larger census
// (which takes a long time to run), or a smaller census?
// #define LARGE_CENSUS

#ifdef LARGE_CENSUS
    #define DIM3_MIN_CLOSED_CENSUS_SIZE 6
    #define DIM3_CLOSED_CENSUS_SIZE 4
    #define DIM3_BOUNDED_CENSUS_SIZE 3
    #define DIM3_IDEAL_CENSUS_SIZE 4

    #define DIM3_SMALL_MIN_CLOSED_CENSUS_SIZE 4
    #define DIM3_SMALL_CLOSED_CENSUS_SIZE 3
    #define DIM3_SMALL_BOUNDED_CENSUS_SIZE 2
    #define DIM3_SMALL_IDEAL_CENSUS_SIZE 3
#else
    #define DIM3_MIN_CLOSED_CENSUS_SIZE 4
    #define DIM3_CLOSED_CENSUS_SIZE 3
    #define DIM3_BOUNDED_CENSUS_SIZE 2
    #define DIM3_IDEAL_CENSUS_SIZE 3

    #define DIM3_SMALL_MIN_CLOSED_CENSUS_SIZE 3
    #define DIM3_SMALL_CLOSED_CENSUS_SIZE 2
    #define DIM3_SMALL_BOUNDED_CENSUS_SIZE 1
    #define DIM3_SMALL_IDEAL_CENSUS_SIZE 2
#endif

using regina::NBoolSet;
using regina::NFacePairing;
using regina::NGluingPermSearcher;
using regina::NTriangulation;

namespace {
    struct TestFunctionHolder3 {
        NTriangulationTestFunction f_;
        regina::NBoolSet finite_;
        bool minimal_;

        TestFunctionHolder3(NTriangulationTestFunction f,
                regina::NBoolSet finite, bool minimal = false) :
                f_(f), finite_(finite), minimal_(minimal) {}
    };

    void foundGluingPerms3(const NGluingPermSearcher* perms, void* holder) {
        if (perms) {
            TestFunctionHolder3* h = static_cast<TestFunctionHolder3*>(holder);
            NTriangulation* tri = perms->triangulate();
            if (tri->isValid() &&
                    (! (h->finite_ == NBoolSet::sTrue && tri->isIdeal())) &&
                    (! (h->finite_ == NBoolSet::sFalse && ! tri->isIdeal()))) {
                tri->setPacketLabel(tri->isoSig());
                (*(h->f_))(tri);
            }
            delete tri;
        }
    }

    void foundFacetPairing3(const NFacePairing* pairing,
            const NFacePairing::IsoList* autos, void* holder) {
        if (pairing) {
            TestFunctionHolder3* h = static_cast<TestFunctionHolder3*>(holder);
            NGluingPermSearcher::findAllPerms(pairing, autos,
                false /* orientable only */,
                ! h->finite_.hasFalse() /* finite only */,
                (h->minimal_ ?
                    NGluingPermSearcher::PURGE_NON_MINIMAL_PRIME |
                    NGluingPermSearcher::PURGE_P2_REDUCIBLE : 0) /* purge */,
                    false, // collapse
                &foundGluingPerms3, holder);
        }
    }
}

void runCensusMinClosed(NTriangulationTestFunction testFunction, bool small_) {
    TestFunctionHolder3 f(testFunction, NBoolSet::sTrue /* finite */,
        true /* minimal */);
    NFacePairing::findAllPairings(
        (small_ ? DIM3_SMALL_MIN_CLOSED_CENSUS_SIZE :
            DIM3_MIN_CLOSED_CENSUS_SIZE),
        NBoolSet::sFalse /* bounded */, -1, /* bdry faces */
        &foundFacetPairing3, &f);
}

void runCensusAllClosed(NTriangulationTestFunction testFunction, bool small_) {
    TestFunctionHolder3 f(testFunction, NBoolSet::sTrue /* finite */);
    NFacePairing::findAllPairings(
        (small_ ? DIM3_SMALL_CLOSED_CENSUS_SIZE : DIM3_CLOSED_CENSUS_SIZE),
        NBoolSet::sFalse /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing3, &f);
}

void runCensusAllBounded(NTriangulationTestFunction testFunction, bool small_) {
    TestFunctionHolder3 f(testFunction, NBoolSet::sTrue /* finite */);
    NFacePairing::findAllPairings(
        (small_ ? DIM3_SMALL_BOUNDED_CENSUS_SIZE : DIM3_BOUNDED_CENSUS_SIZE),
        NBoolSet::sTrue /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing3, &f);
}

void runCensusAllIdeal(NTriangulationTestFunction testFunction, bool small_) {
    TestFunctionHolder3 f(testFunction, NBoolSet::sFalse /* finite */);
    NFacePairing::findAllPairings(
        (small_ ? DIM3_SMALL_IDEAL_CENSUS_SIZE : DIM3_IDEAL_CENSUS_SIZE),
        NBoolSet::sFalse /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing3, &f);
}

void runCensusAllNoBdry(NTriangulationTestFunction testFunction, bool small_) {
    TestFunctionHolder3 f(testFunction, NBoolSet::sBoth /* finite */);
    NFacePairing::findAllPairings(
        (small_ ? DIM3_SMALL_IDEAL_CENSUS_SIZE : DIM3_IDEAL_CENSUS_SIZE),
        NBoolSet::sFalse /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing3, &f);
}
