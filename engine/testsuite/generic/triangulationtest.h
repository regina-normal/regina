
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
using regina::Triangulation;
using regina::Vertex;

/**
 * Implements several tests for triangulations in dimension \a dim.
 *
 * Test fixtures in each dimension should use TriangulationTest<dim>
 * as a base class, since this base class provides example triangulations
 * that can be shared between tests.
 */
template <int dim>
class TriangulationTest : public testing::Test {
    protected:
        struct TestCase {
            Triangulation<dim> tri;
            const char* name;
        };

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
         * Clear all computed properties of the given triangulation.
         *
         * We allow the triangulation to be const, since the intent of this
         * operation is to not change the triangulation, but just to force it to
         * forget its cached properties.
         */
        static void clearProperties(const Triangulation<dim>& tri) {
            // Make and undo a trivial modification that will cause all
            // computed properties to be flushed.
            const_cast<Triangulation<dim>&>(tri).newSimplex();
            const_cast<Triangulation<dim>&>(tri).removeSimplexAt(tri.size()-1);
        }

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

        static void verifyValidity(const TestCase& test, bool isValid) {
            SCOPED_TRACE_CSTRING(test.name);

            EXPECT_EQ(test.tri.isValid(), isValid);

            regina::for_constexpr<0, dim>([&test](auto subdim) {
                SCOPED_TRACE_NUMERIC(subdim);
                for (size_t i = 0; i < test.tri.template countFaces<subdim>();
                        ++i) {
                    SCOPED_TRACE_NAMED_NUMERIC("face", i);
                    auto f = test.tri.template face<subdim>(i);

                    EXPECT_TRUE(f->isValid());
                    if constexpr (regina::standardDim(dim) && dim > 2)
                        EXPECT_FALSE(f->hasBadLink());
                    if constexpr (subdim > 0)
                        EXPECT_FALSE(f->hasBadIdentification());
                }
            });
        }

        void validityGenericCases() {
            verifyValidity(empty, true);
            verifyValidity(sphere, true);
            verifyValidity(simpSphere, true);
            verifyValidity(sphereBundle, true);
            verifyValidity(twistedSphereBundle, true);
            verifyValidity(ball, true);
            verifyValidity(ballBundle, true);
            verifyValidity(twistedBallBundle, true);
        }

        static void verifyConnectivity(const TestCase& test, bool isConnected) {
            SCOPED_TRACE_CSTRING(test.name);

            EXPECT_EQ(test.tri.isConnected(), isConnected);
        }

        void connectivityGenericCases() {
            verifyConnectivity(empty, true);
            verifyConnectivity(sphere, true);
            verifyConnectivity(simpSphere, true);
            verifyConnectivity(sphereBundle, true);
            verifyConnectivity(twistedSphereBundle, true);
            verifyConnectivity(ball, true);
            verifyConnectivity(ballBundle, true);
            verifyConnectivity(twistedBallBundle, true);
        }

        static void verifyOrientability(const TestCase& test,
                bool isOrientable) {
            SCOPED_TRACE_CSTRING(test.name);

            EXPECT_EQ(test.tri.isOrientable(), isOrientable);
        }

        void orientabilityGenericCases() {
            verifyOrientability(empty, true);
            verifyOrientability(sphere, true);
            verifyOrientability(simpSphere, true);
            verifyOrientability(sphereBundle, true);
            verifyOrientability(twistedSphereBundle, false);
            verifyOrientability(ball, true);
            verifyOrientability(ballBundle, true);
            verifyOrientability(twistedBallBundle, false);
        }

        static void verifyEulerCharTri(const TestCase& test,
                long expectedEuler) {
            SCOPED_TRACE_CSTRING(test.name);

            EXPECT_EQ(test.tri.eulerCharTri(), expectedEuler);
        }

        void eulerCharTriGenericCases() {
            verifyEulerCharTri(empty, 0);
            verifyEulerCharTri(sphere, (dim % 2 ? 0 : 2));
            verifyEulerCharTri(simpSphere, (dim % 2 ? 0 : 2));
            verifyEulerCharTri(sphereBundle, 0);
            verifyEulerCharTri(twistedSphereBundle, 0);
            verifyEulerCharTri(ball, 1);
            verifyEulerCharTri(ballBundle, 0);
            verifyEulerCharTri(twistedBallBundle, 0);
        }

