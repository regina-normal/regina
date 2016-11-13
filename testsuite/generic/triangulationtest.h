
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

#include "algebra/abeliangroup.h"
#include "algebra/grouppresentation.h"
#include "packet/container.h"
#include <cppunit/extensions/HelperMacros.h>

using regina::Isomorphism;
using regina::Perm;
using regina::Triangulation;

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
    static void verifyFacesValid(const Triangulation<dim>& tri) {
        for (size_t i = 0; i < tri.template countFaces<subdim>(); ++i)
            if ((! tri.template face<subdim>(i)->isValid()) ||
                    tri.template face<subdim>(i)->hasBadLink() ||
                    tri.template face<subdim>(i)->hasBadIdentification()) {
                std::ostringstream msg;
                msg << subdim << "-face " << i << " of triangulation "
                    << tri.label() << " is reported as invalid.";
                CPPUNIT_FAIL(msg.str());
            }

        ValidityHelper<dim, subdim-1, testLinks>::verifyFacesValid(tri);
    }
};

template <int dim, int subdim>
struct ValidityHelper<dim, subdim, false> {
    static void verifyFacesValid(const Triangulation<dim>& tri) {
        // In this specialisation, we cannot test Face<...>::hasBadLink().
        for (size_t i = 0; i < tri.template countFaces<subdim>(); ++i)
            if ((! tri.template face<subdim>(i)->isValid()) ||
                    tri.template face<subdim>(i)->hasBadIdentification()) {
                std::ostringstream msg;
                msg << subdim << "-face " << i << " of triangulation "
                    << tri.label() << " is reported as invalid.";
                CPPUNIT_FAIL(msg.str());
            }

        ValidityHelper<dim, subdim-1, false>::verifyFacesValid(tri);
    }
};

template <int dim>
struct ValidityHelper<dim, 0, true> {
    static void verifyFacesValid(const Triangulation<dim>& tri) {
        // For vertices, we cannot test hasBadIdentification().
        for (size_t i = 0; i < tri.countVertices(); ++i)
            if ((! tri.vertex(i)->isValid()) ||
                    tri.vertex(i)->hasBadLink()) {
                std::ostringstream msg;
                msg << "Vertex " << i << " of triangulation "
                    << tri.label() << " is reported as invalid.";
                CPPUNIT_FAIL(msg.str());
            }
    }
};

