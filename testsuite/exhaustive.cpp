
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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
#include "census/ncensus.h"
#include "packet/ncontainer.h"
#include "triangulation/ntriangulation.h"

// When we run tests over an entire census, do we use a larger census
// (which takes a long time to run), or a smaller census?
// #define LARGE_CENSUS

#ifdef LARGE_CENSUS
    #define MIN_CLOSED_CENSUS_SIZE 5
    #define CLOSED_CENSUS_SIZE 4
    #define BOUNDED_CENSUS_SIZE 3
    #define IDEAL_CENSUS_SIZE 4

    #define SMALL_MIN_CLOSED_CENSUS_SIZE 4
    #define SMALL_CLOSED_CENSUS_SIZE 3
    #define SMALL_BOUNDED_CENSUS_SIZE 2
    #define SMALL_IDEAL_CENSUS_SIZE 3
#else
    #define MIN_CLOSED_CENSUS_SIZE 4
    #define CLOSED_CENSUS_SIZE 3
    #define BOUNDED_CENSUS_SIZE 2
    #define IDEAL_CENSUS_SIZE 3

    #define SMALL_MIN_CLOSED_CENSUS_SIZE 3
    #define SMALL_CLOSED_CENSUS_SIZE 2
    #define SMALL_BOUNDED_CENSUS_SIZE 1
    #define SMALL_IDEAL_CENSUS_SIZE 2
#endif

using regina::NBoolSet;
using regina::NCensus;

namespace {
    struct TestFunctionHolder {
        // Work around the fact that we cannot cast between function
        // pointers and void*.
        NTriangulationTestFunction f_;
        TestFunctionHolder(NTriangulationTestFunction f) : f_(f) {}
    };

    static bool testCensusTriangulation(regina::NTriangulation* tri,
            void* testFunctionHolder) {
        tri->setPacketLabel(tri->isoSig());
        (*(static_cast<TestFunctionHolder*>(testFunctionHolder)->f_))(tri);
        return false;
    }
}

void runCensusMinClosed(NTriangulationTestFunction testFunction, bool small) {
    regina::NContainer parent;
    TestFunctionHolder f(testFunction);
    NCensus::formCensus(&parent,
        (small ? SMALL_MIN_CLOSED_CENSUS_SIZE : MIN_CLOSED_CENSUS_SIZE),
        NBoolSet::sTrue /* finite */,
        NBoolSet::sBoth /* orientable */,
        NBoolSet::sFalse /* bounded */,
        -1, /* bdry faces */
        NCensus::PURGE_NON_MINIMAL_PRIME | NCensus::PURGE_P2_REDUCIBLE,
        testCensusTriangulation, &f);
}

void runCensusAllClosed(NTriangulationTestFunction testFunction, bool small) {
    regina::NContainer parent;
    TestFunctionHolder f(testFunction);
    NCensus::formCensus(&parent,
        (small ? SMALL_CLOSED_CENSUS_SIZE : CLOSED_CENSUS_SIZE),
        NBoolSet::sTrue /* finite */,
        NBoolSet::sBoth /* orientable */,
        NBoolSet::sFalse /* bounded */,
        -1 /* bdry faces */, 0 /* purge */,
        testCensusTriangulation, &f);
}

void runCensusAllBounded(NTriangulationTestFunction testFunction, bool small) {
    regina::NContainer parent;
    TestFunctionHolder f(testFunction);
    NCensus::formCensus(&parent,
        (small ? SMALL_BOUNDED_CENSUS_SIZE : BOUNDED_CENSUS_SIZE),
        NBoolSet::sTrue /* finite */,
        NBoolSet::sBoth /* orientable */,
        NBoolSet::sTrue /* bounded */,
        -1 /* bdry faces */, 0 /* purge */,
        testCensusTriangulation, &f);
}

void runCensusAllIdeal(NTriangulationTestFunction testFunction, bool small) {
    regina::NContainer parent;
    TestFunctionHolder f(testFunction);
    NCensus::formCensus(&parent,
        (small ? SMALL_IDEAL_CENSUS_SIZE : IDEAL_CENSUS_SIZE),
        NBoolSet::sFalse /* finite */,
        NBoolSet::sBoth /* orientable */,
        NBoolSet::sFalse /* bounded */,
        -1 /* bdry faces */, 0 /* purge */,
        testCensusTriangulation, &f);
}

