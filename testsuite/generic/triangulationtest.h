
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

#include "algebra/abeliangroup.h"
#include "algebra/grouppresentation.h"
#include "packet/container.h"
#include <cppunit/extensions/HelperMacros.h>

using regina::Isomorphism;
using regina::Perm;
using regina::Triangulation;
using regina::Vertex;

/**
 * Clear all computed properties of the given triangulation.
 *
 * We allow the triangulation to be const, since the intent of this
 * operation is to not change the triangulation, but just to force it to
 * forget its cached properties.
 */
template <int dim>
void clearProperties(const Triangulation<dim>& tri) {
    // Make and undo a trivial modification that will cause all
    // computed properties to be flushed.
    const_cast<Triangulation<dim>&>(tri).newSimplex();
    const_cast<Triangulation<dim>&>(tri).removeSimplexAt(tri.size() - 1);
}

/**
 * Used to extract information about faces of a triangulation, including
 * the case subdim == dim.
 */
template <int dim, int subdim>
struct FaceHelper {
    static size_t count(const Triangulation<dim>& tri) {
        return tri.template countFaces<subdim>();
    }

    static regina::Face<dim, subdim>* face(Triangulation<dim>& tri,
            size_t index) {
        return tri.template face<subdim>(index);
    }

    static regina::Face<dim, subdim>* face(
            regina::Simplex<dim>* s, Perm<dim + 1> vertices) {
        return s->template face<subdim>(
            regina::Face<dim, subdim>::faceNumber(vertices));
    }
};

template <int dim>
struct FaceHelper<dim, dim> {
    static size_t count(const Triangulation<dim>& tri) {
        return tri.size();
    }

    static regina::Simplex<dim>* face(Triangulation<dim>& tri, size_t index) {
        return tri.simplex(index);
    }

    static regina::Simplex<dim>* face(regina::Simplex<dim>* s, Perm<dim + 1>) {
        return s;
    }
};

/**
 * Used to perform barycentric subdivisions in those dimensions that
 * support them.
 */
template <int dim, bool supported = regina::standardDim(dim)>
struct BarycentricHelper;

template <int dim>
struct BarycentricHelper<dim, true> {
    static void subdivideAndSimplify(Triangulation<dim>& t) {
        t.barycentricSubdivision();
        t.intelligentSimplify();
    }
};


template <int dim>
struct BarycentricHelper<dim, false> {
    static void subdivideAndSimplify(Triangulation<dim>&) {
        // Silently do nothing, since this dimension does not support
        // barycentric subdivision.
    }
};

/**
 * Used to determine the type of a boundary component.
 */
template <int dim, bool supportsVertices = (dim == 3 || dim == 4)>
struct BoundaryTypeHelper;

template <int dim>
struct BoundaryTypeHelper<dim, true> {
    static bool isReal(regina::BoundaryComponent<dim>* bc) {
        return bc->isReal();
    }

    static bool isIdeal(regina::BoundaryComponent<dim>* bc) {
        return bc->isIdeal();
    }

    static bool isInvalidVertex(regina::BoundaryComponent<dim>* bc) {
        return bc->isInvalidVertex();
    }
};

template <int dim>
struct BoundaryTypeHelper<dim, false> {
    static bool isReal(regina::BoundaryComponent<dim>*) {
        return true;
    }

    static bool isIdeal(regina::BoundaryComponent<dim>*) {
        return false;
    }

    static bool isInvalidVertex(regina::BoundaryComponent<dim>*) {
        return false;
    }
};

/**
 * Used to verify that faces of a given triangulation are valid.
 * Specifically, this class checks all faces of dimensions 0,...,subdim.
 */
template <int dim, int subdim,
        bool testLinks = (regina::standardDim(dim) && dim > 2)>
struct ValidityHelper {
    static void verifyFacesValid(const Triangulation<dim>& tri,
            const char* name) {
        for (size_t i = 0; i < tri.template countFaces<subdim>(); ++i)
            if ((! tri.template face<subdim>(i)->isValid()) ||
                    tri.template face<subdim>(i)->hasBadLink() ||
                    tri.template face<subdim>(i)->hasBadIdentification()) {
                std::ostringstream msg;
                msg << subdim << "-face " << i << " of triangulation "
                    << name << " is reported as invalid.";
                CPPUNIT_FAIL(msg.str());
            }

        ValidityHelper<dim, subdim-1, testLinks>::verifyFacesValid(tri, name);
    }
};

template <int dim, int subdim>
struct ValidityHelper<dim, subdim, false> {
    static void verifyFacesValid(const Triangulation<dim>& tri,
            const char* name) {
        // In this specialisation, we cannot test Face<...>::hasBadLink().
        for (size_t i = 0; i < tri.template countFaces<subdim>(); ++i)
            if ((! tri.template face<subdim>(i)->isValid()) ||
                    tri.template face<subdim>(i)->hasBadIdentification()) {
                std::ostringstream msg;
                msg << subdim << "-face " << i << " of triangulation "
                    << name << " is reported as invalid.";
                CPPUNIT_FAIL(msg.str());
            }

        ValidityHelper<dim, subdim-1, false>::verifyFacesValid(tri, name);
    }
};

