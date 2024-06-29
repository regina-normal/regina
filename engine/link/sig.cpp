
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
#include "utilities/fixedarray.h"
#include "utilities/sigutils.h"

namespace regina {

namespace {
    struct SigData {
        size_t crossing;
        int strand;
        int sign;

        bool operator < (const SigData& rhs) const {
            if (crossing < rhs.crossing) return true;
            if (crossing > rhs.crossing) return false;
            if (strand > rhs.strand) return true; /* upper first */
            if (strand < rhs.strand) return false;
            if (sign > rhs.sign) return true; /* positive first */
            return false;
        }
    };

    struct ComponentData {
        StrandRef start;
        bool reverse;
    };
}

std::string Link::knotSig(bool allowReflection, bool allowReversal) const {
    if (! isConnected())
        throw NotImplemented(
            "Signatures are only implemented for connected link diagrams");

    // The original knot signatures (Regina ≤ 7.3):
    //
    // - Minimise: (crossing, strand, sign) ... (crossing, strand, sign)
    // - Ordering: crossing by ID; strand upper first; sign positive first
    // - Text: n c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]
    //
    // Extending to connected diagrams with multiple components (Regina ≥ 7.4):
    //
    // - In the sequence above, insert a sentinel (n, 0, 0) between different
    //   link components (but not after the final component);
    // - In the text output, include sentinels in the list of crossings (but
    //   not in the strand/sign bits);
    // - Under these rules, the empty link needs to be special-cased since the
    //   sequence [ 0 ] already represents the 0-crossing unknot: here we
    //   cheat and give the empty link a symbol that is not part of our usual
    //   base64 set (Base64SigEncoder::spare[0]).

    // Get the zero-crossing cases out of the way first.
    if (size() == 0) {
        if (isEmpty()) {
            return { Base64SigEncoder::spare[0] };
        } else {
            // Since the diagram is connected, we must have a 0-crossing unknot.
            Base64SigEncoder enc;
            enc.encodeSize(0);
            return enc.str();
        }
    }

    // We have at least one crossing, and at least one component.

    size_t n = crossings_.size();
    FixedArray<SigData> best(2 * n + components_.size() - 1);
    FixedArray<SigData> curr(2 * n + components_.size() - 1);
    bool firstAttempt = true;

    // The image and preimage for each crossing.
    FixedArray<ssize_t> image(n, -1);
    FixedArray<ssize_t> preimage(n, -1);

    // How many times have we visited each crossing?
    // (0, 1, 2, 3) = (never, lower only, upper only, both).
    // Indices are images under our relabelling.
    // Upper/lower strands are original, not rotated.
    FixedArray<int> seen(n, 0);

    // Information about each component that we process.
    FixedArray<ComponentData> comp(components_.size());

    for (int reflect = 0; reflect < 2; ++reflect) {
        for (int rotate = 0; rotate < 2; ++rotate) {
            // Find a starting point for the first component.
            for (auto start : crossings_) {
                // If we are allowed to reflect, then we can guarantee to
                // start with a positive crossing.
                if (allowReflection && start->sign() == (reflect ? 1 : -1))
                    continue;

                // We will map start to crossing 0, and we will always start
                // on the upper strand (after taking into account rotation).
                image[start->index()] = 0;
                preimage[0] = start->index();
                size_t nextUnused = 1;

                // Prepare to make choices for traversing the this component.
                comp[0].start = start->strand(rotate ? 0 : 1);
                comp[0].reverse = false;

                ssize_t doneComp = 0;
                size_t pos = 0;
                while (doneComp < components_.size()) {
                    // Traverse comp[doneComp].
                    StrandRef s = comp[doneComp].start;
                    while (true) {
                        seen[image[s.crossing()->index()]] |= (s.strand() + 1);

                        curr[pos].crossing = image[s.crossing()->index()];
                        curr[pos].strand = (rotate ?
                            (s.strand() ^ 1) : s.strand());
                        curr[pos].sign = (reflect ?
                            -s.crossing()->sign() : s.crossing()->sign());

                        // TODO: check for non-canonical

                        if (comp[doneComp].reverse)
                            --s;
                        else
                            ++s;
                        ++pos;
                        if (s == comp[doneComp].start)
                            break;

                        if (image[s.crossing()->index()] < 0) {
                            // This is a new crossing.
                            preimage[nextUnused] = s.crossing()->index();
                            image[s.crossing()->index()] = nextUnused++;
                        }
                    }

                    ++doneComp;
                    if (doneComp < components_.size()) {
                        // Find the strand to start the next component from.
                        // This should be the lowest index crossing, under our
                        // relabelling, which has been visited only once.
                        // Since the diagram is connected, such a crossing
                        // should exist.
                        //
                        // TODO: We can probably do this in amortised constant
                        // time instead of the linear search we use here..?
                        for (size_t i = 0; i < size(); ++i)
                            if (seen[i] == 1) {
                                comp[doneComp].start =
                                    crossings_[preimage[i]]->upper();
                                break;
                            } else if (seen[i] == 2) {
                                comp[doneComp].start =
                                    crossings_[preimage[i]]->lower();
                                break;
                            } else if (seen[i] == 0) {
                                // This should never happen - it means the
                                // diagram was disconnected, and we have
                                // already tested for this.
                                throw NotImplemented(
                                    "Link::knotSig() is seeing behaviour "
                                    "that should only occur for a "
                                    "disconnected link diagram");
                            }

                        comp[doneComp].reverse = false;

                        // Add the sentinel between components.
                        curr[pos].crossing = n;
                        curr[pos].strand = curr[pos].sign = 0;
                        ++pos;

                        continue;
                    }

                    // We have a complete labelling.
                    // TODO: Replace this with a piecewise test.
                    if (firstAttempt) {
                        std::copy(curr.begin(), curr.end(), best.begin());
                        firstAttempt = false;
                    } else if (std::lexicographical_compare(
                            curr.begin(), curr.end(),
                            best.begin(), best.end())) {
                        std::copy(curr.begin(), curr.end(), best.begin());
                    }

wind_back:
                    --doneComp;
                    while (doneComp >= 0) {
                        // Wind back comp[doneComp].
                        // The direction of traversal does not matter here.
                        StrandRef s = comp[doneComp].start;
                        do {
                            size_t idx = s.crossing()->index();
                            size_t idxImg = image[idx];
                            if (seen[idxImg] == 3)
                                seen[idxImg] ^= (s.strand() + 1);
                            else {
                                // This crossing becomes unseen.
                                seen[idxImg] = 0;

                                // Note: we shouldn't remove image/preimage
                                // for the starting point of diagram traversal,
                                // since this is managed separately.
                                if (idxImg > 0) {
                                    image[idx] = -1;
                                    preimage[idxImg] = -1;
                                    if (nextUnused > idxImg)
                                        nextUnused = idxImg;
                                }
                            }
                            ++s;
                            --pos;
                        } while (s != comp[doneComp].start);

                        if (allowReversal && ! comp[doneComp].reverse) {
                            // Try this same component in the other direction.
                            comp[doneComp].reverse = true;
                            break;
                        } else {
                            // We are done with this component.
                            --doneComp;
                            --pos; // moves back past the sentinel
                        }
                    }

                    // Loop around again to traverse comp[doneComp].
                }

                // Time to try a different starting point.
                image[start->index()] = -1;
                preimage[0] = -1;
            }

            // Here we move on to the next choice of rotate.
        }

        // Here we move on to the next choice of reflect.
        if (! allowReflection)
            break;
    }

    // Text: n c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]
    Base64SigEncoder enc;

    // Output crossings in order.
    int charsPerInt = enc.encodeSize(n);
    for (const auto& dat : best)
        enc.encodeInt(dat.crossing, charsPerInt);

    // Output strands and signs, each as a packed sequence of bits.
    // Note: both the strands and the signs could be written using n bits each,
    // not 2n bits each (we are basically writing everything twice) - however,
    // the old knot signatured wrote 2n bits and it would be bad to break
    // compatibility with those.  Ah well.  An extra 2n bits ~ n/3 chars
    // is not the end of the world: it only multiplies the length of the
    // signature by 7/6 (or less, if ints require more than one char).
    int val = 0, bit = 0;

    for (size_t i = 0; i < 2 * n + components_.size() - 1; ++i) {
        if (best[i].crossing == n)
            continue; // this is a sentinel
        if (best[i].strand)
            val |= (1 << bit);
        if (++bit == 6) {
            enc.encodeSingle(val);
            val = bit = 0;
        }
    }
    if (bit) {
        enc.encodeSingle(val);
        val = bit = 0;
    }

    for (size_t i = 0; i < 2 * n + components_.size() - 1; ++i) {
        if (best[i].crossing == n)
            continue; // this is a sentinel
        if (best[i].sign > 0)
            val |= (1 << bit);
        if (++bit == 6) {
            enc.encodeSingle(val);
            val = bit = 0;
        }
    }
    if (bit) {
        enc.encodeSingle(val);
        val = bit = 0; // we could drop this, but it helps for readability.
    }

    return std::move(enc).str();
}

Link Link::fromKnotSig(const std::string& sig) {
    Link ans;

    Base64SigDecoder dec(sig.begin(), sig.end()); // skips leading whitespace

    // Get the empty link out of the way first.
    if (dec.peek() == Base64SigEncoder::spare[0]) {
        dec.skip();
        if (! dec.done())
            throw InvalidArgument("fromKnotSig(): "
                "Unexpected additional characters");
        return ans;
    }

    try {
        auto [ n, charsPerInt ] = dec.decodeSize();
        if (n == 0) {
            // Zero-crossing unknot.
            if (! dec.done())
                throw InvalidArgument("fromKnotSig(): "
                    "Unexpected additional characters");
            ans.components_.emplace_back();
            return ans;
        }

        FixedArray<size_t> crossing(2 * n);
        FixedArray<int> sign(2 * n);
        FixedArray<int> strand(2 * n);

        // A connected diagram with n ≥ 1 crossings can have at most
        // n link components, since each component uses ≥ 2 strands.
        // Here compStart[i] is the index into crossing[] at which component i
        // begins, and we terminate compStart[] with an extra value of 2n.
        FixedArray<size_t> compStart(n + 1);

        size_t i = 0;    // next index into crossing[] to read
        size_t comp = 0; // current component being read
        compStart[0] = 0;
        while (i < 2 * n) {
            crossing[i] = dec.decodeInt<size_t>(charsPerInt);
            if (crossing[i] < n) {
                ++i;
            } else if (crossing[i] == n) {
                // A sentinel that indicates the start of a new link component.
                compStart[++comp] = i;
            } else {
                throw InvalidArgument("fromKnotSig(): "
                    "invalid destination crossing");
            }
        }
        compStart[++comp] = 2 * n;

        for (i = 0; i < 2 * n; i += 6) {
            unsigned bits = dec.decodeSingle<unsigned>();
            for (int j = 0; j < 6 && i + j < 2 * n; ++j) {
                strand[i + j] = (bits & 1);
                bits >>= 1;
            }
            if (bits) {
                throw InvalidArgument("fromKnotSig(): extraneous strand bits");
            }
        }
        for (i = 0; i < 2 * n; i += 6) {
            unsigned bits = dec.decodeSingle<unsigned>();
            for (int j = 0; j < 6 && i + j < 2 * n; ++j) {
                sign[i + j] = ((bits & 1) ? 1 : -1);
                bits >>= 1;
            }
            if (bits) {
                throw InvalidArgument("fromKnotSig(): extraneous sign bits");
            }
        }

        if (! dec.done())
            throw InvalidArgument("fromKnotSig(): "
                "Unexpected additional characters");

        // At this point we are finished with our base64 decoder.

        for (i = 0; i < n; ++i)
            ans.crossings_.push_back(new Crossing());

        comp = 0;
        for (i = 0; i < 2 * n; ++i) {
            Crossing* cr = ans.crossings_[crossing[i]];
            if (cr->sign_ == 0)
                cr->sign_ = sign[i];
            else if (cr->sign_ != sign[i]) {
                throw InvalidArgument(
                    "fromKnotSig(): inconsistent crossing signs");
            }

            if (cr->next_[strand[i]].crossing_) {
                throw InvalidArgument(
                    "fromKnotSig(): invalid outgoing connection");
            }

            size_t nextIdx;
            if (i + 1 == compStart[comp + 1]) {
                nextIdx = compStart[comp];
                ans.components_.push_back(
                    ans.crossings_[crossing[nextIdx]]->strand(strand[nextIdx]));
                ++comp;
            } else {
                nextIdx = i + 1;
            }
            cr->next_[strand[i]].crossing_ = ans.crossings_[crossing[nextIdx]];
            cr->next_[strand[i]].strand_ = strand[nextIdx];

            if (ans.crossings_[crossing[nextIdx]]->prev_[strand[nextIdx]]) {
                throw InvalidArgument(
                    "fromKnotSig(): invalid incoming connection");
            }
            ans.crossings_[crossing[nextIdx]]->
                prev_[strand[nextIdx]].crossing_ = cr;
            ans.crossings_[crossing[nextIdx]]->
                prev_[strand[nextIdx]].strand_ = strand[i];
        }

        return ans;
    } catch (const InvalidInput&) {
        // Any exception caught here was thrown by Base64SigDecoder.
        throw InvalidArgument(
            "fromKnotSig(): incomplete or invalid base64 encoding");
    }
}

} // namespace regina

