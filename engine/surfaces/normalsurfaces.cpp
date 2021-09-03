
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

#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"
#include "utilities/xmlutils.h"

namespace regina {

// This should really be inline.  However, when inline, it seems to
// trigger an instantiation of the generic Triangulation<3> as opposed
// to the specialised Triangulation<3>.  A forward declaration of the
// specialisation is not enough to stop it.  I wish I understood how to
// avoid this, but in the meantime, here we are.
MatrixInt NormalSurfaces::recreateMatchingEquations() const {
    // Although makeMatchingEquations() returns a std::optional, we are
    // guaranteed in our scenario here that this will always contain a value.
    return *makeMatchingEquations(triangulation(), coords_);
}

namespace {
    inline constexpr NormalCoords transformCoords(NormalCoords src,
            NormalTransform t) {
        switch (t) {
            case NS_CONV_REDUCED_TO_STD:
                switch (src) {
                    case NS_QUAD: return NS_STANDARD;
                    case NS_AN_QUAD_OCT: return NS_AN_STANDARD;
                    default:
                        throw FailedPrecondition("Reduced-to-standard "
                            "conversion is only available from coordinate "
                            "systems NS_QUAD and NS_AN_QUAD_OCT");
                }
            case NS_CONV_STD_TO_REDUCED:
                switch (src) {
                    case NS_STANDARD: return NS_QUAD;
                    case NS_AN_STANDARD: return NS_AN_QUAD_OCT;
                    default:
                        throw FailedPrecondition("Standard-to-reduced "
                            "conversion is only available from coordinate "
                            "systems NS_STANDARD and NS_AN_STANDARD");
                }
            default:
                return src;
        }
    }

    inline NormalList transformList(NormalList src, NormalTransform t) {
        switch (t) {
            case NS_CONV_REDUCED_TO_STD:
            case NS_CONV_STD_TO_REDUCED:
                if (src != (NS_EMBEDDED_ONLY | NS_VERTEX))
                    throw FailedPrecondition("Conversion between "
                        "standard and reduced coordinate systems requires "
                        "the source list to contain exactly all "
                        "embedded vertex surfaces");
                return src;
            case NS_FILTER_COMPATIBLE:
            case NS_FILTER_DISJOINT:
            case NS_FILTER_INCOMPRESSIBLE:
                return NS_CUSTOM | NS_EMBEDDED_ONLY;
            default:
                return NS_CUSTOM;
        }
    }

    inline NormalAlg transformAlg(NormalAlg src, NormalTransform t) {
        switch (t) {
            case NS_CONV_REDUCED_TO_STD:
                return src | NS_VERTEX_VIA_REDUCED;
            default:
                return NS_ALG_CUSTOM;
        }
    }
}

NormalSurfaces::NormalSurfaces(const NormalSurfaces& src,
        NormalTransform transform) :
        NormalSurfaces(transformCoords(src.coords_, transform),
            transformList(src.which_, transform),
            transformAlg(src.algorithm_, transform), src.triangulation_) {
    switch (transform) {
        case NS_CONV_REDUCED_TO_STD:
            if (src.triangulation_->isIdeal() ||
                    ! src.triangulation_->isValid())
                throw FailedPrecondition("Conversion from reduced to "
                    "standard coordinate systems requires a "
                    "valid, non-ideal triangulation");

            buildStandardFromReduced(src.surfaces_);
            break;
        case NS_CONV_STD_TO_REDUCED:
            if (src.triangulation_->isIdeal() ||
                    ! src.triangulation_->isValid())
                throw FailedPrecondition("Conversion from standard to "
                    "reduced coordinate systems requires a "
                    "valid, non-ideal triangulation");

            buildReducedFromStandard(src.surfaces_);
            break;
        case NS_FILTER_COMPATIBLE:
        case NS_FILTER_DISJOINT:
        case NS_FILTER_INCOMPRESSIBLE:
            throw FailedPrecondition(
                "TODO: Not yet implemented");
        default:
            throw FailedPrecondition(
                "The transformation type was not recognised");
    }
}

void NormalSurfaces::writeAllSurfaces(std::ostream& out) const {
    out << "Number of surfaces is " << size() << '\n';
    for (const NormalSurface& s : surfaces_) {
        s.writeTextShort(out);
        out << '\n';
    }
}

void NormalSurfaces::writeTextShort(std::ostream& out) const {
    out << surfaces_.size();

    if (which_.has(regina::NS_EMBEDDED_ONLY))
        out << " embedded,";
    else if (which_.has(regina::NS_IMMERSED_SINGULAR))
        out << " embedded / immersed / singular,";
    else
        out << " unknown,";

    if (which_.has(regina::NS_VERTEX))
        out << " vertex";
    else if (which_.has(regina::NS_FUNDAMENTAL))
        out << " fundamental";
    else if (which_.has(regina::NS_CUSTOM))
        out << " custom";
    else if (which_.has(regina::NS_LEGACY))
        out << " legacy";
    else
        out << " unknown";

    out << " surface";
    if (surfaces_.size() != 1)
        out << 's';

    out << " (" << NormalInfo::name(coords_) << ')';
}

void NormalSurfaces::writeTextLong(std::ostream& out) const {
    if (which_.has(regina::NS_EMBEDDED_ONLY))
        out << "Embedded,";
    else if (which_.has(regina::NS_IMMERSED_SINGULAR))
        out << "Embedded / immersed / singular,";
    else
        out << "Unknown,";

    if (which_.has(regina::NS_VERTEX))
        out << " vertex";
    else if (which_.has(regina::NS_FUNDAMENTAL))
        out << " fundamental";
    else if (which_.has(regina::NS_CUSTOM))
        out << " custom";
    else if (which_.has(regina::NS_LEGACY))
        out << " legacy";
    else
        out << " unknown";

    out << " surfaces\n";

    out << "Coordinates: " << NormalInfo::name(coords_) << '\n';

    writeAllSurfaces(out);
}

void NormalSurfaces::writeXMLPacketData(std::ostream& out) const {
    // Write the surface list parameters.
    out << "  <params "
        << "type=\"" << which_.intValue() << "\" "
        << "algorithm=\"" << algorithm_.intValue() << "\" "
        << "flavourid=\"" << coords_ << "\"\n";
    out << "\tflavour=\""
        << regina::xml::xmlEncodeSpecialChars(NormalInfo::name(coords_))
        << "\"/>\n";

    // Write the individual surfaces.
    for (const NormalSurface& s : surfaces_)
        s.writeXMLData(out);
}

Packet* NormalSurfaces::internalClonePacket(Packet* parent) const {
    NormalSurfaces* ans = new NormalSurfaces(
        coords_, which_, algorithm_, *static_cast<Triangulation<3>*>(parent));
    for (const NormalSurface& s : surfaces_)
        ans->surfaces_.push_back(NormalSurface(s, ans->triangulation_));
    return ans;
}

} // namespace regina

