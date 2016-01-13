
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

#include <list>
#include <thread>
#include "dim4/dim4triangulation.h"
#include "enumerate/ndoubledescription.h"
#include "enumerate/nhilbertdual.h"
#include "enumerate/nhilbertprimal.h"
#include "hypersurface/nnormalhypersurfacelist.h"
#include "hypersurface/hscoordregistry.h"
#include "maths/nmatrixint.h"
#include "progress/nprogresstracker.h"
#include "utilities/xmlutils.h"

namespace regina {

namespace {
    struct ZeroVector : public Returns<NNormalHypersurfaceVector*> {
        template <typename Coords>
        inline NNormalHypersurfaceVector* operator() (
                const Dim4Triangulation* tri) {
            return Coords::Class::makeZeroVector(tri);
        }
    };
}

NNormalHypersurfaceVector* makeZeroVector(
        const Dim4Triangulation* triangulation, HyperCoords coords) {
    return forCoords(coords, ZeroVector(), 0, triangulation);
}

namespace {
    struct MatchingEquations : public Returns<NMatrixInt*> {
        template <typename Coords>
        inline NMatrixInt* operator() (const Dim4Triangulation* tri) {
            return Coords::Class::makeMatchingEquations(tri);
        }
    };
}

NMatrixInt* makeMatchingEquations(const Dim4Triangulation* triangulation,
        HyperCoords coords) {
    return forCoords(coords, MatchingEquations(), 0, triangulation);
}

namespace {
    struct EmbeddedConstraints : public Returns<NEnumConstraintList*> {
        template <typename Coords>
        inline NEnumConstraintList* operator() (const Dim4Triangulation* tri) {
            return Coords::Class::makeEmbeddedConstraints(tri);
        }
    };
}

NEnumConstraintList* makeEmbeddedConstraints(
        const Dim4Triangulation* triangulation, HyperCoords coords) {
    return forCoords(coords, EmbeddedConstraints(), 0, triangulation);
}

template <typename Coords>
void NNormalHypersurfaceList::VertexEnumerator::operator() (
        NNormalHypersurfaceList* list, Dim4Triangulation* triang,
        NProgressTracker* tracker) {
    if (tracker)
        tracker->newStage("Enumerating vertex hypersurfaces");

    // Fetch any necessary validity constraints.
    NEnumConstraintList* constraints = 0;
    if (list->embedded_)
        constraints = makeEmbeddedConstraints(triang, list->coords_);

    // Form the matching equations and starting cone.
    NMatrixInt* eqns = makeMatchingEquations(triang, list->coords_);

    // Find the normal hypersurfaces.
    NDoubleDescription::enumerateExtremalRays<typename Coords::Class>(
        HypersurfaceInserter(*list, triang), *eqns, constraints, tracker);

    delete eqns;
    delete constraints;

    // All done!
    if (! (tracker && tracker->isCancelled()))
        triang->insertChildLast(list);

    if (tracker)
        tracker->setFinished();
}

template <typename Coords>
void NNormalHypersurfaceList::FundPrimalEnumerator::operator() (
        NNormalHypersurfaceList* list, Dim4Triangulation* triang,
        NNormalHypersurfaceList* vtxSurfaces, NProgressTracker* tracker) {
    if (tracker)
        tracker->newStage("Initialising Hilbert basis enumeration", 0.1);

    // Fetch any necessary validity constraints.
    NEnumConstraintList* constraints = 0;
    if (list->embedded_)
        constraints = makeEmbeddedConstraints(triang, list->coords_);

    if (tracker)
        tracker->newStage("Enumerating extremal rays", 0.4);

    NNormalHypersurfaceList* useVtxSurfaces = vtxSurfaces;
    if (! vtxSurfaces) {
        // Enumerate all vertex normal hypersurfaces using the default
        // (and hopefully best possible) algorithm.
        useVtxSurfaces = new NNormalHypersurfaceList(list->coords_,
            list->embedded_);
        VertexEnumerator().operator()<Coords>(useVtxSurfaces, triang, 0);
    }

    if (tracker)
        tracker->newStage("Expanding to Hilbert basis", 0.5);

    // Find the normal hypersurfaces.
    NHilbertPrimal::enumerateHilbertBasis<typename Coords::Class>(
        HypersurfaceInserter(*list, triang),
        useVtxSurfaces->beginVectors(), useVtxSurfaces->endVectors(),
        constraints, tracker);

    delete constraints;
    if (! vtxSurfaces)
        delete useVtxSurfaces;

    // All done!
    if (! (tracker && tracker->isCancelled()))
        triang->insertChildLast(list);

    if (tracker)
        tracker->setFinished();
}

template <typename Coords>
void NNormalHypersurfaceList::FundDualEnumerator::operator() (
        NNormalHypersurfaceList* list, Dim4Triangulation* triang,
        NProgressTracker* tracker) {
    if (tracker)
        tracker->newStage("Enumerating Hilbert basis\n(dual method)");

    // Fetch any necessary validity constraints.
    NEnumConstraintList* constraints = 0;
    if (list->embedded_)
        constraints = makeEmbeddedConstraints(triang, list->coords_);

    // Form the matching equations and starting cone.
    NMatrixInt* eqns = makeMatchingEquations(triang, list->coords_);

    // Find the normal hypersurfaces.
    NHilbertDual::enumerateHilbertBasis<typename Coords::Class>(
        HypersurfaceInserter(*list, triang), *eqns, constraints, tracker);

    delete eqns;
    delete constraints;

    // All done!
    if (! (tracker && tracker->isCancelled()))
        triang->insertChildLast(list);

    if (tracker)
        tracker->setFinished();
}

NNormalHypersurfaceList* NNormalHypersurfaceList::enumerate(
        Dim4Triangulation* owner, HyperCoords coords, bool embeddedOnly,
        NProgressTracker* tracker) {
    NNormalHypersurfaceList* ans = new NNormalHypersurfaceList(
        coords, embeddedOnly);

    if (tracker)
        std::thread(&forCoords<VertexEnumerator, NNormalHypersurfaceList*,
                Dim4Triangulation*, NProgressTracker*>,
            coords, VertexEnumerator(), ans, owner, tracker)
            .detach();
    else
        forCoords(coords, VertexEnumerator(), ans, owner, tracker);
    return ans;
}

NNormalHypersurfaceList* NNormalHypersurfaceList::enumerateFundPrimal(
        Dim4Triangulation* owner, HyperCoords coords, bool embeddedOnly,
        NNormalHypersurfaceList* vtxSurfaces, NProgressTracker* tracker) {
    NNormalHypersurfaceList* ans = new NNormalHypersurfaceList(
        coords, embeddedOnly);

    if (tracker)
        std::thread(forCoords<FundPrimalEnumerator, NNormalHypersurfaceList*,
                Dim4Triangulation*, NNormalHypersurfaceList*,
                NProgressTracker*>,
            coords, FundPrimalEnumerator(), ans, owner, vtxSurfaces, tracker)
            .detach();
    else
        forCoords(coords, FundPrimalEnumerator(), ans, owner, vtxSurfaces,
            tracker);
    return ans;
}

NNormalHypersurfaceList* NNormalHypersurfaceList::enumerateFundDual(
        Dim4Triangulation* owner, HyperCoords coords, bool embeddedOnly,
        NProgressTracker* tracker) {
    NNormalHypersurfaceList* ans = new NNormalHypersurfaceList(
        coords, embeddedOnly);

    if (tracker)
        std::thread(forCoords<FundDualEnumerator, NNormalHypersurfaceList*,
                Dim4Triangulation*, NProgressTracker*>,
            coords, FundDualEnumerator(), ans, owner, tracker)
            .detach();
    else
        forCoords(coords, FundDualEnumerator(), ans, owner, tracker);
    return ans;
}

Dim4Triangulation* NNormalHypersurfaceList::triangulation() const {
    return dynamic_cast<Dim4Triangulation*>(parent());
}

Dim4Triangulation* NNormalHypersurfaceList::getTriangulation() const {
    return dynamic_cast<Dim4Triangulation*>(parent());
}

namespace {
    struct NameFunction : public Returns<const char*> {
        template <typename Coords>
        inline const char* operator() () { return Coords::name(); }
    };
}

void NNormalHypersurfaceList::writeTextShort(std::ostream& o) const {
    o << surfaces_.size() << " vertex normal hypersurface";
    if (surfaces_.size() != 1)
        o << 's';
    o << " (" << forCoords(coords_, NameFunction(), "Unknown") << ')';
}

void NNormalHypersurfaceList::writeTextLong(std::ostream& o) const {
    if (embedded_)
        o << "Embedded ";
    else
        o << "Embedded, immersed & singular ";
    o << "vertex normal hypersurfaces\n";
    o << "Coordinates: " << forCoords(coords_, NameFunction(), "Unknown")
        << '\n';

    unsigned long n = size();
    o << "Number of hypersurfaces is " << n << '\n';
    for (unsigned long i = 0; i < n; i++) {
        getHypersurface(i)->writeTextShort(o);
        o << '\n';
    }
}

void NNormalHypersurfaceList::writeXMLPacketData(std::ostream& out) const {
    // Write the surface list parameters.
    out << "  <params embedded=\"" << (embedded_ ? 'T' : 'F')
        << "\" flavourid=\"" << coords_ << "\"\n";
    out << "\tflavour=\""
        << regina::xml::xmlEncodeSpecialChars(forCoords(
           coords_, NameFunction(), "Unknown")) << "\"/>\n";

    // Write the individual hypersurfaces.
    std::vector<NNormalHypersurface*>::const_iterator it;
    for (it = surfaces_.begin(); it != surfaces_.end(); it++)
        (*it)->writeXMLData(out);
}

NPacket* NNormalHypersurfaceList::internalClonePacket(NPacket* /* parent */)
        const {
    NNormalHypersurfaceList* ans = new NNormalHypersurfaceList();
    ans->coords_ = coords_;
    ans->embedded_ = embedded_;
    transform(surfaces_.begin(), surfaces_.end(), back_inserter(ans->surfaces_),
        FuncNewClonePtr<NNormalHypersurface>());
    return ans;
}

} // namespace regina

