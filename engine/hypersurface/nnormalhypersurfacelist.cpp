
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
        inline NNormalHypersurfaceVector* operator() (Coords,
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
        inline NMatrixInt* operator() (Coords,
                const Dim4Triangulation* tri) {
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
        inline NEnumConstraintList* operator() (Coords,
                const Dim4Triangulation* tri) {
            return Coords::Class::makeEmbeddedConstraints(tri);
        }
    };
}

NEnumConstraintList* makeEmbeddedConstraints(
        const Dim4Triangulation* triangulation, HyperCoords coords) {
    return forCoords(coords, EmbeddedConstraints(), 0, triangulation);
}

void* NNormalHypersurfaceList::VertexEnumerator::run(void*) {
    forCoords(list_->coords_, *this);
    return 0;
}

template <typename Coords>
void NNormalHypersurfaceList::VertexEnumerator::operator() (Coords) {
    if (tracker_)
        tracker_->newStage("Enumerating vertex hypersurfaces");

    // Fetch any necessary validity constraints.
    NEnumConstraintList* constraints = 0;
    if (list_->embedded_)
        constraints = makeEmbeddedConstraints(triang_, list_->coords_);

    // Form the matching equations and starting cone.
    NMatrixInt* eqns = makeMatchingEquations(triang_, list_->coords_);

    // Find the normal hypersurfaces.
    NDoubleDescription::enumerateExtremalRays<typename Coords::Class>(
        HypersurfaceInserter(*list_, triang_), *eqns, constraints, tracker_);

    delete eqns;
    delete constraints;

    // All done!
    if (! (tracker_ && tracker_->isCancelled()))
        triang_->insertChildLast(list_);

    if (tracker_)
        tracker_->setFinished();
}

void* NNormalHypersurfaceList::FundPrimalEnumerator::run(void*) {
    forCoords(list_->coords_, *this);
    return 0;
}

template <typename Coords>
void NNormalHypersurfaceList::FundPrimalEnumerator::operator() (Coords) {
    if (tracker_)
        tracker_->newStage("Initialising Hilbert basis enumeration", 0.1);

    // Fetch any necessary validity constraints.
    NEnumConstraintList* constraints = 0;
    if (list_->embedded_)
        constraints = makeEmbeddedConstraints(triang_, list_->coords_);

    if (tracker_)
        tracker_->newStage("Enumerating extremal rays", 0.4);

    NNormalHypersurfaceList* useVtxSurfaces = vtxSurfaces_;
    if (! vtxSurfaces_) {
        // Enumerate all vertex normal hypersurfaces using the default
        // (and hopefully best possible) algorithm.
        useVtxSurfaces = new NNormalHypersurfaceList(list_->coords_,
            list_->embedded_);
        VertexEnumerator e(useVtxSurfaces, triang_, 0);
        e.run(0);
    }

    if (tracker_)
        tracker_->newStage("Expanding to Hilbert basis", 0.5);

    // Find the normal hypersurfaces.
    NHilbertPrimal::enumerateHilbertBasis<typename Coords::Class>(
        HypersurfaceInserter(*list_, triang_),
        useVtxSurfaces->beginVectors(), useVtxSurfaces->endVectors(),
        constraints, tracker_);

    delete constraints;
    if (! vtxSurfaces_)
        delete useVtxSurfaces;

    // All done!
    if (! (tracker_ && tracker_->isCancelled()))
        triang_->insertChildLast(list_);

    if (tracker_)
        tracker_->setFinished();
}

void* NNormalHypersurfaceList::FundDualEnumerator::run(void*) {
    forCoords(list_->coords_, *this);
    return 0;
}

template <typename Coords>
void NNormalHypersurfaceList::FundDualEnumerator::operator() (Coords) {
    if (tracker_)
        tracker_->newStage("Enumerating Hilbert basis\n(dual method)");

    // Fetch any necessary validity constraints.
    NEnumConstraintList* constraints = 0;
    if (list_->embedded_)
        constraints = makeEmbeddedConstraints(triang_, list_->coords_);

    // Form the matching equations and starting cone.
    NMatrixInt* eqns = makeMatchingEquations(triang_, list_->coords_);

    // Find the normal hypersurfaces.
    NHilbertDual::enumerateHilbertBasis<typename Coords::Class>(
        HypersurfaceInserter(*list_, triang_), *eqns, constraints, tracker_);

    delete eqns;
    delete constraints;

    // All done!
    if (! (tracker_ && tracker_->isCancelled()))
        triang_->insertChildLast(list_);

    if (tracker_)
        tracker_->setFinished();
}

NNormalHypersurfaceList* NNormalHypersurfaceList::enumerate(
        Dim4Triangulation* owner, HyperCoords coords, bool embeddedOnly,
        NProgressTracker* tracker) {
    NNormalHypersurfaceList* ans = new NNormalHypersurfaceList(
        coords, embeddedOnly);
    VertexEnumerator* e = new VertexEnumerator(ans, owner, tracker);

    if (tracker) {
        if (! e->start(0, true)) {
            delete ans;
            return 0;
        }
        return ans;
    } else {
        e->run(0);
        delete e;
        return ans;
    }
}

NNormalHypersurfaceList* NNormalHypersurfaceList::enumerateFundPrimal(
        Dim4Triangulation* owner, HyperCoords coords, bool embeddedOnly,
        NNormalHypersurfaceList* vtxSurfaces, NProgressTracker* tracker) {
    NNormalHypersurfaceList* ans = new NNormalHypersurfaceList(
        coords, embeddedOnly);
    FundPrimalEnumerator* e = new FundPrimalEnumerator(ans, owner, vtxSurfaces,
        tracker);

    if (tracker) {
        if (! e->start(0, true)) {
            delete ans;
            return 0;
        }
        return ans;
    } else {
        e->run(0);
        delete e;
        return ans;
    }
}

NNormalHypersurfaceList* NNormalHypersurfaceList::enumerateFundDual(
        Dim4Triangulation* owner, HyperCoords coords, bool embeddedOnly,
        NProgressTracker* tracker) {
    NNormalHypersurfaceList* ans = new NNormalHypersurfaceList(
        coords, embeddedOnly);
    FundDualEnumerator* e = new FundDualEnumerator(ans, owner, tracker);

    if (tracker) {
        if (! e->start(0, true)) {
            delete ans;
            return 0;
        }
        return ans;
    } else {
        e->run(0);
        delete e;
        return ans;
    }
}

Dim4Triangulation* NNormalHypersurfaceList::getTriangulation() const {
    return dynamic_cast<Dim4Triangulation*>(getTreeParent());
}

namespace {
    struct NameFunction : public Returns<const char*> {
        template <typename Coords>
        inline const char* operator() (Coords f) { return f.name(); }
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

