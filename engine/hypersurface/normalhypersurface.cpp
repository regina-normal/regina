
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

#include <algorithm>
#include "hypersurface/normalhypersurfaces.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "utilities/xmlutils.h"

namespace regina {

NormalHypersurface::NormalHypersurface(const Triangulation<4>& tri) :
        enc_(HyperEncoding::empty()),
        vector_(tri.size() * enc_.block()),
        triangulation_(tri) {
}

NormalHypersurface::NormalHypersurface(
        const SnapshotRef<Triangulation<4>>& tri) :
        enc_(HyperEncoding::empty()),
        vector_(tri->size() * enc_.block()),
        triangulation_(tri) {
}

LargeInteger NormalHypersurface::edgeWeight(size_t edgeIndex) const {
    // Find a pentachoron next to the edge in question.
    const EdgeEmbedding<4>& emb = triangulation_->edge(edgeIndex)->front();
    const size_t pentPos = enc_.block() * emb.pentachoron()->index();
    int start = emb.vertices()[0];
    int end = emb.vertices()[1];

    // Add up the tetrahedra and prisms meeting that edge.
    // Tetrahedra:
    LargeInteger ans = vector_[pentPos + start] + vector_[pentPos + end];
    // Prisms:
    int e = Edge<4>::edgeNumber[start][end];
    for (int i = 0; i < 3; ++i) {
        ans += vector_[pentPos + 5 +
            Edge<4>::edgeNumber[start][Triangle<4>::triangleVertex[e][i]]];
        ans += vector_[pentPos + 5 +
            Edge<4>::edgeNumber[end][Triangle<4>::triangleVertex[e][i]]];
    }
    return ans;
}

bool NormalHypersurface::isCompact() const {
    if (compact_.has_value())
        return *compact_;

    if (enc_.couldBeNonCompact()) {
        // It is only the tetrahedron coordinates that could be infinite.
        // Ignore prisms.
        size_t nPent = triangulation_->size();
        for (size_t pent = 0; pent < nPent; pent++) {
            for (int type = 0; type < 5; type++)
                if (tetrahedra(pent, type).isInfinite())
                    return *(compact_ = false);
        }
    }
    return *(compact_ = true);
}

void NormalHypersurface::writeTextShort(std::ostream& out) const {
    size_t nPents = triangulation_->size();
    for (size_t pent = 0; pent < nPents; pent++) {
        if (pent > 0)
            out << " || ";
        for (int j = 0; j < 5; j++)
            out << tetrahedra(pent, j) << ' ';
        out << ';';
        for (int j = 0; j < 10; j++)
            out << ' ' << prisms(pent, j);
    }
}

void NormalHypersurface::writeXMLData(std::ostream& out, FileFormat format,
        const NormalHypersurfaces* list) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    bool stripTetrahedra = (format == FileFormat::XmlGen2 && list &&
        enc_.storesTetrahedra() &&
        ! HyperEncoding(list->coords()).storesTetrahedra());

    if (! stripTetrahedra) {
        // Write the opening tag including vector length.
        size_t vecLen = vector_.size();
        out << "  <hypersurface";
        if (format != FileFormat::XmlGen2)
            out << " enc=\"" << enc_.intValue() << '\"';
        out << " len=\"" << vecLen << '\"';
        if (format == FileFormat::XmlGen2 || ! name_.empty())
            out << " name=\"" << xmlEncodeSpecialChars(name_) << '\"';
        out << '>';

        // Write all non-zero entries.
        for (size_t i = 0; i < vecLen; i++) {
            LargeInteger entry = vector_[i];
            if (entry != 0)
                out << ' ' << i << ' ' << entry;
        }
    } else {
        // We know this is FileFormat::XmlGen2.
        int oldBlock = enc_.block();
        int newBlock = oldBlock - 5;
        size_t nBlocks = vector_.size() / oldBlock;

        out << "  <hypersurface len=\"" << (nBlocks * newBlock) << "\""
            " name=\"" << xmlEncodeSpecialChars(name_) << "\">";

        for (size_t i = 0; i < nBlocks; ++i)
            for (int j = 0; j < newBlock; ++j) {
                LargeInteger entry = vector_[(i * oldBlock) + j + 5];
                if (entry != 0)
                    out << ' ' << ((i * newBlock) + j) << ' ' << entry;
            }
    }

    // Write properties.
    if (realBoundary_.has_value())
        out << "\n\t" << xmlValueTag("realbdry", *realBoundary_);
    if (compact_.has_value())
        out << "\n\t" << xmlValueTag("compact", *compact_);

    // Write the closing tag.
    out << " </hypersurface>\n";
}

bool NormalHypersurface::operator == (const NormalHypersurface& other) const {
    if (enc_ == other.enc_) {
        // This is a common case, and a straight left-to-right scan
        // should be faster than jumping around the vectors.
        return vector_ == other.vector_;
    }

    size_t nPents = triangulation_->size();
    if (nPents != other.triangulation_->size())
        return false;

    for (size_t p = 0; p < nPents; ++p) {
        for (int i = 0; i < 5; ++i)
            if (tetrahedra(p, i) != other.tetrahedra(p, i))
                return false;
        for (int i = 0; i < 10; ++i)
            if (prisms(p, i) != other.prisms(p, i))
                return false;
    }
    return true;
}

