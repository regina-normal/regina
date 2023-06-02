
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

#include <cstring>
#include <string>
#include "census/gluingpermsearcher2.h"
#include "census/gluingpermsearcher3.h"
#include "census/gluingpermsearcher4.h"
#include "enumerate/treetraversal.h"
#include "link/examplelink.h"
#include "link/link.h"
#include "link/modellinkgraph.h"
#include "snappea/examplesnappea.h"
#include "snappea/snappeatriangulation.h"
#include "split/sigcensus.h"
#include "subcomplex/satregion-impl.h"
#include "triangulation/example2.h"
#include "triangulation/example3.h"
#include "triangulation/example4.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "triangulation/facetpairing.h"
#include "triangulation/facetpairing3.h"

#include "testhelper.h"

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

static void verifyPassedByReference(const Arg& arg, const char* function) {
    SCOPED_TRACE_CSTRING(function);

    // Ensure that the argument was not copied or moved.
    EXPECT_FALSE(arg.copied());
    EXPECT_FALSE(arg.moved());

    // Ensure that the argument *was* passed by reference.
    EXPECT_TRUE(arg.flagged());
}

TEST(CallbacksTest, passByReference) {
    // ----- Isomorphism / subcomplex testing -----

    {
        regina::Triangulation<3> t = regina::Example<3>::s2xs1();

        Arg b;
        t.findAllIsomorphisms(t,
                [](const regina::Isomorphism<3>&, Arg& arg) {
            arg.flag();
            return false;
        }, b);
        verifyPassedByReference(b,
            "Triangulation<3>::findAllIsomorphisms()");

        Arg d;
        t.findAllSubcomplexesIn(t,
                [](const regina::Isomorphism<3>&, Arg& arg) {
            arg.flag();
            return false;
        }, d);
        verifyPassedByReference(d,
            "Triangulation<3>::findAllSubcomplexesIn()");
    }

    // ----- Retriangulation / rewriting -----

    {
        Arg b;
        regina::Example<3>::s2xs1().retriangulate(1, 1, nullptr,
                [](const regina::Triangulation<3>&, Arg& arg) {
            arg.flag();
            return false;
        }, b);
        verifyPassedByReference(b, "Triangulation<3>::retriangulate()");

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
        Arg b;
        regina::Example<4>::rp4().retriangulate(2, 1, nullptr,
                [](const regina::Triangulation<4>&, Arg& arg) {
            arg.flag();
            return false;
        }, b);
        verifyPassedByReference(b, "Triangulation<4>::retriangulate()");

        Arg d;
        regina::Example<4>::rp4().retriangulate(2, 1, nullptr,
                [](const std::string&, const regina::Triangulation<4>&,
                    Arg& arg) {
            arg.flag();
            return false;
        }, d);
        verifyPassedByReference(d, "Triangulation<4>::retriangulate()");
    }
    {
        Arg b;
        regina::ExampleLink::trefoil().rewrite(2, 1, nullptr,
                [](const regina::Link&, Arg& arg) {
            arg.flag();
            return false;
        }, b);
        verifyPassedByReference(b, "Link::rewrite()");

        Arg d;
        regina::ExampleLink::trefoil().rewrite(2, 1, nullptr,
                [](const std::string&, const regina::Link&, Arg& arg) {
            arg.flag();
            return false;
        }, d);
        verifyPassedByReference(d, "Link::rewrite()");
    }

    // ----- Finite-index subgroups -----

    {
        Arg b;
        regina::GroupPresentation(2).enumerateCovers<2>([](
                const regina::GroupPresentation&, Arg& arg) {
            arg.flag();
        }, b);
        verifyPassedByReference(b,
            "GroupPresentation::enumerateCovers()");
    }
    {
        Arg b;
        regina::ExampleSnapPea::figureEight().enumerateCovers(
                2, regina::SnapPeaTriangulation::all_covers,
                [](const regina::SnapPeaTriangulation&,
                    regina::SnapPeaTriangulation::CoverType, Arg& arg) {
            arg.flag();
        }, b);
        verifyPassedByReference(b,
            "SnapPeaTriangulation::enumerateCovers()");
    }

    // ----- Polytope vertex enumeration -----

    {
        auto tri = regina::Example<3>::figureEight();

        Arg b;
        regina::TreeEnumeration(tri, regina::NS_STANDARD).run(
                [](const regina::TreeEnumeration<>&, Arg& arg) {
            arg.flag();
            return false;
        }, b);
        verifyPassedByReference(b, "TreeEnumeration::run()");

        Arg d;
        regina::TautEnumeration(tri).run(
                [](const regina::TautEnumeration<>&, Arg& arg) {
            arg.flag();
            return false;
        }, d);
        verifyPassedByReference(d, "TautEnumeration::run()");
    }

    // ----- Subcomplex testing -----

    {
        Arg b;
        regina::SatRegion::find(
                regina::Example<3>::sfsOverSphere(2, 1, 3, 1, 4, 1),
                false, [](std::unique_ptr<regina::SatRegion>,
                    const regina::SatBlock::TetList&, Arg& arg) {
            arg.flag();
            return false;
        }, b);
        verifyPassedByReference(b, "SatRegion::find()");
    }

    // ----- Census enumeration -----

    {
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
        Arg b;
        regina::SigCensus::formCensus(1,
                [](const regina::Signature&,
                    const regina::SigCensus::IsoList&, Arg& arg) {
            arg.flag();
        }, b);
        verifyPassedByReference(b, "SigCensus::formCensus()");
    }
    {
        auto g = regina::ModelLinkGraph::fromPlantri(
            "bbcc,acca,abba");

        Arg b;
        g.generateMinimalLinks([](const regina::Link&, Arg& arg) {
            arg.flag();
        }, b);
        verifyPassedByReference(b,
            "ModelLinkGraph::generateMinimalLinks()");
    }
    {
        const regina::FacetPairing<2> p(regina::Example<2>::sphere());
        auto isos = p.findAutomorphisms();

        Arg a;
        regina::GluingPermSearcher<2>::bestSearcher(p, isos, true)->
            runSearch([](const regina::GluingPerms<2>&, Arg& arg) {
                arg.flag();
            }, a);
        verifyPassedByReference(a,
            "GluingPermSearcher<2>::bestSearcher()");

        Arg b;
        regina::GluingPermSearcher<2>::findAllPerms(p, isos, true,
                [](const regina::GluingPerms<2>&, Arg& arg) {
            arg.flag();
        }, b);
        verifyPassedByReference(b,
            "GluingPermSearcher<2>::findAllPerms()");

        Arg c;
        regina::GluingPermSearcher<2> searcher(p, isos, true);
        std::ostringstream data;
        searcher.dumpData(data);
        std::string taggedData = searcher.taggedData();
        searcher.runSearch([](const regina::GluingPerms<2>&, Arg& arg) {
            arg.flag();
        }, c);
        verifyPassedByReference(c,
            "GluingPermSearcher<2> standard constructor");

        Arg d;
        std::istringstream iData(data.str());
        regina::GluingPermSearcher<2>(iData).runSearch(
                [](const regina::GluingPerms<2>&, Arg& arg) {
            arg.flag();
        }, d);
        verifyPassedByReference(d,
            "GluingPermSearcher<2> istream constructor");

        Arg e;
        regina::GluingPermSearcher<2>::fromTaggedData(taggedData)->
            runSearch([](const regina::GluingPerms<2>&, Arg& arg) {
                arg.flag();
            }, e);
        verifyPassedByReference(e,
            "GluingPermSearcher<2>::fromTaggedData()");
    }
    {
        const regina::FacetPairing<3> p(regina::Example<3>::sphere());
        auto isos = p.findAutomorphisms();

        Arg a;
        regina::GluingPermSearcher<3>::bestSearcher(p, isos, true, true,
            {})->runSearch([](const regina::GluingPerms<3>&, Arg& arg) {
                arg.flag();
            }, a);
        verifyPassedByReference(a,
            "GluingPermSearcher<3>::bestSearcher()");

        Arg b;
        regina::GluingPermSearcher<3>::findAllPerms(p, isos,
                true, true, {},
                [](const regina::GluingPerms<3>&, Arg& arg) {
            arg.flag();
        }, b);
        verifyPassedByReference(b,
            "GluingPermSearcher<3>::findAllPerms()");

        Arg c;
        regina::GluingPermSearcher<3> searcher(p, isos, true, true, {});
        std::ostringstream data;
        searcher.dumpData(data);
        std::string taggedData = searcher.taggedData();
        searcher.runSearch([](const regina::GluingPerms<3>&, Arg& arg) {
            arg.flag();
        }, c);
        verifyPassedByReference(c,
            "GluingPermSearcher<3> standard constructor");

        Arg d;
        std::istringstream iData(data.str());
        regina::GluingPermSearcher<3>(iData).runSearch(
                [](const regina::GluingPerms<3>&, Arg& arg) {
            arg.flag();
        }, d);
        verifyPassedByReference(d,
            "GluingPermSearcher<3> istream constructor");

        Arg e;
        regina::GluingPermSearcher<3>::fromTaggedData(taggedData)->
            runSearch([](const regina::GluingPerms<3>&, Arg& arg) {
                arg.flag();
            }, e);
        verifyPassedByReference(e,
            "GluingPermSearcher<3>::fromTaggedData()");
    }
    {
        // Note: ClosedPrimeMinSearcher insists on >= 3 tetrahedra.
        auto p = regina::FacetPairing<3>::fromTextRep(
            "0 1 0 0 1 0 1 1 0 2 0 3 2 0 2 1 1 2 1 3 2 3 2 2");
        auto isos = p.findAutomorphisms();

        Arg c;
        regina::ClosedPrimeMinSearcher searcher(p, isos, true);
        std::ostringstream data;
        searcher.dumpData(data);
        std::string taggedData = searcher.taggedData();
        searcher.runSearch([](const regina::GluingPerms<3>&, Arg& arg) {
            arg.flag();
        }, c);
        verifyPassedByReference(c,
            "ClosedPrimeMinSearcher standard constructor");

        Arg d;
        std::istringstream iData(data.str());
        regina::ClosedPrimeMinSearcher(iData).runSearch(
                [](const regina::GluingPerms<3>&, Arg& arg) {
            arg.flag();
        }, d);
        verifyPassedByReference(d,
            "ClosedPrimeMinSearcher istream constructor");

        Arg e;
        regina::GluingPermSearcher<3>::fromTaggedData(taggedData)->
            runSearch([](const regina::GluingPerms<3>&, Arg& arg) {
                arg.flag();
            }, e);
        verifyPassedByReference(e,
            "ClosedPrimeMinSearcher variant of fromTaggedData()");
    }
    {
        const regina::FacetPairing<3> p(regina::Example<3>::sphere());
        auto isos = p.findAutomorphisms();

        Arg c;
        regina::CompactSearcher searcher(p, isos, true, {});
        std::ostringstream data;
        searcher.dumpData(data);
        std::string taggedData = searcher.taggedData();
        searcher.runSearch([](const regina::GluingPerms<3>&, Arg& arg) {
            arg.flag();
        }, c);
        verifyPassedByReference(c,
            "CompactSearcher standard constructor");

        Arg d;
        std::istringstream iData(data.str());
        regina::CompactSearcher(iData).runSearch(
                [](const regina::GluingPerms<3>&, Arg& arg) {
            arg.flag();
        }, d);
        verifyPassedByReference(d,
            "CompactSearcher istream constructor");

        Arg e;
        regina::GluingPermSearcher<3>::fromTaggedData(taggedData)->
            runSearch([](const regina::GluingPerms<3>&, Arg& arg) {
                arg.flag();
            }, e);
        verifyPassedByReference(e,
            "CompactSearcher variant of fromTaggedData()");
    }
    {
        const regina::FacetPairing<3> p(regina::Example<3>::sphere());
        auto isos = p.findAutomorphisms();

        Arg c;
        regina::EulerSearcher searcher(0, p, isos, true, {});
        std::ostringstream data;
        searcher.dumpData(data);
        std::string taggedData = searcher.taggedData();
        searcher.runSearch([](const regina::GluingPerms<3>&, Arg& arg) {
            arg.flag();
        }, c);
        verifyPassedByReference(c,
            "EulerSearcher standard constructor");

        Arg d;
        std::istringstream iData(data.str());
        regina::EulerSearcher(iData).runSearch(
                [](const regina::GluingPerms<3>&, Arg& arg) {
            arg.flag();
        }, d);
        verifyPassedByReference(d,
            "EulerSearcher istream constructor");

        Arg e;
        regina::GluingPermSearcher<3>::fromTaggedData(taggedData)->
            runSearch([](const regina::GluingPerms<3>&, Arg& arg) {
                arg.flag();
            }, e);
        verifyPassedByReference(e,
            "EulerSearcher variant of fromTaggedData()");
    }
    {
        const regina::FacetPairing<3> p(regina::Example<3>::sphere());
        auto isos = p.findAutomorphisms();

        Arg c;
        regina::HyperbolicMinSearcher searcher(p, isos, true);
        std::ostringstream data;
        searcher.dumpData(data);
        std::string taggedData = searcher.taggedData();
        searcher.runSearch([](const regina::GluingPerms<3>&, Arg& arg) {
            arg.flag();
        }, c);
        verifyPassedByReference(c,
            "HyperbolicMinSearcher standard constructor");

        Arg d;
        std::istringstream iData(data.str());
        regina::HyperbolicMinSearcher(iData).runSearch(
                [](const regina::GluingPerms<3>&, Arg& arg) {
            arg.flag();
        }, d);
        verifyPassedByReference(d,
            "HyperbolicMinSearcher istream constructor");

        Arg e;
        regina::GluingPermSearcher<3>::fromTaggedData(taggedData)->
            runSearch([](const regina::GluingPerms<3>&, Arg& arg) {
                arg.flag();
            }, e);
        verifyPassedByReference(e,
            "HyperbolicMinSearcher variant of fromTaggedData()");
    }
    {
        const regina::FacetPairing<4> p(regina::Example<4>::sphere());
        auto isos = p.findAutomorphisms();

        Arg a;
        regina::GluingPermSearcher<4>::bestSearcher(p, isos, true, true)
            ->runSearch([](const regina::GluingPerms<4>&, Arg& arg) {
                arg.flag();
            }, a);
        verifyPassedByReference(a,
            "GluingPermSearcher<4>::bestSearcher()");

        Arg b;
        regina::GluingPermSearcher<4>::findAllPerms(p, isos, true, true,
                [](const regina::GluingPerms<4>&, Arg& arg) {
            arg.flag();
        }, b);
        verifyPassedByReference(b,
            "GluingPermSearcher<4>::findAllPerms()");

        Arg c;
        regina::GluingPermSearcher<4> searcher(p, isos, true, true);
        std::ostringstream data;
        searcher.dumpData(data);
        std::string taggedData = searcher.taggedData();
        searcher.runSearch([](const regina::GluingPerms<4>&, Arg& arg) {
            arg.flag();
        }, c);
        verifyPassedByReference(c,
            "GluingPermSearcher<4> standard constructor");

        Arg d;
        std::istringstream iData(data.str());
        regina::GluingPermSearcher<4>(iData).runSearch(
                [](const regina::GluingPerms<4>&, Arg& arg) {
            arg.flag();
        }, d);
        verifyPassedByReference(d,
            "GluingPermSearcher<4> istream constructor");

        Arg e;
        regina::GluingPermSearcher<4>::fromTaggedData(taggedData)->
            runSearch([](const regina::GluingPerms<4>&, Arg& arg) {
                arg.flag();
            }, e);
        verifyPassedByReference(e,
            "GluingPermSearcher<4>::fromTaggedData()");
    }

    // Routines that use callbacks but whose callbacks don't
    // take additional user-supplied arguments:
    //
    // CensusDB::lookup()
    // DoubleDescription::enumerate()
    // HilbertCD::enumerate()
    // HilbertDual::enumerate()
    // HilbertPrimal::enumerate()
}

