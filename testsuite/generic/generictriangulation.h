
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include <cppunit/extensions/HelperMacros.h>

using regina::Isomorphism;
using regina::Triangulation;

template <int dim>
class TriangulationTest : public CppUnit::TestFixture {
    public:
        static void verifyMakeCanonical(Triangulation<dim>* tri) {
            // Currently makeCanonical() insists on connected
            // triangulations only.
            if (! tri->isConnected())
                return;

            const int trials = 10;

            Triangulation<dim> canonical(*tri);
            canonical.makeCanonical();

            for (int i = 0; i < trials; ++i) {
                Isomorphism<dim>* iso = Isomorphism<dim>::random(
                    tri->getNumberOfSimplices());
                Triangulation<dim>* t = iso->apply(tri);
                delete iso;

                t->makeCanonical();

                if (! t->isIsomorphicTo(*tri).get()) {
                    std::ostringstream msg;
                    msg << "Canonical form for "
                        << tri->getPacketLabel() << " is non-isomorphic.";
                    CPPUNIT_FAIL(msg.str());
                }
                if (t->detail() != canonical.detail()) {
                    std::ostringstream msg;
                    msg << "Canonical form for "
                        << tri->getPacketLabel() << " is inconsistent.";
                    CPPUNIT_FAIL(msg.str());
                }

                delete t;
            }
        }

        static void verifyIsomorphismSignature(Triangulation<dim>* tri) {
            std::string sig = tri->isoSig();

            if (sig.empty()) {
                std::ostringstream msg;
                msg << tri->getPacketLabel()
                    << ": Cannot create isomorphism signature.";
                CPPUNIT_FAIL(msg.str());
            }

            size_t sigSize = Triangulation<dim>::isoSigComponentSize(sig);
            if (tri->getNumberOfSimplices() == 0) {
                if (sigSize != 0) {
                    std::ostringstream msg;
                    msg << tri->getPacketLabel()
                        << ": isoSigSize() returns incorrect value: "
                        << sigSize << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            } else {
                size_t c;
                for (c = 0; c < tri->getNumberOfComponents(); ++c)
                    if (sigSize == tri->getComponent(c)->getNumberOfSimplices())
                        break;
                if (c == tri->getNumberOfComponents()) {
                    std::ostringstream msg;
                    msg << tri->getPacketLabel()
                        << ": isoSigSize() returns incorrect value: "
                        << sigSize << '.';
                    CPPUNIT_FAIL(msg.str());
                }
            }

            Triangulation<dim>* rebuild = Triangulation<dim>::fromIsoSig(sig);
            if (! rebuild) {
                std::ostringstream msg;
                msg << tri->getPacketLabel()
                    << ": Cannot reconstruct from isomorphism "
                    "signature \"" << sig << "\".";
                CPPUNIT_FAIL(msg.str());
            }
            if (! rebuild->isIsomorphicTo(*tri).get()) {
                std::ostringstream msg;
                msg << tri->getPacketLabel()
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
                msg << tri->getPacketLabel()
                    << ": Cannot reconstruct from isomorphism "
                    "signature \"" << sig << "\" with whitespace.";
                CPPUNIT_FAIL(msg.str());
            }
            if (! rebuild->isIsomorphicTo(*tri).get()) {
                std::ostringstream msg;
                msg << tri->getPacketLabel()
                    << ": Reconstruction from \"" << sig
                    << "\" with whitespace is not isomorphic to the original.";
                CPPUNIT_FAIL(msg.str());
            }
            delete rebuild;

            if (tri->getNumberOfSimplices() == 0)
                return;

            std::string otherSig;
            for (unsigned i = 0; i < 10; ++i) {
                Isomorphism<dim>* iso = Isomorphism<dim>::random(
                    tri->getNumberOfSimplices());
                Triangulation<dim>* other = iso->apply(tri);
                delete iso;

                otherSig = other->isoSig();
                if (otherSig != sig) {
                    std::ostringstream msg;
                    msg << tri->getPacketLabel()
                        << ": Random isomorphism gives different "
                        "signature: " << otherSig << " != " << sig << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }

                delete other;
            }
            for (unsigned i = 0; i < 10; ++i) {
                Isomorphism<dim>* iso = Isomorphism<dim>::random(
                    tri->getNumberOfSimplices());
                Triangulation<dim> other(*tri);
                iso->applyInPlace(&other);
                delete iso;

                otherSig = other.isoSig();
                if (otherSig != sig) {
                    std::ostringstream msg;
                    msg << tri->getPacketLabel()
                        << ": Random in-place isomorphism gives "
                        "different signature: "
                        << otherSig << " != " << sig << std::endl;
                    CPPUNIT_FAIL(msg.str());
                }
            }

            if (tri->getNumberOfComponents() == 1) {
                Isomorphism<dim>* relabelling;
                tri->isoSig(&relabelling);

                Triangulation<dim>* rebuild =
                    Triangulation<dim>::fromIsoSig(sig);
                Triangulation<dim>* relabel = relabelling->apply(tri);

                if (relabel->detail() != rebuild->detail()) {
                    std::ostringstream msg;
                    msg << tri->getPacketLabel()
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
};

