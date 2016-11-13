
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "census/census.h"
#include "census/gluingpermsearcher3.h"
#include "file/globaldirs.h"
#include "packet/container.h"
#include "triangulation/dim3.h"
#include "testsuite/census/testcensus.h"

using regina::BoolSet;
using regina::Census;
using regina::CensusHit;
using regina::CensusHits;
using regina::FacetPairing;
using regina::GluingPerms;
using regina::GluingPermSearcher;
using regina::Triangulation;

class Census3Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Census3Test);

    CPPUNIT_TEST(lookup);
    CPPUNIT_TEST(rawCounts);
    CPPUNIT_TEST(rawCountsCompact);
    CPPUNIT_TEST(rawCountsPrimeMinimalOr);
    CPPUNIT_TEST(rawCountsPrimeMinimalNor);
    CPPUNIT_TEST(rawCountsBounded);
    CPPUNIT_TEST(rawCountsHypMin);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void verifyLookupNone(const char* isoSig) {
            CensusHits* hits = Census::lookup(isoSig);
            if (! (hits->empty() && hits->count() == 0 && ! hits->first())) {
                std::ostringstream msg;
                msg << "Census lookup for " << isoSig
                    << " should return no matches.";
                CPPUNIT_FAIL(msg.str());
            }
            delete hits;
        }

        void verifyLookup(const char* isoSig, const char* name) {
            CensusHits* hits = Census::lookup(isoSig);
            if (hits->empty() || hits->count() != 1 || (! hits->first()) ||
                    hits->first()->next()) {
                std::ostringstream msg;
                msg << "Census lookup for " << isoSig
                    << " should return exactly one match.";
                CPPUNIT_FAIL(msg.str());
            }
            if (hits->first()->name() != name) {
                std::ostringstream msg;
                msg << "Census lookup for " << isoSig << " returned "
                    << hits->first()->name() << " instead of " << name << ".";
                CPPUNIT_FAIL(msg.str());
            }
            delete hits;
        }

        void verifyLookup(const char* isoSig, const char* name1,
                const char* name2) {
            CensusHits* hits = Census::lookup(isoSig);
            if (hits->empty() || hits->count() != 2 || (! hits->first()) ||
                    (! hits->first()->next()) ||
                    hits->first()->next()->next()) {
                std::ostringstream msg;
                msg << "Census lookup for " << isoSig
                    << " should return exactly two matches.";
                CPPUNIT_FAIL(msg.str());
            }
            if (hits->first()->name() != name1) {
                std::ostringstream msg;
                msg << "Census lookup for " << isoSig << " returned "
                    << hits->first()->name() << " instead of " << name1
                    << " for hit #1.";
                CPPUNIT_FAIL(msg.str());
            }
            if (hits->first()->next()->name() != name2) {
                std::ostringstream msg;
                msg << "Census lookup for " << isoSig << " returned "
                    << hits->first()->name() << " instead of " << name2
                    << " for hit #2.";
                CPPUNIT_FAIL(msg.str());
            }
            delete hits;
        }

        void lookup() {
            // Make sure that the database library is working, and that
            // we can access all censuses.

#ifdef TESTSUITE_CENSUS_DIR
            // Temporarily redirect the home directory so that Regina
            // finds the census data files in the source tree.
            std::string census = regina::GlobalDirs::census();
            std::string python = regina::GlobalDirs::pythonModule();
            regina::GlobalDirs::setDirs("", python, TESTSUITE_CENSUS_DIR);
#endif
            
            verifyLookupNone("");
            verifyLookupNone("abcdefg");
            verifyLookup("fvPQcdecedekrsnrs",
                "SFS [S2: (2,1) (3,1) (5,-4)] : #1"); // closed or
            verifyLookup("kLLvLQQkcdjgjijhihihsfrovojgng",
                "Hyp_1.28448530 (Z_6) : #12",
                "1.2844853004683544 : m004(6, 1)"); // closed or, closed hyp
            verifyLookup("gvLQQcdefeffdwnplhe",
                "T x I / [ 1,1 | 1,0 ] : #1"); // closed nor
            verifyLookup("cPcbbbiht",
                "m004 : #1", "L104001"); // cusped or, hyp knots & links
            verifyLookup("bkaaid", "m000 : #1"); // cusped nor
            verifyLookup("kLLPLLQkceefejjiijiiiatdmpamxt",
                "L408001", "L410005"); // hyp knots & links, multiple times

#ifdef TESTSUITE_CENSUS_DIR
            regina::GlobalDirs::setDirs("", python, census);
#endif
        }

        void rawCounts() {
            unsigned nAll[] = { 1, 5, 61, 1581 };
            rawCountsCompare(1, 3, nAll, "closed/ideal",
                BoolSet::sBoth, BoolSet::sBoth, BoolSet::sFalse,
                0, 0, false);

            unsigned nOrientable[] = { 1, 4, 35, 454, 13776 };
            rawCountsCompare(1, 3, nOrientable, "closed/ideal orbl",
                BoolSet::sBoth, BoolSet::sTrue, BoolSet::sFalse,
                0, 0, false);
        }

        void rawCountsCompact() {
            unsigned nAll[] = { 1, 4, 17, 81, 577, 5184, 57753 };
            rawCountsCompare(1, 4, nAll, "closed compact",
                BoolSet::sTrue, BoolSet::sBoth, BoolSet::sFalse,
                0, 0, false);

            unsigned nOrientable[] = { 1, 4, 16, 76, 532, 4807, 52946 };
            rawCountsCompare(1, 4, nOrientable, "closed compact orbl",
                BoolSet::sTrue, BoolSet::sTrue, BoolSet::sFalse,
                0, 0, false);
        }

        void rawCountsPrimeMinimalOr() {
            unsigned nOrientable[] = { 1, 4, 11, 7, 17, 50 };
            rawCountsCompare(1, 4, nOrientable, "closed orbl prime minimal",
                BoolSet::sTrue, BoolSet::sTrue, BoolSet::sFalse, 0,
                GluingPermSearcher<3>::PURGE_NON_MINIMAL_PRIME, true);
        }

        void rawCountsPrimeMinimalNor() {
            unsigned nNonOrientable[] = { 0, 0, 1, 0, 2, 4 };
            rawCountsCompare(1, 4, nNonOrientable,
                "closed non-orbl prime minimal P2-irreducible",
                BoolSet::sTrue, BoolSet::sFalse, BoolSet::sFalse, 0,
                GluingPermSearcher<3>::PURGE_NON_MINIMAL_PRIME |
                GluingPermSearcher<3>::PURGE_P2_REDUCIBLE, true);
        }

        void rawCountsBounded() {
            unsigned nAll[] = { 1, 3, 17, 156, 2308 };
            rawCountsCompare(1, 3, nAll, "bounded compact",
                BoolSet::sTrue, BoolSet::sBoth, BoolSet::sTrue,
                -1, 0, false);

            unsigned nOrientable[] = { 1, 3, 14, 120, 1531 };
            rawCountsCompare(1, 3, nOrientable, "bounded compact orbl",
                BoolSet::sTrue, BoolSet::sTrue, BoolSet::sTrue,
                -1, 0, false);
        }

        void rawCountsHypMin() {
            // Enforced: all vertices torus/KB, no low-degree edges.
            unsigned nAll[] = { 1, 1, 7, 31, 224, 1075, 6348 };
            rawCountsCompare(1, 4, nAll, "candidate minimal cusped hyperbolic",
                BoolSet::sFalse, BoolSet::sBoth, BoolSet::sFalse, -1,
                GluingPermSearcher<3>::PURGE_NON_MINIMAL_HYP, false);

            unsigned nOrientable[] = { 1, 0, 3, 14, 113, 590, 3481 };
            rawCountsCompare(1, 5, nOrientable,
                "candidate minimal cusped hyperbolic orbl",
                BoolSet::sFalse, BoolSet::sTrue, BoolSet::sFalse, -1,
                GluingPermSearcher<3>::PURGE_NON_MINIMAL_HYP, false);
        }

        struct CensusSpec {
            BoolSet finite_;
            BoolSet orbl_;
            int purge_;
            bool minimal_;

            unsigned long count_;

            CensusSpec(BoolSet finite, BoolSet orbl,
                    int purge, bool minimal) :
                    finite_(finite), orbl_(orbl),
                    purge_(purge), minimal_(minimal), count_(0) {}
        };

        static void foundPerms(const GluingPermSearcher<3>* perms, void* spec) {
            if (perms) {
                CensusSpec* s = static_cast<CensusSpec*>(spec);
                Triangulation<3>* tri = perms->triangulate();
                if (tri->isValid() &&
                        (! (s->minimal_ &&
                            tri->simplifyToLocalMinimum(false))) &&
                        (! (s->orbl_ == BoolSet::sTrue &&
                            ! tri->isOrientable())) &&
                        (! (s->orbl_ == BoolSet::sFalse &&
                            tri->isOrientable())) &&
                        (! (s->finite_ == BoolSet::sTrue &&
                            tri->isIdeal())) &&
                        (! (s->finite_ == BoolSet::sFalse &&
                            ! tri->isIdeal())))
                    ++s->count_;
                delete tri;
            }
        }

        static void foundPairing(const FacetPairing<3>* pairing,
                const FacetPairing<3>::IsoList* autos, void* spec) {
            if (pairing) {
                CensusSpec* s = static_cast<CensusSpec*>(spec);
                GluingPermSearcher<3>::findAllPerms(pairing, autos,
                    ! s->orbl_.hasFalse(), ! s->finite_.hasFalse(),
                    s->purge_, foundPerms, spec);
            }
        }

        static void rawCountsCompare(unsigned minTets, unsigned maxTets,
                const unsigned* realAns, const char* censusType,
                BoolSet finiteness, BoolSet orientability,
                BoolSet boundary, int nBdryFaces, int whichPurge,
                bool minimal) {
            for (unsigned nTets = minTets; nTets <= maxTets; nTets++) {
                CensusSpec spec(finiteness, orientability, whichPurge, minimal);

                FacetPairing<3>::findAllPairings(nTets, boundary, nBdryFaces,
                    foundPairing, &spec);

                std::ostringstream msg;
                msg << "Census count for " << nTets << " tetrahedra ("
                    << censusType << ") should be " << realAns[nTets]
                    << ", not " << spec.count_ << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    spec.count_ == realAns[nTets]);
            }
        }
};

void addCensus3(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Census3Test::suite());
}

