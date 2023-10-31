
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

#include <cassert>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stack>

#include "link/link.h"
#include "snappea/snappeatriangulation.h"
#include "triangulation/dim3.h"
#include "utilities/stringutils.h"
#include "utilities/xmlutils.h"

namespace regina {

Triangulation<3>::Triangulation(const std::string& description) {
    try {
        *this = fromIsoSig(description);
        return;
    } catch (const InvalidArgument&) {
    }

    try {
        *this = rehydrate(description);
        return;
    } catch (const InvalidArgument&) {
    }

    try {
        *this = fromSnapPea(description);
        return;
    } catch (const InvalidArgument&) {
    } catch (const FileError&) {
    }

    throw InvalidArgument("The given string could not be interpreted "
        "as representing a 3-dimensional triangulation");
}

void Triangulation<3>::clearAllProperties() {
    clearBaseProperties();

    // Properties of the triangulation:
    prop_.zeroEfficient_.reset();
    prop_.splittingSurface_.reset();
    prop_.niceTreeDecomposition_.reset();

    // Properties of the manifold:
    if (! topologyLocked()) {
        prop_.H1Rel_.reset();
        prop_.H1Bdry_.reset();
        prop_.H2_.reset();
        prop_.twoSphereBoundaryComponents_.reset();
        prop_.negativeIdealBoundaryComponents_.reset();
        prop_.threeSphere_.reset();
        prop_.handlebody_.reset();
        prop_.TxI_.reset();
        prop_.irreducible_.reset();
        prop_.compressingDisc_.reset();
        prop_.haken_.reset();
        prop_.turaevViroCache_.clear();
    }

    strictAngleStructure_ = false; // computation not attempted
    generalAngleStructure_ = false; // computation not attempted
}

void Triangulation<3>::swap(Triangulation<3>& other) {
    if (&other == this)
        return;

    // We use a basic PacketChangeSpan here, not a richer ChangeAndClearSpan,
    // since we do not want to touch computed properties.  Our intention here
    // is to swap them, not clear them.
    PacketChangeSpan span1(*this);
    PacketChangeSpan span2(other);

    // Note: swapBaseData() calls Snapshottable::swap().
    swapBaseData(other);

    // Properties stored directly:
    std::swap(ideal_, other.ideal_);
    std::swap(standard_, other.standard_);

    // Properties stored using std::... helper classes:
    prop_.H1Rel_.swap(other.prop_.H1Rel_);
    prop_.H1Bdry_.swap(other.prop_.H1Bdry_);
    prop_.H2_.swap(other.prop_.H2_);

    prop_.twoSphereBoundaryComponents_.swap(
        other.prop_.twoSphereBoundaryComponents_);
    prop_.negativeIdealBoundaryComponents_.swap(
        other.prop_.negativeIdealBoundaryComponents_);

    prop_.zeroEfficient_.swap(other.prop_.zeroEfficient_);
    prop_.splittingSurface_.swap(other.prop_.splittingSurface_);

    prop_.threeSphere_.swap(other.prop_.threeSphere_);
    prop_.handlebody_.swap(other.prop_.handlebody_);
    prop_.TxI_.swap(other.prop_.TxI_);
    prop_.irreducible_.swap(other.prop_.irreducible_);
    prop_.compressingDisc_.swap(other.prop_.compressingDisc_);
    prop_.haken_.swap(other.prop_.haken_);

    strictAngleStructure_.swap(other.strictAngleStructure_);
    generalAngleStructure_.swap(other.generalAngleStructure_);
    prop_.niceTreeDecomposition_.swap(other.prop_.niceTreeDecomposition_);

    // Properties stored using std::... containers:
    prop_.turaevViroCache_.swap(other.prop_.turaevViroCache_);
}

long Triangulation<3>::eulerCharManifold() const {
    // Begin with V - E + F - T.
    // This call to eulerCharTri() also ensures that the skeleton has
    // been calculated.
    long ans = eulerCharTri();

    // Truncate any ideal vertices.
    for (auto bc : boundaryComponents())
        if (bc->isIdeal())
            ans += bc->eulerChar() - 1;

    // If we have an invalid triangulation, we need to locate invalid
    // vertices (i.e., non-standard boundary vertices) and also invalid edges,
    // and truncate those unwanted bits also.
    if (! valid_) {
        for (Vertex<3>* v : vertices())
            if (v->linkType() == Vertex<3>::INVALID)
                ans += v->linkEulerChar() - 1;
        for (Edge<3>* e : edges())
            if (! e->isValid())
                ++ans;
    }

    return ans;
}

bool Triangulation<3>::hasMinimalBoundary() const {
    for (auto b : boundaryComponents())
        if (b->countTriangles() > 2 && b->countVertices() > 1)
            return false;
    return true;
}

bool Triangulation<3>::hasMinimalVertices() const {
    for (auto c : components())
        if (c->isClosed()) {
            if (c->countVertices() != 1)
                return false;
        } else {
            size_t expect = 0;
            for (auto b : c->boundaryComponents()) {
                if (b->countTriangles() > 2 && b->countVertices() > 1)
                    return false;
                expect += b->countVertices();
            }
            if (c->countVertices() != expect)
                return false;
        }
    return true;
}

Triangulation<3>::Triangulation(const Triangulation<3>& src, bool cloneProps,
        bool cloneLocks) : TriangulationBase<3>(src, cloneProps, cloneLocks) {
    if (! cloneProps)
        return;

    // Clone properties:
    prop_ = src.prop_;

    // Any cached angle structures must be remade to live in this triangulation.
    if (std::holds_alternative<AngleStructure>(src.strictAngleStructure_))
        strictAngleStructure_ = AngleStructure(
            std::get<AngleStructure>(src.strictAngleStructure_), *this);
    else
        strictAngleStructure_ = std::get<bool>(src.strictAngleStructure_);
    if (std::holds_alternative<AngleStructure>(src.generalAngleStructure_))
        generalAngleStructure_ = AngleStructure(
            std::get<AngleStructure>(src.generalAngleStructure_), *this);
    else
        generalAngleStructure_ = std::get<bool>(src.generalAngleStructure_);

    // We do not need to copy skeletal properties (e.g., ideal_ or standard_),
    // since this is computed on demand with the rest of the skeleton.
}

std::string Triangulation<3>::snapPea() const {
    std::ostringstream out;
    snapPea(out);
    return out.str();
}

void Triangulation<3>::snapPea(std::ostream& out) const {
    // Sanity checks.
    if (! isValid())
        throw NotImplemented("SnapPea exports are only available "
            "for valid triangulations");
    if (hasBoundaryTriangles())
        throw NotImplemented("SnapPea exports are only available "
            "for triangulations with no boundary triangles");
    if (simplices_.empty())
        throw NotImplemented("SnapPea exports are only available "
            "for non-empty triangulations");

    // Write header information.
    out << "% Triangulation\n";
    out << "Regina_Triangulation\n";

    // Write general details.
    out << "not_attempted 0.0\n";
    out << "unknown_orientability\n";
    out << "CS_unknown\n";

    // Write cusps.
    out << "0 0\n";

    // Write tetrahedra.
    out << size() << '\n';

    int i, j;
    for (Tetrahedron<3>* tet : tetrahedra()) {
        // Although our precondition states that there are no boundary
        // triangles, we test for this anyway.  If somebody makes a mistake and
        // calls this routine with a bounded triangulation, we don't want
        // to wind up calling nullptr->index() and crashing.
        for (i = 0; i < 4; i++)
            if (tet->adjacentTetrahedron(i))
                out << "   " << tet->adjacentTetrahedron(i)->index() << ' ';
            else
                out << "   -1 ";
        out << '\n';
        for (i = 0; i < 4; i++)
            out << ' ' << tet->adjacentGluing(i).str();
        out << '\n';

        // Incident cusps.
        for (i = 0; i < 4; i++)
            out << "  -1 ";
        out << '\n';

        // Meridians and longitudes.
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 16; j++)
                out << "  0";
            out << '\n';
        }

