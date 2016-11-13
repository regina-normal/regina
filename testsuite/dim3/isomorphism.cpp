
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

#include <algorithm>
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "algebra/abeliangroup.h"
#include "triangulation/example3.h"
#include "triangulation/dim3.h"
#include "testsuite/dim3/testtriangulation.h"

using regina::AbelianGroup;
using regina::Example;
using regina::Isomorphism;
using regina::Perm;
using regina::Tetrahedron;
using regina::Triangulation;

class Isomorphism3Test : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(Isomorphism3Test);

    CPPUNIT_TEST(enumeration);
    CPPUNIT_TEST(application);
    CPPUNIT_TEST(isomorphic);
    CPPUNIT_TEST(automorphismsAndSubcomplexes);

    CPPUNIT_TEST_SUITE_END();

    private:
        typedef void (Isomorphism3Test::*IsoTest)(const Isomorphism<3>&,
            unsigned long);

        Triangulation<3> rp2xs1;
            /**< A three-tetrahedron closed non-orientable triangulation. */

        Triangulation<3> lens8_1;
            /**< A highly symmetric layered lens space. */
        Triangulation<3> lens13_3;
            /**< A less symmetric layered lens space. */
        Triangulation<3> twisted5;
            /**< A twisted layered loop. */
        Triangulation<3> untwisted5;
            /**< An untwisted layered loop. */
        Triangulation<3> fig8;
            /**< The figure eight knot complement. */
        Triangulation<3> aug;
            /**< A triangulation with no non-trivial symmetries whatsoever. */

        Triangulation<3> ball;
            /**< A standalone tetrahedron. */

    public:
        void setUp() {
            Triangulation<3>* t = Example<3>::rp2xs1();
            rp2xs1.insertTriangulation(*t);
            delete t;

            lens8_1.insertLayeredLensSpace(8, 1);
            lens13_3.insertLayeredLensSpace(13, 3);
            twisted5.insertLayeredLoop(5, true);
            untwisted5.insertLayeredLoop(5, false);
            aug.insertAugTriSolidTorus(3, -1, 5, -3, 2, -1);
            ball.newTetrahedron();
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

            unsigned* facePermIndex = new unsigned[n];

            Isomorphism<3> iso(n);
            unsigned long which = 0;
            do {
                // We have a permutation of tetrahedra.
                // Set up the initial isomorphism with identity face/vertex
                // mappings, and then look through all possible face/vertex
                // rearrangements.
                for (i = 0; i < n; i++) {
                    iso.tetImage(i) = tetPerm[i];
                    iso.facePerm(i) = Perm<4>::S4[facePermIndex[i] = 0];
                }

                while (1) {
                    if (test)
                        (this->*test)(iso, which);
                    which++;

                    // Move to the next face/vertex mapping.
                    pos = 0;
                    while (pos < n && facePermIndex[pos] == nVtxPerms - 1)
                        pos++;

                    if (pos == n)
                        break;

                    iso.facePerm(pos) = Perm<4>::S4[++facePermIndex[pos]];
                    while (pos > 0) {
                        pos--;
                        iso.facePerm(pos) = Perm<4>::S4[facePermIndex[pos] = 0];
                    }
                }
            } while (std::next_permutation(tetPerm, tetPerm + n));

            delete[] facePermIndex;
            delete[] tetPerm;
            return which;
        }

        void enumerationTest(const Isomorphism<3>& iso, unsigned long which) {
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
                &Isomorphism3Test::enumerationTest);

            unsigned long expected = nIsomorphisms(3);
            if (tot != expected) {
                std::ostringstream msg;
                msg << "A total of " << tot <<
                    " order 3 isomorphism(s) were found, not "
                    << expected << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void applicationTest(const Isomorphism<3>& iso, unsigned long which) {
            // Sigh.  This is slow; cut down the processing time.
            if (which % 11 != 0)
                return;

            Triangulation<3>* image = iso.apply(&rp2xs1);

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

            const AbelianGroup& h1 = image->homology();
            if (h1.rank() != 1 || h1.countInvariantFactors() != 1 ||
                    h1.invariantFactor(0) != 2)
                CPPUNIT_FAIL(msg.str() + "that had homology different from "
                    "Z + Z_2.");
            delete image;
        }

        void application() {
            enumerate(3, &Isomorphism3Test::applicationTest);
        }

        void isomorphicTest(const Isomorphism<3>& iso, unsigned long which) {
            // Sigh.  This is slow; cut down the processing time.
            if (which % 11 != 0)
                return;

            Triangulation<3>* image = iso.apply(&rp2xs1);
            if (! rp2xs1.isIsomorphicTo(*image).get()) {
                std::ostringstream msg;
                msg << "Isomorphism #" << which << " created a triangulation "
                    "that was not isomorphic to the original.";
                CPPUNIT_FAIL(msg.str());
            }
            delete image;
        }

        void isomorphic() {
            enumerate(3, &Isomorphism3Test::isomorphicTest);
        }

        void testAutomorphismsAndSubcomplexes(const Triangulation<3>& t,
                const char* name, unsigned long symmetries) {
            Triangulation<3> t2(t);

            if (! t2.isIsomorphicTo(t).get()) {
                std::ostringstream msg;
                msg << "Triangulation " << name <<
                    " is not isomorphic to itself.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! t2.isContainedIn(t).get()) {
                std::ostringstream msg;
                msg << "Triangulation " << name <<
                    " is not a subcomplex of itself.";
                CPPUNIT_FAIL(msg.str());
            }

            std::list<Isomorphism<3>*> isos;
            std::list<Isomorphism<3>*>::iterator it;
            unsigned long count;

            count = t2.findAllSubcomplexesIn(t, back_inserter(isos));
            if (count != symmetries) {
                std::ostringstream msg;
                msg << "Triangulation " << name << " has "
                    << count << " symmetries, not " << symmetries
                    << " as expected.";
                CPPUNIT_FAIL(msg.str());
            }
            if (isos.size() != count) {
                std::ostringstream msg;
                msg << "Triangulation " << name <<
                    " has a mismatched symmetry count (" << count
                    << " != " << isos.size() << ").";
                CPPUNIT_FAIL(msg.str());
            }

            for (it = isos.begin(); it != isos.end(); ++it)
                delete *it;
            isos.clear();

            // Some of these tests cannot be run on the standalone tetrahedron.
            bool standalone = (t.size() == 1 && t.countTriangles() == 4);

            // Unglue a face of t2.
            if (! standalone) {
                t2.tetrahedron(0)->unjoin(2);
                if (! t2.isContainedIn(t).get()) {
                    std::ostringstream msg;
                    msg << "Unjoining a face of " << name <<
                        " does not result in a subcomplex.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (t.isContainedIn(t2).get()) {
                    std::ostringstream msg;
                    msg << "Unjoining a face of " << name <<
                        " results in a supercomplex (and should not).";
                    CPPUNIT_FAIL(msg.str());
                }
            }

            // Completely remove a tetrahedron of t2.
            t2.removeTetrahedronAt(0);
            if (! t2.isContainedIn(t).get()) {
                std::ostringstream msg;
                msg << "Removing a tetrahedron of " << name <<
                    " does not result in a subcomplex.";
                CPPUNIT_FAIL(msg.str());
            }
            if (t.isContainedIn(t2).get()) {
                std::ostringstream msg;
                msg << "Removing a tetrahedron of " << name <<
                    " results in a supercomplex (and should not).";
                CPPUNIT_FAIL(msg.str());
            }

            // Add a lone tetrahedron.
            Tetrahedron<3>* tet = t2.newTetrahedron();
            if (! t2.isContainedIn(t).get()) {
                std::ostringstream msg;
                msg << "Isolating a tetrahedron of " << name <<
                    " does not result in a subcomplex.";
                CPPUNIT_FAIL(msg.str());
            }
            if ((! standalone) && t.isContainedIn(t2).get()) {
                std::ostringstream msg;
                msg << "Isolating a tetrahedron of " << name <<
                    " results in a supercomplex (and should not).";
                CPPUNIT_FAIL(msg.str());
            }

            // Make it no longer a subcomplex.
            // Do this by joining things together in a wacky invalid way.
            tet->join(0, tet, regina::Perm<4>(3, 2, 1, 0));
            if (t2.isContainedIn(t).get()) {
                std::ostringstream msg;
                msg << "Making a tetrahedron of " << name <<
                    " invalid results in a subcomplex (and should not).";
                CPPUNIT_FAIL(msg.str());
            }
            if ((! standalone) && t.isContainedIn(t2).get()) {
                std::ostringstream msg;
                msg << "Making a tetrahedron of " << name <<
                    " invalid results in a supercomplex (and should not).";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void automorphismsAndSubcomplexes() {
            testAutomorphismsAndSubcomplexes(lens8_1, "L(8,1)", 4);
            testAutomorphismsAndSubcomplexes(lens13_3, "L(13,3)", 2);
            testAutomorphismsAndSubcomplexes(twisted5, "C~(5)", 20);
            testAutomorphismsAndSubcomplexes(untwisted5, "C(5)", 20);
            testAutomorphismsAndSubcomplexes(aug, "A(3,-1 | 5,-3)", 1);
            testAutomorphismsAndSubcomplexes(ball, "Ball", 24);
        }
};

void addIsomorphism3(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(Isomorphism3Test::suite());
}

