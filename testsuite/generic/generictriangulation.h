
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

#include "algebra/nabeliangroup.h"
#include "algebra/ngrouppresentation.h"
#include "packet/container.h"
#include <cppunit/extensions/HelperMacros.h>

using regina::Isomorphism;
using regina::Triangulation;

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
                    regina::NAbelianGroup hCover(cover.homology());
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
};