template <int dim>
struct ValidityHelper<dim, 0, true> {
    static void verifyFacesValid(const Triangulation<dim>& tri,
            const char* name) {
        // For vertices, we cannot test hasBadIdentification().
        for (size_t i = 0; i < tri.countVertices(); ++i)
            if ((! tri.vertex(i)->isValid()) ||
                    tri.vertex(i)->hasBadLink()) {
                std::ostringstream msg;
                msg << "Vertex " << i << " of triangulation "
                    << name << " is reported as invalid.";
                CPPUNIT_FAIL(msg.str());
            }
    }
};

template <int dim>
struct ValidityHelper<dim, 0, false> {
    static void verifyFacesValid(const Triangulation<dim>& tri,
            const char* name) {
        // In this specialisation we cannot test for either bad links or
        // bad self-identifications.  (Which means that all vertices are
        // valid... but we still test isValid() regardless.)
        for (size_t i = 0; i < tri.countVertices(); ++i)
            if ((! tri.vertex(i)->isValid())) {
                std::ostringstream msg;
                msg << "Vertex " << i << " of triangulation "
                    << name << " is reported as invalid.";
                CPPUNIT_FAIL(msg.str());
            }
    }
};

/**
 * Used to verify that a triangulated boundary component has its faces
 * labelled and ordered correctly.  Specifically, this class checks all
 * faces of dimensions 0,...,subdim.
 */
