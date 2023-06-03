
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include "census/gluingpermsearcher4.h"
#include "census/census.h"
#include "census/gluingpermsearcher3.h"
#include "packet/container.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

#include "testexhaustive.h"

// When we run tests over an entire census, do we use a larger census
// (which takes a long time to run), or a smaller census?
// #define LARGE_CENSUS

#ifdef LARGE_CENSUS
    #define DIM3_MIN_CLOSED_CENSUS_SIZE 6
    #define DIM3_CLOSED_CENSUS_SIZE 4
    #define DIM3_BOUNDED_CENSUS_SIZE 4
    #define DIM3_IDEAL_CENSUS_SIZE 4

    #define DIM3_SMALL_MIN_CLOSED_CENSUS_SIZE 4
    #define DIM3_SMALL_CLOSED_CENSUS_SIZE 3
    #define DIM3_SMALL_BOUNDED_CENSUS_SIZE 3
    #define DIM3_SMALL_IDEAL_CENSUS_SIZE 3
#else
    #define DIM3_MIN_CLOSED_CENSUS_SIZE 4
    #define DIM3_CLOSED_CENSUS_SIZE 3
    #define DIM3_BOUNDED_CENSUS_SIZE 3
    #define DIM3_IDEAL_CENSUS_SIZE 3

    #define DIM3_SMALL_MIN_CLOSED_CENSUS_SIZE 3
    #define DIM3_SMALL_CLOSED_CENSUS_SIZE 2
    #define DIM3_SMALL_BOUNDED_CENSUS_SIZE 2
    #define DIM3_SMALL_IDEAL_CENSUS_SIZE 2
#endif

#define DIM4_CLOSED_CENSUS_SIZE 2
#define DIM4_BOUNDED_CENSUS_SIZE 2
#define DIM4_IDEAL_CENSUS_SIZE 2

using regina::BoolSet;
using regina::FacetPairing;
using regina::GluingPerms;
using regina::GluingPermSearcher;
using regina::Triangulation;

namespace {
    void foundGluingPerms3(const GluingPerms<3>& perms,
            Triangulation3TestFunction f, BoolSet finite, bool minimal) {
        Triangulation<3> tri = perms.triangulate();
        if (tri.isValid() &&
                (! (finite == true && tri.isIdeal())) &&
                (! (finite == false && ! tri.isIdeal()))) {
            f(tri, tri.isoSig().c_str());
        }
    }

    void foundFacetPairing3(const FacetPairing<3>& pairing,
            FacetPairing<3>::IsoList autos,
            Triangulation3TestFunction f, BoolSet finite, bool minimal) {
        GluingPermSearcher<3>::findAllPerms(pairing, std::move(autos),
            false /* orientable only */,
            ! finite.hasFalse() /* finite only */,
            (minimal ?
                regina::PURGE_NON_MINIMAL_PRIME | regina::PURGE_P2_REDUCIBLE :
                regina::PURGE_NONE),
            &foundGluingPerms3, f, finite, minimal);
    }

    void foundGluingPerms4(const GluingPerms<4>& perms,
            Triangulation4TestFunction f, BoolSet finite) {
        Triangulation<4> tri = perms.triangulate();
        if (tri.isValid() &&
                (! (finite == true && tri.isIdeal())) &&
                (! (finite == false && ! tri.isIdeal()))) {
            f(tri, tri.isoSig().c_str());
        }
    }

    void foundFacetPairing4(const FacetPairing<4>& pairing,
            FacetPairing<4>::IsoList autos,
            Triangulation4TestFunction f, BoolSet finite) {
        GluingPermSearcher<4>::findAllPerms(pairing, std::move(autos),
            false /* orientable only */,
            ! finite.hasFalse() /* finite only */,
            &foundGluingPerms4, f, finite);
    }
}

void runCensusMinClosed(Triangulation3TestFunction testFunction, bool small_) {
    FacetPairing<3>::findAllPairings(
        (small_ ? DIM3_SMALL_MIN_CLOSED_CENSUS_SIZE :
            DIM3_MIN_CLOSED_CENSUS_SIZE),
        false /* bounded */, -1, /* bdry faces */
        &foundFacetPairing3, testFunction, true /* finite */, true /* min */);
}

void runCensusAllClosed(Triangulation3TestFunction testFunction, bool small_) {
    FacetPairing<3>::findAllPairings(
        (small_ ? DIM3_SMALL_CLOSED_CENSUS_SIZE : DIM3_CLOSED_CENSUS_SIZE),
        false /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing3, testFunction, true /* finite */, false /* min */);
}

void runCensusAllBounded(Triangulation3TestFunction testFunction, bool small_) {
    FacetPairing<3>::findAllPairings(
        (small_ ? DIM3_SMALL_BOUNDED_CENSUS_SIZE : DIM3_BOUNDED_CENSUS_SIZE),
        true /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing3, testFunction, true /* finite */, false /* min */);
}

void runCensusAllIdeal(Triangulation3TestFunction testFunction, bool small_) {
    FacetPairing<3>::findAllPairings(
        (small_ ? DIM3_SMALL_IDEAL_CENSUS_SIZE : DIM3_IDEAL_CENSUS_SIZE),
        false /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing3, testFunction, false /* finite */, false /* min */);
}

void runCensusAllNoBdry(Triangulation3TestFunction testFunction, bool small_) {
    FacetPairing<3>::findAllPairings(
        (small_ ? DIM3_SMALL_IDEAL_CENSUS_SIZE : DIM3_IDEAL_CENSUS_SIZE),
        false /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing3, testFunction, BoolSet(true, true) /* finite */,
            false /* min */);
}

void runCensusAllClosed(Triangulation4TestFunction testFunction) {
    FacetPairing<4>::findAllPairings(
        DIM4_CLOSED_CENSUS_SIZE,
        false /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing4, testFunction, true /* finite */);
}

void runCensusAllBounded(Triangulation4TestFunction testFunction) {
    FacetPairing<4>::findAllPairings(
        DIM4_BOUNDED_CENSUS_SIZE,
        true /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing4, testFunction, true /* finite */);
}

void runCensusAllNoBdry(Triangulation4TestFunction testFunction) {
    FacetPairing<4>::findAllPairings(
        DIM4_IDEAL_CENSUS_SIZE,
        false /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing4, testFunction, BoolSet(true, true) /* finite */);
}
