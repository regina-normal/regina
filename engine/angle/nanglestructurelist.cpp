
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

#include "angle/nanglestructurelist.h"
#include "enumerate/ndoubledescription.h"
#include "maths/nmatrixint.h"
#include "progress/nprogressmanager.h"
#include "progress/nprogresstypes.h"
#include "surfaces/nnormalsurface.h"
#include "triangulation/ntriangulation.h"
#include "utilities/xmlutils.h"

// Property IDs:
#define PROPID_ALLOWSTRICT 1
#define PROPID_ALLOWTAUT 2

namespace regina {

typedef std::vector<NAngleStructure*>::const_iterator StructureIteratorConst;

void* NAngleStructureList::Enumerator::run(void*) {
    NProgressPercent* progress = 0;
    if (manager) {
        progress = new NProgressPercent();
        manager->setProgress(progress);
    }

    // Form the matching equations (one per non-boundary edge plus
    // one per tetrahedron).
    unsigned long nTetrahedra = triang->getNumberOfTetrahedra();
    unsigned long nCoords = 3 * nTetrahedra + 1;

    long nEquations = long(triang->getNumberOfEdges()) +
        long(triang->getNumberOfTetrahedra());
    for (NTriangulation::BoundaryComponentIterator bit =
            triang->getBoundaryComponents().begin();
            bit != triang->getBoundaryComponents().end(); bit++)
        nEquations -= (*bit)->getNumberOfEdges();

    NMatrixInt eqns(nEquations, nCoords);
    unsigned long row = 0;

    std::deque<NEdgeEmbedding>::const_iterator embit;
    NPerm4 perm;
    unsigned long index;
    for (NTriangulation::EdgeIterator eit = triang->getEdges().begin();
            eit != triang->getEdges().end(); eit++) {
        if ((*eit)->isBoundary())
            continue;
        for (embit = (*eit)->getEmbeddings().begin();
                embit != (*eit)->getEmbeddings().end(); embit++) {
            index = triang->tetrahedronIndex((*embit).getTetrahedron());
            perm = (*embit).getVertices();
            eqns.entry(row, 3 * index + vertexSplit[perm[0]][perm[1]]) += 1;
        }
        eqns.entry(row, nCoords - 1) = -2;
        row++;
    }
    for (index = 0; index < nTetrahedra; index++) {
        eqns.entry(row, 3 * index) = 1;
        eqns.entry(row, 3 * index + 1) = 1;
        eqns.entry(row, 3 * index + 2) = 1;
        eqns.entry(row, nCoords - 1) = -1;
        row++;
    }

    // Form the taut constraints, if we need them.
    NEnumConstraintList* constraints = 0;
    if (list->tautOnly_) {
        constraints = new NEnumConstraintList(triang->getNumberOfTetrahedra());

        unsigned base = 0;
        for (unsigned c = 0; c < constraints->size(); ++c) {
            (*constraints)[c].insert((*constraints)[c].end(), base++);
            (*constraints)[c].insert((*constraints)[c].end(), base++);
            (*constraints)[c].insert((*constraints)[c].end(), base++);
        }
    }

    // Find the angle structures.
    NDoubleDescription::enumerateExtremalRays<NAngleStructureVector>(
        StructureInserter(*list, triang), eqns, constraints, progress);

    // All done!
    delete constraints;

    if (! (progress && progress->isCancelled()))
        triang->insertChildLast(list);

    if (progress)
        progress->setFinished();

    return 0;
}

NAngleStructureList* NAngleStructureList::enumerate(NTriangulation* owner,
        bool tautOnly, NProgressManager* manager) {
    NAngleStructureList* ans = new NAngleStructureList(tautOnly);
    Enumerator* e = new Enumerator(ans, owner, manager);

    if (manager) {
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

NTriangulation* NAngleStructureList::getTriangulation() const {
    return dynamic_cast<NTriangulation*>(getTreeParent());
}

void NAngleStructureList::writeTextShort(std::ostream& o) const {
    o << structures.size() << " vertex angle structure";
    if (structures.size() != 1)
        o << 's';
    o << " (" << (tautOnly_ ? "taut only" : "no restrictions")  << ')';
}

void NAngleStructureList::writeTextLong(std::ostream& o) const {
    writeTextShort(o);
    o << ":\n";

    for (StructureIteratorConst it = structures.begin();
            it != structures.end(); it++) {
        (*it)->writeTextShort(o);
        o << '\n';
    }
}

void NAngleStructureList::writeXMLPacketData(std::ostream& out) const {
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

NPacket* NAngleStructureList::internalClonePacket(NPacket* /* parent */)
        const {
    NAngleStructureList* ans = new NAngleStructureList(tautOnly_);
    transform(structures.begin(), structures.end(),
        back_inserter(ans->structures), FuncNewClonePtr<NAngleStructure>());

    if (doesSpanStrict.known())
        ans->doesSpanStrict = doesSpanStrict;
    if (doesSpanTaut.known())
        ans->doesSpanTaut = doesSpanTaut;

    return ans;
}

void NAngleStructureList::calculateSpanStrict() const {
    if (structures.empty()) {
        doesSpanStrict = false;
        return;
    }

    unsigned long nTets = getTriangulation()->getNumberOfTetrahedra();
    if (nTets == 0) {
        doesSpanStrict = true;
        return;
    }

    // We run into trouble if there's a 0 or pi angle that never changes.
    NRational* fixedAngles = new NRational[nTets * 3];
    unsigned long nFixed = 0;

    // Get the list of bad unchanging angles from the first structure.
    StructureIteratorConst it = structures.begin();
    const NAngleStructure* s = *it;

    NRational angle;
    unsigned long tet;
    int edges;
    for (tet = 0; tet < nTets; tet++)
        for (edges = 0; edges < 3; edges++) {
            angle = s->getAngle(tet, edges);
            if (angle == NRational::zero || angle == NRational::one) {
                fixedAngles[3 * tet + edges] = angle;
                nFixed++;
            } else
                fixedAngles[3 * tet + edges] = NRational::undefined;
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
                if (fixedAngles[3 * tet + edges] == NRational::undefined)
                    continue;
                if (s->getAngle(tet, edges) != fixedAngles[3 * tet + edges]) {
                    // Here's a bad angle that finally changed.
                    fixedAngles[3 * tet + edges] = NRational::undefined;
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

void NAngleStructureList::calculateSpanTaut() const {
    doesSpanTaut = (find_if(structures.begin(), structures.end(),
        std::mem_fun(&NAngleStructure::isTaut)) != structures.end());
}

} // namespace regina

