
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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "census/gluingpermsearcher4.h"
#include "packet/container.h"
#include "triangulation/dim4.h"
#include "testsuite/census/testcensus.h"

using regina::FacetPairing;
using regina::GluingPerms;
using regina::GluingPermSearcher;
using regina::Triangulation;
using regina::BoolSet;

class Census4Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Census4Test);

    CPPUNIT_TEST(rawCounts);
    CPPUNIT_TEST(rawCountsCompact);
    CPPUNIT_TEST(rawCountsBounded);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() override {
        }

        void tearDown() override {
        }

        void rawCounts() {
            unsigned nAll[] = { 1, 0, 23, 0, 8656, 0 };
            rawCountsCompare(1, 3, nAll, "closed/ideal",
                BoolSet(true, true), BoolSet(true, true), false, 0);

            unsigned nOrientable[] = { 1, 0, 15, 0, 4150, 0 };
            rawCountsCompare(1, 3, nOrientable, "closed/ideal",
                BoolSet(true, true), true, false, 0);
        }

        void rawCountsCompact() {
            unsigned nAll[] = { 1, 0, 10, 0 };
            rawCountsCompare(1, 3, nAll, "closed compact",
                true, BoolSet(true, true), false, 0);

            unsigned nOrientable[] = { 1, 0, 8, 0 };
            rawCountsCompare(1, 3, nOrientable, "closed compact orbl",
                true, true, false, 0);
        }

        void rawCountsBounded() {
            unsigned nAll[] = { 1, 7, 51, 939, 25265 };
            rawCountsCompare(1, 2, nAll, "bounded",
                BoolSet(true, true), BoolSet(true, true), true, -1);

            unsigned nCompact[] = { 1, 5, 38, 782 };
            rawCountsCompare(1, 2, nCompact, "bounded compact",
                true, BoolSet(true, true), true, -1);

            unsigned nOrientable[] = { 1, 4, 27, 457 };
            rawCountsCompare(1, 2, nOrientable, "bounded compact orbl",
                true, true, true, -1);
        }

        struct CensusSpec {
            BoolSet finite_;
            BoolSet orbl_;

            unsigned long count_;

            CensusSpec(BoolSet finite, BoolSet orbl) :
                finite_(finite), orbl_(orbl), count_(0) {}
        };

        static void foundPerms(const GluingPerms<4>& perms, CensusSpec* spec) {
            Triangulation<4> tri = perms.triangulate();
            if (tri.isValid() &&
                    (! (spec->orbl_ == true && ! tri.isOrientable())) &&
                    (! (spec->orbl_ == false && tri.isOrientable())) &&
                    (! (spec->finite_ == true && tri.isIdeal())) &&
                    (! (spec->finite_ == false && ! tri.isIdeal())))
                ++spec->count_;
        }

        static void foundPairing(const FacetPairing<4>& pairing,
            FacetPairing<4>::IsoList autos, CensusSpec* spec) {
            GluingPermSearcher<4>::findAllPerms(pairing, std::move(autos),
                ! spec->orbl_.hasFalse(), ! spec->finite_.hasFalse(),
                foundPerms, spec);
        }

        static void rawCountsCompare(unsigned minPent, unsigned maxPent,
                const unsigned* realAns, const char* censusType,
                BoolSet finiteness, BoolSet orientability,
                BoolSet boundary, int nBdryFacets) {
            for (unsigned nPent = minPent; nPent <= maxPent; nPent++) {
                CensusSpec spec(finiteness, orientability);

                FacetPairing<4>::findAllPairings(nPent, boundary, nBdryFacets,
                    foundPairing, &spec);

                std::ostringstream msg;
                msg << "Census count for " << nPent << " pentachora ("
                    << censusType << ") should be " << realAns[nPent]
                    << ", not " << spec.count_ << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    spec.count_ == realAns[nPent]);
            }
        }
};

void addCensus4(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Census4Test::suite());
}

