
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

#include "link/link.h"
#include "triangulation/detail/retriangulate-impl.h"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace regina {

namespace detail {
    template <>
    struct RetriangulateParams<Link> {
        static std::string sig(const Link& link) {
            return link.sig();
        }

        static constexpr const char* progressStage = "Exploring diagrams";

        template <class Retriangulator>
        static void propagateFrom(const std::string& sig, size_t maxSize,
                Retriangulator* retriang) {
            Link t = Link::fromSig(sig);

            if (t.size() == 0) {
                // We have a zero-crossing unlink (possibly empty).
                if (t.isEmpty() || maxSize == 0) {
                    // No moves are available at all.
                    return;
                }
                // The link is non-empty, and we are allowed to add crossings.
                {
                    // Add a twist to a single unknot component.
                    // The side does not matter, since both options are
                    // equivalent under reversal of individual link components.
                    // The sign does not matter either, since there no
                    // pre-existing crossings, and so the two options are
                    // equivalent under reflection of the entire diagram.
                    Link alt(t, false);
                    alt.r1(regina::StrandRef(), 0 /* side */, 1 /* sign */);
                    if (retriang->candidate(std::move(alt), sig))
                        return;
                }
                if (t.countComponents() >= 2 && maxSize >= 2) {
                    // Pass one unknot component over another.
                    // The sides do not matter, since the different options
                    // are equivalent under reversal of individual link
                    // components.
                    Link alt(t, false);
                    alt.r2(regina::StrandRef(), 0 /* left side */,
                        regina::StrandRef(), 0 /* left side */);
                    if (retriang->candidate(std::move(alt), sig))
                        return;
                }
                return;
            }

            // From here we assume >= 1 crossing.
            size_t i;
            int strand, side, sign;

            // Moves that reduce the number of crossings:

            for (i = 0; i < t.size(); ++i)
                if (auto alt = t.withR1(t.crossing(i)))
                    if (retriang->candidate(std::move(*alt), sig))
                        return;

            for (i = 0; i < t.size(); ++i)
                if (auto alt = t.withR2(t.crossing(i)))
                    if (retriang->candidate(std::move(*alt), sig))
                        return;

            // Moves that preserve the number of crossings:

            for (i = 0; i < t.size(); ++i)
                for (side = 0; side < 2; ++side)
                    if (auto alt = t.withR3(t.crossing(i), side))
                        if (retriang->candidate(std::move(*alt), sig))
                            return;

            // All that remains is moves that increase the number of crossings.
            if (t.size() >= maxSize)
                return;

            // We need to know the number of 0-crossing link components
            // (though not precisely - we just need to know 0 vs 1 vs ≥2.
            size_t nTrivial = 0;
            for (auto c : t.components())
                if (! c)
                    if (++nTrivial == 2)
                        break;

            // R1 twist moves on arcs are always valid.
            for (i = 0; i < t.size(); ++i)
                for (strand = 0; strand < 2; ++strand)
                    for (side = 0; side < 2; ++side)
                        for (sign = -1; sign <= 1; sign += 2) {
                            Link alt(t, false);
                            alt.r1(alt.crossing(i)->strand(strand), side, sign);
                            if (retriang->candidate(std::move(alt), sig))
                                return;
                        }
            if (nTrivial > 0) {
                for (sign = -1; sign <= 1; sign += 2) {
                    // The side does not matter, since both options are
                    // equivalent under reversal of individual link components.
                    Link alt(t, false);
                    alt.r1(regina::StrandRef(), 0, sign);
                    if (retriang->candidate(std::move(alt), sig))
                        return;
                }
            }

            if (t.size() + 1 < maxSize) {
                StrandRef upperArc, lowerArc;
                int upperSide, lowerSide;
                for (i = 0; i < t.size(); ++i)
                    for (strand = 0; strand < 2; ++strand) {
                        upperArc = t.crossing(i)->strand(strand);
                        for (upperSide = 0; upperSide < 2; ++upperSide) {
                            // Walk around the 2-cell containing upperArc.
                            // This code follows the (better documented)
                            // code in reidemeister.cpp for testing r2 validity.
                            //
                            // We walk around the 2-cell from upper, ensuring
                            // that we always turn left.
                            //
                            // At each stage we consider an edge of this 2-cell:
                            //
                            // - ref points to the strand of the crossing at the
                            //   beginning of the edge, with respect to the
                            //   direction in which we are walking around the
                            //   cell;
                            // - lowerArc points to the strand of the crossing
                            //   at the beginning of the edge, with respect to
                            //   the orientation of the link.
                            // - forward indicates whether these two directions
                            //   are the same.
                            //
                            // Note that we don't actually set lowerArc until we
                            // get near the end of the while loop.
                            //
                            StrandRef ref = upperArc;
                            bool forward;
                            if (upperSide == 0) {
                                forward = true;
                            } else {
                                // Since we are traversing the arc backwards,
                                // we need to jump to the other endpoint.
                                ref = ref.next();
                                forward = false;
                            }

                            while (true) {
                                // Move to the next edge around this 2-cell.
                                if (forward) {
                                    ref = ref.next();
                                    ref.jump();

                                    // forward remains true for (sign, strand):
                                    // +, 0
                                    // -, 1
                                    if (ref.crossing()->sign() > 0)
                                        forward = (0 == ref.strand());
                                    else
                                        forward = (0 != ref.strand());
                                } else {
                                    ref = ref.prev();
                                    ref.jump();

                                    // forward becomes true for (sign, strand):
                                    // -, 0
                                    // +, 1
                                    if (ref.crossing()->sign() > 0)
                                        forward = (0 != ref.strand());
                                    else
                                        forward = (0 == ref.strand());
                                }

                                lowerArc = (forward ? ref : ref.prev());

                                // By planarity, the 2-cell can meet one side of
                                // an arc, but never both.
                                if (lowerArc == upperArc) {
                                    // We completed the cycle.
                                    break;
                                }

                                // Try arc as the lower strand.
                                // Make sure we're on the correct side.
                                lowerSide = (forward ? 0 : 1);

                                // The r2() check is expensive when adding
                                // two crossings - use the variant where we
                                // promise that it's going to be legal.
                                Link alt(t, false);
                                alt.r2(
                                    alt.translate(upperArc), upperSide,
                                    alt.translate(lowerArc), lowerSide,
                                    regina::unprotected);
                                if (retriang->candidate(std::move(alt), sig))
                                    return;
                            }
                        }
                    }

                if (nTrivial > 0) {
                    // We can also use a 0-crossing unknot as one of the
                    // actors in our R2 move.
                    //
                    // The choice of sides does not matter here, since all
                    // options give the same diagram modulo reversal of the
                    // 0-crossing unknot component.  It does, however, matter
                    // which strand gets pushed over vs under.
                    for (i = 0; i < t.size(); ++i)
                        for (strand = 0; strand < 2; ++strand) {
                            StrandRef arc = t.crossing(i)->strand(strand);
                            {
                                Link alt(t, false);
                                alt.r2(alt.translate(arc), 0 /* left side */,
                                    StrandRef(), 0 /* left side */);
                                if (retriang->candidate(std::move(alt), sig))
                                    return;
                            }
                            {
                                Link alt(t, false);
                                alt.r2(StrandRef(), 0 /* left side */,
                                    alt.translate(arc), 0 /* left side */);
                                if (retriang->candidate(std::move(alt), sig))
                                    return;
                            }
                        }

                    if (nTrivial > 1) {
                        // Finally, try performing the R2 move on two distinct
                        // 0-crossing unknot components.  Again, all choices
                        // of side are equivalent under component reversal.
                        Link alt(t, false);
                        alt.r2(regina::StrandRef(), 0 /* left side */,
                            regina::StrandRef(), 0 /* left side */);
                        if (retriang->candidate(std::move(alt), sig))
                            return;
                    }
                }
            }
        }
    };
} // namespace detail

// Instantiate all necessary rewriting/retriangulation template functions
// so the full implementation can stay out of the headers.

template bool regina::detail::retriangulateInternal<Link, true>(
    const Link&, int, unsigned, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Link, true>&&);

template bool regina::detail::retriangulateInternal<Link, false>(
    const Link&, int, unsigned, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Link, false>&&);

} // namespace regina

