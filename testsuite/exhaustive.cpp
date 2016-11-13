
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#include "exhaustive.h"
#include "census/gluingpermsearcher4.h"
#include "census/census.h"
#include "census/gluingpermsearcher3.h"
#include "packet/container.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

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

#define DIM4_CLOSED_CENSUS_SIZE 2
#define DIM4_BOUNDED_CENSUS_SIZE 2
#define DIM4_IDEAL_CENSUS_SIZE 2

using regina::BoolSet;
using regina::FacetPairing;
using regina::GluingPerms;
using regina::GluingPermSearcher;
using regina::Triangulation;

namespace {
    struct TestFunctionHolder3 {
        Triangulation3TestFunction f_;
        regina::BoolSet finite_;
        bool minimal_;

        TestFunctionHolder3(Triangulation3TestFunction f,
                regina::BoolSet finite, bool minimal = false) :
                f_(f), finite_(finite), minimal_(minimal) {}
    };

    void foundGluingPerms3(const GluingPermSearcher<3>* perms, void* holder) {
        if (perms) {
            TestFunctionHolder3* h = static_cast<TestFunctionHolder3*>(holder);
            Triangulation<3>* tri = perms->triangulate();
            if (tri->isValid() &&
                    (! (h->finite_ == BoolSet::sTrue && tri->isIdeal())) &&
                    (! (h->finite_ == BoolSet::sFalse && ! tri->isIdeal()))) {
                tri->setLabel(tri->isoSig());
                (*(h->f_))(tri);
            }
            delete tri;
        }
    }

    void foundFacetPairing3(const FacetPairing<3>* pairing,
            const FacetPairing<3>::IsoList* autos, void* holder) {
        if (pairing) {
            TestFunctionHolder3* h = static_cast<TestFunctionHolder3*>(holder);
            GluingPermSearcher<3>::findAllPerms(pairing, autos,
                false /* orientable only */,
                ! h->finite_.hasFalse() /* finite only */,
                (h->minimal_ ?
                    GluingPermSearcher<3>::PURGE_NON_MINIMAL_PRIME |
                    GluingPermSearcher<3>::PURGE_P2_REDUCIBLE : 0) /* purge */,
                &foundGluingPerms3, holder);
        }
    }

    struct TestFunctionHolder4 {
        Triangulation4TestFunction f_;
        regina::BoolSet finite_;

        TestFunctionHolder4(Triangulation4TestFunction f,
                regina::BoolSet finite) :
                f_(f), finite_(finite) {}
    };

    void foundGluingPerms4(const GluingPermSearcher<4>* perms, void* holder) {
        if (perms) {
            TestFunctionHolder4* h = static_cast<TestFunctionHolder4*>(holder);
            Triangulation<4>* tri = perms->triangulate();
            if (tri->isValid() &&
                    (! (h->finite_ == BoolSet::sTrue && tri->isIdeal())) &&
                    (! (h->finite_ == BoolSet::sFalse && ! tri->isIdeal()))) {
                tri->setLabel(tri->isoSig());
                (*(h->f_))(tri);
            }
            delete tri;
        }
    }

    void foundFacetPairing4(const FacetPairing<4>* pairing,
            const FacetPairing<4>::IsoList* autos, void* holder) {
        if (pairing) {
            TestFunctionHolder4* h = static_cast<TestFunctionHolder4*>(holder);
            GluingPermSearcher<4>::findAllPerms(pairing, autos,
                false /* orientable only */,
                ! h->finite_.hasFalse() /* finite only */,
                &foundGluingPerms4, holder);
        }
    }
}

void runCensusMinClosed(Triangulation3TestFunction testFunction, bool small_) {
    TestFunctionHolder3 f(testFunction, BoolSet::sTrue /* finite */,
        true /* minimal */);
    FacetPairing<3>::findAllPairings(
        (small_ ? DIM3_SMALL_MIN_CLOSED_CENSUS_SIZE :
            DIM3_MIN_CLOSED_CENSUS_SIZE),
        BoolSet::sFalse /* bounded */, -1, /* bdry faces */
        &foundFacetPairing3, &f);
}

void runCensusAllClosed(Triangulation3TestFunction testFunction, bool small_) {
    TestFunctionHolder3 f(testFunction, BoolSet::sTrue /* finite */);
    FacetPairing<3>::findAllPairings(
        (small_ ? DIM3_SMALL_CLOSED_CENSUS_SIZE : DIM3_CLOSED_CENSUS_SIZE),
        BoolSet::sFalse /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing3, &f);
}

void runCensusAllBounded(Triangulation3TestFunction testFunction, bool small_) {
    TestFunctionHolder3 f(testFunction, BoolSet::sTrue /* finite */);
    FacetPairing<3>::findAllPairings(
        (small_ ? DIM3_SMALL_BOUNDED_CENSUS_SIZE : DIM3_BOUNDED_CENSUS_SIZE),
        BoolSet::sTrue /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing3, &f);
}

void runCensusAllIdeal(Triangulation3TestFunction testFunction, bool small_) {
    TestFunctionHolder3 f(testFunction, BoolSet::sFalse /* finite */);
    FacetPairing<3>::findAllPairings(
        (small_ ? DIM3_SMALL_IDEAL_CENSUS_SIZE : DIM3_IDEAL_CENSUS_SIZE),
        BoolSet::sFalse /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing3, &f);
}

void runCensusAllNoBdry(Triangulation3TestFunction testFunction, bool small_) {
    TestFunctionHolder3 f(testFunction, BoolSet::sBoth /* finite */);
    FacetPairing<3>::findAllPairings(
        (small_ ? DIM3_SMALL_IDEAL_CENSUS_SIZE : DIM3_IDEAL_CENSUS_SIZE),
        BoolSet::sFalse /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing3, &f);
}

void runCensusAllClosed(Triangulation4TestFunction testFunction) {
    TestFunctionHolder4 f(testFunction, BoolSet::sTrue /* finite */);
    FacetPairing<4>::findAllPairings(
        DIM4_CLOSED_CENSUS_SIZE,
        BoolSet::sFalse /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing4, &f);
}

void runCensusAllBounded(Triangulation4TestFunction testFunction) {
    TestFunctionHolder4 f(testFunction, BoolSet::sTrue /* finite */);
    FacetPairing<4>::findAllPairings(
        DIM4_BOUNDED_CENSUS_SIZE,
        BoolSet::sTrue /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing4, &f);
}

void runCensusAllNoBdry(Triangulation4TestFunction testFunction) {
    TestFunctionHolder4 f(testFunction, BoolSet::sBoth /* finite */);
    FacetPairing<4>::findAllPairings(
        DIM4_IDEAL_CENSUS_SIZE,
        BoolSet::sFalse /* bounded */, -1 /* bdry faces */,
        &foundFacetPairing4, &f);
}