        // Tetrahedron shape.
        out << "0.0 0.0\n";
    }
}

bool Triangulation<3>::saveSnapPea(const char* filename) const {
    // Sanity checks.
    // Although snapPea() will also check these conditions, we need to
    // check them now so if we fail then we do so before the file is opened.
    if ((! isValid()) || hasBoundaryTriangles() || simplices_.empty())
        return false;

    std::ofstream out(filename);
    if (!out)
        return false;
    snapPea(out);
    return true;
}

std::string Triangulation<3>::recogniser() const {
    std::ostringstream out;
    recogniser(out);
    return out.str();
}

std::string Triangulation<3>::recognizer() const {
    std::ostringstream out;
    recogniser(out);
    return out.str();
}

void Triangulation<3>::recogniser(std::ostream& out) const {
    // Sanity checks.
    if (! isValid())
        throw NotImplemented("Recogniser exports are only available "
            "for valid triangulations");
    if (hasBoundaryTriangles())
        throw NotImplemented("Recogniser exports are only available "
            "for triangulations with no boundary triangles");

    // Write the header.
    out << "triangulation" << std::endl;

    // Write face gluings.
    Triangle<3>* f;
    Tetrahedron<3>* tet;
    Perm<4> vert;
    for (unsigned i = 0; i < countTriangles(); ++i) {
        f = triangle(i);

        tet = f->embedding(0).tetrahedron();
        vert = f->embedding(0).vertices();
        out << 't' << (tet->index() + 1)
            << '(' << (vert[0] + 1)
            << ',' << (vert[1] + 1)
            << ',' << (vert[2] + 1) << ") - ";

        tet = f->embedding(1).tetrahedron();
        vert = f->embedding(1).vertices();
        out << 't' << (tet->index() + 1)
            << '(' << (vert[0] + 1)
            << ',' << (vert[1] + 1)
            << ',' << (vert[2] + 1) << ')';

        if (i != countTriangles() - 1)
            out << ',';
        out << std::endl;
    }

    // Write the footer.
    out << "end" << std::endl;
}

