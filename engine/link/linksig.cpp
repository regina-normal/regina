
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

#include "link/linksig-impl.h"
#include "utilities/bitmask.h"
#include "utilities/fixedarray.h"
#include "utilities/sigutils.h"

namespace regina {

namespace {
    /**
     * A convenience struct that makes it easy to analyse how a link behaves
     * under a particular choice of reflection/reversal/rotation.
     *
     * This struct does _not_ initialise or maintain its
     * reflection/reversal/rotation data members - this is the responsibility
     * of the loop that iterates through them.
     *
     * We use integers for the reflect/reverse/rotate data members so that
     * these can be iterated over using a for loop.
     */
    struct Symmetries {
        /**
         * A map from strand IDs to link component numbers.
         */
        const FixedArray<size_t> compFor; // strand ID -> component number

        /**
         * 0 indicates original, 1 indicates reflected.
         */
        int reflect;
        /**
         * A bitmask where the ith bit indicates the orientation of the ith
         * componnt: 0 indicates original, and 1 indicates reversed.
         */
        uint64_t reverse;
        /**
         * 0 indicates original, 1 indicates rotated.
         */
        int rotate;

        Symmetries(const Link& link) : compFor(link.componentsByStrand()) {
        }

        bool isReversed(const StrandRef& strand) const {
            return reverse & (uint64_t(1) << compFor[strand.id()]);
        }

        int apparentStrand(const StrandRef& strand) const {
            return (rotate ? strand.strand() ^ 1 : strand.strand());
        }

