
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "algebra/abeliangroup.h"
#include "algebra/grouppresentation.h"
#include "link/examplelink.h"
#include "link/link.h"
#include "snappea/snappeatriangulation.h"
#include "triangulation/example3.h"

#include "testsuite/algebra/testalgebra.h"

using regina::AbelianGroup;
using regina::GroupPresentation;
using regina::Link;
using regina::SnapPeaTriangulation;
using regina::Triangulation;

class CoversTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(CoversTest);

    CPPUNIT_TEST(trivial);
    CPPUNIT_TEST(manifolds);
    CPPUNIT_TEST(knots);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() {
        }

        void tearDown() {
        }

        template <int degree>
        std::string viaSnapPea(const Triangulation<3>& tri) {
            std::vector<std::string> covers;

            SnapPeaTriangulation s(tri);
            if (s.isNull())
                return "Null_SnapPea";

            s.enumerateCovers(degree, SnapPeaTriangulation::all_covers,
                    [&](const SnapPeaTriangulation& cover,
                    SnapPeaTriangulation::CoverType type) {
                const AbelianGroup* ab = cover.homologyFilled();
                if (ab)
                    covers.push_back(ab->str());
                else
                    covers.push_back("Null_AbelianGroup");
            });
            std::sort(covers.begin(), covers.end());

            std::string ans;
            for (const auto& s : covers) {
                ans += s;
                ans += " | ";
            }
            return ans;
        }

        template <int degree>
        std::string viaRegina(const Triangulation<3>& tri) {
            std::vector<std::string> covers;

            tri.fundamentalGroup().enumerateCovers<degree>([&](
                    GroupPresentation& g) {
                covers.push_back(g.abelianisation().str());
            });
            std::sort(covers.begin(), covers.end());

            std::string ans;
            for (const auto& s : covers) {
                ans += s;
                ans += " | ";
            }
            return ans;
        }

        template <int maxDegree>
        void verifyAndDelete(Triangulation<3>* tri,
                const char* name = nullptr) {
            std::string invSnapPea = viaSnapPea<maxDegree>(*tri);
            std::string invRegina = viaRegina<maxDegree>(*tri);
            // std::cerr << "INV: " << maxDegree << " -> " << invRegina << std::endl;
            if (invSnapPea != invRegina) {
                std::ostringstream msg;
                msg << "Invariants differ for "
                    << (name ? name : tri->label())
                    << " : Snappea -> " << invSnapPea
                    << " ; Regina -> " << invRegina;
                CPPUNIT_FAIL(msg.str());
            }

            if constexpr (maxDegree > 2) {
                verifyAndDelete<maxDegree - 1>(tri, name);
            } else {
                delete tri;
            }
        }

        template <int maxDegree>
        void verifyAndDelete(Link* link, const char* name = nullptr) {
            verifyAndDelete<maxDegree>(link->complement(),
                name ? name : link->label().c_str());
            delete link;
        }

        void trivial() {
            // No covers:
            verifyAndDelete<5>(regina::Example<3>::sphere());
        }

        void manifolds() {
            // Cover only for degree 5:
            verifyAndDelete<5>(regina::Example<3>::poincareHomologySphere());

            // Trivial cover only for degree 3:
            verifyAndDelete<5>(regina::Example<3>::lens(3, 1));

            // Covers for all degrees:
            verifyAndDelete<5>(regina::Example<3>::s2xs1());

            // Many covers for degree 5:
            verifyAndDelete<5>(regina::Example<3>::weeks());

            // Many, many covers for degree 5:
            verifyAndDelete<5>(regina::Example<3>::weberSeifert());
        }

        void knots() {
            verifyAndDelete<5>(regina::ExampleLink::unknot());
            verifyAndDelete<5>(regina::ExampleLink::trefoilRight());
            verifyAndDelete<5>(regina::ExampleLink::conway());
        }
};

void addCovers(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(CoversTest::suite());
}

