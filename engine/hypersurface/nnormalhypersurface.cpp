
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

#include <algorithm>
#include "dim4/dim4triangulation.h"
#include "hypersurface/nnormalhypersurface.h"
#include "utilities/xmlutils.h"

// Property IDs:
#define PROPID_HS_REALBOUNDARY 5
#define PROPID_HS_COMPACT 6
#define PROPID_HS_SURFACENAME 100

namespace regina {

bool NNormalHypersurfaceVector::isCompact(Dim4Triangulation* triang) const {
    unsigned long nPents = triang->getNumberOfPentachora();
    unsigned long pent;
    int type;
    for (pent = 0; pent < nPents; pent++) {
        for (type = 0; type < 5; type++)
            if (getTetrahedronCoord(pent, type, triang).isInfinite())
                return false;
        for (type = 0; type < 10; type++)
            if (getPrismCoord(pent, type, triang).isInfinite())
                return false;
    }
    return true;
}

NNormalHypersurface::NNormalHypersurface(Dim4Triangulation* triangulation,
        NNormalHypersurfaceVector* vector) :
        vector_(vector), triangulation_(triangulation) {
}

NNormalHypersurface* NNormalHypersurface::clone() const {
    NNormalHypersurface* ans = new NNormalHypersurface(triangulation_,
        dynamic_cast<NNormalHypersurfaceVector*>(vector_->clone()));

    ans->realBoundary_ = realBoundary_;
    ans->compact_ = compact_;

    return ans;
}

void NNormalHypersurface::writeTextShort(std::ostream& out) const {
    unsigned long nPents = triangulation_->getNumberOfPentachora();
    unsigned long pent;
    unsigned j;
    for (pent=0; pent < nPents; pent++) {
        if (pent > 0)
            out << " || ";
        for (j=0; j<5; j++)
            out << getTetrahedronCoord(pent, j) << ' ';
        out << ';';
        for (j=0; j<10; j++)
            out << ' ' << getPrismCoord(pent, j);
    }
}

void NNormalHypersurface::writeXMLData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    // Write the opening tag including vector length.
    unsigned vecLen = vector_->size();
    out << "  <hypersurface len=\"" << vecLen << "\" name=\""
        << xmlEncodeSpecialChars(name_) << "\">";

    // Write all non-zero entries.
    NLargeInteger entry;
    for (unsigned i = 0; i < vecLen; i++) {
        entry = (*vector_)[i];
        if (entry != 0)
            out << ' ' << i << ' ' << entry;
    }

    // Write properties.
    if (realBoundary_.known())
        out << "\n\t" << xmlValueTag("realbdry", realBoundary_.value());
    if (compact_.known())
        out << "\n\t" << xmlValueTag("compact", compact_.value());

    // Write the closing tag.
    out << " </hypersurface>\n";
}

bool NNormalHypersurface::isEmpty() const {
    unsigned long nPents = triangulation_->getNumberOfPentachora();

    unsigned long p;
    int i;

    for (p = 0; p < nPents; ++p) {
        for (i = 0; i < 5; ++i)
            if (getTetrahedronCoord(p, i) != 0)
                return false;

        for (i = 0; i < 10; ++i)
            if (getPrismCoord(p, i) != 0)
                return false;
    }

    return true;
}

bool NNormalHypersurface::sameSurface(const NNormalHypersurface& other) const {
    unsigned long nPents = triangulation_->getNumberOfPentachora();

    unsigned long p;
    int i;

    for (p = 0; p < nPents; ++p) {
        for (i = 0; i < 5; ++i)
            if (getTetrahedronCoord(p, i) != other.getTetrahedronCoord(p, i))
                return false;

        for (i = 0; i < 10; ++i)
            if (getPrismCoord(p, i) != other.getPrismCoord(p, i))
                return false;
    }

    return true;
}

void NNormalHypersurface::calculateRealBoundary() const {
    if (triangulation_->isClosed()) {
        realBoundary_ = false;
        return;
    }

    unsigned long index;
    unsigned long tot = triangulation_->getNumberOfPentachora();
    Dim4Pentachoron* pent;
    int type, facet;

    for (index = 0; index < tot; index++) {
        pent = triangulation_->getPentachoron(index);
        if (pent->hasBoundary()) {
            // Check for piece types with boundary
            for (type = 0; type < 10; type++) {
                if (getPrismCoord(index, type) > 0) {
                    realBoundary_ = true;
                    return;
                }
            }
            for (type = 0; type < 5; type++)
                if (getTetrahedronCoord(index, type) > 0) {
                    // Make sure the tetrahedron actually hits the
                    // boundary.
                    for (facet = 0; facet < 5; facet++) {
                        if (facet == type)
                            continue;
                        if (pent->adjacentPentachoron(facet) == 0) {
                            realBoundary_ = true;
                            return;
                        }
                    }
                }
        }
    }
    realBoundary_ = false;
}

} // namespace regina

