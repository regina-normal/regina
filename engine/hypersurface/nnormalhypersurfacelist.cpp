
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

#include <list>
#include <thread>
#include "dim4/dim4triangulation.h"
#include "enumerate/doubledescription.h"
#include "enumerate/hilbertdual.h"
#include "enumerate/hilbertprimal.h"
#include "hypersurface/nnormalhypersurfacelist.h"
#include "hypersurface/hscoordregistry.h"
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "utilities/xmlutils.h"

namespace regina {

namespace {
    struct ZeroVector : public Returns<NNormalHypersurfaceVector*> {
        template <typename Coords>
        inline NNormalHypersurfaceVector* operator() (
                const Dim4Triangulation* tri) {
            return Coords::Class::makeZeroVector(tri);
        }
    };
}

NNormalHypersurfaceVector* makeZeroVector(
        const Dim4Triangulation* triangulation, HyperCoords coords) {
    return forCoords(coords, ZeroVector(), 0, triangulation);
}

namespace {
    struct MatchingEquations : public Returns<NMatrixInt*> {
        template <typename Coords>
        inline NMatrixInt* operator() (const Dim4Triangulation* tri) {
            return Coords::Class::makeMatchingEquations(tri);
        }
    };
}

NMatrixInt* makeMatchingEquations(const Dim4Triangulation* triangulation,
        HyperCoords coords) {
    return forCoords(coords, MatchingEquations(), 0, triangulation);
}

namespace {
    struct EmbeddedConstraints : public Returns<EnumConstraints*> {
        template <typename Coords>
        inline EnumConstraints* operator() (const Dim4Triangulation* tri) {
            return Coords::Class::makeEmbeddedConstraints(tri);
        }
    };
}

EnumConstraints* makeEmbeddedConstraints(
        const Dim4Triangulation* triangulation, HyperCoords coords) {
    return forCoords(coords, EmbeddedConstraints(), 0, triangulation);
}

Dim4Triangulation* NNormalHypersurfaceList::triangulation() const {
    return dynamic_cast<Dim4Triangulation*>(parent());
}

namespace {
    struct NameFunction : public Returns<const char*> {
        template <typename Coords>
        inline const char* operator() () { return Coords::name(); }
    };
}

void NNormalHypersurfaceList::writeTextShort(std::ostream& out) const {
    out << surfaces_.size();

    if (which_.has(regina::HS_EMBEDDED_ONLY))
        out << " embedded,";
    else if (which_.has(regina::HS_IMMERSED_SINGULAR))
        out << " embedded / immersed / singular,";
    else
        out << " unknown,";

    if (which_.has(regina::HS_VERTEX))
        out << " vertex";
    else if (which_.has(regina::HS_FUNDAMENTAL))
        out << " fundamental";
    else if (which_.has(regina::HS_CUSTOM))
        out << " custom";
    else if (which_.has(regina::HS_LEGACY))
        out << " legacy";
    else
        out << " unknown";

    out << " hypersurface";
    if (surfaces_.size() != 1)
        out << 's';
    out << " (" << forCoords(coords_, NameFunction(), "Unknown") << ')';
}

void NNormalHypersurfaceList::writeTextLong(std::ostream& out) const {
    if (which_.has(regina::HS_EMBEDDED_ONLY))
        out << "Embedded,";
    else if (which_.has(regina::HS_IMMERSED_SINGULAR))
        out << "Embedded / immersed / singular,";
    else
        out << "Unknown,";

    if (which_.has(regina::HS_VERTEX))
        out << " vertex";
    else if (which_.has(regina::HS_FUNDAMENTAL))
        out << " fundamental";
    else if (which_.has(regina::HS_CUSTOM))
        out << " custom";
    else if (which_.has(regina::HS_LEGACY))
        out << " legacy";
    else
        out << " unknown";

    out << " hypersurfaces\n";

    out << "Coordinates: " << forCoords(coords_, NameFunction(), "Unknown")
        << '\n';

    size_t n = surfaces_.size();
    out << "Number of hypersurfaces is " << n << '\n';
    for (auto s : surfaces_) {
        s->writeTextShort(out);
        out << '\n';
    }
}

void NNormalHypersurfaceList::writeXMLPacketData(std::ostream& out) const {
    // Write the surface list parameters.
    out << "  <params "
        << "type=\"" << which_.intValue() << "\" "
        << "algorithm=\"" << algorithm_.intValue() << "\" "
        << "flavourid=\"" << coords_ << "\"\n";
    out << "\tflavour=\""
        << regina::xml::xmlEncodeSpecialChars(forCoords(
           coords_, NameFunction(), "Unknown")) << "\"/>\n";

    // Write the individual hypersurfaces.
    std::vector<NNormalHypersurface*>::const_iterator it;
    for (it = surfaces_.begin(); it != surfaces_.end(); it++)
        (*it)->writeXMLData(out);
}

Packet* NNormalHypersurfaceList::internalClonePacket(Packet* /* parent */)
        const {
    NNormalHypersurfaceList* ans = new NNormalHypersurfaceList(
        coords_, which_, algorithm_);
    transform(surfaces_.begin(), surfaces_.end(), back_inserter(ans->surfaces_),
        FuncNewClonePtr<NNormalHypersurface>());
    return ans;
}

} // namespace regina

