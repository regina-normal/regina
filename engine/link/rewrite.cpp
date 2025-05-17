
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
    /**
     * Provides domain-specific details for the link rewriting process.
     *
     * For link propagation, we do make use of the options type
     * `Retriangulator::Options`.  This type should be one of:
     *
     * - `std::true_type`, to indicate that only classical Reidemeister moves
     *   should be allowed;
     *
     * - `std::false_type`, to indicate that both classical and virtual
     *   Reidemeister moves should be allowed.
     */
    template <>
    struct RetriangulateParams<Link> {
        static std::string sig(const Link& link) {
            return link.sig();
        }

        static constexpr const char* progressStage = "Exploring diagrams";

        template <class Retriangulator>
        static void propagateFrom(const std::string& sig, size_t maxSize,
                Retriangulator* retri) {
            constexpr bool classicalOnly = Retriangulator::Options::value;

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
                    alt.r1(StrandRef(), 0 /* side */, 1 /* sign */);
                    if (retri->candidate(std::move(alt), sig))
                        return;
                }
                if constexpr (! classicalOnly) {
                    if (maxSize > 1) {
                        // There are only two essentially different diagrams
                        // that we can obtain from a zero-crossing unknot
                        // using a virtual R2.  These are obtained via
                        // (firstSide == firstStrand) and
                        // (firstSide != firstStrand).
                        for (int firstSide = 0; firstSide < 2; ++firstSide) {
                            Link alt(t, false);
                            alt.r2Virtual({}, firstSide, 1 /* firstStrand */);
                            if (retri->candidate(std::move(alt), sig))
                                return;
                        }
                    }
                }
                // We promise not to merge diagram components, so we do not
                // consider moves that pass one unknot component over another.
                return;
            }

            // From here we assume >= 1 crossing.

            // Moves that reduce the number of crossings:

            for (size_t i = 0; i < t.size(); ++i)
                if (auto alt = t.withR1(t.crossing(i)))
                    if (retri->candidate(std::move(*alt), sig))
                        return;

            for (size_t i = 0; i < t.size(); ++i)
                if (auto alt = t.withR2(t.crossing(i)))
                    if (retri->candidate(std::move(*alt), sig))
                        return;

            // Moves that preserve the number of crossings:

            for (size_t i = 0; i < t.size(); ++i)
                for (int side = 0; side < 2; ++side)
                    if (auto alt = t.withR3(t.crossing(i), side))
                        if (retri->candidate(std::move(*alt), sig))
                            return;

            // All that remains is moves that increase the number of crossings.
            if (t.size() >= maxSize)
                return;

            // We need to know whether there are any 0-crossing link components.
            bool hasTrivial = false;
            for (auto c : t.components())
                if (! c) {
                    hasTrivial = true;
                    break;
                }

            // R1 twist moves on arcs are always valid.
            for (size_t i = 0; i < t.size(); ++i)
                for (int strand = 0; strand < 2; ++strand)
                    for (int side = 0; side < 2; ++side)
                        for (int sign = -1; sign <= 1; sign += 2) {
                            Link alt(t, false);
                            alt.r1(alt.crossing(i)->strand(strand), side, sign);
                            if (retri->candidate(std::move(alt), sig))
                                return;
                        }
            if (hasTrivial) {
                for (int sign = -1; sign <= 1; sign += 2) {
                    // The side does not matter, since both options are
                    // equivalent under reversal of individual link components.
                    Link alt(t, false);
                    alt.r1(StrandRef(), 0, sign);
                    if (retri->candidate(std::move(alt), sig))
                        return;
                }
            }

            if (t.size() + 1 < maxSize) {
                if constexpr (! classicalOnly) {
                    // Testing for virtual R2 moves is very fast, and these
                    // moves (as enumerated below) are always valid.
                    // However, we do have to be sure not to mix different
                    // diagram components.

                    // Moves that work on different strands:
                    auto [ comp, nComp ] = t.diagramComponentIndices();
                    for (size_t cr1 = 0; cr1 < t.size(); ++cr1) {
                        for (size_t cr2 = 0; cr2 < t.size(); ++cr2) {
                            if (comp[cr1] != comp[cr2])
                                continue;

                            for (int str1 = 0; str1 < 2; ++str1) {
                                for (int str2 = 0; str2 < 2; ++str2) {
                                    // Do not operate on the same strand.
                                    if (cr1 == cr2 && str1 == str2)
                                        continue;

                                    for (int side1 = 0; side1 < 2; ++side1) {
                                        for (int side2 = 0; side2 < 2; ++side2) {
                                            Link alt(t, false);
                                            alt.r2Virtual(
                                                alt.crossing(cr1)->strand(str1),
                                                side1,
                                                alt.crossing(cr2)->strand(str2),
                                                side2);
                                            if (retri->candidate(
                                                    std::move(alt), sig))
                                                return;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // Moves that work on the same strand:
                    for (size_t cr = 0; cr < t.size(); ++cr)
                        for (int strand = 0; strand < 2; ++strand)
                            for (int fSide = 0; fSide < 2; ++fSide)
                                for (int fStrand = 0; fStrand < 2; ++fStrand) {
                                    Link alt(t, false);
                                    alt.r2Virtual(
                                        alt.crossing(cr)->strand(strand),
                                        fSide, fStrand);
                                    if (retri->candidate(std::move(alt), sig))
                                        return;
                                }
                    if (hasTrivial) {
                        // There are only two possible diagrams that can come
                        // from a virtual R2 on a zero-crossing unknot:
                        // one with firstSide == firstStrand, and
                        // one with firstSide != firstStrand.
                        for (int firstSide = 0; firstSide < 2; ++firstSide) {
                            Link alt(t, false);
                            alt.r2Virtual({}, firstSide, 1 /* firstStrand */);
                            if (retri->candidate(std::move(alt), sig))
                                return;
                        }
                    }
                } else {
                    // We are restricting ourselves to classical moves.
                    for (size_t i = 0; i < t.size(); ++i)
                        for (int strand = 0; strand < 2; ++strand) {
                            StrandRef uArc = t.crossing(i)->strand(strand);
                            for (int uSide = 0; uSide < 2; ++uSide) {
                                // Walk around the 2-cell containing uArc.
                                // This code follows the (better documented)
                                // code in reidemeister.cpp for testing r2
                                // validity.
                                //
                                // We walk around the 2-cell from upper,
                                // ensuring that we always turn left.
                                //
                                // At each stage we consider an edge of this
                                // 2-cell:
                                //
                                // - ref points to the strand of the crossing at
                                //   the beginning of the edge, with respect to
                                //   the direction in which we are walking
                                //   around the cell;
                                // - lArc points to the strand of the crossing
                                //   at the beginning of the edge, with respect
                                //   to the orientation of the link;
                                // - fwd indicates whether these two
                                //   directions are the same.
                                //
                                // Note that we don't actually set lArc
                                // until we get near the end of the while loop.
                                //
                                StrandRef ref = uArc;
                                bool fwd;
                                if (uSide == 0) {
                                    fwd = true;
                                } else {
                                    // We are traversing the arc backwards, so
                                    // we need to jump to the other endpoint.
                                    ref = ref.next();
                                    fwd = false;
                                }

                                while (true) {
                                    // Move to the next edge around this 2-cell.
                                    if (fwd) {
                                        ref = ref.next();
                                        ref.jump();

                                        // fwd remains true iff
                                        // (sign, strand) == (+, 0) or (-, 1).
                                        if (ref.crossing()->sign() > 0)
                                            fwd = (0 == ref.strand());
                                        else
                                            fwd = (0 != ref.strand());
                                    } else {
                                        ref = ref.prev();
                                        ref.jump();

                                        // fwd becomes true iff
                                        // (sign, strand) == (-, 0) or (+, 1).
                                        if (ref.crossing()->sign() > 0)
                                            fwd = (0 != ref.strand());
                                        else
                                            fwd = (0 == ref.strand());
                                    }

                                    StrandRef lArc = (fwd ? ref : ref.prev());
                                    int lSide = (fwd ? 0 : 1);

                                    if (lArc == uArc && lSide == uSide) {
                                        // We completed the cycle.
                                        break;
                                    }

                                    // The r2() check is expensive when adding
                                    // two crossings.  We already know this move
                                    // is legal (in the classical sense), so use
                                    // r2Virtual() instead which avoids the
                                    // expensive planarity test.
                                    Link alt(t, false);
                                    alt.r2Virtual(
                                        alt.translate(uArc), uSide,
                                        alt.translate(lArc), lSide);
                                    if (retri->candidate(std::move(alt), sig))
                                        return;
                                }
                            }
                        }

                    // We promise not to merge diagram components, so we do not
                    // consider moves that pass an unknot component over some
                    // other component.
                }
            }
        }
    };
} // namespace detail

// Instantiate all necessary rewriting/retriangulation template functions
// so the full implementation can stay out of the headers.

template bool detail::retriangulateInternal<Link, true, std::true_type>(
    const Link&, int, unsigned, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Link, true>&&);

template bool detail::retriangulateInternal<Link, false, std::true_type>(
    const Link&, int, unsigned, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Link, false>&&);

template bool detail::retriangulateInternal<Link, true, std::false_type>(
    const Link&, int, unsigned, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Link, true>&&);

template bool detail::retriangulateInternal<Link, false, std::false_type>(
    const Link&, int, unsigned, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Link, false>&&);

} // namespace regina