bool Triangulation<3>::saveRecogniser(const char* filename) const {
    // Sanity checks.
    // Although recogniser() will also check these conditions, we need to
    // check them now so if we fail then we do so before the file is opened.
    if ((! isValid()) || hasBoundaryTriangles())
        return false;

    // Write to file or stdout as appropriate.
    std::ofstream out(filename);
    if (! out)
        return false;
    recogniser(out);
    return true;
}

SnapPeaTriangulation* Triangulation<3>::isSnapPea() {
    return (heldBy_ == HELD_BY_SNAPPEA ?
        static_cast<SnapPeaTriangulation*>(this) : nullptr);
}

const SnapPeaTriangulation* Triangulation<3>::isSnapPea() const {
    return (heldBy_ == HELD_BY_SNAPPEA ?
        static_cast<const SnapPeaTriangulation*>(this) : nullptr);
}

std::shared_ptr<Packet> Triangulation<3>::inAnyPacket() {
    switch (heldBy_) {
        case HELD_BY_PACKET:
            return static_cast<PacketOf<Triangulation<3>>*>(this)->
                shared_from_this();
        case HELD_BY_SNAPPEA:
            return static_cast<SnapPeaTriangulation*>(this)->
                PacketData<SnapPeaTriangulation>::packet();
        default:
            return nullptr;
    }
}

std::shared_ptr<const Packet> Triangulation<3>::inAnyPacket() const {
    switch (heldBy_) {
        case HELD_BY_PACKET:
            return static_cast<const PacketOf<Triangulation<3>>*>(this)->
                shared_from_this();
        case HELD_BY_SNAPPEA:
            return static_cast<const SnapPeaTriangulation*>(this)->
                PacketData<SnapPeaTriangulation>::packet();
        default:
            return nullptr;
    }
}

void Triangulation<3>::snapPeaPreChange() {
    // This is in the .cpp file so we can keep snappeatriangulation.h
    // out of the main Triangulation<3> headers.
    auto* s = static_cast<SnapPeaTriangulation*>(this);

    // We do not nullify the triangulation until after the change,
    // since the routine performing the change probably expects
    // the original (non-empty) Triangulation<3> data.
    //
    // However, if the SnapPeaTriangulation is held by a packet, we
    // *should* be firing a packet pre-change event now to acknowledge that
    // the triangulation will be nullified.  Unfortunately this requires us
    // to read and edit the SnapPeaTriangulation's packetChangeSpans_ member,
    // which is private and inaccessible to Triangulation<3>.
    //
    // See the SnapPeaTriangulation class notes for more details on this issue,
    // and why it is not enormously important.
    //
    // If this is ever fixed, we should also remember to put the corresponding
    // packet post-change event code in snapPeaPostChange() also.

    ++s->reginaPacketChangeSpans_;
}

void Triangulation<3>::snapPeaPostChange() {
    // This is in the .cpp file so we can keep snappeatriangulation.h
    // out of the main Triangulation<3> headers.
    auto* s = static_cast<SnapPeaTriangulation*>(this);
    --s->reginaPacketChangeSpans_;

    // The triangulation changes might be nested.  Only nullify the SnapPea
    // triangulation once all of them are finished, since we do not want to
    // clear out the triangulation while a complex change set is still
    // happening.
    if (! s->reginaPacketChangeSpans_)
        static_cast<SnapPeaTriangulation*>(this)->nullify();
}

Triangulation<3>& static_triangulation3_cast(Packet& p) {
    // This is in the .cpp file so we can keep snappeatriangulation.h
    // out of the main Triangulation<3> headers.
    if (p.type() == PacketType::SnapPea)
        return static_packet_cast<SnapPeaTriangulation>(p);
    else
        return static_packet_cast<Triangulation<3>>(p);
}

const Triangulation<3>& static_triangulation3_cast(const Packet& p) {
    // This is in the .cpp file so we can keep snappeatriangulation.h
    // out of the main Triangulation<3> headers.
    if (p.type() == PacketType::SnapPea)
        return static_packet_cast<const SnapPeaTriangulation>(p);
    else
        return static_packet_cast<const Triangulation<3>>(p);
}

} // namespace regina