template <int dim>
struct ValidityHelper<dim, 0, false> {
    static void verifyFacesValid(const Triangulation<dim>& tri) {
        // In this specialisation we cannot test for either bad links or
        // bad self-identifications.  (Which means that all vertices are
        // valid... but we still test isValid() regardless.)
        for (size_t i = 0; i < tri.countVertices(); ++i)
            if ((! tri.vertex(i)->isValid())) {
                std::ostringstream msg;
                msg << "Vertex " << i << " of triangulation "
                    << tri.label() << " is reported as invalid.";
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
            const Triangulation<dim-1>* built) {
        BoundaryHelper<dim, subdim-1>::verifyFaces(bc, built);

        if (bc->template countFaces<subdim>() !=
                built->template countFaces<subdim>()) {
            std::ostringstream msg;
            msg << "Boundary component " << bc->index()
                << " of triangulation " << bc->triangulation()->label()
                << " gives the wrong number of " << subdim << "-faces"
                << " when triangulated."
                << std::endl;
            CPPUNIT_FAIL(msg.str());
        }

        size_t i, j;
        for (i = 0; i < bc->size(); ++i) {
            const regina::Simplex<dim-1>* innerSimp = built->simplex(i);
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
                        << " of triangulation " << bc->triangulation()->label()
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
                        << " of triangulation " << bc->triangulation()->label()
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
            const Triangulation<dim-1>*) {
    }
};

template <int dim, int subdim>
struct BoundaryHelper<dim, subdim, false> {
    static void verifyFaces(const regina::BoundaryComponent<dim>*,
            const Triangulation<dim-1>*) {
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
            const Triangulation<dim>& cover) {
        if (cover.template countFaces<subdim>() !=
                2 * orig.template countFaces<subdim>()) {
            std::ostringstream msg;
            msg << orig.label()
                << ": Orientable double cover does not "
                "contain precisely twice as many "
                << subdim << "-faces.";
            CPPUNIT_FAIL(msg.str());
        }

        DoubleCoverHelper<dim, subdim-1>::verifyFaceCounts(orig, cover);
    }
};

template <int dim>
struct DoubleCoverHelper<dim, 0, false> {
    static void verifyFaceCounts(const Triangulation<dim>& orig,
            const Triangulation<dim>& cover) {
        if (cover.countVertices() != 2 * orig.countVertices()) {
            std::ostringstream msg;
            msg << orig.label()
                << ": Orientable double cover does not "
                "contain precisely twice as many vertices.";
            CPPUNIT_FAIL(msg.str());
        }
    }
};

template <int dim>
struct DoubleCoverHelper<dim, 0, true> {
    static void verifyFaceCounts(const Triangulation<dim>& orig,
            const Triangulation<dim>& cover) {
        // Only count vertices for non-ideal triangulations.
        if (! orig.isIdeal())
            DoubleCoverHelper<dim, 0, false>::verifyFaceCounts(orig, cover);
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
            /**< The dim-sphere, with two simplices whose boundaries
                 are identified according to the identity map. */
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
        void copyAndDelete(Triangulation<dim>& dest,
                Triangulation<dim>* source, const char* name = 0) {
            dest.insertTriangulation(*source);
            if (name)
                dest.setLabel(name);
            else
                dest.setLabel(source->label());
            delete source;
        }

        void setUp() {
            // The empty triangulation needs no initialisation whatsoever.
            empty.setLabel("Empty triangulation");

            // Some examples are ready-made via Example<dim>.
            copyAndDelete(sphere, regina::Example<dim>::sphere());
            copyAndDelete(simplicialSphere,
                regina::Example<dim>::simplicialSphere());
            copyAndDelete(sphereBundle, regina::Example<dim>::sphereBundle());
            copyAndDelete(twistedSphereBundle,
                regina::Example<dim>::twistedSphereBundle());

            copyAndDelete(ball, regina::Example<dim>::ball());
            copyAndDelete(ballBundle, regina::Example<dim>::ballBundle());
            copyAndDelete(twistedBallBundle,
                regina::Example<dim>::twistedBallBundle());
        }

        static void verifyValid(const Triangulation<dim>& tri,
                bool isValid = true) {
            if (! isValid) {
                if (tri.isValid()) {
                    CPPUNIT_FAIL("Triangulation " + tri.label() +
                        " is reported as valid.");
                }
                return;
            }

            if (! tri.isValid()) {
                CPPUNIT_FAIL("Triangulation " + tri.label() +
                    " is reported as invalid.");
            }

            ValidityHelper<dim, dim-2>::verifyFacesValid(tri);
        }

        static void verifyOrientable(const Triangulation<dim>& tri,
                bool isOrientable = true) {
            if (isOrientable) {
                if (! tri.isOrientable())
                    CPPUNIT_FAIL("Triangulation " + tri.label() +
                        " is reported as non-orientable.");
            } else {
                if (tri.isOrientable())
                    CPPUNIT_FAIL("Triangulation " + tri.label() +
                        " is reported as orientable.");
            }
        }

        static void verifyConnected(const Triangulation<dim>& tri,
                bool isConnected = true) {
            if (isConnected) {
                if (! tri.isConnected())
                    CPPUNIT_FAIL("Triangulation " + tri.label() +
                        " is reported as disconnected.");
            } else {
                if (tri.isConnected())
                    CPPUNIT_FAIL("Triangulation " + tri.label() +
                        " is reported as connected.");
            }
        }

        static void verifyEulerCharTri(const Triangulation<dim>& tri,
                long expectedTri) {
            long eulerTri = tri.eulerCharTri();

            if (eulerTri != expectedTri) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.label() << " gives "
                    "triangulation Euler characteristic = " << eulerTri
                    << " instead of the expected " << expectedTri << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void verifyOrient(const Triangulation<dim>* original,
                const Triangulation<dim>* oriented) {
            if (original->isOrientable() != oriented->isOrientable()) {
                std::ostringstream msg;
                msg << "Oriented version of " << original->label()
                    << " has different orientability.";
                CPPUNIT_FAIL(msg.str());
            }
            if (original->isOrientable() && ! oriented->isOriented()) {
                std::ostringstream msg;
                msg << "Oriented version of " << original->label()
                    << " is not seen to be oriented.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! oriented->isIsomorphicTo(*original).get()) {
                std::ostringstream msg;
                msg << "Oriented version of " << original->label()
                    << " is not isomorphic to the original.";
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void verifyOrient(Triangulation<dim>* tri) {
            const int trials = 10;

            Triangulation<dim>* oriented = new Triangulation<dim>(*tri, false);
            oriented->orient();
            verifyOrient(tri, oriented);
            delete oriented;

            for (int i = 0; i < trials; ++i) {
                Isomorphism<dim>* iso = Isomorphism<dim>::random(tri->size());
                Triangulation<dim>* t = iso->apply(tri);
                delete iso;

                t->orient();
                verifyOrient(tri, t);
                delete t;
            }
        }

        static void verifyMakeCanonical(Triangulation<dim>* tri) {
            // Currently makeCanonical() insists on connected
            // triangulations only.
            if (! tri->isConnected())
                return;

            const int trials = 10;

            Triangulation<dim> canonical(*tri);
            canonical.makeCanonical();

            for (int i = 0; i < trials; ++i) {
                Isomorphism<dim>* iso = Isomorphism<dim>::random(tri->size());
                Triangulation<dim>* t = iso->apply(tri);
                delete iso;

                t->makeCanonical();

                if (! t->isIsomorphicTo(*tri).get()) {
                    std::ostringstream msg;
                    msg << "Canonical form for "
                        << tri->label() << " is non-isomorphic.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (t->detail() != canonical.detail()) {
                    std::ostringstream msg;
                    msg << "Canonical form for "
                        << tri->label() << " is inconsistent.";
                    CPPUNIT_FAIL(msg.str());
                }

                delete t;
            }
        }

        static void verifyIsomorphismSignature(Triangulation<dim>* tri) {
            std::string sig = tri->isoSig();

            if (sig.empty()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Cannot create isomorphism signature.";
                CPPUNIT_FAIL(msg.str());
            }

            size_t sigSize = Triangulation<dim>::isoSigComponentSize(sig);
            if (tri->isEmpty()) {
                if (sigSize != 0) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": isoSigSize() returns incorrect value: "
                        << sigSize << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                size_t c;
                for (c = 0; c < tri->countComponents(); ++c)
                    if (sigSize == tri->component(c)->size())
                        break;
                if (c == tri->countComponents()) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": isoSigSize() returns incorrect value: "
                        << sigSize << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }

            Triangulation<dim>* rebuild = Triangulation<dim>::fromIsoSig(sig);
            if (! rebuild) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Cannot reconstruct from isomorphism "
                    "signature \"" << sig << "\".";
                CPPUNIT_FAIL(msg.str());
            }
            if (! rebuild->isIsomorphicTo(*tri).get()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Reconstruction from \"" << sig
                    << "\" is not isomorphic to the original.";
                CPPUNIT_FAIL(msg.str());
            }
            delete rebuild;

            // Does rebuilding still work if the signature has whitespace?
            rebuild = Triangulation<dim>::fromIsoSig(
                std::string("\t " + sig + "\t \n"));
            if (! rebuild) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Cannot reconstruct from isomorphism "
                    "signature \"" << sig << "\" with whitespace.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! rebuild->isIsomorphicTo(*tri).get()) {
                std::ostringstream msg;
                msg << tri->label()
                    << ": Reconstruction from \"" << sig
                    << "\" with whitespace is not isomorphic to the original.";
                CPPUNIT_FAIL(msg.str());
            }
            delete rebuild;

            if (tri->isEmpty())
                return;

            std::string otherSig;
            for (unsigned i = 0; i < 10; ++i) {
                Isomorphism<dim>* iso = Isomorphism<dim>::random(tri->size());
                Triangulation<dim>* other = iso->apply(tri);
                delete iso;

                otherSig = other->isoSig();
                if (otherSig != sig) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": Random isomorphism gives different "
                        "signature: " << otherSig << " != " << sig << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }

                delete other;
            }
            for (unsigned i = 0; i < 10; ++i) {
                Isomorphism<dim>* iso = Isomorphism<dim>::random(tri->size());
                Triangulation<dim> other(*tri);
                iso->applyInPlace(&other);
                delete iso;

                otherSig = other.isoSig();
                if (otherSig != sig) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": Random in-place isomorphism gives "
                        "different signature: "
                        << otherSig << " != " << sig << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (tri->countComponents() == 1) {
                Isomorphism<dim>* relabelling;
                tri->isoSig(&relabelling);

                Triangulation<dim>* rebuild =
                    Triangulation<dim>::fromIsoSig(sig);
                Triangulation<dim>* relabel = relabelling->apply(tri);

                if (relabel->detail() != rebuild->detail()) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": relabelling returned from "
                        "isoSig() does not recover fromIsoSig(\""
                        << sig << "\")." << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }

                delete relabelling;
                delete rebuild;
                delete relabel;
            }
        }

        static void verifyHomology(const Triangulation<dim>& tri,
                const char* H1) {
            std::string ans = tri.homology().str();
            if (ans != H1) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.label()
                    << " has homology H1 = " << ans
                    << " instead of the expected " << H1 << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void verifyFundGroup(const Triangulation<dim>& tri,
                const char* group) {
            std::string ans = tri.fundamentalGroup().recogniseGroup();
            if (ans != group) {
                std::string showAns = ans;
                if (showAns.empty())
                    showAns = "<unrecognised>";

                std::string showGroup = group;
                if (showGroup.empty())
                    showGroup = "<unrecognised>";

                std::ostringstream msg;
                msg << "Triangulation " << tri.label()
                    << " has fundamental group = " << showAns
                    << " instead of the expected " << showGroup << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void verifyDoubleCover(Triangulation<dim>* tri) {
            // PRE: tri is either empty or connected.
            if (! tri->isConnected())
                return;

            Triangulation<dim> cover(*tri, false);
            cover.makeDoubleCover();

            if (tri->isEmpty()) {
                if (! cover.isEmpty())
                    CPPUNIT_FAIL("Empty triangulation: "
                        "Double cover is non-empty.");
                return;
            }

            // We have a non-empty connected triangulation.
            if (tri->isOrientable()) {
                // We should simply come away with two identical copies of tri.
                regina::Container parent;
                if (cover.splitIntoComponents(&parent) != 2) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": Orientable double cover does not "
                        "contain precisely two components.";
                    CPPUNIT_FAIL(msg.str());
                }

                Triangulation<dim>* child = static_cast<Triangulation<dim>*>(
                    parent.firstChild());
                while (child) {
                    if (! tri->isIsomorphicTo(*child).get()) {
                        std::ostringstream msg;
                        msg << tri->label()
                            << ": Orientable double cover "
                            "contains a component not isomorphic to the "
                            "original.";
                        CPPUNIT_FAIL(msg.str());
                    }

                    child = static_cast<Triangulation<dim>*>(
                        child->nextSibling());
                }
            } else {
                // We should come away with a proper connected double cover.
                if (cover.countComponents() != 1) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": Orientable double cover does not "
                        "contain precisely one component.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (! cover.isOrientable()) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": Orientable double cover is not orientable.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (cover.size() != 2 * tri->size()) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": Orientable double cover does not "
                        "contain precisely twice as many simplices.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (cover.template countFaces<dim-1>() !=
                        2 * tri->template countFaces<dim-1>()) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << ": Orientable double cover does not "
                        "contain precisely twice as many "
                        << (dim-1) << "-faces.";
                    CPPUNIT_FAIL(msg.str());
                }

                if (tri->isValid())
                    DoubleCoverHelper<dim, dim-2>::verifyFaceCounts(
                        *tri, cover);

                /**
                 * Commenting out this claim about homology groups,
                 * which is nonsense.
                // We expect the first homology group to be identical,
                // or to be missing a copy of Z_2.
                if (tri->isValid() && (tri->homology() != cover.homology())) {
                    regina::AbelianGroup hCover(cover.homology());
                    hCover.addTorsionElement(2);
                    if (tri->homology() != hCover) {
                        std::ostringstream msg;
                        msg << tri->label()
                            << ": Orientable double cover has H1 = "
                            << cover.homology().str()
                            << ", which does not match the original H1 = "
                            << tri->homology().str() << '.';
                        CPPUNIT_FAIL(msg.str());
                    }
                }
                */
            }
        }

        static void verifyBoundaryFacets(Triangulation<dim>* tri) {
            unsigned long found = 0;

            unsigned long i, j;
            for (i = 0; i < tri->size(); ++i)
                for (j = 0; j <= dim; ++j)
                    if (! tri->simplex(i)->adjacentSimplex(j))
                        ++found;

            if (found != tri->countBoundaryFacets()) {
                std::ostringstream msg;
                msg << tri->label()
                    << " reports the wrong number of boundary facets.";
                CPPUNIT_FAIL(msg.str());
            }

            unsigned long c;
            regina::Component<dim>* comp;
            for (c = 0; c < tri->countComponents(); ++c) {
                comp = tri->component(c);
                found = 0;

                for (i = 0; i < comp->size(); ++i)
                    for (j = 0; j <= dim; ++j)
                        if (! comp->simplex(i)->adjacentSimplex(j))
                            ++found;

                if (found != comp->countBoundaryFacets()) {
                    std::ostringstream msg;
                    msg << tri->label()
                        << " reports the wrong number of "
                        "boundary facets in component " << c << ".";
                    CPPUNIT_FAIL(msg.str());
                }
            }
        }

        static void verifyBoundaryCount(const Triangulation<dim>& tri,
                size_t nReal, size_t nIdeal = 0, size_t nInvalid = 0) {
            size_t ans = tri.countBoundaryComponents();
            if (ans != nReal + nIdeal + nInvalid) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.label() << " gives "
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
                msg << "Triangulation " << tri.label() << " gives " << ans
                    << " real boundary component(s) instead of the expected "
                    << nReal << "." << std::endl;
                CPPUNIT_FAIL(msg.str());
            }
            if (foundIdeal != nIdeal) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.label() << " gives " << ans
                    << " ideal boundary component(s) instead of the expected "
                    << nIdeal << "." << std::endl;
                CPPUNIT_FAIL(msg.str());
            }
            if (foundInvalid != nInvalid) {
                std::ostringstream msg;
                msg << "Triangulation " << tri.label() << " gives " << ans
                    << " invalid vertex boundary component(s) instead of "
                    "the expected " << nInvalid << "." << std::endl;
                CPPUNIT_FAIL(msg.str());
            }
        }

        static void verifyBoundaryBuild(Triangulation<dim>* tri) {
            for (auto bc : tri->boundaryComponents())
                if (BoundaryTypeHelper<dim>::isReal(bc)) {
                    // We have a real boundary component.
                    const Triangulation<dim-1>* built = bc->build();

                    if (bc->size() != built->size()) {
                        std::ostringstream msg;
                        msg << "Boundary component " << bc->index()
                            << " of triangulation " << tri->label()
                            << " gives the wrong number of top-dimensional "
                            "simplices when triangulated." << std::endl;
                        CPPUNIT_FAIL(msg.str());
                    }

                    // Check that [built] and [bc] have the same numbers
                    // of faces of each lower dimension, and that these faces
                    // appear to be ordered and labelled correctly.

                    BoundaryHelper<dim, dim-2>::verifyFaces(bc, built);
                }
        }

        static void verifyBoundaryH1(const Triangulation<dim>& tri,
                size_t whichBdry, const char* h1) {
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
            Triangulation<dim-1> t(
                *(tri.boundaryComponent(whichBdry)->build()));
            BarycentricHelper<dim-1>::subdivideAndSimplify(t);

            std::string ans = t.homology().str();

            if (ans != h1) {
                std::ostringstream msg;
                msg << "Boundary component " << whichBdry
                    << " of triangulation " << tri.label()
                    << " has first homology " << ans
                    << " instead of the expected " << h1 << ".";
                CPPUNIT_FAIL(msg.str());
            }
        }
};

