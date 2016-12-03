
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

typedef std::vector<AngleStructure*>::const_iterator StructureIteratorConst;

void AngleStructures::enumerateInternal(Triangulation<3>* triang,
        ProgressTracker* tracker) {
    // Form the matching equations.
    MatrixInt* eqns = AngleStructureVector::makeAngleEquations(triang);

    if (tautOnly_ && (! triang->isEmpty())) {
        // For now just stick to arbitrary precision arithmetic.
        // TODO: Use native integer types when the angle equation matrix
        // is sufficiently small / simple.
        if (tracker)
            tracker->newStage("Enumerating taut angle structures");

        TautEnumeration<LPConstraintNone, BanNone, Integer> search(triang);
        while (search.next(tracker)) {
            structures.push_back(search.buildStructure());
            if (tracker && tracker->isCancelled())
                break;
        }

        if (! (tracker && tracker->isCancelled()))
            triang->insertChildLast(this);

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
        DoubleDescription::enumerateExtremalRays<AngleStructureVector>(
            StructureInserter(*this, triang), *eqns, 0 /* constraints */,
            tracker);

        // All done!
        if (! (tracker && tracker->isCancelled()))
            triang->insertChildLast(this);

        if (tracker)
            tracker->setFinished();
    }

    delete eqns;
}

AngleStructures* AngleStructures::enumerate(Triangulation<3>* owner,
        bool tautOnly, ProgressTracker* tracker) {
    AngleStructures* ans = new AngleStructures(tautOnly);

    if (tracker)
        std::thread(&AngleStructures::enumerateInternal,
            ans, owner, tracker).detach();
    else
        ans->enumerateInternal(owner);
    return ans;
}

AngleStructures* AngleStructures::enumerateTautDD(
        Triangulation<3>* owner) {
    AngleStructures* ans = new AngleStructures(true /* taut only */);

    // Form the matching equations.
    MatrixInt* eqns = AngleStructureVector::makeAngleEquations(owner);

    // Form the taut constraints.
    EnumConstraints* constraints = new EnumConstraints(owner->size());

    unsigned base = 0;
    for (unsigned c = 0; c < constraints->size(); ++c) {
        (*constraints)[c].insert((*constraints)[c].end(), base++);
        (*constraints)[c].insert((*constraints)[c].end(), base++);
        (*constraints)[c].insert((*constraints)[c].end(), base++);
    }

    // Find the angle structures.
    DoubleDescription::enumerateExtremalRays<AngleStructureVector>(
        StructureInserter(*ans, owner), *eqns, constraints, 0 /* tracker */);

    // All done!
    owner->insertChildLast(ans);

    delete eqns;
    delete constraints;
    return ans;
}

Triangulation<3>* AngleStructures::triangulation() const {
    return dynamic_cast<Triangulation<3>*>(parent());
}

void AngleStructures::writeTextShort(std::ostream& o) const {
    o << structures.size() << " vertex angle structure";
    if (structures.size() != 1)
        o << 's';
    o << " (" << (tautOnly_ ? "taut only" : "no restrictions")  << ')';
}

void AngleStructures::writeTextLong(std::ostream& o) const {
    writeTextShort(o);
    o << ":\n";

    for (StructureIteratorConst it = structures.begin();
            it != structures.end(); it++) {
        (*it)->writeTextShort(o);
        o << '\n';
    }
}

void AngleStructures::writeXMLPacketData(std::ostream& out) const {
    using regina::xml::xmlValueTag;

    // Write the enumeration parameters.
    out << "  <angleparams tautonly=\"" << (tautOnly_ ? 'T' : 'F') << "\"/>\n";

    // Write the individual structures.
    for (StructureIteratorConst it = structures.begin();
            it != structures.end(); it++)
        (*it)->writeXMLData(out);

    // Write the properties.
    if (doesSpanStrict.known())
        out << "  " << xmlValueTag("spanstrict", doesSpanStrict.value())
            << '\n';
    if (doesSpanTaut.known())
        out << "  " << xmlValueTag("spantaut", doesSpanTaut.value())
            << '\n';
}

Packet* AngleStructures::internalClonePacket(Packet* /* parent */)
        const {
    AngleStructures* ans = new AngleStructures(tautOnly_);
    transform(structures.begin(), structures.end(),
        back_inserter(ans->structures), FuncNewClonePtr<AngleStructure>());

    if (doesSpanStrict.known())
        ans->doesSpanStrict = doesSpanStrict;
    if (doesSpanTaut.known())
        ans->doesSpanTaut = doesSpanTaut;

    return ans;
}

void AngleStructures::calculateSpanStrict() const {
    if (structures.empty()) {
        doesSpanStrict = false;
        return;
    }

    unsigned long nTets = triangulation()->size();
    if (nTets == 0) {
        doesSpanStrict = true;
        return;
    }

    // We run into trouble if there's a 0 or pi angle that never changes.
    Rational* fixedAngles = new Rational[nTets * 3];
    unsigned long nFixed = 0;

    // Get the list of bad unchanging angles from the first structure.
    StructureIteratorConst it = structures.begin();
    const AngleStructure* s = *it;

    Rational angle;
    unsigned long tet;
    int edges;
    for (tet = 0; tet < nTets; tet++)
        for (edges = 0; edges < 3; edges++) {
            angle = s->angle(tet, edges);
            if (angle == Rational::zero || angle == Rational::one) {
                fixedAngles[3 * tet + edges] = angle;
                nFixed++;
            } else
                fixedAngles[3 * tet + edges] = Rational::undefined;
        }

    if (nFixed == 0) {
        doesSpanStrict = true;
        delete[] fixedAngles;
        return;
    }

    // Run through the rest of the structures to see if these bad angles
    // do ever change.
    for (it++; it != structures.end(); it++) {
        s = *it;
        for (tet = 0; tet < nTets; tet++)
            for (edges = 0; edges < 3; edges++) {
                if (fixedAngles[3 * tet + edges] == Rational::undefined)
                    continue;
                if (s->angle(tet, edges) != fixedAngles[3 * tet + edges]) {
                    // Here's a bad angle that finally changed.
                    fixedAngles[3 * tet + edges] = Rational::undefined;
                    nFixed--;
                    if (nFixed == 0) {
                        doesSpanStrict = true;
                        delete[] fixedAngles;
                        return;
                    }
                }
            }
    }

    // Some of the bad angles never changed.
    doesSpanStrict = false;
    delete[] fixedAngles;
}

void AngleStructures::calculateSpanTaut() const {
    doesSpanTaut = (find_if(structures.begin(), structures.end(),
        std::mem_fun(&AngleStructure::isTaut)) != structures.end());
}

} // namespace regina

