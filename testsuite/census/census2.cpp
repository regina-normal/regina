
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
#include "census/gluingpermsearcher2.h"
#include "packet/container.h"
#include "triangulation/dim2.h"
#include "testsuite/census/testcensus.h"

using regina::BoolSet;
using regina::FacetPairing;
using regina::GluingPerms;
using regina::GluingPermSearcher;
using regina::Triangulation;

class Census2Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Census2Test);

    CPPUNIT_TEST(rawCountsClosed);
    CPPUNIT_TEST(rawCountsBounded);
    CPPUNIT_TEST(rawCountsClosedMinimal);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        void rawCountsClosed() {
            // All counts taken from an enumeration using Regina 4.94.
            unsigned nAll[] = { 1, 0, 7, 0, 51, 0, 738, 0, 20540, 0, 911677 };
            rawCountsCompare(1, 8, nAll, "closed",
                BoolSet::sBoth, BoolSet::sFalse, 0, false);

            unsigned nOrientable[] = { 1, 0, 3, 0, 11, 0, 73, 0, 838, 0, 15840};
            rawCountsCompare(1, 10, nOrientable, "closed orbl",
                BoolSet::sTrue, BoolSet::sFalse, 0, false);
        }

        void rawCountsBounded() {
            // All counts taken from an enumeration using Regina 4.94.
            unsigned nAll[] = { 1, 3, 6, 26, 105, 622, 3589, 28031, 202169 };
            rawCountsCompare(1, 7, nAll, "bounded",
                BoolSet::sBoth, BoolSet::sTrue, -1, false);

            unsigned nOrientable[] =
                { 1, 2, 4, 11, 41, 155, 750, 3967, 23260, 148885, 992299 };
            rawCountsCompare(1, 8, nOrientable, "bounded orbl",
                BoolSet::sTrue, BoolSet::sTrue, -1, false);
        }

        void rawCountsClosedMinimal() {
            // All counts taken from an enumeration using Regina 4.94.
            unsigned nOrientable[] = { 1, 0, 3 /* sphere + torus */, 0, 0, 0,
                8, 0, 0, 0, 927 };
            rawCountsCompare(1, 10, nOrientable, "closed orbl minimal",
                BoolSet::sTrue, BoolSet::sFalse, 0, true);

            unsigned nNonOrientable[] = { 1, 0, 4 /* PP + KB */, 0, 11, 0,
                144, 0, 3627, 0, 149288 };
            rawCountsCompare(1, 8, nNonOrientable, "closed non-orbl minimal",
                BoolSet::sFalse, BoolSet::sFalse, 0, true);
        }

        struct CensusSpec {
            BoolSet orbl_;
            bool minimal_;

            unsigned long count_;

            CensusSpec(BoolSet orbl, bool minimal) :
                orbl_(orbl), minimal_(minimal), count_(0) {}
        };

        static void foundPerms(const GluingPermSearcher<2>* perms, void* spec) {
            if (perms) {
                CensusSpec* s = static_cast<CensusSpec*>(spec);
                Triangulation<2>* tri = perms->triangulate();
                if ((! (s->minimal_ && ! tri->isMinimal())) &&
                        (! (s->orbl_ == BoolSet::sTrue &&
                            ! tri->isOrientable())) &&
                        (! (s->orbl_ == BoolSet::sFalse &&
                            tri->isOrientable())))
                    ++s->count_;
                delete tri;
            }
        }

        static void foundPairing(const FacetPairing<2>* pairing,
                const FacetPairing<2>::IsoList* autos, void* spec) {
            if (pairing) {
                CensusSpec* s = static_cast<CensusSpec*>(spec);
                GluingPermSearcher<2>::findAllPerms(pairing, autos,
                    ! s->orbl_.hasFalse(), foundPerms, spec);
            }
        }


        static void rawCountsCompare(unsigned minTris, unsigned maxTris,
                const unsigned* realAns, const char* censusType,
                BoolSet orientability, BoolSet boundary, int nBdryFaces,
                bool minimal) {
            for (unsigned nTris = minTris; nTris <= maxTris; nTris++) {
                CensusSpec spec(orientability, minimal);
                FacetPairing<2>::findAllPairings(nTris, boundary, nBdryFaces,
                    foundPairing, &spec);

                std::ostringstream msg;
                msg << "Census count for " << nTris << " tetrahedra ("
                    << censusType << ") should be " << realAns[nTris]
                    << ", not " << spec.count_ << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    spec.count_ == realAns[nTris]);
            }
        }
};

void addCensus2(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Census2Test::suite());
}

