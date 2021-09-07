
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
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "utilities/xmlutils.h"

namespace regina {

LargeInteger NormalSurface::edgeWeight(size_t edgeIndex) const {
    // Find a tetrahedron next to the edge in question.
    const EdgeEmbedding<3>& emb = triangulation_->edge(edgeIndex)->front();
    const size_t tetPos = enc_.block() * emb.tetrahedron()->index();
    int start = emb.vertices()[0];
    int end = emb.vertices()[1];

    // Add up the discs meeting that edge.
    // Triangles:
    LargeInteger ans = vector_[tetPos + start] + vector_[tetPos + end];
    // Quads:
    ans += vector_[tetPos + 4 + quadMeeting[start][end][0]];
    ans += vector_[tetPos + 4 + quadMeeting[start][end][1]];
    // Octagons:
    if (enc_.storesOctagons()) {
        ans += vector_[tetPos + 7];
        ans += vector_[tetPos + 8];
        ans += vector_[tetPos + 9];
        ans += vector_[tetPos + 7 + quadSeparating[start][end]];
    }
    return ans;
}

LargeInteger NormalSurface::arcs(size_t triIndex, int triVertex) const {
    // Find a tetrahedron next to the triangle in question.
    const TriangleEmbedding<3>& emb = triangulation_->triangle(triIndex)->
        front();
    const size_t tetPos = enc_.block() * emb.tetrahedron()->index();
    int vertex = emb.vertices()[triVertex];
    int backOfFace = emb.vertices()[3];

    // Add up the discs meeting that triangle in that required arc.
    // Triangles:
    LargeInteger ans = vector_[tetPos + vertex];
    // Quads:
    ans += vector_[tetPos + 4 + quadSeparating[vertex][backOfFace]];
    if (enc_.storesOctagons()) {
        // Octagons:
        ans += vector_[tetPos + 7 + quadMeeting[vertex][backOfFace][0]];
        ans += vector_[tetPos + 7 + quadMeeting[vertex][backOfFace][1]];
    }
    return ans;
}

NormalSurface NormalSurface::doubleSurface() const {
    // Don't use the copy constructor, because we want to choose which
    // properties we keep.
    NormalSurface ans(triangulation_, enc_, vector_);

    ans.vector_ += ans.vector_;

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
    for (size_t tet = 0; tet < nTets; tet++) {
        if (tet > 0)
            out << " || ";
        for (int j=0; j<4; j++)
            out << triangles(tet, j) << ' ';
        out << ';';
        for (int j=0; j<3; j++)
            out << ' ' << quads(tet, j);
        if (enc_.storesOctagons()) {
            out << " ;";
            for (int j=0; j<3; j++)
                out << ' ' << octs(tet, j);
        }
    }
}

bool NormalSurface::hasMultipleOctDiscs() const {
    if (! enc_.storesOctagons())
        return false;

    size_t nTets = triangulation_->size();
    for (size_t tet=0; tet<nTets; tet++)
        for (int oct=0; oct<3; oct++) {
            LargeInteger coord = octs(tet, oct);
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

bool NormalSurface::isCompact() const {
    if (compact_.has_value())
        return *compact_;

    if (enc_.couldBeNonCompact()) {
        // It is only the triangle coordinates that could be infinite.
        // Ignore quads and (if present) octagons.
        size_t nTets = triangulation_->size();
        for (size_t tet = 0; tet < nTets; tet++) {
            for (int type = 0; type < 4; type++)
                if (triangles(tet, type).isInfinite())
                    return *(compact_ = false);
        }
    }
    return *(compact_ = true);
}

bool NormalSurface::isSplitting() const {
    size_t nTets = triangulation_->size();
    for (size_t tet = 0; tet < nTets; tet++) {
        for (int type = 0; type < 4; type++)
            if (triangles(tet, type) != 0)
                return false;
        LargeInteger tot; // initialised to zero
        for (int type = 0; type < 3; type++)
            tot += quads(tet, type);
        if (tot != 1)
            return false;
    }
    if (enc_.storesOctagons())
        for (size_t tet = 0; tet < nTets; tet++)
            for (int type = 0; type < 3; type++)
                if (octs(tet, type) != 0)
                    return false;
    return true;
}

size_t NormalSurface::isCentral() const {
    size_t nTets = triangulation_->size();
    size_t tot = 0;
    for (size_t tet = 0; tet < nTets; tet++) {
        LargeInteger tetTot; // initialised to zero
        for (int type = 0; type < 4; type++)
            tetTot += triangles(tet, type);
        for (int type = 0; type < 3; type++)
            tetTot += quads(tet, type);
        if (enc_.storesOctagons())
            for (int type = 0; type < 3; type++)
                tetTot += octs(tet, type);
        if (tetTot > 1)
            return 0;
        if (tetTot > 0)
            ++tot;
    }
    return tot;
}

bool NormalSurface::sameSurface(const NormalSurface& other) const {
    if (enc_ == other.enc_) {
        // This is a common case, and a straight left-to-right scan
        // should be faster than jumping around the vectors.
        return vector_ == other.vector_;
    }

    size_t nTet = triangulation_->size();
    bool checkAlmostNormal =
        (enc_.storesOctagons() || other.enc_.storesOctagons());

    for (size_t t = 0; t < nTet; ++t) {
        for (int i = 0; i < 4; ++i)
            if (triangles(t, i) != other.triangles(t, i))
                return false;
        for (int i = 0; i < 3; ++i)
            if (quads(t, i) != other.quads(t, i))
                return false;
        if (checkAlmostNormal)
            for (int i = 0; i < 3; ++i)
                if (octs(t, i) != other.octs(t, i))
                    return false;
    }
    return true;
}

bool NormalSurface::embedded() const {
    size_t nTets = triangulation_->size();

    for (size_t tet = 0; tet < nTets; ++tet) {
        int found = 0;
        for (int type = 0; type < 3; ++type)
            if (quads(tet, type) > 0)
                ++found;
        if (enc_.storesOctagons())
            for (int type = 0; type < 3; ++type)
                if (octs(tet, type) > 0)
                    ++found;
        if (found > 1)
            return false;
    }
    return true;
}

bool NormalSurface::locallyCompatible(const NormalSurface& other) const {
    size_t nTets = triangulation_->size();

    for (size_t tet = 0; tet < nTets; ++tet) {
        int found = 0;
        for (int type = 0; type < 3; ++type)
            if (quads(tet, type) > 0 || other.quads(tet, type) > 0)
                ++found;
        for (int type = 0; type < 3; ++type)
            if (octs(tet, type) > 0 || other.octs(tet, type) > 0)
                ++found;
        if (found > 1)
            return false;
    }
    return true;
}

void NormalSurface::calculateOctPosition() const {
    if (! enc_.storesOctagons()) {
        octPosition_ = DiscType();
        return;
    }

    size_t nTets = triangulation_->size();
    for (size_t tetIndex = 0; tetIndex < nTets; ++tetIndex)
        for (int type = 0; type < 3; ++type)
            if (octs(tetIndex, type) != 0) {
                octPosition_ = DiscType(tetIndex, type);
                return;
            }

    octPosition_ = DiscType();
    return;
}

void NormalSurface::calculateEulerChar() const {
    LargeInteger ans; // initialised to zero

    // Add vertices.
    size_t tot = triangulation_->countEdges();
    for (size_t index = 0; index < tot; index++)
        ans += edgeWeight(index);

    // Subtract edges.
    tot = triangulation_->countTriangles();
    for (size_t index = 0; index < tot; index++)
        for (int type = 0; type < 3; type++)
            ans -= arcs(index, type);

    // Add faces.
    tot = triangulation_->size();
    for (size_t index = 0; index < tot; index++) {
        for (int type = 0; type < 4; type++)
            ans += triangles(index, type);
        for (int type = 0; type < 3; type++)
            ans += quads(index, type);
        if (enc_.storesOctagons())
            for (int type = 0; type < 3; type++)
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

    // Get a local reference to the triangulation so we do not have to
    // repeatedly bounce through the snapshot.
    const Triangulation<3>& tri(*triangulation_);
    size_t tot = tri.size();
    for (size_t index = 0; index < tot; index++) {
        const Tetrahedron<3>* tet = tri.tetrahedron(index);
        if (tet->hasBoundary()) {
            // Check for disk types with boundary
            for (int type=0; type<3; type++)
                if (quads(index, type) > 0) {
                    realBoundary_ = true;
                    return;
                }
            if (enc_.storesOctagons())
                for (int type=0; type<3; type++)
                    if (octs(index, type) > 0) {
                        realBoundary_ = true;
                        return;
                    }
            for (int type=0; type<4; type++)
                if (triangles(index, type) > 0) {
                    // Make sure the triangle actually hits the
                    // boundary.
                    for (int face=0; face<4; face++) {
                        if (face == type)
                            continue;
                        if (! tet->adjacentTetrahedron(face)) {
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
    if (enc_.storesOctagons())
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
        // Note: we are converting from LargeInteger to Integer below.
        for(unsigned int j=0; j < numTet; j++) {
            meridian +=
                equations->entry(2*i, 3*j) *
                    Integer(quads(j, quadSeparating[0][1])) +
                equations->entry(2*i, 3*j+1) *
                    Integer(quads(j, quadSeparating[0][2])) +
                equations->entry(2*i, 3*j+2) *
                    Integer(quads(j, quadSeparating[0][3]));
            longitude +=
                equations->entry(2*i+1, 3*j) *
                    Integer(quads(j, quadSeparating[0][1])) +
                equations->entry(2*i+1, 3*j+1) *
                    Integer(quads(j, quadSeparating[0][2])) +
                equations->entry(2*i+1, 3*j+2) *
                    Integer(quads(j, quadSeparating[0][3]));
        }
        slopes.entry(i,0) = meridian;
        slopes.entry(i,1) = longitude;
    }
    return slopes;
}

void NormalSurface::writeXMLData(std::ostream& out, FileFormat format,
        const NormalSurfaces* list) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    bool stripTriangles = (format == REGINA_XML_GEN_2 && list &&
        enc_.storesTriangles() &&
        ! NormalEncoding(list->coords()).storesTriangles());

    if (! stripTriangles) {
        // Write the opening tag including vector length.
        size_t vecLen = vector_.size();
        out << "  <surface";
        if (format != REGINA_XML_GEN_2)
            out << " enc=\"" << enc_.intValue() << '\"';
        out << " len=\"" << vecLen << '\"';
        if (format == REGINA_XML_GEN_2 || ! name_.empty())
            out << " name=\"" << xmlEncodeSpecialChars(name_) << '\"';
        out << '>';

        // Write all non-zero entries.
        for (size_t i = 0; i < vecLen; i++) {
            LargeInteger entry = vector_[i];
            if (entry != 0)
                out << ' ' << i << ' ' << entry;
        }
    } else {
        // We know this is REGINA_XML_GEN_2.
        int oldBlock = enc_.block();
        int newBlock = oldBlock - 4;
        size_t nBlocks = vector_.size() / oldBlock;

        out << "  <surface len=\"" << (nBlocks * newBlock) << "\""
            " name=\"" << xmlEncodeSpecialChars(name_) << "\">";

        for (size_t i = 0; i < nBlocks; ++i)
            for (int j = 0; j < newBlock; ++j) {
                LargeInteger entry = vector_[(i * oldBlock) + j + 4];
                if (entry != 0)
                    out << ' ' << ((i * newBlock) + j) << ' ' << entry;
            }
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

NormalSurface NormalSurface::operator + (const NormalSurface& rhs) const {
    // First work out the vector sum.
    //
    // Given our current conditions on vector storage, the underlying
    // integer vectors should both store triangles and quadrilaterals.
    // The only possible difference is wrt storing octagons.
    //
    if (enc_.storesOctagons() == rhs.enc_.storesOctagons()) {
        return NormalSurface(triangulation_, enc_ + rhs.enc_,
            vector_ + rhs.vector_);
    } else if (enc_.storesOctagons()) {
        // We must have (blocks of 10 + blocks of 7).
        Vector<LargeInteger> v = vector_;
        size_t posLeft = 0, posRight = 0;
        while (posLeft < vector_.size()) {
            for (int i = 0; i < 7; ++i)
                v[posLeft++] += rhs.vector_[posRight++];
            posLeft += 3;
        }
        return NormalSurface(triangulation_, enc_ + rhs.enc_, std::move(v));
    } else {
        // We must have (blocks of 7 + blocks of 10).
        Vector<LargeInteger> v = rhs.vector_;
        size_t posLeft = 0, posRight = 0;
        while (posRight < rhs.vector_.size()) {
            for (int i = 0; i < 7; ++i)
                v[posRight++] += vector_[posLeft++];
            posRight += 3;
        }
        return NormalSurface(triangulation_, enc_ + rhs.enc_, std::move(v));
    }
}

} // namespace regina

