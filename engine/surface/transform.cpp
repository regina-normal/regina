
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#include <vector>
#include "surface/normalsurfaces.h"
#include "surface/surfacefilter.h"
#include "triangulation/dim3.h"

namespace regina {

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
                if (! src.has(NS_EMBEDDED_ONLY))
                    throw FailedPrecondition("This filter requires "
                        "the input list to contain only embedded surfaces");
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
            for (const auto& a : src.surfaces_)
                for (const auto& b : src.surfaces_)
                    if (&a != &b && a.locallyCompatible(b)) {
                        surfaces_.push_back(a);
                        break;
                    }
            break;
        case NS_FILTER_DISJOINT:
            {
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
        case NS_FILTER_INCOMPRESSIBLE:
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
            (src.which_ & (NS_EMBEDDED_ONLY | NS_IMMERSED_SINGULAR)) |
                NS_CUSTOM,
            src.algorithm_ | NS_ALG_CUSTOM,
            src.triangulation_) {
    for (const NormalSurface& s : src.surfaces_)
        if (filter.accept(s))
            surfaces_.push_back(s);
}

} // namespace regina

