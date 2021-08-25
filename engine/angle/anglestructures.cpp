
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

void AngleStructures::enumerateInternal(Triangulation<3>& triang,
        ProgressTracker* tracker) {
    // Form the matching equations.
    MatrixInt eqns = regina::makeAngleEquations(triang);

    if (tautOnly_ && (! triang.isEmpty())) {
        // For now just stick to arbitrary precision arithmetic.
        // TODO: Use native integer types when the angle equation matrix
        // is sufficiently small / simple.
        if (tracker)
            tracker->newStage("Enumerating taut angle structures");

        TautEnumeration<LPConstraintNone, BanNone, Integer> search(triang);
        while (search.next(tracker)) {
            structures_.push_back(search.buildStructure());
            if (tracker && tracker->isCancelled())
                break;
        }

        if (! (tracker && tracker->isCancelled()))
            triang.insertChildLast(this);

        if (tracker)
            tracker->setFinished();
    } else {
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
            [this, &triang](VectorInt* v) {
                structures_.push_back(AngleStructure(triang, v));
            }, eqns, nullptr /* constraints */, tracker);

        // All done!
        if (! (tracker && tracker->isCancelled()))
            triang.insertChildLast(this);

        if (tracker)
            tracker->setFinished();
    }
}

AngleStructures::AngleStructures(Triangulation<3>& owner,
        bool tautOnly, ProgressTracker* tracker) :
        tautOnly_(tautOnly) {
    if (tracker)
        std::thread(&AngleStructures::enumerateInternal,
            this, std::ref(owner), tracker).detach();
    else
        enumerateInternal(owner);
}

AngleStructures* AngleStructures::enumerateTautDD(Triangulation<3>& owner) {
    AngleStructures* ans = new AngleStructures(true /* taut only */);

    // Form the matching equations.
    MatrixInt eqns = regina::makeAngleEquations(owner);

    // Form the taut constraints.
    EnumConstraints* constraints = new EnumConstraints(owner.size());

    unsigned base = 0;
    for (unsigned c = 0; c < constraints->size(); ++c) {
        (*constraints)[c].insert((*constraints)[c].end(), base++);
        (*constraints)[c].insert((*constraints)[c].end(), base++);
        (*constraints)[c].insert((*constraints)[c].end(), base++);
    }

    // Find the angle structures.
    DoubleDescription::enumerateExtremalRays<VectorInt>(
        [ans, &owner](VectorInt* v) {
            ans->structures_.push_back(AngleStructure(owner, v));
        }, eqns, constraints, nullptr /* tracker */);

    // All done!
    owner.insertChildLast(ans);

    delete constraints;
    return ans;
}

const Triangulation<3>& AngleStructures::triangulation() const {
    return *dynamic_cast<Triangulation<3>*>(parent());
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

void AngleStructures::writeXMLPacketData(std::ostream& out) const {
    using regina::xml::xmlValueTag;

    // Write the enumeration parameters.
    out << "  <angleparams tautonly=\"" << (tautOnly_ ? 'T' : 'F') << "\"/>\n";

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
    AngleStructures* ans = new AngleStructures(tautOnly_);
    for (const AngleStructure& s : structures_)
        ans->structures_.push_back(AngleStructure(s,
            *static_cast<Triangulation<3>*>(parent)));

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

