
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

#include <cmath>
#include <memory>
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "triangulation/example3.h"

#include "testsuite/exhaustive.h"
#include "testsuite/dim3/testtriangulation.h"

using regina::Edge;
using regina::NormalSurface;
using regina::Triangulation;
using regina::Vertex;

class LinkingSurfacesTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(LinkingSurfacesTest);

    CPPUNIT_TEST(vertexLinks);
    CPPUNIT_TEST(edgeLinks);

    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp() override {
        }

        void tearDown() override {
        }

        static void testVertexLinks(const Triangulation<3>& tri,
                const char* name) {
            for (auto v : tri.vertices()) {
                NormalSurface link = v->linkingSurface();
                if (link.isVertexLink() != v) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name <<
                        ", vertex " << v->index() << ": linking surface "
                        << link.vector() << " is not recognised as the "
                        "correct vertex link.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void vertexLinks() {
            runCensusAllClosed(&testVertexLinks);
            runCensusAllBounded(&testVertexLinks);
            runCensusAllIdeal(&testVertexLinks);
        }

        static void testEdgeLinks(const Triangulation<3>& tri,
                const char* name) {
            regina::MatrixInt matching = regina::makeMatchingEquations(
                tri, regina::NS_STANDARD);

            for (auto e : tri.edges()) {
                NormalSurface link = e->linkingSurface();

                if (! (matching * link.vector()).isZero()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name <<
                        ", edge " << e->index() << ": linking surface "
                        << link.vector() << " does not satisfy the "
                        "matching equations.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        void edgeLinks() {
            runCensusAllClosed(&testEdgeLinks);
            runCensusAllBounded(&testEdgeLinks);
            runCensusAllIdeal(&testEdgeLinks);
        }
};

void addLinkingSurfaces(CppUnit::TextUi::TestRunner& runner) {
    runner.addTest(LinkingSurfacesTest::suite());
}

