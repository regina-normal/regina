
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

#include <algorithm>
#include "maths/matrix.h"
#include "snappea/snappeatriangulation.h"
#include "surfaces/normalsurface.h"
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "utilities/xmlutils.h"

namespace regina {

NormalSurface* NormalSurface::clone() const {
    NormalSurface* ans = new NormalSurface(*triangulation_,
        dynamic_cast<NormalSurfaceVector*>(vector_->clone()));

    ans->octPosition_ = octPosition_;
    ans->eulerChar_ = eulerChar_;
    ans->boundaries_ = boundaries_;
    ans->orientable_ = orientable_;
    ans->twoSided_ = twoSided_;
    ans->connected_ = connected_;
    ans->realBoundary_ = realBoundary_;
    ans->compact_ = compact_;

    return ans;
}

NormalSurface NormalSurface::doubleSurface() const {
    NormalSurface ans(*triangulation_,
        dynamic_cast<NormalSurfaceVector*>(vector_->clone()));

    *(ans.vector_) += *(ans.vector_);

    // Some properties can be copied straight across.
    ans.realBoundary_ = realBoundary_;
    ans.compact_ = compact_;
    if (eulerChar_.has_value())
        ans.eulerChar_ = (*eulerChar_) * 2;

    // The following three properties can be used together to deduce how
    // they change in the clone.  However, until we sit down and check
    // through all possible cases we'll just leave them marked unknown.

    // TODO: ans.orientable_, ans.twoSided_, ans.connected_

    // And some other properties are best left recalculated.

    return ans;
}

void NormalSurface::writeTextShort(std::ostream& out) const {
    size_t nTets = triangulation_->size();
    size_t tet;
    unsigned j;
    bool almostNormal = vector_->allowsAlmostNormal();
    for (tet=0; tet<nTets; tet++) {
        if (tet > 0)
            out << " || ";
        for (j=0; j<4; j++)
            out << triangles(tet, j) << ' ';
        out << ';';
        for (j=0; j<3; j++)
            out << ' ' << quads(tet, j);
        if (almostNormal) {
            out << " ;";
            for (j=0; j<3; j++)
                out << ' ' << octs(tet, j);
        }
    }
}

bool NormalSurfaceVector::hasMultipleOctDiscs(const Triangulation<3>& triang)
        const {
    size_t nTets = triang.size();
    int oct;
    LargeInteger coord;
    for (size_t tet=0; tet<nTets; tet++)
        for (oct=0; oct<3; oct++) {
            coord = octs(tet, oct, triang);
            if (coord == 0)
                continue;
            // We have found our one and only oct type!
            if (coord == 1)
                return false;
            return true;
        }
    // There are no octagonal types at all.
    return false;
}

bool NormalSurfaceVector::isCompact(const Triangulation<3>& triang) const {
    size_t nTets = triang.size();
    size_t tet;
    int type;
    for (tet = 0; tet < nTets; tet++) {
        for (type = 0; type < 4; type++)
            if (triangles(tet, type, triang).isInfinite())
                return false;
        for (type = 0; type < 3; type++)
            if (quads(tet, type, triang).isInfinite())
                return false;
    }
    if (allowsAlmostNormal())
        for (tet = 0; tet < nTets; tet++)
            for (type = 0; type < 3; type++)
                if (octs(tet, type, triang).isInfinite())
                    return false;
    return true;
}

bool NormalSurfaceVector::isSplitting(const Triangulation<3>& triang) const {
    size_t nTets = triang.size();
    size_t tet;
    int type;
    LargeInteger tot;
    for (tet = 0; tet < nTets; tet++) {
        for (type = 0; type < 4; type++)
            if (triangles(tet, type, triang) != 0)
                return false;
        tot = 0L;
        for (type = 0; type < 3; type++)
            tot += quads(tet, type, triang);
        if (tot != 1)
            return false;
    }
    if (allowsAlmostNormal())
        for (tet = 0; tet < nTets; tet++)
            for (type = 0; type < 3; type++)
                if (octs(tet, type, triang) != 0)
                    return false;
    return true;
}

LargeInteger NormalSurfaceVector::isCentral(const Triangulation<3>& triang)
        const {
    size_t nTets = triang.size();
    size_t tet;
    int type;
    LargeInteger tot, tetTot;
    for (tet = 0; tet < nTets; tet++) {
        tetTot = 0L;
        for (type = 0; type < 4; type++)
            tetTot += triangles(tet, type, triang);
        for (type = 0; type < 3; type++)
            tetTot += quads(tet, type, triang);
        for (type = 0; type < 3; type++)
            tetTot += octs(tet, type, triang);
        if (tetTot > 1)
            return LargeInteger::zero;
        tot += tetTot;
    }
    return tot;
}

bool NormalSurface::isEmpty() const {
    size_t nTet = triangulation_->size();
    bool checkAlmostNormal = vector_->allowsAlmostNormal();

    size_t t;
    int i;

    for (t = 0; t < nTet; ++t) {
        for (i = 0; i < 4; ++i)
            if (triangles(t, i) != 0)
                return false;

        for (i = 0; i < 3; ++i)
            if (quads(t, i) != 0)
                return false;

        if (checkAlmostNormal)
            for (i = 0; i < 3; ++i)
                if (octs(t, i) != 0)
                    return false;
    }

    return true;
}

bool NormalSurface::sameSurface(const NormalSurface& other) const {
    size_t nTet = triangulation_->size();
    bool checkAlmostNormal =
        (vector_->allowsAlmostNormal() || other.vector_->allowsAlmostNormal());

    size_t t;
    int i;

    for (t = 0; t < nTet; ++t) {
        for (i = 0; i < 4; ++i)
            if (triangles(t, i) != other.triangles(t, i))
                return false;

        for (i = 0; i < 3; ++i)
            if (quads(t, i) != other.quads(t, i))
                return false;

        if (checkAlmostNormal)
            for (i = 0; i < 3; ++i)
                if (octs(t, i) != other.octs(t, i))
                    return false;
    }

    return true;
}

bool NormalSurface::embedded() const {
    size_t nTets = triangulation_->size();

    int type;
    int found;
    for (size_t tet = 0; tet < nTets; ++tet) {
        found = 0;
        for (type = 0; type < 3; ++type)
            if (quads(tet, type) > 0)
                ++found;
        for (type = 0; type < 3; ++type)
            if (octs(tet, type) > 0)
                ++found;
        if (found > 1)
            return false;
    }

    return true;
}

bool NormalSurface::locallyCompatible(const NormalSurface& other) const {
    size_t nTets = triangulation_->size();

    int type;
    int found;
    for (size_t tet = 0; tet < nTets; ++tet) {
        found = 0;
        for (type = 0; type < 3; ++type)
            if (quads(tet, type) > 0 || other.quads(tet, type) > 0)
                ++found;
        for (type = 0; type < 3; ++type)
            if (octs(tet, type) > 0 || other.octs(tet, type) > 0)
                ++found;
        if (found > 1)
            return false;
    }

    return true;
}

void NormalSurface::calculateOctPosition() const {
    if (! vector_->allowsAlmostNormal()) {
        octPosition_ = DiscType();
        return;
    }

    size_t tetIndex;
    int type;

    for (tetIndex = 0; tetIndex < triangulation_->size(); ++tetIndex)
        for (type = 0; type < 3; ++type)
            if (octs(tetIndex, type) != 0) {
                octPosition_ = DiscType(tetIndex, type);
                return;
            }

    octPosition_ = DiscType();
    return;
}

void NormalSurface::calculateEulerChar() const {
    size_t index, tot;
    int type;
    LargeInteger ans = LargeInteger::zero;

    // Add vertices.
    tot = triangulation_->countEdges();
    for (index = 0; index < tot; index++)
        ans += edgeWeight(index);

    // Subtract edges.
    tot = triangulation_->countTriangles();
    for (index = 0; index < tot; index++)
        for (type = 0; type < 3; type++)
            ans -= arcs(index, type);

    // Add faces.
    tot = triangulation_->size();
    for (index = 0; index < tot; index++) {
        for (type=0; type<4; type++)
            ans += triangles(index, type);
        for (type=0; type<3; type++)
            ans += quads(index, type);
        for (type=0; type<3; type++)
            ans += octs(index, type);
    }

    // Done!
    eulerChar_ = ans;
}

void NormalSurface::calculateRealBoundary() const {
    if (triangulation_->isClosed()) {
        realBoundary_ = false;
        return;
    }

    size_t index;
    size_t tot = triangulation_->size();
    const Tetrahedron<3>* tet;
    int type, face;

    for (index = 0; index < tot; index++) {
        tet = triangulation_->tetrahedron(index);
        if (tet->hasBoundary()) {
            // Check for disk types with boundary
            for (type=0; type<3; type++) {
                if (quads(index, type) > 0) {
                    realBoundary_ = true;
                    return;
                }
            }
            for (type=0; type<3; type++) {
                if (octs(index, type) > 0) {
                    realBoundary_ = true;
                    return;
                }
            }
            for (type=0; type<4; type++)
                if (triangles(index, type) > 0) {
                    // Make sure the triangle actually hits the
                    // boundary.
                    for (face=0; face<4; face++) {
                        if (face == type)
                            continue;
                        if (tet->adjacentTetrahedron(face) == 0) {
                            realBoundary_ = true;
                            return;
                        }
                    }
                }
        }
    }
    realBoundary_ = false;
}

std::optional<MatrixInt> NormalSurface::boundaryIntersections() const {
    // Make sure this is really a SnapPea triangulation.
    const SnapPeaTriangulation* snapPea =
        dynamic_cast<const SnapPeaTriangulation*>(&triangulation());
    if (! snapPea)
        return std::nullopt;

    // Check the preconditions.
    if (! snapPea->isOriented())
        return std::nullopt;
    if (vector_->allowsAlmostNormal())
        return std::nullopt;
    for (Vertex<3>* v : snapPea->vertices()) {
        if (! v->isIdeal())
            return std::nullopt;
        if (! v->isLinkOrientable())
            return std::nullopt;
        if (v->linkEulerChar() != 0)
            return std::nullopt;
    }

    std::optional<MatrixInt> equations = snapPea->slopeEquations();
    if (! equations)
        return std::nullopt;

    size_t cusps = equations->rows() / 2;
    size_t numTet = snapPea->size();
    MatrixInt slopes(cusps, 2);
    for(unsigned int i=0; i < cusps; i++) {
        Integer meridian; // constructor sets this to 0
        Integer longitude; // constructor sets this to 0
        for(unsigned int j=0; j < numTet; j++) {
            meridian += 
                equations->entry(2*i, 3*j) * Integer(quads(j, quadSeparating[0][1])) +
                equations->entry(2*i, 3*j+1) * Integer(quads(j, quadSeparating[0][2])) +
                equations->entry(2*i, 3*j+2) * Integer(quads(j, quadSeparating[0][3]));
            longitude += 
                equations->entry(2*i+1, 3*j) * Integer(quads(j, quadSeparating[0][1])) +
                equations->entry(2*i+1, 3*j+1) * Integer(quads(j, quadSeparating[0][2])) +
                equations->entry(2*i+1, 3*j+2) * Integer(quads(j, quadSeparating[0][3]));
        }
        slopes.entry(i,0) = meridian;
        slopes.entry(i,1) = longitude;
    }
    return slopes;
}

void NormalSurface::writeXMLData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    // Write the opening tag including vector length.
    size_t vecLen = vector_->size();
    out << "  <surface len=\"" << vecLen << "\" name=\""
        << xmlEncodeSpecialChars(name_) << "\">";

