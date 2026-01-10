
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <vector>
#include "surface/normalsurfaces.h"
#include "surface/surfacefilter.h"
#include "triangulation/dim3.h"

namespace regina {

namespace {
    inline constexpr NormalCoords transformCoords(NormalCoords src,
            NormalTransform t) {
        switch (t) {
            case NormalTransform::ConvertReducedToStandard:
                switch (src) {
                    case NormalCoords::Quad: return NormalCoords::Standard;
                    case NormalCoords::QuadOct:
                        return NormalCoords::AlmostNormal;
                    default:
                        throw FailedPrecondition("Reduced-to-standard "
                            "conversion is only available from coordinate "
                            "systems NormalCoords::Quad and "
                            "NormalCoords::QuadOct");
                }
            case NormalTransform::ConvertStandardToReduced:
                switch (src) {
                    case NormalCoords::Standard: return NormalCoords::Quad;
                    case NormalCoords::AlmostNormal:
                        return NormalCoords::QuadOct;
                    default:
                        throw FailedPrecondition("Standard-to-reduced "
                            "conversion is only available from coordinate "
                            "systems NormalCoords::Standard and "
                            "NormalCoords::AlmostNormal");
                }
            default:
                return src;
        }
    }

    inline Flags<NormalList> transformList(Flags<NormalList> src,
            NormalTransform t) {
        switch (t) {
            case NormalTransform::ConvertReducedToStandard:
            case NormalTransform::ConvertStandardToReduced:
                if (src != (NormalList::EmbeddedOnly | NormalList::Vertex))
                    throw FailedPrecondition("Conversion between "
                        "standard and reduced coordinate systems requires "
                        "the source list to contain exactly all "
                        "embedded vertex surfaces");
                return src;
            case NormalTransform::FilterCompatible:
            case NormalTransform::FilterDisjoint:
            case NormalTransform::FilterIncompressible:
                if (! src.has(NormalList::EmbeddedOnly))
                    throw FailedPrecondition("This filter requires "
                        "the input list to contain only embedded surfaces");
                return NormalList::Custom | NormalList::EmbeddedOnly;
            default:
                return NormalList::Custom;
        }
    }

    inline Flags<NormalAlg> transformAlg(Flags<NormalAlg> src,
            NormalTransform t) {
        switch (t) {
            case NormalTransform::ConvertReducedToStandard:
                return src | NormalAlg::VertexViaReduced;
            default:
                return NormalAlg::Custom;
        }
    }
}

NormalSurfaces::NormalSurfaces(const NormalSurfaces& src,
        NormalTransform transform) :
        NormalSurfaces(transformCoords(src.coords_, transform),
            transformList(src.which_, transform),
            transformAlg(src.algorithm_, transform), src.triangulation_) {
    switch (transform) {
        case NormalTransform::ConvertReducedToStandard:
            if (src.triangulation_->isIdeal() ||
                    ! src.triangulation_->isValid())
                throw FailedPrecondition("Conversion from reduced to "
                    "standard coordinate systems requires a "
                    "valid, non-ideal triangulation");

            buildStandardFromReduced(src.surfaces_);
            break;
        case NormalTransform::ConvertStandardToReduced:
            if (src.triangulation_->isIdeal() ||
                    ! src.triangulation_->isValid())
                throw FailedPrecondition("Conversion from standard to "
                    "reduced coordinate systems requires a "
                    "valid, non-ideal triangulation");

            buildReducedFromStandard(src.surfaces_);
            break;
        case NormalTransform::FilterCompatible:
            for (const auto& a : src.surfaces_)
                for (const auto& b : src.surfaces_)
                    if (&a != &b && a.locallyCompatible(b)) {
                        surfaces_.push_back(a);
                        break;
                    }
            break;
        case NormalTransform::FilterDisjoint:
            {
                // Note: the calls to isConnected() and disjoint() could throw
                // an UnsolvedCase exception.
                std::vector<const NormalSurface*> interesting;
                for (const NormalSurface& s : src.surfaces_)
                    if ((! s.isEmpty()) && s.isCompact() && s.isConnected())
                        interesting.push_back(&s);
                for (const auto* a : interesting)
                    for (const auto* b : interesting)
                        if (a != b && a->disjoint(*b)) {
                            surfaces_.push_back(*a);
                            break;
                        }
            }
            break;
        case NormalTransform::FilterIncompressible:
            for (const NormalSurface& s : src.surfaces_) {
                if (s.isVertexLinking() || s.isThinEdgeLink().first)
                    continue;

                // If we have a one-sided surface, don't worry about taking the
                // two-sided double cover.  If the complement of the one-sided
                // surface has a compressing disc, then the complement of the
                // double cover has the same compressing disc, and this surface
                // can happily be tossed away.
                if (! s.cutAlong().hasSimpleCompressingDisc())
                    surfaces_.push_back(s);
            }
            break;
        default:
            throw FailedPrecondition(
                "The transformation type was not recognised");
    }
}

NormalSurfaces::NormalSurfaces(const NormalSurfaces& src,
        const SurfaceFilter& filter) :
        NormalSurfaces(
            src.coords_,
            (src.which_ &
                (NormalList::EmbeddedOnly | NormalList::ImmersedSingular)) |
                NormalList::Custom,
            src.algorithm_ | NormalAlg::Custom,
            src.triangulation_) {
    for (const NormalSurface& s : src.surfaces_)
        if (filter.accept(s))
            surfaces_.push_back(s);
}

} // namespace regina