std::weak_ordering NormalHypersurface::operator <=> (
        const NormalHypersurface& rhs) const {
    size_t nPents = triangulation_->size();
    if (nPents != rhs.triangulation_->size())
        return nPents <=> rhs.triangulation_->size();

    for (size_t t = 0; t < nPents; ++t) {
        for (int i = 0; i < 5; ++i) {
            if (tetrahedra(t, i) < rhs.tetrahedra(t, i))
                return std::weak_ordering::less;
            if (tetrahedra(t, i) > rhs.tetrahedra(t, i))
                return std::weak_ordering::greater;
        }
        for (int i = 0; i < 10; ++i) {
            if (prisms(t, i) < rhs.prisms(t, i))
                return std::weak_ordering::less;
            if (prisms(t, i) > rhs.prisms(t, i))
                return std::weak_ordering::greater;
        }
    }

    // The hypersurfaces are equal.
    return std::weak_ordering::equivalent;
}

bool NormalHypersurface::embedded() const {
    size_t nPent = triangulation_->size();

    int prism[2];
    for (size_t pent = 0; pent < nPent; ++pent) {
        // Find all prism types that appear in this pentachoron.
        int found = 0;
        for (int type = 0; type < 10; ++type)
            if (prisms(pent, type) > 0) {
                if (found == 2)
                    return false;
                prism[found++] = type;
            }

        // If we do use two prisms, ensure they are compatible.
        if (found == 2)
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
                    if (Edge<4>::edgeVertex[prism[0]][i] ==
                            Edge<4>::edgeVertex[prism[1]][j])
                        return false;
    }

    return true;
}

bool NormalHypersurface::locallyCompatible(const NormalHypersurface& other)
        const {
    size_t nPent = triangulation_->size();

    int prism[2];
    for (size_t pent = 0; pent < nPent; ++pent) {
        // Find all prism types that appear in this pentachoron.
        int found = 0;
        for (int type = 0; type < 10; ++type)
            if (prisms(pent, type) > 0 || other.prisms(pent, type) > 0) {
                if (found == 2)
                    return false;
                prism[found++] = type;
            }

        // If we do use two prisms, ensure they are compatible.
        if (found == 2)
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
                    if (Edge<4>::edgeVertex[prism[0]][i] ==
                            Edge<4>::edgeVertex[prism[1]][j])
                        return false;
    }

    return true;
}

void NormalHypersurface::calculateRealBoundary() const {
    if (triangulation_->isClosed()) {
        realBoundary_ = false;
        return;
    }

    // Get a local reference to the triangulation so we do not have to
    // repeatedly bounce through the snapshot.
    const Triangulation<4>& tri(*triangulation_);

    size_t tot = tri.size();
    for (size_t index = 0; index < tot; index++) {
        const Pentachoron<4>* pent = tri.pentachoron(index);
        if (pent->hasBoundary()) {
            // Check for piece types with boundary
            for (int type = 0; type < 10; type++) {
                if (prisms(index, type) > 0) {
                    realBoundary_ = true;
                    return;
                }
            }
            for (int type = 0; type < 5; type++)
                if (tetrahedra(index, type) > 0) {
                    // Make sure the tetrahedron actually hits the
                    // boundary.
                    for (int facet = 0; facet < 5; facet++) {
                        if (facet == type)
                            continue;
                        if (! pent->adjacentPentachoron(facet)) {
                            realBoundary_ = true;
                            return;
                        }
                    }
                }
        }
    }
    realBoundary_ = false;
}

void NormalHypersurface::calculateFromTriangulation() const {
    Triangulation<3> me = triangulate();

    orientable_ = me.isOrientable();
    connected_ = me.isConnected();
    H1_ = me.homology();

    twoSided_ = (((*this) * 2).triangulate().countComponents() ==
        2 * me.countComponents());
}

NormalHypersurface NormalHypersurface::operator * (const LargeInteger& coeff)
        const {
    NormalHypersurface ans(triangulation_, enc_, vector_ * coeff);

    if (coeff == 0) {
        ans.orientable_ = true;
        ans.twoSided_ = true;
        ans.connected_ = true;
        ans.realBoundary_ = false;
        ans.compact_ = true;
        ans.H1_ = {};
        ans.linkOf_ = 0; /* need to recompute */
    } else {
        // Deduce some basic properties.
        ans.realBoundary_ = realBoundary_;
        ans.compact_ = compact_;
        ans.linkOf_ = linkOf_;

        // And some other properties are best left recalculated.
    }

    return ans;
}

NormalHypersurface& NormalHypersurface::operator *= (
        const LargeInteger& coeff) {
    vector_ *= coeff;

    // Update properties of the hypersurface where necessary:
    if (coeff == 0) {
        orientable_ = true;
        twoSided_ = true;
        connected_ = true;
        realBoundary_ = false;
        compact_ = true;
        H1_ = {};
        linkOf_ = 0; /* need to recompute */
    } else {
        // Some properties might change, and we will leave them to be
        // recomputed:
        orientable_.reset();
        twoSided_.reset();
        connected_.reset();
        H1_.reset();

        // All other properties are preserved:
        // - realBoundary_, compact_, linkOf_
    }

    return *this;
}

LargeInteger NormalHypersurface::scaleDown() {
    LargeInteger ans = vector_.scaleDown();

    // Some properties might change, and we will leave them to be
    // recomputed:
    orientable_.reset();
    twoSided_.reset();
    connected_.reset();
    H1_.reset();

    // All other properties are preserved:
    // - realBoundary_, compact_, linkOf_

    return ans;
}

} // namespace regina

