
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include <thread>
#include "enumerate/doubledescription.h"
#include "enumerate/hilbertdual.h"
#include "enumerate/hilbertprimal.h"
#include "hypersurface/normalhypersurfaces.h"
#include "hypersurface/hscoordregistry.h"
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "triangulation/dim4.h"
#include "utilities/xmlutils.h"

namespace regina {

NormalHypersurfaces* NormalHypersurfaces::enumerate(
        Triangulation<4>* owner, HyperCoords coords,
        HyperList which, HyperAlg algHints,
        ProgressTracker* tracker) {
    NormalHypersurfaces* list = new NormalHypersurfaces(
        coords, which, algHints);

    if (tracker)
        std::thread(forCoords<Enumerator>,
            coords, Enumerator(list, owner, tracker)).detach();
    else
        forCoords(coords, Enumerator(list, owner, tracker));
    return list;
}

template <typename Coords>
void NormalHypersurfaces::Enumerator::operator() () {
    // Clean up the "type of list" flag.
    list_->which_ &= (
        HS_EMBEDDED_ONLY | HS_IMMERSED_SINGULAR | HS_VERTEX | HS_FUNDAMENTAL);

    list_->which_.ensureOne(HS_VERTEX, HS_FUNDAMENTAL);
    list_->which_.ensureOne(HS_EMBEDDED_ONLY, HS_IMMERSED_SINGULAR);

    // Farm out the real work to list-type-specific routines.
    if (list_->which_.has(HS_VERTEX))
        fillVertex<Coords>();
    else
        fillFundamental<Coords>();

    // Insert the results into the packet tree, but only once they are ready.
    if (! (tracker_ && tracker_->isCancelled()))
        triang_->insertChildLast(list_);

    if (tracker_)
        tracker_->setFinished();
}

template <typename Coords>
void NormalHypersurfaces::Enumerator::fillVertex() {
    // ----- Decide which algorithm to use -----

    // Here we will set the algorithm_ flag to precisely what we plan to do.
    // For now, we only support double description in 4-D.  So this is easy.
    list_->algorithm_ = HS_VERTEX_DD;

    // ----- Run the enumeration algorithm -----

    if (triang_->isEmpty()) {
        // Handle the empty triangulation separately.
        // Nothing to do here.
    } else {
        // A direct enumeration in the chosen coordinate system.
        if (tracker_)
            tracker_->newStage("Enumerating vertex hypersurfaces\n"
                "(double description method)");
        fillVertexDD<Coords>();
    }
}

template <typename Coords>
void NormalHypersurfaces::Enumerator::fillVertexDD() {
    MatrixInt* eqns = makeMatchingEquations(triang_, list_->coords_);

    EnumConstraints* constraints = (list_->which_.has(HS_EMBEDDED_ONLY) ?
        makeEmbeddedConstraints(triang_, list_->coords_) : 0);

    DoubleDescription::enumerateExtremalRays<typename Coords::Class>(
        HypersurfaceInserter(*list_, triang_), *eqns, constraints, tracker_);

    delete constraints;
    delete eqns;
}

template <typename Coords>
void NormalHypersurfaces::Enumerator::fillFundamental() {
    // Get the empty triangulation out of the way separately.
    if (triang_->isEmpty()) {
        list_->algorithm_ = HS_HILBERT_DUAL; /* shrug */
        return;
    }

    // ----- Decide upon and run an appropriate algorithm -----

    // This is where we make the "default" decision for the user.
    if (list_->which_.has(HS_IMMERSED_SINGULAR)) {
        // The primal method makes no sense without the quadrilateral
        // constraints.
        list_->algorithm_.ensureOne(HS_HILBERT_DUAL, HS_HILBERT_PRIMAL);
    } else {
        list_->algorithm_.ensureOne(HS_HILBERT_PRIMAL, HS_HILBERT_DUAL);
    }

    // Run the chosen algorithm.
    if (list_->algorithm_.has(HS_HILBERT_PRIMAL))
        fillFundamentalPrimal<Coords>();
    else
        fillFundamentalDual<Coords>();
}

template <typename Coords>
void NormalHypersurfaces::Enumerator::fillFundamentalPrimal() {
    // We will not set algorithm_ until after the extremal ray
    // enumeration has finished (since we might want to pass additional flags
    // to and/or from that routine).

    if (tracker_)
        tracker_->newStage("Initialising Hilbert basis enumeration", 0.1);

    // Fetch any necessary validity constraints.
    EnumConstraints* constraints = (list_->which_.has(HS_EMBEDDED_ONLY) ?
        makeEmbeddedConstraints(triang_, list_->coords_) : 0);

    // Enumerate all vertex normal hypersurfaces.
    if (tracker_)
        tracker_->newStage("Enumerating extremal rays", 0.4);

    NormalHypersurfaces* vtx = new NormalHypersurfaces(list_->coords_,
        HS_VERTEX | (list_->which_.has(HS_EMBEDDED_ONLY) ?
            HS_EMBEDDED_ONLY : HS_IMMERSED_SINGULAR),
        list_->algorithm_ /* passes through any vertex enumeration flags */);
    Enumerator e(vtx, triang_, 0 /* Don't set another progress tracker */);
    e.fillVertex<Coords>();

    // Finalise the algorithm flags for this list: combine HS_HILBERT_PRIMAL
    // with whatever vertex enumeration flags were used.
    list_->algorithm_ = e.list_->algorithm_ | HS_HILBERT_PRIMAL;

    // Expand this list to a full Hilbert basis.
    if (tracker_)
        tracker_->newStage("Expanding to Hilbert basis", 0.5);

    HilbertPrimal::enumerateHilbertBasis<typename Coords::Class>(
        HypersurfaceInserter(*list_, triang_),
        vtx->beginVectors(), vtx->endVectors(), constraints, tracker_);

    delete vtx;
    delete constraints;
}

template <typename Coords>
void NormalHypersurfaces::Enumerator::fillFundamentalDual() {
    list_->algorithm_ = HS_HILBERT_DUAL;

    if (tracker_)
        tracker_->newStage("Enumerating Hilbert basis\n(dual method)");

    MatrixInt* eqns = makeMatchingEquations(triang_, list_->coords_);

    EnumConstraints* constraints = (list_->which_.has(HS_EMBEDDED_ONLY) ?
        makeEmbeddedConstraints(triang_, list_->coords_) : 0);

    HilbertDual::enumerateHilbertBasis<typename Coords::Class>(
        HypersurfaceInserter(*list_, triang_), *eqns, constraints, tracker_);

    delete constraints;
    delete eqns;
}

} // namespace regina

