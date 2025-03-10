
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "triangulation/dim4.h"
#include "utilities/xmlutils.h"

namespace regina {

namespace {
    /**
     * A class that makes a vector appear as though it uses a different
     * encoding.
     *
     * This can only be used when the desired encoding stores a subset of the
     * coordinates that the source encoding stores.  In other words, it can
     * hide unwanted coordinates but it cannot reconstruct missing coordinates.
     *
     * This is (in particular) used by the two-pass primal Hilbert basis
     * algorithm, where the second pass needs to "forget" any reconstructed
     * tetrahedron coordinates that were added by the NormalHypersurface
     * constructor at the end of the first pass.
     */
    class HSShadowVector {
        private:
            const Vector<LargeInteger>& source_;
            HyperEncoding srcEnc_;
            HyperEncoding destEnc_;
            size_t destSize_;

        public:
            HSShadowVector(const Vector<LargeInteger>& source,
                    HyperEncoding srcEnc, HyperEncoding destEnc) :
                    source_(source), srcEnc_(srcEnc), destEnc_(destEnc),
                    destSize_((source_.size() / srcEnc.block()) *
                        destEnc_.block()) {
            }

            size_t size() const {
                return destSize_;
            }

            const LargeInteger& operator [] (size_t destIndex) const {
                if (srcEnc_.block() == destEnc_.block())
                    return source_[destIndex];

                size_t block_ = destIndex / destEnc_.block();
                size_t type_ = (srcEnc_.storesTetrahedra() &&
                        ! destEnc_.storesTetrahedra()) ?
                    (destIndex % destEnc_.block() + 5) :
                    (destIndex % destEnc_.block());
                return source_[srcEnc_.block() * block_ + type_];
            }
    };
}

void NormalHypersurfaces::Enumerator::enumerate() {
    // Clean up the "type of list" flag.
    list_->which_ &= (
        HyperList::EmbeddedOnly | HyperList::ImmersedSingular |
        HyperList::Vertex | HyperList::Fundamental);

    list_->which_.ensureOne(HyperList::Vertex, HyperList::Fundamental);
    list_->which_.ensureOne(HyperList::EmbeddedOnly,
        HyperList::ImmersedSingular);

    // Farm out the real work to list-type-specific routines.
    if (list_->which_.has(HyperList::Vertex))
        fillVertex();
    else
        fillFundamental();

    // Insert the results into the packet tree, but only once they are ready.
    if (treeParent_ && ! (tracker_ && tracker_->isCancelled()))
        treeParent_->append(static_cast<PacketOf<NormalHypersurfaces>*>(list_)->
            shared_from_this());

    if (tracker_)
        tracker_->setFinished();
}

void NormalHypersurfaces::Enumerator::fillVertex() {
    // ----- Decide which algorithm to use -----

    // Here we will set the algorithm_ flag to precisely what we plan to do.
    // For now, we only support double description in 4-D.  So this is easy.
    list_->algorithm_ = HyperAlg::VertexDD;

    // ----- Run the enumeration algorithm -----

    if (list_->triangulation_->isEmpty()) {
        // Handle the empty triangulation separately.
        // Nothing to do here.
    } else {
        // A direct enumeration in the chosen coordinate system.
        if (tracker_)
            tracker_->newStage("Enumerating vertex hypersurfaces\n"
                "(double description method)");
        fillVertexDD();
    }
}

void NormalHypersurfaces::Enumerator::fillVertexDD() {
    if (list_->which_.has(HyperList::EmbeddedOnly)) {
        ValidityConstraints c = makeEmbeddedConstraints(
            *list_->triangulation_, list_->coords_);
        DoubleDescription::enumerate<Vector<LargeInteger>>(
            [this](Vector<LargeInteger>&& v) {
                list_->surfaces_.emplace_back(list_->triangulation_,
                    list_->coords_, std::move(v));
            }, eqns_, c, tracker_);
    } else {
        DoubleDescription::enumerate<Vector<LargeInteger>>(
            [this](Vector<LargeInteger>&& v) {
                list_->surfaces_.emplace_back(list_->triangulation_,
                    list_->coords_, std::move(v));
            }, eqns_, ValidityConstraints::none, tracker_);
    }
}

void NormalHypersurfaces::Enumerator::fillFundamental() {
    // Get the empty triangulation out of the way separately.
    if (list_->triangulation_->isEmpty()) {
        list_->algorithm_ = HyperAlg::HilbertDual; /* shrug */
        return;
    }

    // ----- Decide upon and run an appropriate algorithm -----

    // This is where we make the "default" decision for the user.
    if (list_->which_.has(HyperList::ImmersedSingular)) {
        // The primal method makes no sense without the quadrilateral
        // constraints.
        list_->algorithm_.ensureOne(HyperAlg::HilbertDual,
            HyperAlg::HilbertPrimal);
    } else {
        list_->algorithm_.ensureOne(HyperAlg::HilbertPrimal,
            HyperAlg::HilbertDual);
    }

    // Run the chosen algorithm.
    if (list_->algorithm_.has(HyperAlg::HilbertPrimal))
        fillFundamentalPrimal();
    else
        fillFundamentalDual();
}

void NormalHypersurfaces::Enumerator::fillFundamentalPrimal() {
    // We will not set algorithm_ until after the extremal ray
    // enumeration has finished (since we might want to pass additional flags
    // to and/or from that routine).

    if (tracker_)
        tracker_->newStage("Initialising Hilbert basis enumeration", 0.1);

    // Enumerate all vertex normal hypersurfaces.
    if (tracker_)
        tracker_->newStage("Enumerating extremal rays", 0.4);

    NormalHypersurfaces vtx(list_->coords_,
        HyperList::Vertex | (list_->which_.has(HyperList::EmbeddedOnly) ?
            HyperList::EmbeddedOnly : HyperList::ImmersedSingular),
        list_->algorithm_ /* passes through any vertex enumeration flags */,
        list_->triangulation_);
    Enumerator e(&vtx, eqns_, nullptr, nullptr);
    e.fillVertex();

    // We cannot use eqns_ beyond this point, since we moved it into e.

    // The next pass, through HilbertPrimal, will need the vertex vectors
    // to appears as though they use the original coordinate system.
    // In particular, we must hide any tetrahedron coordinates that were
    // reconstructed by NormalSurface at the end of the first pass above.
    std::vector<HSShadowVector> shadows;
    shadows.reserve(vtx.size());
    for (const NormalHypersurface& s : vtx)
        shadows.emplace_back(s.vector(), s.encoding(),
            HyperEncoding(list_->coords_));

    // Finalise the algorithm flags for this list: combine HilbertPrimal
    // with whatever vertex enumeration flags were used.
    list_->algorithm_ = e.list_->algorithm_ | HyperAlg::HilbertPrimal;

    // Expand this list to a full Hilbert basis.
    if (tracker_)
        tracker_->newStage("Expanding to Hilbert basis", 0.5);

    if (list_->which_.has(HyperList::EmbeddedOnly)) {
        ValidityConstraints c = makeEmbeddedConstraints(*list_->triangulation_,
            list_->coords_);
        HilbertPrimal::enumerate<Vector<LargeInteger>>(
            [this](Vector<LargeInteger>&& v) {
                list_->surfaces_.emplace_back(list_->triangulation_,
                    list_->coords_, std::move(v));
            }, shadows.begin(), shadows.end(), c, tracker_);
    } else {
        HilbertPrimal::enumerate<Vector<LargeInteger>>(
            [this](Vector<LargeInteger>&& v) {
                list_->surfaces_.emplace_back(list_->triangulation_,
                    list_->coords_, std::move(v));
            }, shadows.begin(), shadows.end(), ValidityConstraints::none,
                tracker_);
    }
}

void NormalHypersurfaces::Enumerator::fillFundamentalDual() {
    list_->algorithm_ = HyperAlg::HilbertDual;

    if (tracker_)
        tracker_->newStage("Enumerating Hilbert basis\n(dual method)");

    if (list_->which_.has(HyperList::EmbeddedOnly)) {
        ValidityConstraints c = makeEmbeddedConstraints(*list_->triangulation_,
            list_->coords_);
        HilbertDual::enumerate<Vector<LargeInteger>>(
            [this](Vector<LargeInteger>&& v) {
                list_->surfaces_.emplace_back(list_->triangulation_,
                    list_->coords_, std::move(v));
            }, eqns_, c, tracker_);
    } else {
        HilbertDual::enumerate<Vector<LargeInteger>>(
            [this](Vector<LargeInteger>&& v) {
                list_->surfaces_.emplace_back(list_->triangulation_,
                    list_->coords_, std::move(v));
            }, eqns_, ValidityConstraints::none, tracker_);
    }
}

} // namespace regina

