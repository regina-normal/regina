
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

#include "algebra/abeliangroup.h"
#include "algebra/grouppresentation.h"
#include "packet/container.h"
#include "triangulation/example.h"
#include "triangulation/isosigtype.h"
#include "triangulation/detail/isosig-impl.h"
#include "utilities/typeutils.h"

#include "utilities/tightencodingtest.h"

using regina::Isomorphism;
using regina::Perm;
using regina::Simplex;
using regina::Triangulation;
using regina::Vertex;

// A size above which we will omit homology checks, in settings where
// operations are performed many times and speed is becoming a problem.
static constexpr size_t HOMOLOGY_THRESHOLD = 40;

/**
 * Clears all computed properties of the given triangulation.
 *
 * We allow the triangulation to be const, since the intent of this operation
 * is to not change the triangulation, but just to force it to forget its
 * cached properties.
 */
template <int dim>
static void clearProperties(const Triangulation<dim>& tri) {
    // Make and undo a trivial modification that will cause all
    // computed properties to be flushed.
    const_cast<Triangulation<dim>&>(tri).newSimplex();
    const_cast<Triangulation<dim>&>(tri).removeSimplexAt(tri.size()-1);
}

/**
 * Implements several tests for triangulations in dimension \a dim.
 *
 * Test fixtures in each dimension should use TriangulationTest<dim>
 * as a base class, since this base class provides example triangulations
 * that can be shared between tests.
 */
template <int dim>
class TriangulationTest : public testing::Test {
    public:
        struct TestCase {
            Triangulation<dim> tri;
            const char* name;
        };

    protected:
        // Trivial case:
        TestCase empty { {}, "Empty" };

        // Closed orientable triangulations:
        TestCase sphere { regina::Example<dim>::sphere(), "Sphere" };
        TestCase simpSphere { regina::Example<dim>::simplicialSphere(),
            "Simplicial sphere" };
        TestCase sphereBundle { regina::Example<dim>::sphereBundle(),
            "Sphere bundle" };

        // Closed non-orientable triangulations:
        TestCase twistedSphereBundle {
            regina::Example<dim>::twistedSphereBundle(),
            "Twisted sphere bundle" };

        // Triangulations with real boundary:
        TestCase ball { regina::Example<dim>::ball(), "Ball" };
        TestCase ballBundle { regina::Example<dim>::ballBundle(),
            "Ball bundle" };
        TestCase twistedBallBundle { regina::Example<dim>::twistedBallBundle(),
            "Twisted ball bundle" };

        /**
         * Run the given test over all of the example triangulations stored in
         * this generic test fixture.
         */
        void testGenericCases(
                void (*f)(const regina::Triangulation<dim>&, const char*)) {
            f(empty.tri, empty.name);
            f(sphere.tri, sphere.name);
            f(simpSphere.tri, simpSphere.name);
            f(sphereBundle.tri, sphereBundle.name);
            f(twistedSphereBundle.tri, twistedSphereBundle.name);
            f(ball.tri, ball.name);
            f(ballBundle.tri, ballBundle.name);
            f(twistedBallBundle.tri, twistedBallBundle.name);
        }

        static void verifyValid(const TestCase& test) {
            SCOPED_TRACE_CSTRING(test.name);

            EXPECT_TRUE(test.tri.isValid());

            regina::for_constexpr<0, dim>([&test](auto subdim) {
                SCOPED_TRACE_NUMERIC(subdim);
                for (size_t i = 0; i < test.tri.template countFaces<subdim>();
                        ++i) {
                    SCOPED_TRACE_NAMED_NUMERIC("face", i);
                    auto f = test.tri.template face<subdim>(i);

                    EXPECT_TRUE(f->isValid());
                    EXPECT_FALSE(f->hasBadIdentification());
                    if constexpr (regina::standardDim(dim))
                        EXPECT_FALSE(f->hasBadLink());
                }
            });
        }

        void validityGenericCases() {
            verifyValid(empty);
            verifyValid(sphere);
            verifyValid(simpSphere);
            verifyValid(sphereBundle);
            verifyValid(twistedSphereBundle);
            verifyValid(ball);
            verifyValid(ballBundle);
            verifyValid(twistedBallBundle);
        }

        void connectivityGenericCases() {
            EXPECT_TRUE(empty.tri.isConnected());
            EXPECT_TRUE(sphere.tri.isConnected());
            EXPECT_TRUE(simpSphere.tri.isConnected());
            EXPECT_TRUE(sphereBundle.tri.isConnected());
            EXPECT_TRUE(twistedSphereBundle.tri.isConnected());
            EXPECT_TRUE(ball.tri.isConnected());
            EXPECT_TRUE(ballBundle.tri.isConnected());
            EXPECT_TRUE(twistedBallBundle.tri.isConnected());
        }

        void orientabilityGenericCases() {
            EXPECT_TRUE(empty.tri.isOrientable());
            EXPECT_TRUE(sphere.tri.isOrientable());
            EXPECT_TRUE(simpSphere.tri.isOrientable());
            EXPECT_TRUE(sphereBundle.tri.isOrientable());
            EXPECT_FALSE(twistedSphereBundle.tri.isOrientable());
            EXPECT_TRUE(ball.tri.isOrientable());
            EXPECT_TRUE(ballBundle.tri.isOrientable());
            EXPECT_FALSE(twistedBallBundle.tri.isOrientable());
        }

        void eulerCharGenericCases() {
            EXPECT_EQ(empty.tri.eulerCharTri(), 0);
            EXPECT_EQ(sphere.tri.eulerCharTri(), (dim % 2 ? 0 : 2));
            EXPECT_EQ(simpSphere.tri.eulerCharTri(), (dim % 2 ? 0 : 2));
            EXPECT_EQ(sphereBundle.tri.eulerCharTri(), 0);
            EXPECT_EQ(twistedSphereBundle.tri.eulerCharTri(), 0);
            EXPECT_EQ(ball.tri.eulerCharTri(), 1);
            EXPECT_EQ(ballBundle.tri.eulerCharTri(), 0);
            EXPECT_EQ(twistedBallBundle.tri.eulerCharTri(), 0);

            if constexpr (regina::standardDim(dim) && dim > 2) {
                // In these dimensions, Regina understands ideal triangulations
                // and thus offers a separate function eulerCharManifold().
                EXPECT_EQ(empty.tri.eulerCharManifold(), 0);
                EXPECT_EQ(sphere.tri.eulerCharManifold(), (dim % 2 ? 0 : 2));
                EXPECT_EQ(simpSphere.tri.eulerCharManifold(),
                    (dim % 2 ? 0 : 2));
                EXPECT_EQ(sphereBundle.tri.eulerCharManifold(), 0);
                EXPECT_EQ(twistedSphereBundle.tri.eulerCharManifold(), 0);
                EXPECT_EQ(ball.tri.eulerCharManifold(), 1);
                EXPECT_EQ(ballBundle.tri.eulerCharManifold(), 0);
                EXPECT_EQ(twistedBallBundle.tri.eulerCharManifold(), 0);
            }
        }