        int apparentSign(Crossing* c) const {
            if (! reverse)
                return reflect ? -(c->sign()) : c->sign();

            if (isReversed(c->lower()) == isReversed(c->upper()))
                return reflect ? -(c->sign()) : c->sign();
            else
                return reflect ? c->sign() : -(c->sign());
        }
    };
}

/**
 * Computes the signature sequence for a single connected link diagram.
 *
 * \pre link is a non-empty connected diagram with at least one crossing
 * and fewer than 64 link components.
 */
LinkSigData::LinkSigData(const Link& link, BoolSet reflectionOptions,
        bool allowReversal, BoolSet rotationOptions) :
        size_(link.size()),
        sequence_(2 * link.size() + link.countComponents() - 1) {
    Symmetries sym(link);

    // Details of the sequence we are trying to minimise,
    // including sentinels:
    LinkSigData::Traversal curr(sequence_.size());
    bool firstAttempt = true;

    // The image and preimage for each crossing:
    FixedArray<ssize_t> image(size_);
    FixedArray<ssize_t> preimage(size_);

    // We can always guarantee to make the first (crossing, strand, sign)
    // tuple (0, 1, ?).  Can we make the _sign_ positive, i.e., (0, 1, 1)?
    bool startPositive;
    if (reflectionOptions.full()) {
        startPositive = true;
    } else if (allowReversal && link.countComponents() > 1) {
        // The link diagram is connected, which means there is some crossing
        // where two different components cross, and *that* crossing can be
        // made positive by reversing only one of those two link components.
        startPositive = true;
    } else {
        // We cannot change any crossing signs.
        startPositive = false;
        int aim = (reflectionOptions.hasFalse() ? 1 : -1);
        for (auto c : link.crossings())
            if (c->sign() == aim) {
                startPositive = true;
                break;
            }
    }

    // How many times have we visited each crossing?
    // (0, 1, 2, 3) = (never, lower only, upper only, both).
    // Indices are images under our relabelling.
    // Upper/lower strands are original, not rotated.
    FixedArray<int> seen(size_);

    // The orientations of all link components will be held in a 64-bit
    // bitmask (0 means original, 1 means reversed).  Here we make a
    // past-the-end value indicating when all such choices have been
    // exhausted.
    const uint64_t reverseEnd =
        (allowReversal ? (uint64_t(1) << link.countComponents()) : 1);

    // Off we go!
    for (sym.reflect = (reflectionOptions.hasFalse() ? 0 : 1);
            sym.reflect < (reflectionOptions.hasTrue() ? 2 : 1);
            ++sym.reflect) {
        for (sym.reverse = 0; sym.reverse != reverseEnd; ++sym.reverse) {
            for (auto start : link.crossings()) {
                int startSign = sym.apparentSign(start);
                if (startPositive && startSign < 0)
                    continue;

                for (sym.rotate = (rotationOptions.hasFalse() ? 0 : 1);
                        sym.rotate < (rotationOptions.hasTrue() ? 2 : 1);
                        ++sym.rotate) {
                    // Follow the link around from this starting point,
                    // using the chosen set of component orientations.

                    std::fill(image.begin(), image.end(), -1);
                    std::fill(preimage.begin(), preimage.end(), -1);
                    std::fill(seen.begin(), seen.end(), 0);

                    image[start->index()] = 0;
                    preimage[0] = start->index();
                    size_t nextUnused = 1;

                    StrandRef compStart = start->strand(sym.rotate ? 0 : 1);
                    bool compReverse = sym.isReversed(compStart);

                    curr[0].crossing = 0;
                    curr[0].strand = 1;
                    curr[0].sign = startSign;
                    seen[0] |= (compStart.strand() + 1);

                    // Since we already checked the sign of the start
                    // crossing, we know that every time we reach this
                    // point in the loop the value of curr[0] will be
                    // initialised the same way.  Therefore there is no
                    // need to test it against sequence_[0].
                    bool currBetter = firstAttempt;

                    StrandRef s = compStart;
                    if (compReverse)
                        --s;
                    else
                        ++s;
                    for (size_t pos = 1; pos < curr.size(); ++pos) {
                        if (s == compStart && curr[pos-1].crossing != size_) {
                            // We are at the start of the component, and it
                            // is not because we just started the component
                            // now.  We must have finished traversing this
                            // component.
                            curr[pos].makeSentinel(size_);

                            // Find the smallest possible starting point for
                            // the next component.  Since the link diagram
                            // is connected, this will be at a crossing that
                            // we've already seen.
                            size_t i = image[compStart.crossing()->index()];
                            while (seen[i] == 3)
                                ++i;
                            compStart = link.crossing(preimage[i])->strand(
                                seen[i] == 1 /* lower seen */ ? 1 : 0);
                            compReverse = sym.isReversed(compStart);
                            s = compStart;
                        } else {
                            size_t idx = s.crossing()->index();
                            if (image[idx] < 0) {
                                // This is a new crossing.
                                preimage[nextUnused] = idx;
                                image[idx] = nextUnused++;
                            }

                            curr[pos].crossing = image[idx];
                            curr[pos].strand = sym.apparentStrand(s);
                            curr[pos].sign = sym.apparentSign(s.crossing());
                            seen[image[idx]] |= (s.strand() + 1);

                            if (compReverse)
                                --s;
                            else
                                ++s;
                        }

                        if (! currBetter) {
                            auto c = curr[pos] <=> sequence_[pos];
                            if (c < 0)
                                currBetter = true;
                            else if (c > 0)
                                goto noncanonical;
                        }
                    }

                    if (currBetter) {
                        curr.swap(sequence_);
                        firstAttempt = false;
                    }

                    noncanonical:
                        ;
                }
            }
        }
    }
}

// The string encoding used in Regina ≤ 7.x:
//
// - For a non-empty connected link diagram:
//
//   * n c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]
//
//   * Sentinels (n, 0, 0) in the LinkSigData sequence appear in the list of
//     crossings but not in the strand/sign bits.
//
// - For the special case of the empty link:
//
//   * We cannot encode the sequence [ 0 ] since this already represents the
//     0-crossing unknot: instead we cheat and give the empty link a symbol
//     that is not part of our usual base64 set (Base64SigEncoder::spare[0]).

std::string LinkSigPrintable::encodeEmpty() {
    return { Base64SigEncoder::spare[0] };
}

std::string LinkSigPrintable::encodeUnknot() {
    Base64SigEncoder enc;
    enc.encodeSize(0);
    return std::move(enc).str();
}

size_t LinkSigPrintable::length(const LinkSigData& data) {
    size_t ans;
    if (data.size() < 63) {
        // The integer width is 1, and does not need to be explicitly encoded.
        ans = 1 + data.sequence().size();
    } else {
        // We begin with two extra characters: 63 (which acts as a marker that
        // the link component is large), and the encoding of the integer width.
        int width = Base64SigEncoder::integerWidth(data.size());
        ans = 2 + (1 + data.sequence().size()) * width;
    }
    return ans + 2 * ((data.size() + 2) / 3);
}

std::string LinkSigPrintable::encode(const LinkSigData& data) {
    // Text: n c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]

    Base64SigEncoder enc;
    enc.reserve(length(data));

    // Output crossings in order.
    int charsPerInt = enc.encodeSize(data.size());
    for (const auto& term : data.sequence())
        enc.encodeInt(term.crossing, charsPerInt);

    // Output strands and signs, each as a packed sequence of bits.
    // Note: both the strands and the signs could be written using n bits
    // each, not 2n bits each (we are basically writing everything twice) -
    // however, the old knot signatures wrote 2n bits and it would be bad
    // to break compatibility with those.  Also, if memory is at a premium
    // then you should be using LinkSigCompact (not LinkSigPrintable) anyway.
    int val = 0, bit = 0;

    for (const auto& term : data.sequence()) {
        if (term.crossing == data.size())
            continue; // this is a sentinel
        if (term.strand)
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

    for (const auto& term : data.sequence()) {
        if (term.crossing == data.size())
            continue; // this is a sentinel
        if (term.sign > 0)
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

std::string LinkSigCompact::encodeEmpty() {
    return { Base64SigEncoder::spare[0] };
}

std::string LinkSigCompact::encodeUnknot() {
    Base64SigEncoder enc;
    enc.encodeSize(0);
    return std::move(enc).str();
}

size_t LinkSigCompact::length(const LinkSigData& data) {
    size_t ans;
    if (data.size() < 63) {
        // The integer width is 1, and does not need to be explicitly encoded.
        ans = 1 + data.sequence().size() - data.size();
    } else {
        // We begin with two extra characters: 63 (which acts as a marker that
        // the link component is large), and the encoding of the integer width.
        int width = Base64SigEncoder::integerWidth(data.size());
        ans = 2 + (1 + data.sequence().size() - data.size()) * width;
    }
    return ans + (2 * data.size() + 2) / 3;
}

std::string LinkSigCompact::encode(const LinkSigData& data) {
    // We write:
    // - the integer n;
    // - 4n packed bits:
    //   * 2n "first time seeing this crossing?" bits (in traversal order),
    //   * n "first strand seen for this crossing" bits (in crossing order),
    //   * n sign bits (in crossing order);
    // - crossing indices for those crossings seen for the second time,
    //   as well as sentinels separating link components (in traversal order).
    //
    // By minimality, we can assume that each crossing seen for the _first_
    // time uses the next available crossing index.
    //
    // All 4n bits are written in a single pack (i.e., we don't artificially
    // move to the next base64 character at the 2n mark and/or the 3n mark).

    Base64SigEncoder enc;
    enc.reserve(length(data));

    int charsPerInt = enc.encodeSize(data.size());

    Bitmask bits(4 * data.size());
    FixedArray<bool> seen(data.size(), false);
    FixedArray<size_t> revisited(data.sequence().size() - data.size());

    size_t pos = 0;
    size_t nextRevisit = 0;
    for (const auto& term : data.sequence()) {
        if (term.crossing == data.size()) {
            // This is a sentinel, and should have no presence in bits.
            revisited[nextRevisit++] = data.size();
        } else if (seen[term.crossing]) {
            // This is a revisited crossing: the corresponding "first time?"
            // bit should be false (which it already is).
            revisited[nextRevisit++] = term.crossing;
            ++pos;
        } else {
            // This is a new crossing: the corresponding "first time?" bit
            // should be true, and the strand/sign bits should be set also.
            bits.set(pos, true);
            if (term.strand)
                bits.set(data.size() * 2 + term.crossing, true);
            if (term.sign > 0)
                bits.set(data.size() * 3 + term.crossing, true);
            seen[term.crossing] = true;
            ++pos;
        }
    }

    enc.encodeBits(4 * data.size(), bits);
    enc.encodeInts(revisited, charsPerInt);
    return std::move(enc).str();
}

Link Link::fromSig(const std::string& sig) {
    Link ans;

    Base64SigDecoder dec(sig.begin(), sig.end()); // strips whitespace

    // Get the empty link out of the way first.
    switch (dec.peek()) {
        case Base64SigEncoder::spare[0]:
            // This is the signature for the empty link.
            dec.skip();
            if (! dec.done())
                throw InvalidArgument("fromSig(): "
                    "unexpected additional characters");
            return ans;
        case 0:
            // An empty string is _not_ the signature for the empty link.
            throw InvalidArgument("fromSig(): signature is empty");
    }

    try {
        while (! dec.done()) {
            // Read one connected component of the link diagram at a time.

            auto [ n, charsPerInt ] = dec.decodeSize();

            // For n == 0, the compact and standard signatures are the same
            // (and contain nothing beyond the initial encoded size).
            //
            // For n > 0, looking past the initial encoded (n, charsPerInt):
            //
            // - A standard signature must follow with 'a' (which encodes 0),
            //   since in a canonical labelling the first crossing visited must
            //   be crossing 0.  This is true regardless of charsPerInt, since
            //   for larger charsPerInt we encode 0 as 'aa..a'.
            //
            // - A compact signature must follow with something other than 'a'.
            //   This is because what follows is a bit pack with bit 0 set,
            //   since the first visited crossing must be previously-unseen.

            if (n == 0) {
                // Zero-crossing unknot.
                ans.components_.emplace_back();
            } else if (dec.peek() != 'a' /* encodes 0 */) {
                // We have a compact signature for a single connected diagram
                // component with a positive number of crossings.
                //
                // Note: peek() above might have returned 0 (i.e., we already
                // reached the end of the encoded string); however, this will
                // be picked up in the call to decodeBits() immediately below.

                Bitmask bits = dec.decodeBits<Bitmask>(4 * n);

                size_t base = ans.crossings_.size();
                for (size_t i = 0; i < n; ++i)
                    ans.crossings_.push_back(
                        new Crossing(bits.get(3 * n + i) ? 1 : -1));

                // Note: since the diagram component is connected and the
                // labelling is canonical, sentinels (which separate
                // topological link components) must always be followed by a
                // revisited crossing.

                FixedArray<bool> seen(n, false);
                size_t nextIndex = 0;
                StrandRef prev;
                Crossing* c;
                int strand;
                for (size_t i = 0; i < 2 * n; ++i) {
                    bool firstVisit = bits.get(i);
                    if (firstVisit) {
                        if (nextIndex == n)
                            throw InvalidArgument("fromSig(): "
                                "too many first-time crossings");
                        strand = bits.get(2 * n + nextIndex) ? 1 : 0;
                        c = ans.crossings_[base + nextIndex];
                        ++nextIndex;
                    } else {
                        size_t index = dec.decodeInt<size_t>(charsPerInt);
                        if (index == n) {
                            // We are starting a new topological link component.
                            if (prev)
                                ans.join(prev, ans.components_.back());
                            else
                                throw InvalidArgument("fromSig(): "
                                    "missing topological link component");
                            prev.reset();
                            index = dec.decodeInt<size_t>(charsPerInt);
                        }
                        if (index >= nextIndex)
                            throw InvalidArgument("fromSig(): "
                                "invalid revisited crossing");
                        if (seen[index])
                            throw InvalidArgument("fromSig(): "
                                "multiply-revisited crossing");
                        seen[index] = true;
                        strand = bits.get(2 * n + index) ? 0 : 1;
                        c = ans.crossings_[base + index];
                    }

                    StrandRef s(c, strand);
                    if (prev)
                        ans.join(prev, s);
                    else
                        ans.components_.push_back(s);
                    prev = s;
                }
                if (prev)
                    ans.join(prev, ans.components_.back());
                else
                    throw InvalidArgument("fromSig(): "
                        "missing topological link component");
            } else {
                // We have a standard signature (the kind used before
                // Regina 8.0) for a single connected diagram component with a
                // positive number of crossings.

                FixedArray<size_t> crossing(2 * n);
                FixedArray<int> sign(2 * n);
                FixedArray<int> strand(2 * n);

                // A connected _virtual_ diagram with n ≥ 1 crossings can have
                // up to n+1 link components.  Here compStart[i] is the index
                // into crossing[] at which component i begins, and we terminate
                // compStart[] with an extra value of 2n.
                FixedArray<size_t> compStart(n + 2);

                size_t i = 0;    // next index into crossing[] to read
                size_t comp = 0; // current component being read
                compStart[0] = 0;
                while (i < 2 * n) {
                    crossing[i] = dec.decodeInt<size_t>(charsPerInt);
                    if (crossing[i] < n) {
                        ++i;
                    } else if (crossing[i] == n) {
                        // A sentinel, i.e., the start of a new link component.
                        compStart[++comp] = i;
                    } else {
                        throw InvalidArgument("fromSig(): "
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
                        throw InvalidArgument(
                            "fromSig(): extraneous strand bits");
                    }
                }
                for (i = 0; i < 2 * n; i += 6) {
                    unsigned bits = dec.decodeSingle<unsigned>();
                    for (int j = 0; j < 6 && i + j < 2 * n; ++j) {
                        sign[i + j] = ((bits & 1) ? 1 : -1);
                        bits >>= 1;
                    }
                    if (bits) {
                        throw InvalidArgument(
                            "fromSig(): extraneous sign bits");
                    }
                }

                // At this point we are finished with our base64 decoder.

                size_t base = ans.crossings_.size();
                for (i = 0; i < n; ++i)
                    ans.crossings_.push_back(new Crossing());

                comp = 0;
                for (i = 0; i < 2 * n; ++i) {
                    Crossing* cr = ans.crossings_[base + crossing[i]];
                    if (cr->sign_ == 0)
                        cr->sign_ = sign[i];
                    else if (cr->sign_ != sign[i]) {
                        throw InvalidArgument(
                            "fromSig(): inconsistent crossing signs");
                    }

                    if (cr->next_[strand[i]].crossing_) {
                        throw InvalidArgument(
                            "fromSig(): invalid outgoing connection");
                    }

                    size_t nextIdx;
                    Crossing* next;
                    if (i + 1 == compStart[comp + 1]) {
                        nextIdx = compStart[comp];
                        next = ans.crossings_[base + crossing[nextIdx]];
                        ans.components_.push_back(
                            next->strand(strand[nextIdx]));
                        ++comp;
                    } else {
                        nextIdx = i + 1;
                        next = ans.crossings_[base + crossing[nextIdx]];
                    }
                    cr->next_[strand[i]].crossing_ = next;
                    cr->next_[strand[i]].strand_ = strand[nextIdx];

                    if (next->prev_[strand[nextIdx]])
                        throw InvalidArgument(
                            "fromSig(): invalid incoming connection");
                    next->prev_[strand[nextIdx]].crossing_ = cr;
                    next->prev_[strand[nextIdx]].strand_ = strand[i];
                }
            }
        }

        return ans;
    } catch (const InvalidInput&) {
        // Any exception caught here was thrown by Base64SigDecoder.
        throw InvalidArgument(
            "fromSig(): incomplete or invalid base64 encoding");
    }
}

template std::string Link::sig<LinkSigPrintable>(bool, bool, bool) const;
template std::string Link::sig<LinkSigCompact>(bool, bool, bool) const;

} // namespace regina

