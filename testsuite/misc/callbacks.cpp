
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

#include <cstring>
#include <string>
#include <cppunit/extensions/HelperMacros.h>
#include "testsuite/utilities/testutilities.h"
#include "link/examplelink.h"
#include "link/link.h"
#include "split/sigcensus.h"
#include "triangulation/example3.h"
#include "triangulation/example4.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/facetpairing3.h"

namespace {
    class Arg {
        private:
            bool copied_ { false };
            bool moved_ { false };
            bool flagged_ { false };

        public:
            Arg() = default;
            Arg(const Arg& src) {
                const_cast<Arg&>(src).copied_ = true;
            }
            Arg(Arg&& src) {
                src.moved_ = true;
            }

            void flag() { flagged_ = true; }

            bool copied() const { return copied_; }
            bool moved() const { return moved_; }
            bool flagged() const { return flagged_; }
    };
}

class CallbacksTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(CallbacksTest);

    CPPUNIT_TEST(passByReference);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() override {
        }

        void tearDown() override {
        }

        void verifyPassedByReference(const Arg& arg,
                const char* function) {
            if (arg.copied()) {
                std::stringstream msg;
                msg << function << " unexpectedly copied its action argument "
                    "instead of passing by reference.";
                CPPUNIT_FAIL(msg.str());
            }
            if (arg.moved()) {
                std::stringstream msg;
                msg << function << " unexpectedly moved its action argument "
                    "instead of passing by reference.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! arg.flagged()) {
                std::stringstream msg;
                msg << function << " does not seem to have passed its "
                    "action argument by reference.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void passByReference() {
            // ModelLinkGraph::generateMinimalLinks()
            //
            // GluingPermSearcher<2,3,4> and subclasses.. lots of stuff
            //
            // TreeEnumeration::run()
            // TautEnumeration::run()
            //
            // GroupPresentation::enumerateCovers()
            // SnapPeaTriangulation::enumerateCovers()
            //
            // SatRegion::find()

            {
                regina::Triangulation<3> t = regina::Example<3>::s2xs1();

                Arg a;
                t.findAllIsomorphisms(t,
                        [](const regina::Isomorphism<3>&, Arg& arg) {
                    arg.flag();
                    return false;
                }, std::ref(a));
                verifyPassedByReference(a,
                    "Triangulation<3>::findAllIsomorphisms()");

                Arg b;
                t.findAllIsomorphisms(t,
                        [](const regina::Isomorphism<3>&, Arg& arg) {
                    arg.flag();
                    return false;
                }, b);
                verifyPassedByReference(b,
                    "Triangulation<3>::findAllIsomorphisms()");

                Arg c;
                t.findAllSubcomplexesIn(t,
                        [](const regina::Isomorphism<3>&, Arg& arg) {
                    arg.flag();
                    return false;
                }, std::ref(c));
                verifyPassedByReference(c,
                    "Triangulation<3>::findAllSubcomplexesIn()");

                Arg d;
                t.findAllSubcomplexesIn(t,
                        [](const regina::Isomorphism<3>&, Arg& arg) {
                    arg.flag();
                    return false;
                }, d);
                verifyPassedByReference(d,
                    "Triangulation<3>::findAllSubcomplexesIn()");
            }
            {
                Arg a;
                regina::Example<3>::s2xs1().retriangulate(1, 1, nullptr,
                        [](const regina::Triangulation<3>&, Arg& arg) {
                    arg.flag();
                    return false;
                }, std::ref(a));
                verifyPassedByReference(a, "Triangulation<3>::retriangulate()");

                Arg b;
                regina::Example<3>::s2xs1().retriangulate(1, 1, nullptr,
                        [](const regina::Triangulation<3>&, Arg& arg) {
                    arg.flag();
                    return false;
                }, b);
                verifyPassedByReference(b, "Triangulation<3>::retriangulate()");

                Arg c;
                regina::Example<3>::s2xs1().retriangulate(1, 1, nullptr,
                        [](const std::string&, const regina::Triangulation<3>&,
                            Arg& arg) {
                    arg.flag();
                    return false;
                }, std::ref(c));
                verifyPassedByReference(c, "Triangulation<3>::retriangulate()");

                Arg d;
                regina::Example<3>::s2xs1().retriangulate(1, 1, nullptr,
                        [](const std::string&, const regina::Triangulation<3>&,
                            Arg& arg) {
                    arg.flag();
                    return false;
                }, d);
                verifyPassedByReference(d, "Triangulation<3>::retriangulate()");
            }
            {
                Arg a;
                regina::Example<4>::rp4().retriangulate(0, 1, nullptr,
                        [](const regina::Triangulation<4>&, Arg& arg) {
                    arg.flag();
                    return false;
                }, std::ref(a));
                verifyPassedByReference(a, "Triangulation<4>::retriangulate()");

                Arg b;
                regina::Example<4>::rp4().retriangulate(0, 1, nullptr,
                        [](const regina::Triangulation<4>&, Arg& arg) {
                    arg.flag();
                    return false;
                }, b);
                verifyPassedByReference(b, "Triangulation<4>::retriangulate()");

                Arg c;
                regina::Example<4>::rp4().retriangulate(0, 1, nullptr,
                        [](const std::string&, const regina::Triangulation<4>&,
                            Arg& arg) {
                    arg.flag();
                    return false;
                }, std::ref(c));
                verifyPassedByReference(c, "Triangulation<4>::retriangulate()");

                Arg d;
                regina::Example<4>::rp4().retriangulate(0, 1, nullptr,
                        [](const std::string&, const regina::Triangulation<4>&,
                            Arg& arg) {
                    arg.flag();
                    return false;
                }, d);
                verifyPassedByReference(d, "Triangulation<4>::retriangulate()");
            }
            {
                Arg a;
                regina::ExampleLink::trefoil().rewrite(2, 1, nullptr,
                        [](const regina::Link&, Arg& arg) {
                    arg.flag();
                    return false;
                }, std::ref(a));
                verifyPassedByReference(a, "Link::rewrite()");

                Arg b;
                regina::ExampleLink::trefoil().rewrite(2, 1, nullptr,
                        [](const regina::Link&, Arg& arg) {
                    arg.flag();
                    return false;
                }, b);
                verifyPassedByReference(b, "Link::rewrite()");

                Arg c;
                regina::ExampleLink::trefoil().rewrite(2, 1, nullptr,
                        [](const std::string&, const regina::Link&, Arg& arg) {
                    arg.flag();
                    return false;
                }, std::ref(c));
                verifyPassedByReference(c, "Link::rewrite()");

                Arg d;
                regina::ExampleLink::trefoil().rewrite(2, 1, nullptr,
                        [](const std::string&, const regina::Link&, Arg& arg) {
                    arg.flag();
                    return false;
                }, d);
                verifyPassedByReference(d, "Link::rewrite()");
            }
            {
                Arg a;
                regina::FacetPairing<3>::findAllPairings(1, false, 0,
                        [](const regina::FacetPairing<3>&,
                            const regina::FacetPairing<3>::IsoList&, Arg& arg) {
                    arg.flag();
                }, std::ref(a));
                verifyPassedByReference(a,
                    "FacetPairing<3>::findAllPairings()");

                Arg b;
                regina::FacetPairing<3>::findAllPairings(1, false, 0,
                        [](const regina::FacetPairing<3>&,
                            const regina::FacetPairing<3>::IsoList&, Arg& arg) {
                    arg.flag();
                }, b);
                verifyPassedByReference(b,
                    "FacetPairing<3>::findAllPairings()");
            }
            {
                Arg a;
                regina::SigCensus::formCensus(1,
                        [](const regina::Signature&,
                            const regina::SigCensus::IsoList&, Arg& arg) {
                    arg.flag();
                }, std::ref(a));
                verifyPassedByReference(a, "SigCensus::formCensus()");

                Arg b;
                regina::SigCensus::formCensus(1,
                        [](const regina::Signature&,
                            const regina::SigCensus::IsoList&, Arg& arg) {
                    arg.flag();
                }, b);
                verifyPassedByReference(b, "SigCensus::formCensus()");
            }

            // Routines that use callbacks but whose callbacks don't
            // have arguments:
            //
            // CensusDB::lookup()
            // DoubleDescription::enumerateExtremalRays()
            // HilbertCD::enumerateHilbertBasis()
            // HilbertDual::enumerateHilbertBasis()
            // HilbertPrimal::enumerateHilbertBasis()
        }
};

void addCallbacks(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(CallbacksTest::suite());
}