        static void verifyBoundaryCount(const TestCase& test,
                size_t nReal, size_t nIdeal, size_t nInvalid) {
            SCOPED_TRACE_CSTRING(test.name);

            EXPECT_EQ(test.tri.countBoundaryComponents(),
                nReal + nIdeal + nInvalid);

            if constexpr (dim == 3 || dim == 4) {
                // These dimensions support single-vertex boundary components.
                size_t foundReal = 0, foundIdeal = 0, foundInvalid = 0;
                for (auto bc : test.tri.boundaryComponents()) {
                    if (bc->isIdeal())
                        ++foundIdeal;
                    if (bc->isInvalidVertex(bc))
                        ++foundInvalid;
                    if (bc->isReal())
                        ++foundReal;
                }

                EXPECT_EQ(foundReal, nReal);
                EXPECT_EQ(foundIdeal, nIdeal);
                EXPECT_EQ(foundInvalid, nInvalid);
            }
        }

        void boundaryCountGenericCases() {
            verifyBoundaryCount(empty, 0, 0, 0);
            verifyBoundaryCount(sphere, 0, 0, 0);
            verifyBoundaryCount(simpSphere, 0, 0, 0);
            verifyBoundaryCount(sphereBundle, 0, 0, 0);
            verifyBoundaryCount(twistedSphereBundle, 0, 0, 0);
            verifyBoundaryCount(ball, 1, 0, 0);
            if constexpr (dim == 2) {
                verifyBoundaryCount(ballBundle, 2, 0, 0);
            } else {
                verifyBoundaryCount(ballBundle, 1, 0, 0);
            }
            verifyBoundaryCount(twistedBallBundle, 1, 0, 0);
        }

