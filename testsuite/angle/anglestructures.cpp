
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

#include <cppunit/extensions/HelperMacros.h>
#include "angle/anglestructures.h"
#include "surfaces/normalsurface.h"
#include "triangulation/example3.h"
#include "triangulation/dim3.h"

#include "testsuite/exhaustive.h"
#include "testsuite/angle/testangle.h"

using regina::AngleStructure;
using regina::AngleStructures;
using regina::AngleStructureVector;
using regina::Example;
using regina::Tetrahedron;
using regina::Triangulation;

class AngleStructuresTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(AngleStructuresTest);

    CPPUNIT_TEST(empty);
    CPPUNIT_TEST(oneTet);
    CPPUNIT_TEST(gieseking);
    CPPUNIT_TEST(figure8);
    CPPUNIT_TEST(loopC2);
    CPPUNIT_TEST(taut);
    CPPUNIT_TEST(tautVsAll);
    CPPUNIT_TEST(tautStrictTreeVsDD);

    CPPUNIT_TEST_SUITE_END();

    private:
        Triangulation<3> triEmpty;
            /**< An empty triangulation. */
        Triangulation<3> triOneTet;
            /**< A single tetrahedron (with no face gluings). */
        Triangulation<3> triGieseking;
            /**< The Gieseking manifold. */
        Triangulation<3> triFigure8;
            /**< The figure eight knot complement. */
        Triangulation<3> triLoopC2;
            /**< An untwisted layered loop of length 2. */

    public:
        void copyAndDelete(Triangulation<3>& dest, Triangulation<3>* source) {
            dest.insertTriangulation(*source);
            delete source;
        }

        void setUp() {
            // Use pre-coded triangulations where we can.
            copyAndDelete(triFigure8,
                Example<3>::figureEight());
            copyAndDelete(triGieseking, Example<3>::gieseking());

            // Layered loops can be constructed automatically.
            triLoopC2.insertLayeredLoop(2, false);

            // Other things must be done manually.
            triOneTet.newTetrahedron();
        }

        void tearDown() {
        }

        void testSize(AngleStructures* list, const char* triName,
                unsigned long expectedSize,
                bool allowStrict, bool allowTaut) {
            {
                std::ostringstream msg;
                msg << "Number of angle structures for " << triName
                    << " should be " << expectedSize << ", not "
                    << list->size() << '.';

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    list->size() == expectedSize);
            }
            {
                std::ostringstream msg;
                msg << "Angle structures for " << triName
                    << " should ";
                if (! allowStrict)
                    msg << "not ";
                msg << "support strict angle structures.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    list->spansStrict() == allowStrict);
            }
            {
                std::ostringstream msg;
                msg << "Angle structures for " << triName
                    << " should ";
                if (! allowTaut)
                    msg << "not ";
                msg << "support taut angle structures.";

                CPPUNIT_ASSERT_MESSAGE(msg.str(),
                    list->spansTaut() == allowTaut);
            }
        }

        void countStructures(const AngleStructures* list,
                const char* triName, unsigned long expectedCount,
                bool strict, bool taut) {
            unsigned long tot = 0;
            unsigned long size = list->size();

            const AngleStructure* s;
            for (unsigned long i = 0; i < size; i++) {
                s = list->structure(i);

                if (s->isStrict() == strict && s->isTaut() == taut)
                    tot++;
            }

            std::ostringstream msg;
            msg << "Number of ";
            if (strict)
                msg << "strict ";
            else if (taut)
                msg << "taut ";
            else
                msg << "generic ";
            msg << "angle structures on " << triName << " should be "
                << expectedCount << ", not " << tot << '.';
            CPPUNIT_ASSERT_MESSAGE(msg.str(), expectedCount == tot);
        }

        void empty() {
            AngleStructures* list = AngleStructures::enumerate(
                &triEmpty);

            testSize(list, "the empty triangulation", 1, true, true);

            delete list;
        }

        void oneTet() {
            AngleStructures* list = AngleStructures::enumerate(
                &triOneTet);

            testSize(list, "a standalone tetrahedron", 3, true, true);
            countStructures(list, "a standalone tetrahedron", 3,
                false /* strict */, true /* taut */);

            delete list;
        }

        void gieseking() {
            AngleStructures* list = AngleStructures::enumerate(
                &triGieseking);

            testSize(list, "the Gieseking manifold", 3, true, true);
            countStructures(list, "the Gieseking manifold", 3,
                false /* strict */, true /* taut */);

            delete list;
        }

        void figure8() {
            AngleStructures* list = AngleStructures::enumerate(
                &triFigure8);

            testSize(list, "the figure eight knot complement", 5, true, true);
            countStructures(list, "the figure eight knot complement", 3,
                false /* strict */, true /* taut */);
            countStructures(list, "the figure eight knot complement", 2,
                false /* strict */, false /* taut */);

            delete list;
        }

        void loopC2() {
            AngleStructures* list = AngleStructures::enumerate(
                &triLoopC2);

            testSize(list, "the untwisted layered loop C(2)", 0, false, false);

            delete list;
        }

        void verifyTaut(const char* isoSig, unsigned long nTaut) {
            Triangulation<3>* tri = Triangulation<3>::fromIsoSig(isoSig);
            if (! tri) {
                std::ostringstream msg;
                msg << "Could not reconstruct from isoSig: " << isoSig << ".";
                CPPUNIT_FAIL(msg.str());
            }

            AngleStructures* a = AngleStructures::enumerate(tri, true);
            if (a->size() != nTaut) {
                std::ostringstream msg;
                msg << "Taut angle structures for " << isoSig << ": "
                    "found " << a->size()
                    << " structures instead of the expected " << nTaut << ".";
                CPPUNIT_FAIL(msg.str());
            }

            unsigned long i, j, k;
            regina::Rational tmp, tot;
            regina::Edge<3>* e;
            for (i = 0; i < a->size(); ++i) {
                const AngleStructure* s = a->structure(i);

                for (j = 0; j < tri->size(); ++j) {
                    tot = 0;
                    for (k = 0; k < 3; ++k) {
                        tmp = s->angle(j, k);
                        if (tmp != 0 && tmp != 1) {
                            std::ostringstream msg;
                            msg << "Taut angle structures #" << i
                                << " for " << isoSig << ": bad angle found.";
                            CPPUNIT_FAIL(msg.str());
                        }
                        tot += tmp;
                    }
                    if (tot != 1) {
                        std::ostringstream msg;
                        msg << "Taut angle structures #" << i
                            << " for " << isoSig << ": wrong number of "
                            "pi angles in tetrahedron " << j << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                for (j = 0; j < tri->countEdges(); ++j) {
                    e = tri->edge(j);
                    if (e->isBoundary())
                        continue;

                    tot = 0;
                    for (k = 0; k < e->degree(); ++k) {
                        tot += s->angle(
                            e->embedding(k).tetrahedron()->index(),
                            regina::quadSeparating[
                                e->embedding(k).vertices()[0]][
                                e->embedding(k).vertices()[1]]);
                    }
                    if (tot != 2) {
                        std::ostringstream msg;
                        msg << "Taut angle structures #" << i
                            << " for " << isoSig << ": wrong sum of "
                            "angles around edge " << j << ".";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }

            delete a;
            delete tri;
        }

        void taut() {
            // The following taut angle structure counts were computed
            // using Regina 4.95, via the old double description method.

            // The trefoil:
            verifyTaut("cPcbbbadu", 1);

            // Some small hyperbolic knots:
            verifyTaut("cPcbbbiht", 3);
            verifyTaut("dLQbcccdero", 4);
            verifyTaut("gLLMQccefeffdfeqldg", 15);
            verifyTaut("fLLQcbcdeeemgopdp", 7);
            verifyTaut("eLPkbcddddcwjb", 4);

            // Some larger hyperbolic knots:
            verifyTaut("qLvALPzLMQMkbfefhhijmonmnoppppapmggfmgxjgjpeeo", 80);
            verifyTaut("qLLLLAzzPPQkcefehikilmnpmnoppiitdsvivjvvukvunb", 189);
            verifyTaut("qLLLLPzPwPQkcefehjkjljopppnooiitdvqiepdtidxfmi", 127);
            verifyTaut("qLLzPvMzPAQkccdghhjjmlonnnppphgggcbagbvdatdasb", 206);
            verifyTaut("rLLLMzwwPAMQccdfegihimlnompqqpqiceakanuandeuoamom", 145);
            verifyTaut("rLvLvvQPAPQQccfimimliokqlqpqoppdejdvdaglcelsgsfgj", 97);
            verifyTaut("vLLvLvMzAzMAQQQcehlnkilrqppootssututuiiiaiicimgggooabfaalll", 440);
            verifyTaut("uLLvLLvMALQMQQcceihoijpsqnorqorsttrtiimriwfiiksokjovllxoj", 440);
            verifyTaut("sLLvLLvLQAQQQceihimlqqmronpoprpriimrwlmmilifskbvlga", 396);
            verifyTaut("qLvLLvPQLQQkbefhnlnkmolkpmoppmtmohhhuaautvbbkb", 62);

            // Examples from Jonathan:
            verifyTaut("hLvQAkcdcfeeggqjjqhnqj", 0);
            verifyTaut("oLLLAAwzPQcbedgfhfilknmnnmxxnxhxjxxkxqaxw", 0);
            verifyTaut("vvLLAvQvMwwQMQQcdheglkjpopsnstqsrutuuwrawwxhwxhclrmhaqwrrrr", 0);
            verifyTaut("CLwvvwQAzAPwQLwLQzQkaciklmhjmonrqptspvuwxyzzyBAABjghqqhxxgaaaahaajaahaharwqqkn", 0);
            verifyTaut("CLvzMzwQwwvzzQPQPPAkccfigkmiljkovqxustywyxzxAyABBqjqwfqqoaajvujaljladbrxwxwvxw", 4);

            verifyTaut("JLwvvvwwLzwLQAQQPwvQLQQQcackhnsqpxtoyuwtzyrvBywGAzHIEDFHIGFGIngqxgabgaabqhqxhqxrarrabkbgcnnnkrrww", 0);
            verifyTaut("JLwvvvwwLzwLMQQQPMzMPMQPcackhnsqpxovutwAzyrvByxuAEzCGDEHFGIHIjgqxgabgabqhxqxhqxrarrjwajwrwrqrrxjj", 0);
            verifyTaut("JLwvvwMwzMLPzAAvMQLwQAQQcacikmhmqounrtpsyxvDwBCEFEFAHCGHGHFIIjghqhxgababhqxrabwabhahajoslfhahawgj", 0);
            verifyTaut("JLvzMzwQwwvPAPwQwAPvPAMQcccfigkmiljkoutuwvutxtyzACBDEGFGFIHHIqjqwfqqoaajvqhlaallalaaaaaaqqajshhrr", 4);
            verifyTaut("JLLLAAwzLLAwQwvvwMAQAAQMcbedgfhfilnnnpoqrstvCxEBDzFAFEGEFHHIIxxnxhxjxxxaxgvcxxafenatpkatbwqrrqfqr", 0);
        }

        void verifyTautVsAll(Triangulation<3>* t, const char* name) {
            AngleStructures* all = AngleStructures::enumerate(t, false);
            AngleStructures* taut = AngleStructures::enumerate(t, true);

            if (all->isTautOnly()) {
                std::ostringstream msg;
                msg << "Non-taut-only enumeration on " << name
                    << " produced a list marked as taut-only.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! taut->isTautOnly()) {
                std::ostringstream msg;
                msg << "Taut-only enumeration on " << name
                    << " produced a list marked as non-taut-only.";
                CPPUNIT_FAIL(msg.str());
            }

            unsigned nAll = 0, nTaut = 0;
            unsigned i;

            for (i = 0; i < all->size(); ++i)
                if (all->structure(i)->isTaut())
                    ++nAll;

            for (i = 0; i < taut->size(); ++i)
                if (taut->structure(i)->isTaut())
                    ++nTaut;
                else {
                    std::ostringstream msg;
                    msg << "Taut-only enumeration on " << name
                        << " produced a non-taut angle structure.";
                    CPPUNIT_FAIL(msg.str());
                }

            if (nAll != nTaut) {
                std::ostringstream msg;
                msg << "Taut counts mismatched for taut-only vs "
                    "all-structures enumeration on " << name <<
                    " (" << nTaut << " vs " << nAll << ")";
                CPPUNIT_FAIL(msg.str());
            }

            delete all;
            delete taut;
        }

        void verifyTautVsAll(const char* dehydration) {
            Triangulation<3>* t = Triangulation<3>::rehydrate(dehydration);
            if (t->isEmpty()) {
                std::ostringstream msg;
                msg << "Failed to rehydrate " << dehydration << ".";
                CPPUNIT_FAIL(msg.str());
            }

            verifyTautVsAll(t, dehydration);

            delete t;
        }

        void tautVsAll() {
            verifyTautVsAll("baaaade"); // m000
            verifyTautVsAll("dadbcccaqrb"); // m010
            verifyTautVsAll("hbnajbcdeefgghvfeevho"); // v1000
            verifyTautVsAll("hepacdefegfggcurmsktu"); // y500

            verifyTautVsAll(&triEmpty, "the empty triangulation");
            verifyTautVsAll(&triOneTet, "a standalone tetrahedron");
        }

        static bool lexLess(const AngleStructureVector* a,
                const AngleStructureVector* b) {
            for (unsigned i = 0; i < a->size(); ++i) {
                if ((*a)[i] < (*b)[i])
                    return true;
                if ((*a)[i] > (*b)[i])
                    return false;
            }
            return false;
        }

        static bool identical(const AngleStructures* lhs,
                const AngleStructures* rhs) {
            if (lhs->size() != rhs->size())
                return false;

            unsigned long n = lhs->size();
            if (n == 0)
                return true;

            typedef const AngleStructureVector* VecPtr;
            VecPtr* lhsRaw = new VecPtr[n];
            VecPtr* rhsRaw = new VecPtr[n];

            unsigned long i;
            for (i = 0; i < n; ++i) {
                lhsRaw[i] = lhs->structure(i)->rawVector();
                rhsRaw[i] = rhs->structure(i)->rawVector();
            }

            std::sort(lhsRaw, lhsRaw + n, lexLess);
            std::sort(rhsRaw, rhsRaw + n, lexLess);

            bool ok = true;
            for (i = 0; i < n; ++i)
                if (! (*(lhsRaw[i]) == *(rhsRaw[i]))) {
                    ok = false;
                    break;
                }

            delete[] lhsRaw;
            delete[] rhsRaw;
            return ok;
        }

        static bool identicalTaut(const AngleStructures* all,
                const AngleStructures* taut) {
            if (all->size() < taut->size())
                return false;

            unsigned long nAll = all->size();
            unsigned long nTaut = taut->size();

            typedef const AngleStructureVector* VecPtr;
            VecPtr* allRaw = new VecPtr[nAll + 1];
            VecPtr* tautRaw = new VecPtr[nTaut + 1];

            unsigned long i;
            unsigned long foundAll = 0;
            for (i = 0; i < nAll; ++i)
                if (all->structure(i)->isTaut())
                    allRaw[foundAll++] = all->structure(i)->rawVector();
            for (i = 0; i < nTaut; ++i)
                tautRaw[i] = taut->structure(i)->rawVector();

            if (foundAll != nTaut) {
                delete[] allRaw;
                delete[] tautRaw;
                return false;
            }

            std::sort(allRaw, allRaw + nTaut, lexLess);
            std::sort(tautRaw, tautRaw + nTaut, lexLess);

            bool ok = true;
            for (i = 0; i < nTaut; ++i)
                if (! (*(allRaw[i]) == *(tautRaw[i]))) {
                    ok = false;
                    break;
                }

            delete[] allRaw;
            delete[] tautRaw;
            return ok;
        }

        static void verifyTreeVsDD(Triangulation<3>* tri) {
            AngleStructures* all = AngleStructures::enumerate(
                tri, false);
            AngleStructures* tautTree = AngleStructures::enumerate(
                tri, true);
            AngleStructures* tautDD = AngleStructures::enumerateTautDD(
                tri);
            bool strictTree = tri->hasStrictAngleStructure();

            if (all->isTautOnly()) {
                std::ostringstream msg;
                msg << "Vertex angle structure enumeration gives "
                    "incorrect flags for " << tri->label() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! tautTree->isTautOnly()) {
                std::ostringstream msg;
                msg << "Taut angle structure enumeration (tree) gives "
                    "incorrect flags for " << tri->label() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! tautDD->isTautOnly()) {
                std::ostringstream msg;
                msg << "Taut angle structure enumeration (DD) gives "
                    "incorrect flags for " << tri->label() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (all->spansTaut() != tautTree->spansTaut() ||
                    all->spansTaut() != tautDD->spansTaut()) {
                std::ostringstream msg;
                msg << "Flag for spansTaut() mismatched between "
                    "different enumeration methods for "
                    << tri->label() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (all->spansStrict() && ! strictTree) {
                std::ostringstream msg;
                msg << "Finding a strict angle structure (tree) gives "
                    "no solution when one should exist for "
                    << tri->label() << ".";
                CPPUNIT_FAIL(msg.str());
            }
            if (strictTree && ! all->spansStrict()) {
                std::ostringstream msg;
                msg << "Finding a strict angle structure (tree) gives "
                    "a solution when none should exist for "
                    << tri->label() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! identical(tautTree, tautDD)) {
                std::ostringstream msg;
                msg << "Taut angle structure enumeration gives "
                    "different solutions for tree vs DD for "
                    << tri->label() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            if (! identicalTaut(all, tautTree)) {
                std::ostringstream msg;
                msg << "Taut angle structure enumeration (tree) gives "
                    "different taut solutions from full vertex enumeration "
                    "for "<< tri->label() << ".";
                CPPUNIT_FAIL(msg.str());
            }

            delete all;
            delete tautTree;
            delete tautDD;
        }

        void tautStrictTreeVsDD() {
            runCensusAllIdeal(verifyTreeVsDD);
            runCensusAllClosed(verifyTreeVsDD); // Should be no solns.
            runCensusAllBounded(verifyTreeVsDD); // May have partial solns.
        }
};

void addAngleStructures(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(AngleStructuresTest::suite());
}