        static void verifyBoundaryBasic(const TestCase& test,
                std::initializer_list<long> expectReal,
                std::initializer_list<long> expectIdeal,
                std::initializer_list<long> expectInvalid) {
            // Verifies boundary counts, types, and (where boundary face
            // counts are available) Euler characteristics.
            SCOPED_TRACE_CSTRING(test.name);

            EXPECT_EQ(test.tri.countBoundaryComponents(),
                expectReal.size() + expectIdeal.size() + expectInvalid.size());
            EXPECT_EQ(test.tri.hasBoundaryFacets(), (expectReal.size() > 0));

            if constexpr (regina::standardDim(dim)) {
                // These dimensions offer functions to query closedness and
                // ideal boundary components.
                EXPECT_EQ(test.tri.isClosed(), (expectReal.size() +
                    expectIdeal.size() + expectInvalid.size() == 0));
                if constexpr (dim < 4) {
                    // Ideal invalid triangulations are allowed.
                    EXPECT_EQ(test.tri.isIdeal(), expectIdeal.size() > 0);
                } else {
                    // To be considered ideal, a triangulation _must_ be valid.
                    EXPECT_EQ(test.tri.isIdeal(),
                        (test.tri.isValid() && expectIdeal.size() > 0));
                }
                if constexpr (dim == 2)
                    EXPECT_EQ(expectIdeal.size(), 0);
                if constexpr (dim <= 3)
                    EXPECT_EQ(expectInvalid.size(), 0);
            } else {
                // These dimensions only support real boundary components.
                EXPECT_EQ(expectIdeal.size(), 0);
                EXPECT_EQ(expectInvalid.size(), 0);
            }

            auto nextReal = expectReal.begin();
            auto nextIdeal = expectIdeal.begin();
            auto nextInvalid = expectInvalid.begin();

            for (auto b : test.tri.boundaryComponents()) {
                if (b->isIdeal()) {
                    EXPECT_FALSE(b->isReal());
                    EXPECT_FALSE(b->isInvalidVertex());

                    if (nextIdeal == expectIdeal.end())
                        ADD_FAILURE() << "Too many ideal boundary components";
                    else {
                        if constexpr (regina::BoundaryComponent<dim>::allFaces)
                            EXPECT_EQ(b->eulerChar(), *nextIdeal);
                        ++nextIdeal;
                    }
                } else if (b->isInvalidVertex()) {
                    EXPECT_FALSE(b->isReal());
                    EXPECT_FALSE(b->isIdeal());

                    if (nextInvalid == expectInvalid.end())
                        ADD_FAILURE() << "Too many invalid boundary components";
                    else {
                        if constexpr (regina::BoundaryComponent<dim>::allFaces)
                            EXPECT_EQ(b->eulerChar(), *nextInvalid);
                        ++nextInvalid;
                    }
                } else {
                    EXPECT_TRUE(b->isReal());
                    EXPECT_FALSE(b->isIdeal());
                    EXPECT_FALSE(b->isInvalidVertex());

                    if (nextReal == expectReal.end())
                        ADD_FAILURE() << "Too many real boundary components";
                    else {
                        if constexpr (regina::BoundaryComponent<dim>::allFaces)
                            EXPECT_EQ(b->eulerChar(), *nextReal);
                        ++nextReal;
                    }
                }
            }

            EXPECT_EQ(nextReal, expectReal.end());
            EXPECT_EQ(nextIdeal, expectIdeal.end());
            EXPECT_EQ(nextInvalid, expectInvalid.end());
        }

        void boundaryBasicGenericCases() {
            verifyBoundaryBasic(empty, {}, {}, {});
            verifyBoundaryBasic(sphere, {}, {}, {});
            verifyBoundaryBasic(simpSphere, {}, {}, {});
            verifyBoundaryBasic(sphereBundle, {}, {}, {});
            verifyBoundaryBasic(twistedSphereBundle, {}, {}, {});
            verifyBoundaryBasic(ball, {dim % 2 ? 2 : 0}, {}, {});
            if constexpr (dim == 2) {
                verifyBoundaryBasic(ballBundle, {0, 0}, {}, {});
            } else {
                verifyBoundaryBasic(ballBundle, {0}, {}, {});
            }
            verifyBoundaryBasic(twistedBallBundle, {0}, {}, {});
        }

        static void verifyBoundaryPinching(const Triangulation<dim>& tri,
                const char* name) {
            static_assert(dim > 2 && regina::BoundaryComponent<dim>::allFaces);

            for (auto* bc : tri.boundaryComponents()) {
                if (bc->size() == 0)
                    continue;

                // We have boundary facets.  Look for pinched faces.
                long adjEuler = 0;
                regina::for_constexpr<0, dim - 2>([bc, &adjEuler](auto subdim) {
                    for (auto f : bc->template faces<subdim>())
                        if (! f->isValid()) {
                            // Beware: face links themselves can have both real
                            // and ideal boundary components.
                            size_t realBdries = 0;
                            for (auto c : f->buildLink().boundaryComponents())
                                if (c->size() > 0)
                                    ++realBdries;
                            if (realBdries > 1) {
                                if constexpr (subdim % 2 == 0)
                                    adjEuler -= (realBdries - 1);
                                else
                                    adjEuler += (realBdries - 1);
                            }
                        }
                });

                EXPECT_EQ(bc->eulerChar(),
                    bc->build().eulerCharTri() + adjEuler);
            }
        }

        static void verifyVertexLinksBasic(const TestCase& test,
                size_t expectSphere, size_t expectBall,
                size_t expectIdeal = 0, size_t expectInvalid = 0) {
            // In higher (non-standard) dimensions regina cannot recognise
            // ideal vertices, and so we treat expectSphere / expectBall as
            // simply "not on real boundary" / "on real boundary".
            SCOPED_TRACE_CSTRING(test.name);

            using regina::Vertex;

            size_t foundSphere = 0, foundBall = 0;
            size_t foundIdeal = 0, foundInvalid = 0;
            for (auto v : test.tri.vertices()) {
                if constexpr (dim > 2 && regina::standardDim(dim)) {
                    if (! v->isValid()) {
                        ++foundInvalid;
                        if constexpr (dim == 3) {
                            EXPECT_TRUE(v->isBoundary());
                            EXPECT_FALSE(v->isLinkClosed());
                            EXPECT_FALSE(v->isStandard());
                            EXPECT_FALSE(v->isIdeal());
                            EXPECT_EQ(v->linkType(),
                                Vertex<dim>::Link::Invalid);
                        }
                    } else if (v->isIdeal()) {
                        ++foundIdeal;
                        if constexpr (dim == 3) {
                            using Link = typename Vertex<dim>::Link;
                            EXPECT_TRUE(v->isBoundary());
                            EXPECT_TRUE(v->isLinkClosed());
                            if (v->isStandard()) {
                                EXPECT_TRUE(
                                    v->linkType() == Link::Torus ||
                                    v->linkType() == Link::KleinBottle);
                                EXPECT_EQ(v->linkEulerChar(), 0);
                                EXPECT_EQ(v->isLinkOrientable(),
                                    v->linkType() == Link::Torus);
                            } else {
                                EXPECT_EQ(v->linkType(), Link::NonStandardCusp);
                                EXPECT_NE(v->linkEulerChar(), 2);
                                EXPECT_NE(v->linkEulerChar(), 0);
                            }
                        }
                    } else if (v->isBoundary()) {
                        ++foundBall;
                        if constexpr (dim == 3) {
                            EXPECT_FALSE(v->isLinkClosed());
                            EXPECT_TRUE(v->isLinkOrientable());
                            EXPECT_TRUE(v->isStandard());
                            EXPECT_EQ(v->linkType(), Vertex<dim>::Link::Disc);
                            EXPECT_EQ(v->linkEulerChar(), 1);
                        }
                    } else {
                        ++foundSphere;
                        if constexpr (dim == 3) {
                            EXPECT_TRUE(v->isLinkClosed());
                            EXPECT_TRUE(v->isLinkOrientable());
                            EXPECT_TRUE(v->isStandard());
                            EXPECT_EQ(v->linkType(), Vertex<dim>::Link::Sphere);
                            EXPECT_EQ(v->linkEulerChar(), 2);
                        }
                    }
                } else {
                    if (v->isBoundary())
                        ++foundBall;
                    else
                        ++foundSphere;
                }
            }

            EXPECT_EQ(foundSphere, expectSphere);
            EXPECT_EQ(foundBall, expectBall);
            EXPECT_EQ(foundIdeal, expectIdeal);
            EXPECT_EQ(foundInvalid, expectInvalid);
        }

        void vertexLinksBasicGenericCases() {
            verifyVertexLinksBasic(empty, 0, 0);
            verifyVertexLinksBasic(sphere, dim + 1, 0);
            verifyVertexLinksBasic(simpSphere, dim + 2, 0);
            verifyVertexLinksBasic(sphereBundle, 1, 0);
            verifyVertexLinksBasic(twistedSphereBundle, 1, 0);
            verifyVertexLinksBasic(ball, 0, dim + 1);
            verifyVertexLinksBasic(ballBundle, 0, dim % 2 ? 1 : 2);
            verifyVertexLinksBasic(twistedBallBundle, 0, dim % 2 ? 2 : 1);
        }

        static void verifyOrient(const Triangulation<dim>& tri,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);

            static constexpr int trials = 10;

            {
                // Test a direct copy:
                Triangulation<dim> oriented(tri, false, true);
                oriented.orient();
                clearProperties(oriented); // forget the cached orientability

                EXPECT_EQ(tri.isOrientable(), oriented.isOrientable());
                EXPECT_TRUE(oriented.isIsomorphicTo(tri));
                if (tri.isOrientable())
                    EXPECT_TRUE(oriented.isOriented());
            }