        static void verifyBoundaryEuler(const Triangulation<dim>& tri,
                const char* name) {
            static_assert(regina::standardDim(dim));
            static_assert(dim % 2 == 0);
            SCOPED_TRACE_CSTRING(name);

            for (auto bc : tri.boundaryComponents())
                EXPECT_EQ(bc->eulerChar(), 0);
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
                                EXPECT_EQ(v->linkType(), Vertex<dim>::SPHERE);
                        } else if (link.isBall()) {
                            EXPECT_TRUE(v->isValid());
                            EXPECT_FALSE(v->isIdeal());
                            if constexpr (dim == 3)
                                EXPECT_EQ(v->linkType(), Vertex<dim>::DISC);
                        } else if (link.isValid() && link.isClosed()) {
                            EXPECT_TRUE(v->isValid());
                            EXPECT_TRUE(v->isIdeal());
                            if constexpr (dim == 3) {
                                if (link.eulerCharTri() == 0) {
                                    if (link.isOrientable())
                                        EXPECT_EQ(v->linkType(),
                                            Vertex<dim>::TORUS);
                                    else
                                        EXPECT_EQ(v->linkType(),
                                            Vertex<dim>::KLEIN_BOTTLE);
                                } else {
                                    allStandard = false;
                                    EXPECT_EQ(v->linkType(),
                                        Vertex<dim>::NON_STANDARD_CUSP);
                                }
                            }
                        } else {
                            allStandard = false;
                            EXPECT_FALSE(v->isValid());
                            EXPECT_FALSE(v->isIdeal());
                            if constexpr (dim == 3)
                                EXPECT_EQ(v->linkType(), Vertex<dim>::INVALID);
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
                EXPECT_EQ(bc->template countFaces<subdim>(),
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

            // Reordering the original:
            {
                Triangulation<dim> a(t);
                a.reorderBFS();
                clearProperties(a);
                EXPECT_TRUE(t.isIsomorphicTo(a));
            }
            {
                Triangulation<dim> b(t);
                b.reorderBFS(true);
                clearProperties(b);
                EXPECT_TRUE(t.isIsomorphicTo(b));
            }

            // Reordering a random relabelling of the original:
            Triangulation<dim> relabel = Isomorphism<dim>::random(t.size())(t);
            clearProperties(relabel);
            EXPECT_TRUE(t.isIsomorphicTo(relabel));
            {
                Triangulation<dim> d(relabel);
                d.reorderBFS();
                clearProperties(d);
                EXPECT_TRUE(t.isIsomorphicTo(d));
            }
            {
                Triangulation<dim> e(relabel);
                e.reorderBFS(true);
                clearProperties(e);
                EXPECT_TRUE(t.isIsomorphicTo(e));
            }
        }

        static void verifyDoubleCover(const Triangulation<dim>& tri,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);

            // The tests below assumed that tri has ≤ 1 connected component.
            if (! tri.isConnected())
                return;

            Triangulation<dim> cover(tri, false, true);
            cover.makeDoubleCover();

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
            clearProperties(canonical);
            EXPECT_TRUE(canonical.isIsomorphicTo(tri));

            for (int i = 0; i < trials; ++i) {
                Triangulation<dim> t =
                    Isomorphism<dim>::random(tri.size())(tri);

                t.makeCanonical();
                clearProperties(t);

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
                if (tri.isValid()) {
                    EXPECT_EQ(result.homology(), tri.homology());
                    // We only test H2 in small dimensions, since for higher
                    // dimensions this becomes too slow.
                    if constexpr (dim == 3 || dim == 4)
                        EXPECT_EQ(result.template homology<2>(),
                            tri.template homology<2>());
                }

                // Randomly relabel the simplices, but preserve orientation.
                Isomorphism<dim> iso = Isomorphism<dim>::random(result.size(),
                    true);
                result = iso(result);
                clearProperties(result);

                if constexpr (k == dim && (dim == 3 || dim == 4)) {
                    // For k == dim, we can undo the Pacher move with an edge
                    // collapse (which is supported for dimensions 3 and 4).
                    regina::Triangulation<dim> collapsed(result);

                    performed = collapsed.collapseEdge(
                        collapsed.simplex(iso.simpImage(tri.size() + dim - 1))->
                            edge(regina::Edge<dim>::edgeNumber
                                [iso.facetPerm(tri.size() + dim - 1)[0]]
                                [iso.facetPerm(tri.size() + dim - 1)[dim]]),
                        true, true);
                    clearProperties(collapsed);

                    EXPECT_TRUE(performed);
                    EXPECT_TRUE(collapsed.isIsomorphicTo(tri));
                    if (tri.isOrientable())
                        EXPECT_TRUE(collapsed.isOriented());
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
                clearProperties(inv);

                EXPECT_TRUE(performed);
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

        static void verifyBarycentricSubdivision(const Triangulation<dim>& tri,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);

            Triangulation<dim> subdiv(tri);
            if (subdiv.isOrientable())
                subdiv.orient();

            subdiv.subdivide();
            clearProperties(subdiv);

            EXPECT_EQ(tri.hasBoundaryFacets(), subdiv.hasBoundaryFacets());
            EXPECT_EQ(tri.isClosed(), subdiv.isClosed());
            EXPECT_EQ(tri.isOrientable(), subdiv.isOrientable());
            if (tri.isOrientable())
                EXPECT_TRUE(subdiv.isOriented());
            EXPECT_EQ(tri.isConnected(), subdiv.isConnected());
            EXPECT_EQ(tri.countComponents(), subdiv.countComponents());
            EXPECT_EQ(tri.countBoundaryComponents(),
                subdiv.countBoundaryComponents());
            EXPECT_EQ(tri.eulerCharTri(), subdiv.eulerCharTri());
            EXPECT_EQ(tri.homology(), subdiv.homology());
        }

        static void verifyTightEncoding(const Triangulation<dim>& tri,
                const char* name) {
            SCOPED_TRACE_CSTRING(name);
            TightEncodingTest<Triangulation<dim>>::verifyTightEncoding(tri);
        }

        template <int k = 1>
        static void verifyHomology(const TestCase& test, const char* expect) {
            SCOPED_TRACE_CSTRING(test.name);
            EXPECT_EQ(test.tri.template homology<k>().str(), expect);
        }

        void homologyH1GenericCases() {
            verifyHomology<1>(empty, "0");
            verifyHomology<1>(sphere, "0");
            verifyHomology<1>(simpSphere, "0");
            if constexpr (dim == 2) {
                verifyHomology<1>(sphereBundle, "2 Z");
                verifyHomology<1>(twistedSphereBundle, "Z + Z_2");
            } else {
                verifyHomology<1>(sphereBundle, "Z");
                verifyHomology<1>(twistedSphereBundle, "Z");
            }
            verifyHomology<1>(ball, "0");
            verifyHomology<1>(ballBundle, "Z");
            verifyHomology<1>(twistedBallBundle, "Z");
        }

        void homologyH2GenericCases() {
            static_assert(dim > 3); // otherwise expected H2 groups are wrong

            // It's a pity that almost all of these examples have trivial H2.
            // We need some more interesting generic constructions.

            verifyHomology<2>(empty, "0");
            verifyHomology<2>(sphere, "0");
            verifyHomology<2>(simpSphere, "0");
            verifyHomology<2>(sphereBundle, "0");
            verifyHomology<2>(twistedSphereBundle, "0");
            verifyHomology<2>(ball, "0");
            verifyHomology<2>(ballBundle, "0");
            verifyHomology<2>(twistedBallBundle, "0");

            if constexpr (dim == 5) {
                using Example = regina::Example<dim>;
                using LowDimExample = regina::Example<dim - 1>;
                verifyHomology<2>({
                    Example::singleCone(LowDimExample::sphereBundle()),
                    "Real/ideal S3 x S1 x I" }, "0");
                verifyHomology<2>({
                    Example::doubleCone(LowDimExample::sphereBundle()),
                    "Ideal S3 x S1 x I" }, "0");

                verifyHomology<2>({
                    Example::singleCone(LowDimExample::twistedSphereBundle()),
                    "Real/ideal S3 x~ S1 x I" }, "0");
                verifyHomology<2>({
                    Example::doubleCone(LowDimExample::twistedSphereBundle()),
                    "Ideal S3 x~ S1 x I" }, "0");

                verifyHomology<2>({
                    Example::singleCone(LowDimExample::s2xs2()),
                    "Real/ideal S2 x S2 x I" }, "2 Z");
                verifyHomology<2>({
                    Example::doubleCone(LowDimExample::s2xs2()),
                    "Ideal S2 x S2 x I" }, "2 Z");
            }
        }

        void homologyH3GenericCases() {
            static_assert(dim > 4); // otherwise expected H3 groups are wrong

            // It's a pity that almost all of these examples have trivial H3.
            // We need some more interesting generic constructions.

            verifyHomology<3>(empty, "0");
            verifyHomology<3>(sphere, "0");
            verifyHomology<3>(simpSphere, "0");
            verifyHomology<3>(sphereBundle, "0");
            verifyHomology<3>(twistedSphereBundle, "0");
            verifyHomology<3>(ball, "0");
            verifyHomology<3>(ballBundle, "0");
            verifyHomology<3>(twistedBallBundle, "0");

            if constexpr (dim == 5) {
                using Example = regina::Example<dim>;
                using LowDimExample = regina::Example<dim - 1>;
                verifyHomology<3>({
                    Example::singleCone(LowDimExample::sphereBundle()),
                    "Real/ideal S3 x S1 x I" }, "Z");
                verifyHomology<3>({
                    Example::doubleCone(LowDimExample::sphereBundle()),
                    "Ideal S3 x S1 x I" }, "Z");

                verifyHomology<3>({
                    Example::singleCone(LowDimExample::twistedSphereBundle()),
                    "Real/ideal S3 x~ S1 x I" }, "Z_2");
                verifyHomology<3>({
                    Example::doubleCone(LowDimExample::twistedSphereBundle()),
                    "Ideal S3 x~ S1 x I" }, "Z_2");

                verifyHomology<3>({
                    Example::singleCone(LowDimExample::s2xs2()),
                    "Real/ideal S2 x S2 x I" }, "0");
                verifyHomology<3>({
                    Example::doubleCone(LowDimExample::s2xs2()),
                    "Ideal S2 x S2 x I" }, "0");
            }
        }

        static void verifyBoundaryH1(const TestCase& test,
                size_t whichBdry, const char* h1) {
            SCOPED_TRACE_CSTRING(test.name);

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
            if constexpr (regina::standardDim(dim - 1)) {
                t.subdivide();
                t.intelligentSimplify();
            }
            EXPECT_EQ(t.homology().str(), h1);
        }

        void boundaryHomologyGenericCases() {
            verifyBoundaryH1(ball, 0, "0");
            verifyBoundaryH1(ballBundle, 0, "Z");
            verifyBoundaryH1(twistedBallBundle, 0, "Z");
        }

        static void verifyFundGroup(const TestCase& test, const char* expect) {
            SCOPED_TRACE_CSTRING(test.name);
            EXPECT_EQ(test.tri.group().recogniseGroup(), expect);
        }

        void fundGroupGenericCases() {
            verifyFundGroup(empty, "0");
            verifyFundGroup(sphere, "0");
            verifyFundGroup(simpSphere, "0");
            if constexpr (dim == 2) {
                verifyFundGroup(sphereBundle, "2 Z");
                verifyFundGroup(twistedSphereBundle,
                    "Z~Z w/monodromy a ↦ a^-1");
            } else {
                verifyFundGroup(sphereBundle, "Z");
                verifyFundGroup(twistedSphereBundle, "Z");
            }
            verifyFundGroup(ball, "0");
            verifyFundGroup(ballBundle, "Z");
            verifyFundGroup(twistedBallBundle, "Z");
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

                // Move construction should use different vertices.
                ASSERT_GT(move.countVertices(), 0);
                Vertex<dim>* v2 = move.vertex(0);
                EXPECT_EQ(v2, v1);

                // Copy assignment should use different vertices.
                Triangulation<dim> copyAss;
                copyAss.newSimplex(); // Give it something to overwrite.
                copyAss = t;
                EXPECT_TRUE(looksIdentical(copyAss, t));

                ASSERT_GT(copyAss.countVertices(), 0);
                Vertex<dim>* v3 = copyAss.vertex(0);
                EXPECT_NE(v3, v0);

                Triangulation<dim> moveAss;
                moveAss.newSimplex(); // Give it something to overwrite.
                moveAss = std::move(copyAss);
                EXPECT_TRUE(looksIdentical(moveAss, t));

                // Move assignment should use different vertices.
                ASSERT_GT(moveAss.countVertices(), 0);
                Vertex<dim>* v4 = moveAss.vertex(0);
                EXPECT_EQ(v4, v3);
            }
        }
};
