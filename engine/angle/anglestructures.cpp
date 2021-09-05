
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include "angle/anglestructures.h"
#include "enumerate/doubledescription.h"
#include "enumerate/treetraversal.h"
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "surfaces/normalsurface.h"
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

void AngleStructures::enumerateInternal(ProgressTracker* tracker,
        Packet* treeParent) {
    // Form the matching equations.
    MatrixInt eqns = regina::makeAngleEquations(*triangulation_);

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
            EnumConstraints* constraints = new EnumConstraints(
                triangulation_->size());

            unsigned base = 0;
            for (unsigned c = 0; c < constraints->size(); ++c) {
                (*constraints)[c].insert((*constraints)[c].end(), base++);
                (*constraints)[c].insert((*constraints)[c].end(), base++);
                (*constraints)[c].insert((*constraints)[c].end(), base++);
            }

            // Find the angle structures.
            DoubleDescription::enumerateExtremalRays<VectorInt>(
                [this](VectorInt&& v) {
                    structures_.push_back(
                        AngleStructure(triangulation_, std::move(v)));
                }, eqns, constraints, tracker);

            // All done!
            delete constraints;
        }

        if (treeParent && ! (tracker && tracker->isCancelled()))
            treeParent->insertChildLast(this);

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

        // Find the angle structures.
        DoubleDescription::enumerateExtremalRays<VectorInt>(
            [this](VectorInt&& v) {
                structures_.push_back(
                    AngleStructure(triangulation_, std::move(v)));
            }, eqns, nullptr /* constraints */, tracker);

        // All done!
        if (treeParent && ! (tracker && tracker->isCancelled()))
            treeParent->insertChildLast(this);

        if (tracker)
            tracker->setFinished();
    }
}

AngleStructures::AngleStructures(const Triangulation<3>& triangulation,
        bool tautOnly, AngleAlg algHints, ProgressTracker* tracker) :
        triangulation_(triangulation), tautOnly_(tautOnly),
        algorithm_(algHints) {
    if (tracker)
        std::thread(&AngleStructures::enumerateInternal,
            this, tracker, nullptr).detach();
    else
        enumerateInternal(nullptr, nullptr);
}

AngleStructures* AngleStructures::enumerate(Triangulation<3>& triangulation,
        bool tautOnly, ProgressTracker* tracker) {
    AngleStructures* ans = new AngleStructures(tautOnly, AS_ALG_DEFAULT,
        triangulation);
    if (tracker)
        std::thread(&AngleStructures::enumerateInternal,
            ans, tracker, &triangulation).detach();
    else
        ans->enumerateInternal(nullptr, &triangulation);
    return ans;
}

AngleStructures* AngleStructures::enumerateTautDD(
        Triangulation<3>& triangulation) {
    AngleStructures* ans = new AngleStructures(true, AS_ALG_DD, triangulation);
    ans->enumerateInternal(nullptr /* tracker */, &triangulation);
    return ans;
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

void AngleStructures::writeXMLPacketData(std::ostream& out, FileFormat) const {
    using regina::xml::xmlValueTag;

    // Write the enumeration parameters.
    out << "  <angleparams "
        "tautonly=\"" << (tautOnly_ ? 'T' : 'F') << "\" "
        "algorithm=\"" << algorithm_.intValue() << "\"/>\n";

    // Write the individual structures.
    for (const AngleStructure& a : structures_)
        a.writeXMLData(out);

    // Write the properties.
    if (doesSpanStrict_.has_value())
        out << "  " << xmlValueTag("spanstrict", *doesSpanStrict_)
            << '\n';
    if (doesSpanTaut_.has_value())
        out << "  " << xmlValueTag("spantaut", *doesSpanTaut_)
            << '\n';
}

Packet* AngleStructures::internalClonePacket(Packet* parent) const {
    AngleStructures* ans = new AngleStructures(tautOnly_, algorithm_,
        *static_cast<Triangulation<3>*>(parent));
    for (const AngleStructure& s : structures_)
        ans->structures_.push_back(AngleStructure(s, ans->triangulation_));

    ans->doesSpanStrict_ = doesSpanStrict_;
    ans->doesSpanTaut_ = doesSpanTaut_;

    return ans;
}

void AngleStructures::calculateSpanStrict() const {
    if (structures_.empty()) {
        doesSpanStrict_ = false;
        return;
    }

    unsigned long nTets = triangulation().size();
    if (nTets == 0) {
        doesSpanStrict_ = true;
        return;
    }

    // We run into trouble if there's a 0 or pi angle that never changes.
    Rational* fixedAngles = new Rational[nTets * 3];
    unsigned long nFixed = 0;

    // Get the list of bad unchanging angles from the first structure.
    auto it = structures_.begin();
    const AngleStructure& s = *it;

    Rational angle;
    unsigned long tet;
    int edges;
    for (tet = 0; tet < nTets; tet++)
        for (edges = 0; edges < 3; edges++) {
            angle = s.angle(tet, edges);
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
        for (tet = 0; tet < nTets; tet++)
            for (edges = 0; edges < 3; edges++) {
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

} // namespace regina