            for (int i = 0; i < trials; ++i) {
                // Test an isomorphic copy:
                Triangulation<dim> oriented =
                    Isomorphism<dim>::random(tri.size())(tri);
                oriented.orient();
                clearProperties(oriented); // forget the cached orientability

                EXPECT_EQ(tri.isOrientable(), oriented.isOrientable());
                EXPECT_TRUE(oriented.isIsomorphicTo(tri));
                if (tri.isOrientable())
                    EXPECT_TRUE(oriented.isOriented());
            }
        }

        static void verifySkeletonDetail(const Triangulation<dim>& tri) {
            // Components and their boundary components and simplices:
            bool allOrbl = true;
            size_t totSize = 0;
            size_t totBdry = 0;
            size_t totBdryFacets = 0;
            for (auto c : tri.components()) {
                bool allOrblInComponent = true;
                bool allValidInComponent = true;
                size_t boundaryFacets = 0;
                size_t doubleDualTree = 0;

                totSize += c->size();
                for (auto s : c->simplices()) {
                    EXPECT_EQ(s->component(), c);
                    EXPECT_EQ(std::abs(s->orientation()), 1);
                    for (int i = 0; i <= dim; ++i) {
                        if (auto adj = s->adjacentSimplex(i)) {
                            if (s->adjacentGluing(i).sign() > 0) {
                                if (adj->orientation() != - s->orientation())
                                    allOrbl = allOrblInComponent = false;
                            } else {
                                if (adj->orientation() != s->orientation())
                                    allOrbl = allOrblInComponent = false;
                            }
                            if (s->facetInMaximalForest(i)) {
                                ++doubleDualTree;
                                EXPECT_TRUE(adj->facetInMaximalForest(
                                    s->adjacentFacet(i)));
                            }
                        } else {
                            ++boundaryFacets;
                            ++totBdryFacets;
                            EXPECT_FALSE(s->facetInMaximalForest(i));
                        }
                    }
                }

                totBdry += c->countBoundaryComponents();
                for (auto b : c->boundaryComponents())
                    EXPECT_EQ(b->component(), c);

                // See if this component contains any invalid faces.
                regina::for_constexpr<0, dim>([c, &allValidInComponent](
                        auto subdim) {
                    if constexpr (regina::standardDim(dim)) {
                        // Access faces directly from the component.
                        for (auto f : c->template faces<subdim>())
                            if (! f->isValid()) {
                                allValidInComponent = false;
                                return; // from lambda
                            }
                    } else {
                        // Access faces via the top-dimensional simplices.
                        for (auto s : c->simplices())
                            for (size_t j = 0;
                                    j < regina::binomSmall(dim+1, subdim+1);
                                    ++j)
                                if (! s->template face<subdim>(j)->isValid()) {
                                    allValidInComponent = false;
                                    return; // from lambda
                                }
                    }
                });

                EXPECT_EQ(c->isOrientable(), allOrblInComponent);
                EXPECT_EQ(c->isValid(), allValidInComponent);
                EXPECT_EQ(c->countBoundaryFacets(), boundaryFacets);
                EXPECT_EQ(doubleDualTree, 2 * (c->size() - 1));
            }
            EXPECT_EQ(tri.isOrientable(), allOrbl);
            EXPECT_EQ(tri.size(), totSize);
            EXPECT_EQ(tri.countBoundaryComponents(), totBdry);
            EXPECT_EQ(tri.countBoundaryFacets(), totBdryFacets);

            // More boundary components:
            totBdryFacets = 0; // reset, since we will count this again
            for (auto b : tri.boundaryComponents()) {
                totBdryFacets += b->size();
                EXPECT_EQ(b->size() * dim, b->countRidges() * 2);

                size_t builtSize = 0;
                if (b->isReal()) {
                    builtSize = b->size();

                    for (auto f : b->facets())
                        EXPECT_EQ(f->boundaryComponent(), b);

                    // NOTE: Below we test whether face->boundaryComponent()
                    // matches the boundary component containing face.
                    // This test could fail for legitimate reasons if the face
                    // is pinched between two different boundary components.
                    // However, none of our test cases have this property,
                    // and so we leave the tests as they are for now.
                    if constexpr (regina::BoundaryComponent<dim>::allFaces) {
                        // Access faces directly from the boundary component.
                        regina::for_constexpr<0, dim-1>([b](auto subdim) {
                            for (auto f : b->template faces<subdim>()) {
                                EXPECT_EQ(f->boundaryComponent(), b);
                            }
                        });
                    } else {
                        // Access faces via the boundary facets.
                        for (auto f : b->facets()) {
                            regina::for_constexpr<0, dim-1>(
                                    [b, f](auto subdim) {
                                // Check all subdim-faces of f.
                                for (size_t j = 0;
                                        j < regina::binomSmall(dim, subdim+1);
                                        ++j) {
                                    auto sub = f->template face<subdim>(j);
                                    EXPECT_EQ(sub->boundaryComponent(), b);
                                }
                            });
                        }
                    }
                } else {
                    if constexpr (regina::BoundaryComponent<dim>::allowVertex) {
                        EXPECT_EQ(b->countVertices(), 1);
                        regina::for_constexpr<1, dim>([b](auto subdim) {
                            EXPECT_EQ(b->template countFaces<subdim>(), 0);
                        });

                        auto v = b->vertex(0);
                        builtSize = v->degree();
                        // NOTE: This next test could fail for legitimate
                        // reasons if the vertex is pinched between two
                        // different boundary components.  See above for
                        // further explanation.
                        EXPECT_EQ(v->boundaryComponent(), b);
                    } else {
                        ADD_FAILURE() << "Vertex-only boundary component "
                            "not allowed in this dimension";
                    }
                }

                if constexpr (dim > 2) {
                    Triangulation<dim - 1> built = b->build();
                    EXPECT_EQ(built.size(), builtSize);
                    EXPECT_EQ(built.isOrientable(), b->isOrientable());
                }
            }
            EXPECT_EQ(tri.countBoundaryFacets(), totBdryFacets);

            // Faces:
            bool allValid = true;
            regina::for_constexpr<0, dim>([&tri, &allValid](auto subdim) {
                size_t bdry = 0;
                size_t degreeSum = 0;
                for (auto f : tri.template faces<subdim>()) {
                    if (! f->isValid())
                        allValid = false;
                    if (f->isBoundary())
                        ++bdry;
                    degreeSum += f->degree();

                    EXPECT_EQ(f->component(),
                        f->front().simplex()->component());

                    if (! f->hasBadIdentification()) {
                        // We already test link orientability more precisely
                        // for dim == 3,4 further below (we compare the
                        // cached link orientability to the
                        // orientability of the full triangulated link).
                        // Therefore the tests here only need to be things
                        // that are relevant in higher dimensions.
                        if (f->isLinkOrientable()) {
                            // What is there that's sensible to test here?
                        } else {
                            EXPECT_FALSE(f->component()->isOrientable());
                        }
                    }

                    for (auto emb : f->embeddings()) {
                        auto s = emb.simplex();
                        auto v = emb.vertices();
                        auto which = regina::Face<dim, subdim>::faceNumber(v);
                        EXPECT_EQ(s->template face<subdim>(which), f);
                        EXPECT_EQ(s->template faceMapping<subdim>(which), v);
                    }
                }
                EXPECT_EQ(bdry, tri.template countBoundaryFaces<subdim>());
                EXPECT_EQ(degreeSum,
                    tri.size() * regina::binomSmall(dim + 1, subdim + 1));
            });
            EXPECT_EQ(tri.isValid(), allValid);

            // Additional skeletal data for low dimensions:
            if constexpr (regina::standardDim(dim)) {
                regina::for_constexpr<0, dim>([&tri](auto subdim) {
                    size_t count = 0;
                    for (auto c : tri.components()) {
                        for (auto f : c->template faces<subdim>()) {
                            EXPECT_EQ(f->component(), c);
                            ++count;
                        }
                    }
                    EXPECT_EQ(count, tri.template countFaces<subdim>());
                });
            }
            if constexpr (dim == 3 || dim == 4) {
                bool foundIdeal = false;
                bool allStandard = true;
                for (auto c : tri.components()) {
                    bool foundIdealInComponent = false;
                    for (auto v : c->vertices()) {
                        if (v->isIdeal())
                            foundIdeal = foundIdealInComponent = true;

                        const auto& link = v->buildLink();
                        EXPECT_EQ(v->isLinkOrientable(), link.isOrientable());

                        if (link.isSphere()) {
                            EXPECT_TRUE(v->isValid());
                            EXPECT_FALSE(v->isIdeal());
                            if constexpr (dim == 3)
                                EXPECT_EQ(v->linkType(),
                                    Vertex<dim>::Link::Sphere);
                        } else if (link.isBall()) {
                            EXPECT_TRUE(v->isValid());
                            EXPECT_FALSE(v->isIdeal());
                            if constexpr (dim == 3)
                                EXPECT_EQ(v->linkType(),
                                    Vertex<dim>::Link::Disc);
                        } else if (link.isValid() && link.isClosed()) {
                            EXPECT_TRUE(v->isValid());
                            EXPECT_TRUE(v->isIdeal());
                            if constexpr (dim == 3) {
                                if (link.eulerCharTri() == 0) {
                                    if (link.isOrientable())
                                        EXPECT_EQ(v->linkType(),
                                            Vertex<dim>::Link::Torus);
                                    else
                                        EXPECT_EQ(v->linkType(),
                                            Vertex<dim>::Link::KleinBottle);
                                } else {
                                    allStandard = false;
                                    EXPECT_EQ(v->linkType(),
                                        Vertex<dim>::Link::NonStandardCusp);
                                }
                            }
                        } else {
                            allStandard = false;
                            EXPECT_FALSE(v->isValid());
                            EXPECT_FALSE(v->isIdeal());
                            if constexpr (dim == 3)
                                EXPECT_EQ(v->linkType(),
                                    Vertex<dim>::Link::Invalid);
                        }

                        if constexpr (dim == 3)
                            EXPECT_EQ(link.eulerCharTri(), v->linkEulerChar());
                    }
                    if constexpr (dim == 4) {
                        for (auto e : c->edges()) {
                            const auto& link = e->buildLink();
                            if (! e->hasBadIdentification())
                                EXPECT_EQ(e->isLinkOrientable(),
                                    link.isOrientable());

                            EXPECT_EQ(e->hasBadLink(),
                                ! (link.isSphere() || link.isBall()));
                        }
                    }
                    EXPECT_EQ(c->isIdeal(), foundIdealInComponent);
                }
                if constexpr (dim == 4) {
                    // In 4-D, we restrict the notion of "ideal triangulations"
                    // to only include valid triangulations.
                    // See Triangulation<4>::isIdeal() for why.
                    if (tri.isValid())
                        EXPECT_EQ(tri.isIdeal(), foundIdeal);
                    else
                        EXPECT_FALSE(tri.isIdeal());
                } else /* dim == 3 */ {
                    EXPECT_EQ(tri.isIdeal(), foundIdeal);
                    EXPECT_EQ(tri.isStandard(), allStandard);
                }
            }
            if constexpr (dim == 3) {
                // All triangle types should, at this point, be not yet
                // determined.
                for (auto t : tri.triangles()) {
                    int sub = t->subtype();
                    switch (t->type()) {
                        case regina::Triangle<dim>::TRIANGLE:
                        case regina::Triangle<dim>::PARACHUTE:
                        case regina::Triangle<dim>::L31:
                            EXPECT_EQ(sub, -1);
                            break;

                        case regina::Triangle<dim>::SCARF:
                        case regina::Triangle<dim>::CONE:
                        case regina::Triangle<dim>::MOBIUS:
                        case regina::Triangle<dim>::HORN:
                        case regina::Triangle<dim>::DUNCEHAT:
                            EXPECT_GE(sub, 0);
                            EXPECT_LE(sub, 2);
                            break;

                        default:
                            ADD_FAILURE() << "Unexpected triangle type";
                            break;
                    }
                }
            }
        }

        static void verifySkeleton(const Triangulation<dim>& tri,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);
            verifySkeletonDetail(tri);

            // A side-effect of the above is that tri's skeleton is computed.
            // Now test that the skeleton is cloned correctly.
            SCOPED_TRACE("Clone");
            Triangulation<dim> copy(tri);
            verifySkeletonDetail(copy);
        }

        static void verifyBoundaryLabellingDetail(
                const regina::BoundaryComponent<dim>* bc,
                const regina::Triangulation<dim - 1>& built,
                const char* context) {
            static_assert(dim > 2);
            static_assert(regina::BoundaryComponent<dim>::allFaces);

            SCOPED_TRACE_CSTRING(context);

            regina::for_constexpr<0, dim-1>([bc, &built](auto subdim) {
                SCOPED_TRACE_NUMERIC(subdim);

                // The labelling and ordering of subdim-faces is only
                // guaranteed if no subdim-face is pinched.  Conversely, if
                // some subdim-face *is* pinched then that face will appear
                // multiple times in the triangulated boundary, and so such a
                // labelling / ordering will be impossible.

                bool hasPinched = false;
                if constexpr (subdim <= dim - 3) {
                    for (auto f : bc->template faces<subdim>()) {
                        const auto& link = f->buildLink();
                        size_t realBdry = 0;
                        for (auto sub : link.boundaryComponents())
                            if (sub->isReal())
                                ++realBdry;
                        if (realBdry > 1) {
                            hasPinched = true;
                            break;
                        }
                    }
                }

                if (hasPinched) {
                    // We cannot check the labelling / ordering, but we should
                    // still ensure that the triangulated boundary component
                    // has strictly more subdim-faces.
                    EXPECT_LT(bc->template countFaces<subdim>(),
                            built.template countFaces<subdim>());
                    return; // from lambda
                }

                // There are no pinched faces; go ahead and verify the full
                // labelling / ordering.
                ASSERT_EQ(bc->template countFaces<subdim>(),
                        built.template countFaces<subdim>());

                for (size_t i = 0; i < bc->size(); ++i) {
                    const auto* innerSimp = built.simplex(i);
                    const auto* outerSimp = bc->template face<dim-1>(i);

                    for (size_t j = 0; j < regina::binomSmall(dim, subdim+1);
                            ++j) {
                        auto* innerFace = innerSimp->template face<subdim>(j);
                        auto* outerFace = outerSimp->template face<subdim>(j);
                        EXPECT_EQ(
                            bc->template face<subdim>(innerFace->index()),
                            outerFace);

                        Perm<dim> innerPerm = innerSimp->template
                            faceMapping<subdim>(j);
                        Perm<dim+1> outerPerm = outerSimp->template
                            faceMapping<subdim>(j);
                        EXPECT_EQ(
                            innerPerm.trunc(subdim+1),
                            outerPerm.trunc(subdim+1));
                    }
                }
            });
        }

        static void verifyBoundaryLabelling(const Triangulation<dim>& tri,
                const char* name) {
            // This test verifies that a triangulated boundary component
            // has the correct number of faces of each dimension and these
            // faces are ordered and labelled correctly.
            //
            // Currently we define "correctly" as "matches the
            // ordering/labelling of the original boundary component", which
            // means we can only use this test in dimensions where boundary
            // components store all of their lower-dimensional faces.
            //
            static_assert(dim > 2);
            static_assert(regina::BoundaryComponent<dim>::allFaces);

            SCOPED_TRACE_CSTRING(name);

            for (auto bc : tri.boundaryComponents())
                if (bc->isReal()) {
                    // We have a real boundary component.
                    SCOPED_TRACE_NAMED_NUMERIC("index", bc->index());
                    const Triangulation<dim-1>& built = bc->build();

                    verifyBoundaryLabellingDetail(bc, built, "built");

                    // Try this again with copies of the triangulated boundary
                    // (instead of the reference to the cached property of tri).
                    // This allows us to test that deep copies preserve the
                    // numbering/labelling of lower-dimensional faces.
                    {
                        // Make a deep copy of the triangulated boundary.
                        Triangulation<dim-1> clone(built);
                        verifyBoundaryLabellingDetail(bc, clone, "clone");
                    }
                    {
                        // This time make a "light" deep copy that does not
                        // clone properties (but should still clone the
                        // skeleton).
                        Triangulation<dim-1> clone(built, false, false);
                        verifyBoundaryLabellingDetail(bc, clone, "light clone");
                    }
                    {
                        Triangulation<dim-1> assigned;
                        assigned.newSimplex(); // junk for assignment to replace
                        assigned = built;
                        verifyBoundaryLabellingDetail(bc, assigned, "assigned");
                    }

                    // Verify the gluings between (dim-2)-faces.
                    ASSERT_EQ(bc->size(), built.size());
                    for (size_t i = 0; i < bc->size(); ++i) {
                        const auto* innerSimp = built.simplex(i);
                        const auto* outerSimp = bc->template face<dim-1>(i);

                        for (int j = 0; j < dim; ++j) {
                            auto innerAdj = innerSimp->adjacentSimplex(j);
                            ASSERT_NE(innerAdj, nullptr);
                            auto outerAdj = bc->template face<dim-1>(
                                innerAdj->index());
                            EXPECT_EQ(outerAdj->template face<dim-2>(
                                    innerSimp->adjacentFacet(j)),
                                outerSimp->template face<dim-2>(j));
                        }
                    }
                }
        }

        void edgeAccess() {
            // Ensure that Simplex<dim>::edge(i, j) returns the correct edge.

            // Find ourselves a top-dimensional simplex with all edges distinct.
            auto s = ball.tri.simplex(0);

            for (int i = 0; i <= dim; ++i)
                for (int j = 0; j <= dim; ++j) {
                    if (i == j)
                        continue;

                    // Build a permutation that maps (0,1) -> (i,j).
                    Perm<dim+1> p;
                    if (j == 0)
                        p = Perm<dim+1>(1, i) * Perm<dim+1>(0, 1);
                    else
                        p = Perm<dim+1>(0, i) * Perm<dim+1>(1, j);

                    EXPECT_EQ(s->edge(i, j),
                        s->edge(regina::Edge<dim>::faceNumber(p)));
                }
        }

        static void verifyReordering(const Triangulation<dim>& t,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);

            // Note: we explicitly don't clear properties after reorderBFS().
            // The reorder operation _preserves_ the skeleton despite
            // reordering top-dimensional simplices, and we should work with
            // this preserved skeleton because this is more likely to cause
            // problems than a freshly computed skeleton.

            // Reordering the original:
            {
                Triangulation<dim> a(t);
                a.reorderBFS();
                EXPECT_TRUE(t.isIsomorphicTo(a));
            }
            {
                Triangulation<dim> b(t);
                b.reorderBFS(true);
                EXPECT_TRUE(t.isIsomorphicTo(b));
            }

            // Reordering a random relabelling of the original:
            Triangulation<dim> relabel = Isomorphism<dim>::random(t.size())(t);
            clearProperties(relabel); // recompute the skeleton here
            EXPECT_TRUE(t.isIsomorphicTo(relabel));
            {
                Triangulation<dim> d(relabel);
                d.reorderBFS();
                EXPECT_TRUE(t.isIsomorphicTo(d));
            }
            {
                Triangulation<dim> e(relabel);
                e.reorderBFS(true);
                EXPECT_TRUE(t.isIsomorphicTo(e));
            }
        }

        static void verifyDoubleCover(const Triangulation<dim>& tri,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);

            // The tests below assumed that tri has ≤ 1 connected component.
            if (! tri.isConnected())
                return;

            Triangulation<dim> cover = tri.doubleCover();

            if (tri.isEmpty()) {
                EXPECT_TRUE(cover.isEmpty());
                return;
            }

            // We have a non-empty connected triangulation.
            if (tri.isOrientable()) {
                // We should simply come away with two identical copies of tri.
                auto components = cover.triangulateComponents();
                EXPECT_EQ(components.size(), 2);
                for (const Triangulation<dim>& c : components)
                    EXPECT_TRUE(tri.isIsomorphicTo(c));
            } else {
                // We should come away with a proper connected double cover.
                EXPECT_EQ(cover.countComponents(), 1);
                EXPECT_TRUE(cover.isOrientable());
                EXPECT_EQ(cover.size(), 2 * tri.size());

                // Verify that the face counts double in each facial dimension.
                EXPECT_EQ(cover.template countFaces<dim-1>(),
                    2 * tri.template countFaces<dim-1>());
                if (tri.isValid()) {
                    // There are legitimate reasons for (0..dim-2)-face counts
                    // to *not* double in invalid triangulations.
                    // Likewise for vertex counts in ideal triangulations.
                    regina::for_constexpr<0, dim-1>([&tri, &cover](
                            auto subdim) {
                        if constexpr (dim == 3 || dim == 4) {
                            // These dimensions support ideal triangulations.
                            if constexpr (subdim == 0) {
                                if (tri.isIdeal())
                                    return; // from lambda
                            }
                        }
                        EXPECT_EQ(cover.template countFaces<subdim>(),
                            2 * tri.template countFaces<subdim>());
                    });
                }

                /**
                 * Commenting out this claim about homology groups,
                 * which is nonsense.
                // We expect the first homology group to be identical,
                // or to be missing a copy of Z_2.
                if (tri.isValid() && (tri.homology() != cover.homology())) {
                    regina::AbelianGroup hCover(cover.homology());
                    hCover.addTorsion(2);
                    EXPECT_EQ(tri.homology(), hCover);
                }
                */
            }
        }

        static void verifyMakeCanonical(const Triangulation<dim>& tri,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);

            // Currently makeCanonical() insists on connected triangulations.
            if (! tri.isConnected())
                return;

            static constexpr int trials = 5;

            Triangulation<dim> canonical(tri);
            canonical.makeCanonical();
            clearProperties(canonical); // recompute skeleton for detail()
            EXPECT_TRUE(canonical.isIsomorphicTo(tri));

            for (int i = 0; i < trials; ++i) {
                Triangulation<dim> t =
                    Isomorphism<dim>::random(tri.size())(tri);

                t.makeCanonical();
                clearProperties(t); // recompute skeleton for detail()

                EXPECT_EQ(t, canonical);
                EXPECT_EQ(t.detail(), canonical.detail());
            }
        }

        template <template <int> class Type>
        static void verifyIsomorphismSignatureUsing(
                const Triangulation<dim>& tri) {
            SCOPED_TRACE_TYPE(Type<dim>);

            std::string sig = tri.template isoSig<Type<dim>>();
            SCOPED_TRACE_STDSTRING(sig);

            ASSERT_FALSE(sig.empty());

            size_t sigSize = Triangulation<dim>::isoSigComponentSize(sig);
            if (tri.isEmpty()) {
                EXPECT_EQ(sigSize, 0);
            } else {
                size_t c;
                for (c = 0; c < tri.countComponents(); ++c)
                    if (sigSize == tri.component(c)->size())
                        break;
                if (c == tri.countComponents())
                    ADD_FAILURE() << "isoSigComponentSize() does not "
                        "match any component";
            }

            ASSERT_NO_THROW({
                EXPECT_TRUE(regina::Triangulation<dim>::fromIsoSig(sig).
                    isIsomorphicTo(tri));
            });

            // Does rebuilding still work if the signature has whitespace?
            EXPECT_NO_THROW({
                EXPECT_TRUE(regina::Triangulation<dim>::fromIsoSig(
                    std::string("\t " + sig + "\t \n")).isIsomorphicTo(tri));
            });

            if (tri.isEmpty())
                return;

            static constexpr int trials = 5;

            for (int i = 0; i < trials; ++i) {
                auto other = Isomorphism<dim>::random(tri.size())(tri);
                EXPECT_EQ(other.template isoSig<Type<dim>>(), sig);
            }

            if (tri.countComponents() == 1) {
                auto detail = tri.template isoSigDetail<Type<dim>>();

                EXPECT_EQ(detail.first, sig);
                EXPECT_EQ(detail.second(tri),
                    Triangulation<dim>::fromIsoSig(detail.first));
            }
        }

        static void verifyIsomorphismSignature(const Triangulation<dim>& tri,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);

            verifyIsomorphismSignatureUsing<regina::IsoSigClassic>(tri);
            verifyIsomorphismSignatureUsing<regina::IsoSigEdgeDegrees>(tri);
        }

        template <int k>
        static void verifyPachnerDetail(const Triangulation<dim>& tri,
                bool standardSimplex) {
            // Tests Pachner moves on k-faces, and their inverses.
            static_assert(0 <= k && k <= dim);
            SCOPED_TRACE_NAMED_NUMERIC("subdim", k);

            Triangulation<dim> oriented(tri);
            if (tri.isOrientable())
                oriented.orient();

            for (size_t i = 0; i < tri.template countFaces<k>(); ++i) {
                SCOPED_TRACE_NAMED_NUMERIC("face", i);

                Triangulation<dim> result(oriented);

                // Perform the move (if we can).
                bool performed;
                if constexpr (k == dim) {
                    // Moves on top-dimensional simplices are always allowed.
                    performed = result.pachner(result.simplex(i));
                    EXPECT_TRUE(performed);
                } else {
                    // For the simplicial sphere, all k-faces can be used.
                    // Otherwise, the legality of the move is still easy to
                    // know in advance for k == dim - 1.
                    auto face = result.template face<k>(i);
                    if constexpr (k == dim - 1) {
                        bool expected = ! (face->isBoundary() ||
                            face->embedding(0).simplex() ==
                            face->embedding(1).simplex());
                        performed = result.pachner(face); // destroys face
                        EXPECT_EQ(performed, expected);
                    } else {
                        performed = result.pachner(face); // destroys face
                    }
                    if (standardSimplex)
                        EXPECT_TRUE(performed);
                }
                // Ensure that properties we are about to verify have been
                // explicitly recomputed.
                clearProperties(result);

                if (! performed) {
                    // Verify that the move was indeed not performed.
                    EXPECT_EQ(result, oriented);
                    continue;
                }

                // The move was performed.

                EXPECT_EQ(result.size(), tri.size() + 2 * k - dim);
                EXPECT_EQ(result.isValid(), tri.isValid());
                EXPECT_EQ(result.isOrientable(), tri.isOrientable());
                if (tri.isOrientable())
                    EXPECT_TRUE(result.isOriented());
                EXPECT_EQ(result.countBoundaryComponents(),
                    tri.countBoundaryComponents());
                EXPECT_EQ(result.eulerCharTri(), tri.eulerCharTri());

                // Closedness can only be tested in standard dimensions.
                if constexpr (regina::standardDim(dim))
                    EXPECT_EQ(result.isClosed(), tri.isClosed());

                // Homology can only be tested for valid triangulations.
                if (tri.size() <= HOMOLOGY_THRESHOLD && tri.isValid()) {
                    EXPECT_EQ(result.homology(), tri.homology());
                    // We only test H2 in small dimensions, since for higher
                    // dimensions this becomes too slow.
                    if constexpr (dim == 3 || dim == 4)
                        EXPECT_EQ(result.template homology<2>(),
                            tri.template homology<2>());
                }

                // Randomly relabel the simplices, but preserve orientation.
                Isomorphism<dim> iso = result.randomiseLabelling(true);

                if constexpr (k == dim && (dim == 3 || dim == 4)) {
                    // For k == dim, we can undo the Pacher move with an edge
                    // collapse (which is supported for dimensions 3 and 4).
                    regina::Triangulation<dim> inv(result);

                    performed = inv.collapseEdge(
                        inv.simplex(iso.simpImage(tri.size() + dim - 1))->
                            edge(regina::Edge<dim>::edgeNumber
                                [iso.facetPerm(tri.size() + dim - 1)[0]]
                                [iso.facetPerm(tri.size() + dim - 1)[dim]]),
                        true, true);
                    EXPECT_TRUE(performed);

                    // Don't clear properties from inv, since what we're about
                    // to test does not rely on computed topological properties.
                    EXPECT_TRUE(inv.isIsomorphicTo(tri));
                    if (tri.isOrientable())
                        EXPECT_TRUE(inv.isOriented());
                }

                // For all k, we can undo the original Pachner move by
                // performing the inverse Pachner move.
                regina::Triangulation<dim> inv(result);
                if constexpr (k == 0) {
                    performed = inv.pachner(
                        inv.simplex(iso.simpImage(result.size() - 1)),
                        true, true);
                } else {
                    auto face = inv.simplex(iso.simpImage(result.size() - 1))->
                        template face<dim - k>(
                            regina::Face<dim, dim - k>::faceNumber(
                                iso.facetPerm(result.size() - 1)));
                    performed = inv.pachner(face, true, true);
                }
                EXPECT_TRUE(performed);

                // Don't clear properties from inv, since what we're about to
                // test does not rely on computed topological properties.
                EXPECT_TRUE(inv.isIsomorphicTo(tri));
                if (tri.isOrientable())
                    EXPECT_TRUE(inv.isOriented());
            }
        }

        static void verifyPachner(const Triangulation<dim>& tri,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);
            regina::for_constexpr<0, dim + 1>([&tri](auto subdim) {
                verifyPachnerDetail<subdim>(tri, false);
            });
        }

        void verifyPachnerSimplicial() {
            SCOPED_TRACE("Simplicial sphere");
            regina::for_constexpr<0, dim + 1>([this](auto subdim) {
                verifyPachnerDetail<subdim>(simpSphere.tri, true);
            });
        }

        static void verifyTwoZeroVertex(const Triangulation<dim>& tri,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);

            Triangulation<dim> oriented(tri);
            if (oriented.isOrientable())
                oriented.orient();

            for (size_t i = 0; i < oriented.countVertices(); ++i) {
                SCOPED_TRACE_NUMERIC(i);

                Vertex<dim>* v = oriented.vertex(i);
                if (v->degree() != 2 || v->isBoundary()) {
                    EXPECT_FALSE(oriented.twoZeroMove(v, true, false));
                    continue;
                }

                regina::VertexEmbedding<dim> emb0 = v->front();
                regina::VertexEmbedding<dim> emb1 = v->back();
                if (emb0.simplex() == emb1.simplex()) {
                    EXPECT_FALSE(oriented.twoZeroMove(v, true, false));
                    continue;
                }

                int v0 = emb0.face();
                int v1 = emb1.face();
                auto opp0 = emb0.simplex()->template face<dim-1>(v0);
                auto opp1 = emb1.simplex()->template face<dim-1>(v1);
                if (opp0 == opp1 ||
                        (opp0->isBoundary() && opp1->isBoundary())) {
                    EXPECT_FALSE(oriented.twoZeroMove(v, true, false));
                    continue;
                }

                auto glue = emb0.simplex()->adjacentGluing(v0 != 0 ? 0 : 1);
                bool correctLink = true;
                for (int i = 0; i <= dim; ++i)
                    if (i != v0 && ! (
                            emb0.simplex()->adjacentSimplex(i) ==
                                emb1.simplex() &&
                            emb0.simplex()->adjacentGluing(i) == glue)) {
                        correctLink = false;
                        break;
                    }
                if (! correctLink) {
                    EXPECT_FALSE(oriented.twoZeroMove(v, true, false));
                    continue;
                }

                // The move should be legal.
                Triangulation<dim> alt(oriented);
                EXPECT_TRUE(alt.twoZeroMove(alt.vertex(i)));
                EXPECT_EQ(alt.isOriented(), alt.isOrientable());

                // Verify that the move did the right thing.
                // Here the "right thing" is a 2-dim Pachner move followed by a
                // (dim+1)-1 Pachner move.
                Triangulation<dim> alt2(oriented);
                Simplex<dim>* simp0 = alt2.simplex(emb0.simplex()->index());
                Simplex<dim>* simp1 = alt2.simplex(emb1.simplex()->index());
                if (simp0->adjacentSimplex(v0)) {
                    EXPECT_TRUE(alt2.pachner(simp0->template face<dim-1>(v0)));
                    EXPECT_TRUE(alt2.pachner(simp1->vertex(v1)));
                } else {
                    EXPECT_TRUE(alt2.pachner(simp1->template face<dim-1>(v1)));
                    EXPECT_TRUE(alt2.pachner(simp0->vertex(v0)));
                }
                EXPECT_TRUE(alt.isIsomorphicTo(alt2));
            }
        }

        static void verifyBarycentricSubdivision(const Triangulation<dim>& tri,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);

            Triangulation<dim> subdiv(tri);
            if (subdiv.isOrientable())
                subdiv.orient();

            subdiv.subdivide();
            // Ensure that properties we are about to verify have been
            // explicitly recomputed.
            clearProperties(subdiv);

            EXPECT_EQ(tri.hasBoundaryFacets(), subdiv.hasBoundaryFacets());
            EXPECT_EQ(tri.isOrientable(), subdiv.isOrientable());
            if (tri.isOrientable())
                EXPECT_TRUE(subdiv.isOriented());
            EXPECT_EQ(tri.isConnected(), subdiv.isConnected());
            EXPECT_EQ(tri.countComponents(), subdiv.countComponents());

            // Subdivisions can turn invalid triangulations into valid
            // triangulations (specifically when there are bad face
            // identifications involved).  These wreaks havoc on several tests
            // in cases where the incoming triangulation is not valid.
            if (tri.isValid()) {
                EXPECT_TRUE(subdiv.isValid());
                EXPECT_EQ(tri.isClosed(), subdiv.isClosed());
                EXPECT_EQ(tri.isIdeal(), subdiv.isIdeal());
                EXPECT_EQ(tri.countBoundaryComponents(),
                    subdiv.countBoundaryComponents());
                EXPECT_EQ(tri.eulerCharTri(), subdiv.eulerCharTri());
                if constexpr (regina::standardDim(dim) && dim > 2)
                    EXPECT_EQ(tri.eulerCharManifold(),
                        subdiv.eulerCharManifold());
            } else {
                // Subdivision can _create_ ideal vertices, but cannot remove
                // them.
                if (! tri.isClosed())
                    EXPECT_FALSE(subdiv.isClosed());
                if (tri.isIdeal())
                    EXPECT_TRUE(subdiv.isIdeal());
                EXPECT_LE(tri.countBoundaryComponents(),
                    subdiv.countBoundaryComponents());
            }

            // Some tests that are better done _after_ simplification:
            if constexpr (dim > 2) {
                subdiv.simplify();
                // While we're here: simplification shouldn't break orientation.
                if (tri.isOrientable())
                    EXPECT_TRUE(subdiv.isOriented());
            }

            // Note: homology<k>() requires a valid triangulation for k > 1,
            // and even with k == 1, bad face identifications can mess with
            // the comparison (since these become ideal vertices after
            // subdivision).
            if (tri.isValid()) {
                regina::for_constexpr<1, dim/2 + 1>([&tri, &subdiv](auto k) {
                    EXPECT_EQ(tri.template homology<k>(),
                        subdiv.template homology<k>());
                });
            }
        }

        static void verifyTightEncoding(const Triangulation<dim>& tri,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);
            TightEncodingTest<Triangulation<dim>>::verifyTightEncoding(tri);
        }

        void homologyH1GenericCases() {
            using regina::AbelianGroup;

            EXPECT_EQ(empty.tri.template homology<1>(), AbelianGroup());
            EXPECT_EQ(sphere.tri.template homology<1>(), AbelianGroup());
            EXPECT_EQ(simpSphere.tri.template homology<1>(), AbelianGroup());
            if constexpr (dim == 2) {
                EXPECT_EQ(sphereBundle.tri.template homology<1>(),
                    AbelianGroup(2));
                EXPECT_EQ(twistedSphereBundle.tri.template homology<1>(),
                    AbelianGroup(1, {2}));
            } else {
                EXPECT_EQ(sphereBundle.tri.template homology<1>(),
                    AbelianGroup(1));
                EXPECT_EQ(twistedSphereBundle.tri.template homology<1>(),
                    AbelianGroup(1));
            }
            EXPECT_EQ(ball.tri.template homology<1>(), AbelianGroup());
            EXPECT_EQ(ballBundle.tri.template homology<1>(), AbelianGroup(1));
            EXPECT_EQ(twistedBallBundle.tri.template homology<1>(),
                AbelianGroup(1));
        }

        void homologyH2GenericCases() {
            static_assert(dim > 2);
            using regina::AbelianGroup;

            // It's a pity that almost all of these examples have trivial H2.
            // We need some more interesting generic constructions.

            EXPECT_EQ(empty.tri.template homology<2>(), AbelianGroup());
            EXPECT_EQ(sphere.tri.template homology<2>(), AbelianGroup());
            EXPECT_EQ(simpSphere.tri.template homology<2>(), AbelianGroup());
            if constexpr (dim == 3) {
                EXPECT_EQ(sphereBundle.tri.template homology<2>(),
                    AbelianGroup(1));
                EXPECT_EQ(twistedSphereBundle.tri.template homology<2>(),
                    AbelianGroup(0, {2}));
            } else {
                EXPECT_EQ(sphereBundle.tri.template homology<2>(),
                    AbelianGroup());
                EXPECT_EQ(twistedSphereBundle.tri.template homology<2>(),
                    AbelianGroup());
            }
            EXPECT_EQ(ball.tri.template homology<2>(), AbelianGroup());
            EXPECT_EQ(ballBundle.tri.template homology<2>(), AbelianGroup());
            EXPECT_EQ(twistedBallBundle.tri.template homology<2>(),
                AbelianGroup());

            if constexpr (dim == 5) {
                using Example = regina::Example<dim>;
                using LowDimExample = regina::Example<dim - 1>;
                EXPECT_EQ(Example::singleCone(LowDimExample::sphereBundle()).
                    template homology<2>(), AbelianGroup());
                EXPECT_EQ(Example::doubleCone(LowDimExample::sphereBundle()).
                    template homology<2>(), AbelianGroup());

                EXPECT_EQ(
                    Example::singleCone(LowDimExample::twistedSphereBundle()).
                    template homology<2>(), AbelianGroup());
                EXPECT_EQ(
                    Example::doubleCone(LowDimExample::twistedSphereBundle()).
                    template homology<2>(), AbelianGroup());

                EXPECT_EQ(
                    Example::singleCone(LowDimExample::s2xs2()).
                    template homology<2>(), AbelianGroup(2));
                EXPECT_EQ(
                    Example::doubleCone(LowDimExample::s2xs2()).
                    template homology<2>(), AbelianGroup(2));
            }
        }

        void homologyH3GenericCases() {
            static_assert(dim >= 4);
            using regina::AbelianGroup;

            // It's a pity that almost all of these examples have trivial H3.
            // We need some more interesting generic constructions.

            EXPECT_EQ(empty.tri.template homology<3>(), AbelianGroup());
            EXPECT_EQ(sphere.tri.template homology<3>(), AbelianGroup());
            EXPECT_EQ(simpSphere.tri.template homology<3>(), AbelianGroup());
            if constexpr (dim == 4) {
                EXPECT_EQ(sphereBundle.tri.template homology<3>(),
                    AbelianGroup(1));
                EXPECT_EQ(twistedSphereBundle.tri.template homology<3>(),
                    AbelianGroup(0, {2}));
            } else {
                EXPECT_EQ(sphereBundle.tri.template homology<3>(),
                    AbelianGroup());
                EXPECT_EQ(twistedSphereBundle.tri.template homology<3>(),
                    AbelianGroup());
            }
            EXPECT_EQ(ball.tri.template homology<3>(), AbelianGroup());
            EXPECT_EQ(ballBundle.tri.template homology<3>(), AbelianGroup());
            EXPECT_EQ(twistedBallBundle.tri.template homology<3>(),
                AbelianGroup());

            if constexpr (dim == 5) {
                using Example = regina::Example<dim>;
                using LowDimExample = regina::Example<dim - 1>;
                EXPECT_EQ(Example::singleCone(LowDimExample::sphereBundle()).
                    template homology<3>(), AbelianGroup(1));
                EXPECT_EQ(Example::doubleCone(LowDimExample::sphereBundle()).
                    template homology<3>(), AbelianGroup(1));

                EXPECT_EQ(
                    Example::singleCone(LowDimExample::twistedSphereBundle()).
                    template homology<3>(), AbelianGroup(0, {2}));
                EXPECT_EQ(
                    Example::doubleCone(LowDimExample::twistedSphereBundle()).
                    template homology<3>(), AbelianGroup(0, {2}));

                EXPECT_EQ(Example::singleCone(LowDimExample::s2xs2()).
                    template homology<3>(), AbelianGroup());
                EXPECT_EQ(Example::doubleCone(LowDimExample::s2xs2()).
                    template homology<3>(), AbelianGroup());
            }
        }

        static void verifyBoundaryH1(const TestCase& test,
                size_t whichBdry, const regina::AbelianGroup& expect) {
            static_assert(dim > 2);

            SCOPED_TRACE_CSTRING(test.name);
            ASSERT_LT(whichBdry, test.tri.countBoundaryComponents());

            // Calling homology() does not truncate ideal boundaries
            // at the centroids of invalid (dim-3)-faces that are
            // self-identified under a non-trivial map.
            //
            // This problem only appears in dimension dim ≥ 4.
            // Unfortunately, to fix it we need to do a barycentric
            // subdivision, which is currently only available in
            // dimension dim ≤ 5 (i.e., where the boundary triangulation
            // has dimension ≤ 4).
            //
            // So: for the time being, we perform this subdivision for
            // the cases dim ≤ 5 only.
            auto t = test.tri.boundaryComponent(whichBdry)->build();
            if constexpr (dim >= 4 && regina::standardDim(dim - 1)) {
                t.subdivide();
                t.simplify();
            }
            EXPECT_EQ(t.homology(), expect);
        }

        void boundaryHomologyGenericCases() {
            static_assert(dim > 2);

            verifyBoundaryH1(ball, 0, {});
            if constexpr (dim == 3) {
                verifyBoundaryH1(ballBundle, 0, {2});
                verifyBoundaryH1(twistedBallBundle, 0, {1, {2}});
            } else {
                verifyBoundaryH1(ballBundle, 0, {1});
                verifyBoundaryH1(twistedBallBundle, 0, {1});
            }
        }

        void fundGroupGenericCases() {
            EXPECT_EQ(empty.tri.group().recogniseGroup(), "0");
            EXPECT_EQ(sphere.tri.group().recogniseGroup(), "0");
            EXPECT_EQ(simpSphere.tri.group().recogniseGroup(), "0");
            if constexpr (dim == 2) {
                EXPECT_EQ(sphereBundle.tri.group().recogniseGroup(), "2 Z");
                EXPECT_EQ(twistedSphereBundle.tri.group().recogniseGroup(),
                    "Z~Z w/monodromy a ↦ a^-1");
            } else {
                EXPECT_EQ(sphereBundle.tri.group().recogniseGroup(), "Z");
                EXPECT_EQ(twistedSphereBundle.tri.group().recogniseGroup(),
                    "Z");
            }
            EXPECT_EQ(ball.tri.group().recogniseGroup(), "0");
            EXPECT_EQ(ballBundle.tri.group().recogniseGroup(), "Z");
            EXPECT_EQ(twistedBallBundle.tri.group().recogniseGroup(), "Z");
        }

        template <int k>
        static void verifyChainComplexDetail(const Triangulation<dim>& tri) {
            static_assert(0 < k && k < dim);
            SCOPED_TRACE_NUMERIC(k);

            // These tests use homology on the skeleton: invalid or empty
            // triangulations are explicitly disallowed, and ideal
            // triangluations will give wrong answers (since the ideal
            // vertices will not be considered as truncated).
            if (tri.isEmpty() || ! tri.isValid())
                return;
            if (tri.isIdeal())
                return;

            regina::MatrixInt m = tri.template boundaryMap<k>();
            regina::MatrixInt n = tri.template boundaryMap<k + 1>();

            ASSERT_EQ(m.columns(), n.rows());
            ASSERT_TRUE((m * n).isZero());

            // Verify that homology with Z coefficients is correct:
            regina::AbelianGroup g1(m, n);
            regina::MarkedAbelianGroup g2 = tri.template markedHomology<k>();
            ASSERT_EQ(g1, g2.unmarked());
            EXPECT_EQ(tri.template homology<k>(), g1);

            // Verify that homology with Z_2 coefficients looks believable:
            regina::AbelianGroup g1z2(m, n, 2);
            regina::MarkedAbelianGroup g2z2(m, n, 2);
            EXPECT_EQ(g1z2, g2z2.unmarked());
            EXPECT_EQ(g1z2.rank(), 0);
            size_t z2rank = g1z2.countInvariantFactors();
            for (size_t i = 0; i < z2rank; ++i)
                EXPECT_EQ(g1z2.invariantFactor(i), 2);
            if constexpr (k == 2 && dim == 3) {
                // For this special case, we can verify the group precisely.
                EXPECT_EQ(tri.homologyH2Z2(), z2rank);
            }
        }

        static void verifyChainComplex(const Triangulation<dim>& tri,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);
            regina::for_constexpr<1, dim>([&tri](auto k) {
                verifyChainComplexDetail<k>(tri);
            });
        }

        template <int k>
        static void verifyDualChainComplexDetail(
                const Triangulation<dim>& tri) {
            static_assert(0 < k && k < dim);
            SCOPED_TRACE_NUMERIC(k);

            // These tests use homology on the dual skeleton: invalid or
            // empty triangulations are explicitly disallowed, but ideal
            // triangulations are fine.
            if (tri.isEmpty() || ! tri.isValid())
                return;

            regina::MatrixInt m = tri.template dualBoundaryMap<k>();
            regina::MatrixInt n = tri.template dualBoundaryMap<k + 1>();

            ASSERT_EQ(m.columns(), n.rows());
            ASSERT_TRUE((m * n).isZero());

            // Verify that homology with Z coefficients is correct:
            regina::AbelianGroup g1(m, n);
            EXPECT_EQ(tri.template homology<k>(), g1);

            // Verify that homology with Z_2 coefficients looks believable:
            regina::AbelianGroup g1z2(m, n, 2);
            EXPECT_EQ(g1z2.rank(), 0);
            size_t z2rank = g1z2.countInvariantFactors();
            for (size_t i = 0; i < z2rank; ++i)
                EXPECT_EQ(g1z2.invariantFactor(i), 2);
            if constexpr (k == 2 && dim == 3) {
                // For this special case, we can verify the group precisely.
                EXPECT_EQ(tri.homologyH2Z2(), z2rank);
            }
        }

        static void verifyDualChainComplex(const Triangulation<dim>& tri,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);
            regina::for_constexpr<1, dim>([&tri](auto k) {
                verifyDualChainComplexDetail<k>(tri);
            });
        }

        template <int k>
        static void verifyDualToPrimalDetail(const Triangulation<dim>& tri) {
            static_assert(0 <= k && k < dim);
            SCOPED_TRACE_NUMERIC(k);

            // Do not try to work with triangulations that fail the
            // preconditions for dualToPrimal().
            if (tri.isEmpty() || ! tri.isValid())
                return;
            if constexpr (regina::standardDim(dim))
                if (tri.isIdeal())
                    return;

            regina::MatrixInt map = tri.template dualToPrimal<k>();

            // This map sends homologous cycles to homologous cycles;
            // in particular, this means it must send boundaries to boundaries.
            //
            // Also, the map should describe an isomorphism between the dual
            // and primal homology groups.

            // Start with what is easy to test.

            if constexpr (regina::standardDim(dim) || k + 1 < dim) {
                auto dualBoundariesAsPrimal =
                    map * tri.template dualBoundaryMap<k + 1>();

                if constexpr (0 < k)
                    EXPECT_TRUE((tri.template boundaryMap<k>() *
                            dualBoundariesAsPrimal).isZero());

                if (! dualBoundariesAsPrimal.isZero()) {
                    // Test whether the column space for dualBoundariesAsPrimal
                    // lives within the column space for boundaryMap<k + 1>.
                    auto b = tri.template boundaryMap<k + 1>();
                    auto rank = b.columnEchelonForm();

                    regina::MatrixInt comb(b.rows(),
                        b.columns() + dualBoundariesAsPrimal.columns());
                    for (size_t row = 0; row < b.rows(); ++row) {
                        for (size_t col = 0; col < b.columns(); ++col)
                            comb.entry(row, col) = b.entry(row, col);
                        for (size_t col = 0;
                                col < dualBoundariesAsPrimal.columns(); ++col)
                            comb.entry(row, b.columns() + col) =
                                dualBoundariesAsPrimal.entry(row, col);
                    }

                    EXPECT_EQ(rank, std::move(comb).rank());
                }

                if constexpr (0 < k) {
                    // We can use HomMarkedAbelianGroup to verify that
                    // this is indeed an isomorphism between homology groups.
                    regina::MarkedAbelianGroup homDual(
                        tri.template dualBoundaryMap<k>(),
                        tri.template dualBoundaryMap<k+1>());
                    regina::MarkedAbelianGroup homPrimal(
                        tri.template boundaryMap<k>(),
                        tri.template boundaryMap<k+1>());
                    regina::HomMarkedAbelianGroup hom(homDual, homPrimal, map);

                    EXPECT_TRUE(hom.isCycleMap());
                    EXPECT_TRUE(hom.isEpic());
                    EXPECT_TRUE(hom.isMonic());
                }
            }
        }

        static void verifyDualToPrimal(const Triangulation<dim>& tri,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);
            regina::for_constexpr<0, dim>([&tri](auto k) {
                verifyDualToPrimalDetail<k>(tri);
            });
        }

        static bool looksIdentical(const Triangulation<dim>& a,
                const Triangulation<dim>& b) {
            if (a.size() != b.size())
                return false;
            if (a.countComponents() != b.countComponents())
                return false;
            if (a != b)
                return false;

            // Test isosigs only in smaller dimensions, since the
            // running time grows with (dim!).
            if constexpr (dim <= 6)
                if (a.isoSig() != b.isoSig())
                    return false;

            return true;
        }

        static void verifyCopyMove(const Triangulation<dim>& t,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);

            if (t.isEmpty()) {
                EXPECT_EQ(t.size(), 0);

                Triangulation<dim> copy(t);
                EXPECT_TRUE(copy.isEmpty());
                EXPECT_TRUE(looksIdentical(copy, t));

                Triangulation<dim> move(std::move(copy));
                EXPECT_TRUE(move.isEmpty());
                EXPECT_TRUE(looksIdentical(move, t));

                Triangulation<dim> copyAss;
                copyAss.newSimplex(); // Give it something to overwrite.
                copyAss = t;
                EXPECT_TRUE(copyAss.isEmpty());
                EXPECT_TRUE(looksIdentical(copyAss, t));

                Triangulation<dim> moveAss;
                moveAss.newSimplex(); // Give it something to overwrite.
                moveAss = std::move(copyAss);
                EXPECT_TRUE(moveAss.isEmpty());
                EXPECT_TRUE(looksIdentical(moveAss, t));
            } else {
                ASSERT_GT(t.size(), 0);

                ASSERT_GT(t.countVertices(), 0);
                Vertex<dim>* v0 = t.vertex(0);

                Triangulation<dim> copy(t);
                EXPECT_TRUE(looksIdentical(copy, t));

                // Copy construction should use different vertices.
                ASSERT_GT(copy.countVertices(), 0);
                Vertex<dim>* v1 = copy.vertex(0);
                EXPECT_NE(v1, v0);

                Triangulation<dim> move(std::move(copy));
                EXPECT_TRUE(looksIdentical(move, t));

                // Move construction should use the same vertices.
                ASSERT_GT(move.countVertices(), 0);
                Vertex<dim>* v2 = move.vertex(0);
                EXPECT_EQ(v2, v1);

                Triangulation<dim> copyAss;
                copyAss.newSimplex(); // Give it something to overwrite.
                copyAss = t;
                EXPECT_TRUE(looksIdentical(copyAss, t));

                // Copy assignment should use different vertices.
                ASSERT_GT(copyAss.countVertices(), 0);
                Vertex<dim>* v3 = copyAss.vertex(0);
                EXPECT_NE(v3, v0);

                Triangulation<dim> moveAss;
                moveAss.newSimplex(); // Give it something to overwrite.
                moveAss = std::move(copyAss);
                EXPECT_TRUE(looksIdentical(moveAss, t));

                // Move assignment should use the same vertices.
                ASSERT_GT(moveAss.countVertices(), 0);
                Vertex<dim>* v4 = moveAss.vertex(0);
                EXPECT_EQ(v4, v3);
            }
        }
};
