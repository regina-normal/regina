
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

#include <algorithm>
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "algebra/nabeliangroup.h"
#include "triangulation/nexampletriangulation.h"
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"
#include "testsuite/triangulation/testtriangulation.h"

using regina::NAbelianGroup;
using regina::NExampleTriangulation;
using regina::NIsomorphism;
using regina::NIsomorphismDirect;
using regina::NIsomorphismIndexed;
using regina::NTriangulation;

class NIsomorphismTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(NIsomorphismTest);

    CPPUNIT_TEST(enumeration);
    CPPUNIT_TEST(application);
    CPPUNIT_TEST(isomorphic);

    CPPUNIT_TEST_SUITE_END();

    private:
        typedef void (NIsomorphismTest::*IsoTest)(const NIsomorphism&,
            unsigned long);

        NTriangulation rp2xs1;
            /**< A three-tetrahedron closed non-orientable triangulation. */

    public:
        void setUp() {
            NTriangulation* t = NExampleTriangulation::rp2xs1();
            rp2xs1.insertTriangulation(*t);
            delete t;
        }

        void tearDown() {
        }

        unsigned long nIsomorphisms(unsigned long n) {
            // Returns the number of isomorphisms of the given order,
            // that is, (n! * 24^n).
            long ans;
            for (ans = 1; n > 0; n--)
                ans = ans * 24 * n;
            return ans;
        }

        unsigned long enumerate(unsigned n, IsoTest test) {
            // Enumerates all isomorphisms of the given order and
            // passes each to the given test routine in turn.
            // Returns the total number of isomorphisms found.
            // Requires n > 0.
            static const int nVtxPerms = 24;

            unsigned i, pos;

            unsigned* tetPerm = new unsigned[n];
            for (i = 0; i < n; i++)
                tetPerm[i] = i;

            NIsomorphismIndexed iso(n);
            unsigned long which = 0;
            do {
                // We have a permutation of tetrahedra.
                // Set up the initial isomorphism with identity face/vertex
                // mappings, and then look through all possible face/vertex
                // rearrangements.
                for (i = 0; i < n; i++) {
                    iso.tetImage(i) = tetPerm[i];
                    iso.facePermIndex(i) = 0;
                }

                while (1) {
                    if (test)
                        (this->*test)(iso, which);
                    which++;

                    // Move to the next face/vertex mapping.
                    pos = 0;
                    while (pos < n && iso.facePermIndex(pos) == nVtxPerms - 1)
                        pos++;

                    if (pos == n)
                        break;

                    iso.facePermIndex(pos)++;
                    while (pos > 0) {
                        pos--;
                        iso.facePermIndex(pos) = 0;
                    }
                }
            } while (std::next_permutation(tetPerm, tetPerm + n));

            delete[] tetPerm;
            return which;
        }

        void enumerationTest(const NIsomorphism& iso, unsigned long which) {
            if (which == 0) {
                if (! iso.isIdentity()) {
                    std::ostringstream msg;
                    msg << "Isomorphism #" << which << " was found to be "
                        "a non-identity isomorphism.";
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                if (iso.isIdentity()) {
                    std::ostringstream msg;
                    msg << "Isomorphism #" << which << " was found to be "
                        "the identity isomorphism.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void enumeration() {
            unsigned long tot = enumerate(3,
                &NIsomorphismTest::enumerationTest);

            unsigned long expected = nIsomorphisms(3);
            if (tot != expected) {
                std::ostringstream msg;
                msg << "A total of " << tot <<
                    " order 3 isomorphism(s) were found, not "
                    << expected << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void applicationTest(const NIsomorphism& iso, unsigned long which) {
            // Sigh.  This is slow; cut down the processing time.
            if (which % 11 != 0)
                return;

            NTriangulation* image = iso.apply(&rp2xs1);

            std::ostringstream msg;
            msg << "Isomorphism #" << which << " created a copy of RP2xS1 ";

            if (image->isOrientable())
                CPPUNIT_FAIL(msg.str() + "that was orientable.");
            if (! image->isValid())
                CPPUNIT_FAIL(msg.str() + "that was invalid.");
            if (! image->isStandard())
                CPPUNIT_FAIL(msg.str() + "that was non-standard.");
            if (! image->isClosed())
                CPPUNIT_FAIL(msg.str() + "that was not closed.");

            const NAbelianGroup& h1 = image->getHomologyH1();
            if (h1.getRank() != 1 || h1.getNumberOfInvariantFactors() != 1 ||
                    h1.getInvariantFactor(0) != 2)
                CPPUNIT_FAIL(msg.str() + "that had homology different from "
                    "Z + Z_2.");
            delete image;
        }

        void application() {
            enumerate(3, &NIsomorphismTest::applicationTest);
        }

        void isomorphicTest(const NIsomorphism& iso, unsigned long which) {
            // Sigh.  This is slow; cut down the processing time.
            if (which % 11 != 0)
                return;

            NTriangulation* image = iso.apply(&rp2xs1);
            if (! rp2xs1.isIsomorphicTo(*image).get()) {
                std::ostringstream msg;
                msg << "Isomorphism #" << which << " created a triangulation "
                    "that was not isomorphic to the original.";
                CPPUNIT_FAIL(msg.str());
            }
            delete image;
        }

        void isomorphic() {
            enumerate(3, &NIsomorphismTest::isomorphicTest);
        }
};

void addNIsomorphism(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(NIsomorphismTest::suite());
}