template <int dim, int subdim, bool storesFaces = regina::standardDim(dim)>
struct BoundaryHelper {
    static void verifyFaces(const regina::BoundaryComponent<dim>* bc,
            const Triangulation<dim-1>& built, const char* name) {
        BoundaryHelper<dim, subdim-1>::verifyFaces(bc, built, name);

        // The labelling and ordering of subdim-faces is only guaranteed if no
        // subdim-face is pinched.  Conversely, if some subdim-face *is*
        // pinched then that face will appear multiple times in the
        // triangulated boundary, and so such a labelling / ordering
        // will be impossible.

        bool hasPinched = false;
        if constexpr (subdim <= dim - 3) {
            for (auto f : bc->template faces<subdim>()) {
                const Triangulation<dim-1-subdim>& link = f->buildLink();
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
            // The triangulated boundary component should have strictly
            // more subdim-faces.
            if (bc->template countFaces<subdim>() >=
                    built.template countFaces<subdim>()) {
                std::ostringstream msg;
                msg << "Boundary component " << bc->index()
                    << " of triangulation " << name
                    << " does not give additional " << subdim << "-faces"
                    << " when triangulated, even though a face is pinched."
                    << std::endl;
                CPPUNIT_FAIL(msg.str());
            }

            // Since the labelling / ordering is impossible, there is
            // nothing more to check.
            return;
        }

        // There are no pinched faces; go ahead and verify the full
        // labelling / ordering.

        if (bc->template countFaces<subdim>() !=
                built.template countFaces<subdim>()) {
            std::ostringstream msg;
            msg << "Boundary component " << bc->index()
                << " of triangulation " << name
                << " gives the wrong number of " << subdim << "-faces"
                << " when triangulated."
                << std::endl;
            CPPUNIT_FAIL(msg.str());
        }

        size_t i, j;
        for (i = 0; i < bc->size(); ++i) {
            const regina::Simplex<dim-1>* innerSimp = built.simplex(i);
            regina::Face<dim, dim-1>* outerSimp = bc->template face<dim-1>(i);

            for (j = 0; j < regina::binomSmall(dim, subdim+1); ++j) {
                regina::Face<dim-1, subdim>* innerFace =
                    innerSimp->template face<subdim>(j);
                regina::Face<dim, subdim>* outerFace =
                    outerSimp->template face<subdim>(j);

                if (bc->template face<subdim>(innerFace->index()) !=
                        outerFace) {
                    std::ostringstream msg;
                    msg << "Boundary component " << bc->index()
                        << " of triangulation " << name
                        << " gives mismatched " << subdim << "-face indices"
                        << " when triangulated."
                        << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }

                Perm<dim> innerPerm = innerSimp->template
                    faceMapping<subdim>(j);
                Perm<dim+1> outerPerm = outerSimp->template
                    faceMapping<subdim>(j);
                if (innerPerm.trunc(subdim+1) != outerPerm.trunc(subdim+1)) {
                    std::ostringstream msg;
                    msg << "Boundary component " << bc->index()
                        << " of triangulation " << name
                        << " gives mismatched " << subdim << "-face labelling"
                        << " when triangulated."
                        << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }
    }
};

template <int dim, bool storesFaces>
struct BoundaryHelper<dim, -1, storesFaces> {
    static void verifyFaces(const regina::BoundaryComponent<dim>*,
            const Triangulation<dim-1>&, const char*) {
    }
};

template <int dim, int subdim>
struct BoundaryHelper<dim, subdim, false> {
    static void verifyFaces(const regina::BoundaryComponent<dim>*,
            const Triangulation<dim-1>&, const char*) {
    }
};

/**
 * Used to verify that the orientable double cover of a non-orientable
 * triangulation contains the correct number of faces of dimensions
 * 0,...,subdim.
 *
 * \pre This helper class must only be used with valid triangulations.
 */
template <int dim, int subdim,
        bool allowsIdealVertices = (dim == 3 || dim == 4)>
struct DoubleCoverHelper {
    static void verifyFaceCounts(const Triangulation<dim>& orig,
            const Triangulation<dim>& cover, const char* name) {
        if (cover.template countFaces<subdim>() !=
                2 * orig.template countFaces<subdim>()) {
            std::ostringstream msg;
            msg << name << ": Orientable double cover does not "
                "contain precisely twice as many " << subdim << "-faces.";
            CPPUNIT_FAIL(msg.str());
        }

        DoubleCoverHelper<dim, subdim-1>::verifyFaceCounts(orig, cover, name);
    }
};

template <int dim>
struct DoubleCoverHelper<dim, 0, false> {
    static void verifyFaceCounts(const Triangulation<dim>& orig,
            const Triangulation<dim>& cover, const char* name) {
        if (cover.countVertices() != 2 * orig.countVertices()) {
            std::ostringstream msg;
            msg << name << ": Orientable double cover does not "
                "contain precisely twice as many vertices.";
            CPPUNIT_FAIL(msg.str());
        }
    }
};

template <int dim>
struct DoubleCoverHelper<dim, 0, true> {
    static void verifyFaceCounts(const Triangulation<dim>& orig,
            const Triangulation<dim>& cover, const char* name) {
        // Only count vertices for non-ideal triangulations.
        if (! orig.isIdeal())
            DoubleCoverHelper<dim, 0, false>::verifyFaceCounts(
                orig, cover, name);
    }
};

/**
 * Performs additional tests related to Pachner moves that are only
 * supported in some dimensions.
 */
template <int dim, int codim>
struct PachnerHelperMoveLegal {
    static bool legal(regina::Face<dim, dim - codim>* f) {
        // This function should never be called, but we do need it
        // to compile.
        return true;
    }
};

template <int dim>
struct PachnerHelperMoveLegal<dim, 1> {
    static bool legal(regina::Face<dim, dim - 1>* f) {
        // For codimension 1 faces, the legality conditions are easy to test.
        return ! (f->isBoundary() ||
            f->embedding(0).simplex() ==
            f->embedding(1).simplex());
    }
};

template <int dim, bool supportsClosed = regina::standardDim(dim)>
struct PachnerHelperClosed {
    static_assert(! regina::standardDim(dim),
        "The no-op PachnerHelperClosed should not be used for "
        "standard dimensions.");
    static bool testClosed(const Triangulation<dim>& /* orig */,
            const Triangulation<dim>& /* altered */) {
        return true;
    };
};

template <int dim>
struct PachnerHelperClosed<dim, true> {
    static bool testClosed(const Triangulation<dim>& orig,
            const Triangulation<dim>& altered) {
        return (altered.isClosed() == orig.isClosed());
    };
};

template <int dim, bool supportsH2 = (dim == 3 || dim == 4)>
struct PachnerHelperH2 {
    static_assert(dim < 3 || dim > 4,
        "The no-op PachnerHelperH2 should not be used for dimensions 3 or 4.");
    static bool testH2(const Triangulation<dim>& /* orig */,
            const Triangulation<dim>& /* altered */) {
        return true;
    };
};

template <int dim>
struct PachnerHelperH2<dim, true> {
    static bool testH2(const Triangulation<dim>& orig,
            const Triangulation<dim>& altered) {
        return (altered.homologyH2() == orig.homologyH2());
    };
};

template <int dim, bool supportsCollapseEdge = (dim == 3 || dim == 4)>
struct PachnerHelperCollapseEdge {
    static_assert(dim < 3 || dim > 4,
        "The no-op PachnerHelperCollapseEdge should not be used for "
        "dimensions 3 or 4.");
    static void verifyCollapseEdge(const Triangulation<dim>& /* orig */,
            const Triangulation<dim>& /* altered */,
            const Isomorphism<dim>& /* iso */, int /* simplex */,
            const char* /* name */) {
    }
};

template <int dim>
struct PachnerHelperCollapseEdge<dim, true> {
    static void verifyCollapseEdge(const Triangulation<dim>& orig,
            const Triangulation<dim>& altered,
            const Isomorphism<dim>& iso, int simplex, const char* name) {
        regina::Triangulation<dim> copy(altered);

        bool res = copy.collapseEdge(
            copy.simplex(iso.simpImage(orig.size() + dim - 1))->edge(
                regina::Edge<dim>::edgeNumber
                    [iso.facetPerm(orig.size() + dim - 1)[0]]
                    [iso.facetPerm(orig.size() + dim - 1)[dim]]),
            true, true);
        clearProperties(copy);

        if (! res) {
            std::ostringstream msg;
            msg << name << ", simplex " << simplex << ": "
                << "1-5 move: could not recollapse edge.";
            CPPUNIT_FAIL(msg.str());
        }

        if (! copy.isIsomorphicTo(orig)) {
            std::ostringstream msg;
            msg << name << ", simplex " << simplex << ": "
                << "1-5 move: recollapse is not isomorphic.";
            CPPUNIT_FAIL(msg.str());
        }
    }
};

/**
 * Inherited by the test classes for all dimensions.
 */
template <int dim>
class TriangulationTest : public CppUnit::TestFixture {
    protected:
        // Trivial:
        Triangulation<dim> empty;
            /**< The empty triangulation. */

        // Closed orientable:
        Triangulation<dim> sphere;
            /**< The dim-sphere, with two simplices whose boundaries
                 are identified according to the identity map. */
        Triangulation<dim> simplicialSphere;
            /**< The dim-sphere, with (dim + 2) simplices glued together
                 to form the boundary of a (dim + 1)-simplex. */
        Triangulation<dim> sphereBundle;
            /**< The product S^(dim-1) x S^1. */

        // Closed non-orientable:
        Triangulation<dim> twistedSphereBundle;
            /**< The twisted product S^(dim-1) x~ S^1. */

        // Real boundary:
        Triangulation<dim> ball;
            /**< A single dim-simplex, with no gluings. */
        Triangulation<dim> ballBundle;
            /**< The product B^(dim-1) x S^1. */
        Triangulation<dim> twistedBallBundle;
            /**< The twisted product B^(dim-1) x~ S^1. */

    public:
        void setUp() {
            // The empty triangulation needs no initialisation whatsoever.

            // Some examples are ready-made via Example<dim>.
            sphere = regina::Example<dim>::sphere();
            simplicialSphere = regina::Example<dim>::simplicialSphere();
            sphereBundle = regina::Example<dim>::sphereBundle();
            twistedSphereBundle = regina::Example<dim>::twistedSphereBundle();

            ball = regina::Example<dim>::ball();
            ballBundle = regina::Example<dim>::ballBundle();
            twistedBallBundle = regina::Example<dim>::twistedBallBundle();
        }

        static bool looksIdentical(const Triangulation<dim>& a,
                const Triangulation<dim>& b) {
            if (a.size() != b.size())
                return false;
            if (a.countComponents() != b.countComponents())
                return false;
            if (! a.isIdenticalTo(b))
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
            Vertex<dim>* v0 = (t.isEmpty() ? nullptr : t.vertex(0));

            Triangulation<dim> copy(t);
            if (! looksIdentical(copy, t)) {
                std::ostringstream msg;
                msg << name << ": copy constructed not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }

            Vertex<dim>* v1 = (copy.isEmpty() ? nullptr : copy.vertex(0));
            if (v1 == v0 && ! t.isEmpty()) {
                std::ostringstream msg;
                msg << name << ": copy constructed uses the same vertices.";
                CPPUNIT_FAIL(msg.str());
            }

            Triangulation<dim> move(std::move(copy));
            if (! looksIdentical(move, t)) {
                std::ostringstream msg;
                msg << name << ": move constructed not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }

            Vertex<dim>* v2 = (move.isEmpty() ? nullptr : move.vertex(0));
            if (v2 != v1) {
                std::ostringstream msg;
                msg << name << ": move constructed does not use the "
                    "same vertices.";
                CPPUNIT_FAIL(msg.str());
            }

            Triangulation<dim> copyAss;
            copyAss.newSimplex(); // Give it something to overwrite.
            copyAss = t;
            if (! looksIdentical(copyAss, t)) {
                std::ostringstream msg;
                msg << name << ": copy assigned not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }

            Vertex<dim>* v3 = (copyAss.isEmpty() ? nullptr : copyAss.vertex(0));
            if (v3 == v0 && ! t.isEmpty()) {
                std::ostringstream msg;
                msg << name << ": copy assigned uses the same vertices.";
                CPPUNIT_FAIL(msg.str());
            }

            Triangulation<dim> moveAss;
            moveAss.newSimplex(); // Give it something to overwrite.
            moveAss = std::move(copyAss);
            if (! looksIdentical(moveAss, t)) {
                std::ostringstream msg;
                msg << name << ": move assigned not identical to original.";
                CPPUNIT_FAIL(msg.str());
            }

            Vertex<dim>* v4 = (moveAss.isEmpty() ? nullptr : moveAss.vertex(0));
            if (v4 != v3) {
                std::ostringstream msg;
                msg << name << ": move assigned does not use the "
                    "same vertices.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void verifyValid(const Triangulation<dim>& tri,
                bool isValid, const char* name) {
            if (! isValid) {
                if (tri.isValid()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name << " is reported as valid.";
                    CPPUNIT_FAIL(msg.str());
                }
                return;
            }

            if (! tri.isValid()) {
                std::ostringstream msg;
                msg << "Triangulation " << name << " is reported as invalid.";
                CPPUNIT_FAIL(msg.str());
            }

            ValidityHelper<dim, dim-2>::verifyFacesValid(tri, name);
        }

        static void verifyOrientable(const Triangulation<dim>& tri,
                bool isOrientable, const char* name) {
            if (isOrientable) {
                if (! tri.isOrientable()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name
                        << " is reported as non-orientable.";
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                if (tri.isOrientable()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name
                        << " is reported as orientable.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        static void verifyConnected(const Triangulation<dim>& tri,
                bool isConnected, const char* name) {
            if (isConnected) {
                if (! tri.isConnected()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name
                        << " is reported as disconnected.";
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                if (tri.isConnected()) {
                    std::ostringstream msg;
                    msg << "Triangulation " << name
                        << " is reported as connected.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        static void verifyEulerCharTri(const Triangulation<dim>& tri,
                long expectedTri, const char* name) {
            long eulerTri = tri.eulerCharTri();

            if (eulerTri != expectedTri) {
                std::ostringstream msg;
                msg << "Triangulation " << name << " gives "
                    "triangulation Euler characteristic = " << eulerTri
                    << " instead of the expected " << expectedTri << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void verifyOrient(const Triangulation<dim>& original,
                const Triangulation<dim>& oriented, const char* name) {
            if (original.isOrientable() != oriented.isOrientable()) {
                std::ostringstream msg;
                msg << "Oriented version of " << name
                    << " has different orientability.";
                CPPUNIT_FAIL(msg.str());
            }
            if (original.isOrientable() && ! oriented.isOriented()) {
                std::ostringstream msg;
                msg << "Oriented version of " << name
                    << " is not seen to be oriented.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! oriented.isIsomorphicTo(original)) {
                std::ostringstream msg;
                msg << "Oriented version of " << name
                    << " is not isomorphic to the original.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void verifyOrient(const Triangulation<dim>& tri,
                const char* name) {
            const int trials = 10;

            {
                Triangulation<dim> oriented(tri, false);
                oriented.orient();
                clearProperties(oriented);
                verifyOrient(tri, oriented, name);
            }

            for (int i = 0; i < trials; ++i) {
                Triangulation<dim> t =
                    Isomorphism<dim>::random(tri.size()).apply(tri);

                t.orient();
                clearProperties(t);
                verifyOrient(tri, t, name);
            }
        }

        static void verifyMakeCanonical(const Triangulation<dim>& tri,
                const char* name) {
            // Currently makeCanonical() insists on connected
            // triangulations only.
            if (! tri.isConnected())
                return;

            const int trials = 10;

            Triangulation<dim> canonical(tri);
            canonical.makeCanonical();
            clearProperties(canonical);

            for (int i = 0; i < trials; ++i) {
                Triangulation<dim> t =
                    Isomorphism<dim>::random(tri.size()).apply(tri);

                t.makeCanonical();
                clearProperties(t);

                if (! t.isIsomorphicTo(tri)) {
                    std::ostringstream msg;
                    msg << "Canonical form for "
                        << name << " is non-isomorphic.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (t.detail() != canonical.detail()) {
                    std::ostringstream msg;
                    msg << "Canonical form for "
                        << name << " is inconsistent.";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        static void verifyIsomorphismSignature(const Triangulation<dim>& tri,
                const char* name) {
            std::string sig = tri.isoSig();

            if (sig.empty()) {
                std::ostringstream msg;
                msg << name << ": Cannot create isomorphism signature.";
                CPPUNIT_FAIL(msg.str());
            }

            size_t sigSize = Triangulation<dim>::isoSigComponentSize(sig);
            if (tri.isEmpty()) {
                if (sigSize != 0) {
                    std::ostringstream msg;
                    msg << name << ": isoSigSize() returns incorrect value: "
                        << sigSize << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                size_t c;
                for (c = 0; c < tri.countComponents(); ++c)
                    if (sigSize == tri.component(c)->size())
                        break;
                if (c == tri.countComponents()) {
                    std::ostringstream msg;
                    msg << name << ": isoSigSize() returns incorrect value: "
                        << sigSize << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }

            try {
                if (! Triangulation<dim>::fromIsoSig(sig).isIsomorphicTo(tri)) {
                    std::ostringstream msg;
                    msg << name << ": Reconstruction from \"" << sig
                        << "\" is not isomorphic to the original.";
                    CPPUNIT_FAIL(msg.str());
                }
            } catch (const regina::InvalidArgument&) {
                std::ostringstream msg;
                msg << name << ": Cannot reconstruct from isomorphism "
                    "signature \"" << sig << "\".";
                CPPUNIT_FAIL(msg.str());
            }

            // Does rebuilding still work if the signature has whitespace?
            try {
                std::string modSig = std::string("\t " + sig + "\t \n");
                if (! Triangulation<dim>::fromIsoSig(modSig).
                        isIsomorphicTo(tri)) {
                    std::ostringstream msg;
                    msg << name << ": Reconstruction from \"" << sig
                        << "\" with whitespace is not isomorphic to "
                        "the original.";
                    CPPUNIT_FAIL(msg.str());
                }
            } catch (const regina::InvalidArgument&) {
                std::ostringstream msg;
                msg << name << ": Cannot reconstruct from isomorphism "
                    "signature \"" << sig << "\" with whitespace.";
                CPPUNIT_FAIL(msg.str());
            }

            if (tri.isEmpty())
                return;

            std::string otherSig;
            for (unsigned i = 0; i < 10; ++i) {
                Triangulation<dim> other =
                    Isomorphism<dim>::random(tri.size()).apply(tri);

                otherSig = other.isoSig();
                if (otherSig != sig) {
                    std::ostringstream msg;
                    msg << name << ": Random isomorphism gives different "
                        "signature: " << otherSig << " != " << sig << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }
            }
            for (unsigned i = 0; i < 10; ++i) {
                Triangulation<dim> other(tri);
                Isomorphism<dim>::random(tri.size()).applyInPlace(other);

                otherSig = other.isoSig();
                if (otherSig != sig) {
                    std::ostringstream msg;
                    msg << name << ": Random in-place isomorphism gives "
                        "different signature: "
                        << otherSig << " != " << sig << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (tri.countComponents() == 1) {
                auto detail = tri.isoSigDetail();

                if (detail.first != sig) {
                    std::ostringstream msg;
                    msg << name << ": isoSig() and isoSigDetail() give "
                        "different signatures: "
                        << sig << " != " << detail.first << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }

                Triangulation<dim> rebuild =
                    Triangulation<dim>::fromIsoSig(detail.first);
                Triangulation<dim> relabel = detail.second.apply(tri);

                if (relabel.detail() != rebuild.detail()) {
                    std::ostringstream msg;
                    msg << name << ": relabelling returned from "
                        "isoSigDetail() does not recover fromIsoSig(\""
                        << sig << "\")." << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        static void verifyHomology(const Triangulation<dim>& tri,
                const char* H1, const char* name) {
            std::string ans = tri.homology().str();
            if (ans != H1) {
                std::ostringstream msg;
                msg << "Triangulation " << name
                    << " has homology H1 = " << ans
                    << " instead of the expected " << H1 << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void verifyFundGroup(const Triangulation<dim>& tri,
                const char* group, const char* name) {
            std::string ans = tri.fundamentalGroup().recogniseGroup();
            if (ans != group) {
                std::string showAns = ans;
                if (showAns.empty())
                    showAns = "<unrecognised>";

                std::string showGroup = group;
                if (showGroup.empty())
                    showGroup = "<unrecognised>";

                std::ostringstream msg;
                msg << "Triangulation " << name
                    << " has fundamental group = " << showAns
                    << " instead of the expected " << showGroup << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void verifyDoubleCover(const Triangulation<dim>& tri,
                const char* name) {
            // PRE: tri is either empty or connected.
            if (! tri.isConnected())
                return;

            Triangulation<dim> cover(tri, false);
            cover.makeDoubleCover();

            if (tri.isEmpty()) {
                if (! cover.isEmpty())
                    CPPUNIT_FAIL("Empty triangulation: "
                        "Double cover is non-empty.");
                return;
            }

            // We have a non-empty connected triangulation.
            if (tri.isOrientable()) {
                // We should simply come away with two identical copies of tri.
                auto components = cover.triangulateComponents();
                if (components.size() != 2) {
                    std::ostringstream msg;
                    msg << name << ": Orientable double cover does not "
                        "contain precisely two components.";
                    CPPUNIT_FAIL(msg.str());
                }

                for (const Triangulation<dim>& c : components) {
                    if (! tri.isIsomorphicTo(c)) {
                        std::ostringstream msg;
                        msg << name << ": Orientable double cover "
                            "contains a component not isomorphic to the "
                            "original.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            } else {
                // We should come away with a proper connected double cover.
                if (cover.countComponents() != 1) {
                    std::ostringstream msg;
                    msg << name << ": Orientable double cover does not "
                        "contain precisely one component.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! cover.isOrientable()) {
                    std::ostringstream msg;
                    msg << name
                        << ": Orientable double cover is not orientable.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (cover.size() != 2 * tri.size()) {
                    std::ostringstream msg;
                    msg << name << ": Orientable double cover does not "
                        "contain precisely twice as many simplices.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (cover.template countFaces<dim-1>() !=
                        2 * tri.template countFaces<dim-1>()) {
                    std::ostringstream msg;
                    msg << name << ": Orientable double cover does not "
                        "contain precisely twice as many "
                        << (dim-1) << "-faces.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri.isValid())
                    DoubleCoverHelper<dim, dim-2>::verifyFaceCounts(
                        tri, cover, name);

                /**
                 * Commenting out this claim about homology groups,
                 * which is nonsense.
                // We expect the first homology group to be identical,
                // or to be missing a copy of Z_2.
                if (tri.isValid() && (tri.homology() != cover.homology())) {
                    regina::AbelianGroup hCover(cover.homology());
                    hCover.addTorsion(2);
                    if (tri.homology() != hCover) {
                        std::ostringstream msg;
                        msg << name << ": Orientable double cover has H1 = "
                            << cover.homology().str()
                            << ", which does not match the original H1 = "
                            << tri.homology().str() << '.';
                        CPPUNIT_FAIL(msg.str());
                    }
                }
                */
            }
        }

        static void verifyBoundaryFacets(const Triangulation<dim>& tri,
                const char* name) {
            unsigned long found = 0;

            unsigned long i, j;
            for (i = 0; i < tri.size(); ++i)
                for (j = 0; j <= dim; ++j)
                    if (! tri.simplex(i)->adjacentSimplex(j))
                        ++found;

            if (found != tri.countBoundaryFacets()) {
                std::ostringstream msg;
                msg << name << " reports the wrong number of boundary facets.";
                CPPUNIT_FAIL(msg.str());
            }

            unsigned long c;
            regina::Component<dim>* comp;
            for (c = 0; c < tri.countComponents(); ++c) {
                comp = tri.component(c);
                found = 0;

                for (i = 0; i < comp->size(); ++i)
                    for (j = 0; j <= dim; ++j)
                        if (! comp->simplex(i)->adjacentSimplex(j))
                            ++found;

                if (found != comp->countBoundaryFacets()) {
                    std::ostringstream msg;
                    msg << name << " reports the wrong number of "
                        "boundary facets in component " << c << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        static void verifyBoundaryCount(const Triangulation<dim>& tri,
                size_t nReal, size_t nIdeal, size_t nInvalid,
                const char* name) {
            size_t ans = tri.countBoundaryComponents();
            if (ans != nReal + nIdeal + nInvalid) {
                std::ostringstream msg;
                msg << "Triangulation " << name << " gives "
                    << ans << " boundary component(s) instead of the expected "
                    << nReal + nIdeal + nInvalid << "." << std::endl;
                CPPUNIT_FAIL(msg.str());
            }

            size_t foundReal = 0, foundIdeal = 0, foundInvalid = 0;
            for (auto bc : tri.boundaryComponents())
                if (BoundaryTypeHelper<dim>::isIdeal(bc))
                    ++foundIdeal;
                else if (BoundaryTypeHelper<dim>::isInvalidVertex(bc))
                    ++foundInvalid;
                else
                    ++foundReal;

            if (foundReal != nReal) {
                std::ostringstream msg;
                msg << "Triangulation " << name << " gives " << ans
                    << " real boundary component(s) instead of the expected "
                    << nReal << "." << std::endl;
                CPPUNIT_FAIL(msg.str());
            }
            if (foundIdeal != nIdeal) {
                std::ostringstream msg;
                msg << "Triangulation " << name << " gives " << ans
                    << " ideal boundary component(s) instead of the expected "
                    << nIdeal << "." << std::endl;
                CPPUNIT_FAIL(msg.str());
            }
            if (foundInvalid != nInvalid) {
                std::ostringstream msg;
                msg << "Triangulation " << name << " gives " << ans
                    << " invalid vertex boundary component(s) instead of "
                    "the expected " << nInvalid << "." << std::endl;
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void verifyBoundaryBuild(const Triangulation<dim>& tri,
                const char* name) {
            for (auto bc : tri.boundaryComponents())
                if (BoundaryTypeHelper<dim>::isReal(bc)) {
                    // We have a real boundary component.
                    const Triangulation<dim-1>& built = bc->build();

                    if (bc->size() != built.size()) {
                        std::ostringstream msg;
                        msg << "Boundary component " << bc->index()
                            << " of triangulation " << name
                            << " gives the wrong number of top-dimensional "
                            "simplices when triangulated." << std::endl;
                        CPPUNIT_FAIL(msg.str());
                    }

                    // Check that [built] and [bc] have the same numbers
                    // of faces of each lower dimension, and that these faces
                    // appear to be ordered and labelled correctly.

                    BoundaryHelper<dim, dim-2>::verifyFaces(bc, built, name);
                }
        }

        static void verifyBoundaryH1(const Triangulation<dim>& tri,
                size_t whichBdry, const char* h1, const char* name) {
            // Calling homology() does not truncate ideal boundaries
            // at the centroids of invalid (dim-3)-faces that are
            // self-identified under a non-trivial map.
            //
            // This problem only appears in dimension dim >= 4.
            // Unfortunately, to fix it we need to do a barycentric
            // subdivision, which is currently only available in
            // dimension dim <= 5 (i.e., where the boundary triangulation
            // has dimension <= 4).
            //
            // So: for the time being, we perform this subdivision for
            // the cases dim = 4,5 only.
            Triangulation<dim-1> t = tri.boundaryComponent(whichBdry)->build();
            BarycentricHelper<dim-1>::subdivideAndSimplify(t);

            std::string ans = t.homology().str();

            if (ans != h1) {
                std::ostringstream msg;
                msg << "Boundary component " << whichBdry
                    << " of triangulation " << name
                    << " has first homology " << ans
                    << " instead of the expected " << h1 << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        void edgeAccess() {
            for (int i = 0; i <= dim; ++i)
                for (int j = 0; j <= dim; ++j) {
                    if (i == j)
                        continue;
                    // Build a permutation that maps (0,1) -> (i,j).
                    Perm<dim+1> p;
                    if (i == 0 && j == 0)
                        p = Perm<dim+1>();
                    else if (i == 0)
                        p = Perm<dim+1>(1, j);
                    else if (j == 0)
                        p = Perm<dim+1>(1, i);
                    else
                        p = Perm<dim+1>(0, i) * Perm<dim+1>(1,j);
                    if (ball.simplex(0)->edge(i, j) !=
                            ball.simplex(0)->edge(
                            regina::Edge<dim>::faceNumber(p))) {
                        std::ostringstream msg;
                        msg << "Simplex<dim>::edge(" << i << ", " << j
                            << ") returns the wrong edge.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
        }

        template <int k>
        static void verifyPachnerDetail(const Triangulation<dim>& tri,
                bool standardSimplex, const char* name) {
            // Tests Pachner moves on k-faces, and their inverses.
            for (size_t i = 0; i < FaceHelper<dim, k>::count(tri); ++i) {
                Triangulation<dim> large(tri);
                if (large.isOrientable())
                    large.orient();

                // Test whether *we* think the move should be allowed.
                // This is only computed for the case k == dim - 1;
                // for all other k it simply returns true.
                bool expected = PachnerHelperMoveLegal<dim, dim - k>::
                    legal(FaceHelper<dim, k>::face(large, i));

                // Perform the move (if we can).
                bool res = large.pachner(FaceHelper<dim, k>::face(large, i));
                clearProperties(large);

                if (standardSimplex || k == dim) {
                    // The move should always be allowed.
                    if (! res) {
                        std::ostringstream msg;
                        msg << name << ", face " << i << ": "
                            << (dim + 1 - k) << '-' << (k + 1) << " move "
                            "not allowed.";
                        CPPUNIT_FAIL(msg.str());
                    }
                } else if (k == dim - 1) {
                    // It is easy to tell when the move should be allowed,
                    // and we just tested this ourselves above.
                    if (res && ! expected) {
                        std::ostringstream msg;
                        msg << name << ", face " << i << ": "
                            << (dim + 1 - k) << '-' << (k + 1) << " move "
                            "was incorrectly allowed.";
                        CPPUNIT_FAIL(msg.str());
                    } else if (expected && ! res) {
                        std::ostringstream msg;
                        msg << name << ", face " << i << ": "
                            << (dim + 1 - k) << '-' << (k + 1) << " move "
                            "was incorrectly disallowed.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                if (! res) {
                    if (large.size() != tri.size()) {
                        std::ostringstream msg;
                        msg << name << ", face " << i << ": "
                            "disallowed "
                            << (dim + 1 - k) << '-' << (k + 1) << " move "
                            "changed # simplices.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    Triangulation<dim> oriented(tri);
                    if (tri.isOrientable())
                        oriented.orient();
                    if (! large.isIdenticalTo(oriented)) {
                        std::ostringstream msg;
                        msg << name << ", face " << i << ": "
                            "disallowed "
                            << (dim + 1 - k) << '-' << (k + 1) << " move "
                            "is not identical.";
                        CPPUNIT_FAIL(msg.str());
                    }
                    continue;
                }

                // The move was performed.

                if (large.size() != tri.size() + 2 * k - dim) {
                    std::ostringstream msg;
                    msg << name << ", face " << i << ": "
                        << (dim + 1 - k) << '-' << (k + 1) << " move "
                        "gives wrong triangulation size.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.isValid() != tri.isValid()) {
                    std::ostringstream msg;
                    msg << name << ", face " << i << ": "
                        << (dim + 1 - k) << '-' << (k + 1) << " move "
                        "changes validity.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.isOrientable() != tri.isOrientable()) {
                    std::ostringstream msg;
                    msg << name << ", face " << i << ": "
                        << (dim + 1 - k) << '-' << (k + 1) << " move "
                        "changes orientability.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri.isOrientable() && ! large.isOriented()) {
                    std::ostringstream msg;
                    msg << name << ", face " << i << ": "
                        << (dim + 1 - k) << '-' << (k + 1) << " move "
                        "loses orientation.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! PachnerHelperClosed<dim>::testClosed(tri, large)) {
                    std::ostringstream msg;
                    msg << name << ", face " << i << ": "
                        << (dim + 1 - k) << '-' << (k + 1) << " move "
                        "loses closedness.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.countBoundaryComponents() !=
                        tri.countBoundaryComponents()) {
                    std::ostringstream msg;
                    msg << name << ", face " << i << ": "
                        << (dim + 1 - k) << '-' << (k + 1) << " move "
                        "changes # boundary components.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (large.eulerCharTri() != tri.eulerCharTri()) {
                    std::ostringstream msg;
                    msg << name << ", face " << i << ": "
                        << (dim + 1 - k) << '-' << (k + 1) << " move "
                        "changes Euler characteristic.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri.isValid()) {
                    if (! (large.homologyH1() == tri.homologyH1())) {
                        std::ostringstream msg;
                        msg << name << ", face " << i << ": "
                            << (dim + 1 - k) << '-' << (k + 1) << " move "
                            "changes H1.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    if (! PachnerHelperH2<dim>::testH2(tri, large)) {
                        std::ostringstream msg;
                        msg << name << ", face " << i << ": "
                            << (dim + 1 - k) << '-' << (k + 1) << " move "
                            "changes H2.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }

                if (dim != 2 && large.isIsomorphicTo(tri)) {
                    std::ostringstream msg;
                    msg << name << ", face " << i << ": "
                        << (dim + 1 - k) << '-' << (k + 1) << " move "
                        "result is isomorphic.";
                    CPPUNIT_FAIL(msg.str());
                }

                // Randomly relabel the simplices, but preserve orientation.
                Isomorphism<dim> iso = Isomorphism<dim>::random(large.size(),
                    true);
                iso.applyInPlace(large);
                clearProperties(large);

                if (k == dim) {
                    // Shrink by edge collapse.
                    PachnerHelperCollapseEdge<dim>::verifyCollapseEdge(
                        tri, large, iso, i, name);
                }

                // Shrink by the inverse Pachner move.
                {
                    regina::Triangulation<dim> copy(large);

                    res = copy.pachner(
                        FaceHelper<dim, dim - k>::face(
                            copy.simplex(iso.simpImage(large.size() - 1)),
                            iso.facetPerm(large.size() - 1)),
                        true, true);
                    clearProperties(copy);

                    if (! res) {
                        std::ostringstream msg;
                        msg << name << ", face " << i << ": "
                            << "could not undo "
                            << (dim + 1 - k) << '-' << (k + 1) << " move "
                            "with inverse move.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    if (! copy.isIsomorphicTo(tri)) {
                        std::ostringstream msg;
                        msg << name << ", face " << i << ": "
                            << (dim + 1 - k) << '-' << (k + 1) << " move "
                            << "followed by inverse move "
                            "is not isomorphic.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    if (tri.isOrientable() && ! copy.isOriented()) {
                        std::ostringstream msg;
                        msg << name << ", face " << i << ": "
                            << (dim + 1 - k) << '-' << (k + 1) << " move "
                            << "followed by inverse move "
                            "loses orientation.";
                        CPPUNIT_FAIL(msg.str());
                    }
                }
            }
        }

        template <int k>
        static void verifyPachner(const Triangulation<dim>& tri,
                const char* name) {
            verifyPachnerDetail<k>(tri, false, name);
        }

        template <int k>
        void verifyPachnerSimplicial() {
            verifyPachnerDetail<k>(simplicialSphere, true, "Simplicial sphere");
        }
};

