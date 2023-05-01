
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include "angle/anglestructures.h"
#include "enumerate/doubledescription.h"
#include "enumerate/treetraversal.h"
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "surface/normalsurface.h"
#include "triangulation/dim3.h"
#include "utilities/xmlutils.h"
#include <thread>

namespace regina {

MatrixInt makeAngleEquations(const Triangulation<3>& tri) {
    size_t n = tri.size();
    size_t cols = 3 * n + 1;

    // We have one equation per non-boundary edge plus one per tetrahedron.
    long rows = long(tri.countEdges()) + long(tri.size());
    for (BoundaryComponent<3>* bc : tri.boundaryComponents())
        rows -= bc->countEdges();

    MatrixInt eqns(rows, cols);
    size_t row = 0;

    size_t index;
    for (Edge<3>* edge : tri.edges()) {
        if (edge->isBoundary())
            continue;
        for (auto& emb : *edge) {
            index = emb.tetrahedron()->index();
            if (emb.edge() < 3)
                eqns.entry(row, 3 * index + emb.edge()) += 1;
            else
                eqns.entry(row, 3 * index + 5 - emb.edge()) += 1;
        }
        eqns.entry(row, cols - 1) = -2;
        ++row;
    }
    for (index = 0; index < n; index++) {
        eqns.entry(row, 3 * index) = 1;
        eqns.entry(row, 3 * index + 1) = 1;
        eqns.entry(row, 3 * index + 2) = 1;
        eqns.entry(row, cols - 1) = -1;
        ++row;
    }

    return eqns;
}

void AngleStructures::swap(AngleStructures& other) {
    if (std::addressof(other) == this)
        return;

    ChangeEventSpan span1(*this);
    ChangeEventSpan span2(other);

    structures_.swap(other.structures_);
    triangulation_.swap(other.triangulation_);
    std::swap(tautOnly_, other.tautOnly_);
    std::swap(algorithm_, other.algorithm_);

    doesSpanStrict_.swap(other.doesSpanStrict_);
    doesSpanTaut_.swap(other.doesSpanTaut_);
}

void AngleStructures::enumerateInternal(ProgressTracker* tracker,
        Packet* treeParent) {
    // Clean up the algorithms flag.
    algorithm_ &= (AS_ALG_TREE | AS_ALG_DD);

    if (tautOnly_ && (! triangulation_->isEmpty())) {
        // We can support either algorithm, but tree traversal should be faster.
        algorithm_.ensureOne(AS_ALG_TREE, AS_ALG_DD);

        if (tracker)
            tracker->newStage("Enumerating taut angle structures");

        if (algorithm_.has(AS_ALG_TREE)) {
            // For now just stick to arbitrary precision arithmetic.
            // TODO: Use native integer types when the angle equation matrix
            // is sufficiently small / simple.
            TautEnumeration<LPConstraintNone, BanNone, Integer> search(
                *triangulation_);
            while (search.next(tracker)) {
                structures_.push_back(search.buildStructure());
                if (tracker && tracker->isCancelled())
                    break;
            }
        } else {
            // Use the double description method.
            MatrixInt eqns = regina::makeAngleEquations(*triangulation_);

            ValidityConstraints compat(3, triangulation_->size(), 1);
            compat.addLocal({ 0, 1, 2 });

            // Find the angle structures.
            DoubleDescription::enumerate<VectorInt>([this](VectorInt&& v) {
                    structures_.emplace_back(triangulation_, std::move(v));
                }, eqns, compat, tracker);
        }

        if (treeParent && ! (tracker && tracker->isCancelled()))
            treeParent->append(static_cast<PacketOf<AngleStructures>*>(this)->
                shared_from_this());

        if (tracker)
            tracker->setFinished();
    } else {
        // Use the double description method: it's all we support.
        algorithm_ = AS_ALG_DD;

        // For the empty triangulation, we fall through here regardless
        // of whether we want taut or all vertex angle structures (but
        // either way, the answer is the same - just one empty structure).
        //
        // For all other triangulations, we fall through here if we are
        // after all vertex angle structures.
        if (tracker)
            tracker->newStage("Enumerating vertex angle structures");

        // Form the matching equations.
        MatrixInt eqns = regina::makeAngleEquations(*triangulation_);

        // Find the angle structures.
        DoubleDescription::enumerate<VectorInt>([this](VectorInt&& v) {
                structures_.emplace_back(triangulation_, std::move(v));
            }, eqns, ValidityConstraints::none, tracker);

        // All done!
        if (treeParent && ! (tracker && tracker->isCancelled()))
            treeParent->append(static_cast<PacketOf<AngleStructures>*>(this)->
                shared_from_this());

        if (tracker)
            tracker->setFinished();
    }
}

void AngleStructures::writeTextShort(std::ostream& o) const {
    o << structures_.size() << " vertex angle structure";
    if (structures_.size() != 1)
        o << 's';
    o << " (" << (tautOnly_ ? "taut only" : "no restrictions")  << ')';
}

void AngleStructures::writeTextLong(std::ostream& o) const {
    writeTextShort(o);
    o << ":\n";

    for (const AngleStructure& a : structures_) {
        a.writeTextShort(o);
        o << '\n';
    }
}

void AngleStructures::calculateSpanStrict() const {
    if (structures_.empty()) {
        doesSpanStrict_ = false;
        return;
    }

    size_t nTets = triangulation().size();
    if (nTets == 0) {
        doesSpanStrict_ = true;
        return;
    }

    // We run into trouble if there's a 0 or pi angle that never changes.
    auto* fixedAngles = new Rational[nTets * 3];
    size_t nFixed = 0;

    // Get the list of bad unchanging angles from the first structure.
    auto it = structures_.begin();
    const AngleStructure& first = *it;

    Rational angle;
    for (size_t tet = 0; tet < nTets; tet++)
        for (int edges = 0; edges < 3; edges++) {
            angle = first.angle(tet, edges);
            if (angle == Rational::zero || angle == Rational::one) {
                fixedAngles[3 * tet + edges] = angle;
                nFixed++;
            } else
                fixedAngles[3 * tet + edges] = Rational::undefined;
        }

    if (nFixed == 0) {
        doesSpanStrict_ = true;
        delete[] fixedAngles;
        return;
    }

    // Run through the rest of the structures to see if these bad angles
    // do ever change.
    for (it++; it != structures_.end(); it++) {
        const AngleStructure& s = *it;
        for (size_t tet = 0; tet < nTets; tet++)
            for (int edges = 0; edges < 3; edges++) {
                if (fixedAngles[3 * tet + edges] == Rational::undefined)
                    continue;
                if (s.angle(tet, edges) != fixedAngles[3 * tet + edges]) {
                    // Here's a bad angle that finally changed.
                    fixedAngles[3 * tet + edges] = Rational::undefined;
                    nFixed--;
                    if (nFixed == 0) {
                        doesSpanStrict_ = true;
                        delete[] fixedAngles;
                        return;
                    }
                }
            }
    }

    // Some of the bad angles never changed.
    doesSpanStrict_ = false;
    delete[] fixedAngles;
}

void AngleStructures::calculateSpanTaut() const {
    for (const AngleStructure& s : structures_) {
        if (s.isTaut()) {
            doesSpanTaut_ = true;
            return;
        }
    }
    doesSpanTaut_ = false;
}

bool AngleStructures::operator == (const AngleStructures& other) const {
    size_t n = structures_.size();
    if (n != other.structures_.size())
        return false;
    if (structures_.empty())
        return other.structures_.empty();
    if (other.structures_.empty())
        return false;

    // Both lists have the same size and are non-empty.
    // Our algorithm will be to sort and then compare.
    auto* lhs = new const AngleStructure*[n];
    auto* rhs = new const AngleStructure*[n];

    const AngleStructure** ptr = lhs;
    for (const auto& s : structures_)
        *ptr++ = std::addressof(s);

    ptr = rhs;
    for (const auto& s : other.structures_)
        *ptr++ = std::addressof(s);

    auto cmp = [](const AngleStructure* x, const AngleStructure* y) {
        return (*x) < (*y);
    };
    std::sort(lhs, lhs + n, cmp);
    std::sort(rhs, rhs + n, cmp);

    bool ans = std::equal(lhs, lhs + n, rhs,
            [](const AngleStructure* x, const AngleStructure* y) {
        return (*x) == (*y);
    });

    delete[] lhs;
    delete[] rhs;
    return ans;
}

} // namespace regina