    // Write all non-zero entries.
    LargeInteger entry;
    for (size_t i = 0; i < vecLen; i++) {
        entry = (*vector_)[i];
        if (entry != 0)
            out << ' ' << i << ' ' << entry;
    }

    // Write properties.
    if (eulerChar_.has_value())
        out << "\n\t" << xmlValueTag("euler", *eulerChar_);
    if (orientable_.has_value())
        out << "\n\t" << xmlValueTag("orbl", *orientable_);
    if (twoSided_.has_value())
        out << "\n\t" << xmlValueTag("twosided", *twoSided_);
    if (connected_.has_value())
        out << "\n\t" << xmlValueTag("connected", *connected_);
    if (realBoundary_.has_value())
        out << "\n\t" << xmlValueTag("realbdry", *realBoundary_);
    if (compact_.has_value())
        out << "\n\t" << xmlValueTag("compact", *compact_);

    // Write the closing tag.
    out << " </surface>\n";
}

// Default implementations for oriented surfaces. Returns zero as any
// coordinate system which supports orientation should override these.
LargeInteger NormalSurfaceVector::orientedTriangles(
        size_t, int, const Triangulation<3>&, bool) const {
    return LargeInteger::zero;
};

LargeInteger NormalSurfaceVector::orientedQuads(
        size_t, int, const Triangulation<3>&, bool) const {
    return LargeInteger::zero;
};

} // namespace regina

