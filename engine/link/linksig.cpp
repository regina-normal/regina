
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

std::string LinkSigPrintable::encodeEmpty() {
    // We cannot encode the sequence [ 0 ] since this already represents the
    // 0-crossing unknot: instead we cheat and give the empty link a symbol
    // that is not part of our usual base64 set (Base64Encoder::spare[0]).
    return { Base64Encoder::spare[0] };
}

std::string LinkSigPrintable::encodeUnknot() {
    Base64Encoder enc;
    enc.encodeSize(0);
    return std::move(enc).str();
}

template <int generation>
requires (generation == 1 || generation == 2)
size_t LinkSigPrintable::length(const LinkSigData& data) {
    if constexpr (generation == 1) {
        size_t ans;
        if (data.size() < 63) {
            // The integer width is 1, and is not explicitly encoded.
            ans = 1 + data.sequence().size();
        } else {
            // We begin with two extra characters: 63 (a marker that the link
            // component is large), and the encoding of the integer width.
            int width = Base64Encoder::integerWidth(data.size());
            ans = 2 + (1 + data.sequence().size()) * width;
        }
        return ans + 2 * ((data.size() + 2) / 3);
    } else {
        // We encode the initial size just like first-generation sigs.
        // Everything else is bit-packed into six bits per character.
        return
            (data.size() < 63 ? 1 :
                2 + Base64Encoder::integerWidth(data.size())) +
            ((4 + bitsRequired(data.size() + 1)) * data.size() + 5) / 6;
    }
}

template <int generation>
requires (generation == 1 || generation == 2)
std::string LinkSigPrintable::encode(const LinkSigData& data) {
    if constexpr (generation == 1) {
        // The first-generation string encoding used in Regina ≤ 7.x:
        //
        // - For a non-empty connected link diagram:
        //
        //   * n c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]
        //
        //   * Sentinels (n, 0, 0) in the LinkSigData sequence appear in the
        //     list of crossings but not in the strand/sign bits.
        //

        Base64Encoder enc;
        enc.reserve(length<generation>(data));

        // Output crossings in order.
        int charsPerInt = enc.encodeSize(data.size());
        for (const auto& term : data.sequence())
            enc.encodeInt(term.crossing, charsPerInt);

        // Output strands and signs, each as a packed sequence of bits.
        // Note: both the strands and the signs could be written using n bits
        // each, not 2n bits each (we are basically writing everything twice) -
        // however, the old knot signatures wrote 2n bits and it would be bad
        // to break compatibility with those.  If memory is at a premium then
        // you should be using second-generation signatures anyway.
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
    } else {
        // We write:
        // - the integer n, encoded in a way that is compatible with
        //   first-generation signatures;
        // - 4n bits:
        //   * 2n "first time seeing this crossing?" bits (in traversal order),
        //   * n "first strand seen for this crossing" bits (in crossing order),
        //   * n sign bits (in crossing order);
        // - crossing indices for those crossings seen for the second time, as
        //   well as sentinels separating link components (in traversal order).
        //
        // By minimality, we can assume that each crossing seen for the _first_
        // time uses the next available crossing index.
        //
        // By connectivity and minimality we can assume that each crossing that
        // appears immediately after a sentinel uses the first index not yet
        // revisited.

        Base64BitEncoder enc;
        enc.reserveChars(length<generation>(data));

        enc.encodeSize(data.size());

        Bitmask bits(4 * data.size());
        FixedArray<bool> seen(data.size(), false);
        FixedArray<size_t> revisited(data.size());

        size_t pos = 0;
        size_t nextRevisit = 0;
        bool skip = false;
        for (const auto& term : data.sequence()) {
            if (term.crossing == data.size()) {
                // This is a sentinel, and should have no presence in bits.
                revisited[nextRevisit++] = data.size();
                skip = true;
            } else if (seen[term.crossing]) {
                // This is a revisited crossing: the corresponding "first time?"
                // bit should be false (which it already is).
                if (skip) {
                    // This is the first crossing after a sentinel (which is
                    // always revisited, and whose index need not be written).
                    skip = false;
                } else
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

        int intBits = bitsRequired(data.size() + 1);
        enc.encodeBitmask(bits, 4 * data.size());
        for (auto c : revisited)
            enc.encodeInt(c, intBits);

        return std::move(enc).str();
    }
}

int LinkSigPrintable::generation(const std::string& sig) {
    Base64Decoder dec(sig.begin(), sig.end()); // strips whitespace

    // Get the empty link out of the way first.
    switch (dec.peek()) {
        case Base64Encoder::spare[0]:
            return 2; // empty link
        case 0:
            return 0;
    }

    try {
        // Read through components until we find one with non-zero size.
        auto sizeAndWidth = dec.decodeSize();
        while (sizeAndWidth.first == 0) {
            // This component is a zero-crossing unknot.  Keep looking.
            if (dec.done())
                return 2; // the entire link is a zero-crossing unlink
            sizeAndWidth = dec.decodeSize();
        }

        // We have found a component with non-zero size.  The next character
        // tells us the generation (see the Link::fromSig() implementation for
        // why).
        return (dec.peek() == 'a' ? 1 : 2);
    } catch (const InvalidInput&) {
        return 0;
    }
}

ByteSequence LinkSigBinary::encodeEmpty() {
    return {};
}

ByteSequence LinkSigBinary::encodeUnknot() {
    // The encoding should just write the size (0) and nothing else.
    return { 0 };
}

template <int generation>
requires (generation == 2)
size_t LinkSigBinary::length(const LinkSigData& data) {
    int intBits = bitsRequired(data.size() + 1);

    size_t prefixBits;
    if (data.size() < 128) {
        // The link size is written to the first byte, and the integer
        // bitwidth is not encoded separately.
        prefixBits = 8;
    } else {
        // The integer bitwidth (and a marker) is written to the first byte,
        // and the link size is written separately beginning at the second byte.
        prefixBits = 8 + intBits;
    }

    return (prefixBits + (4 + intBits) * data.size() + 7) / 8;
}

template <int generation>
requires (generation == 2)
ByteSequence LinkSigBinary::encode(const LinkSigData& data) {
    // As with the second-generation LinkSigPrintable encoding, we write:
    // - the integer n (see below for how this is done);
    // - 4n bits:
    //   * 2n "first time seeing this crossing?" bits (in traversal order),
    //   * n "first strand seen for this crossing" bits (in crossing order),
    //   * n sign bits (in crossing order);
    // - crossing indices for those crossings seen for the second time,
    //   as well as sentinels separating link components (in traversal order).
    //
    // By minimality, we can assume that each crossing seen for the _first_
    // time uses the next available crossing index.
    //
    // By connectivity and minimality we can assume that each crossing that
    // appears immediately after a sentinel uses the first index not yet
    // revisited, and so we do not write that revisited index to the encoding.
    //
    // To write the initial integer n:
    // - if n < 128, we simply write n in the first byte;
    // - if n ≥ 128, we set the highest-order bit of the first byte, use the
    //   seven lower-order bits to encode the _number_ of bits b required for
    //   any integer in the range [0..n], and then encode n using b bits
    //   beginning at the second byte.
    //
    // This impose the restriction that n < 2^128, but this is wildly more
    // than enough for any link diagram.

    BitEncoder enc;
    enc.reserveBytes(length<generation>(data));

    int intBits = bitsRequired(data.size() + 1);
    if (data.size() < 128) {
        enc.encodeInt(data.size(), 8);
    } else {
        if (intBits > 127)
            throw ImpossibleScenario("LinkSigBinary::encode(): "
                "link has ≥ 2^127 crossings");
        enc.encodeInt(static_cast<unsigned>(intBits | 128), 8);
        enc.encodeInt(data.size(), intBits);
    }

    Bitmask bits(4 * data.size());
    FixedArray<bool> seen(data.size(), false);
    FixedArray<size_t> revisited(data.size());

    size_t pos = 0;
    size_t nextRevisit = 0;
    bool skip = false;
    for (const auto& term : data.sequence()) {
        if (term.crossing == data.size()) {
            // This is a sentinel, and should have no presence in bits.
            revisited[nextRevisit++] = data.size();
            skip = true;
        } else if (seen[term.crossing]) {
            // This is a revisited crossing: the corresponding "first time?"
            // bit should be false (which it already is).
            if (skip) {
                // This is the first crossing after a sentinel (which is always
                // revisited, and whose index does not need to be written).
                skip = false;
            } else
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

    enc.encodeBitmask(bits, 4 * data.size());
    for (auto c : revisited)
        enc.encodeInt(c, intBits);
    return std::move(enc).bytes();
}

std::string LinkSigBinary::asString(const ByteSequence& sig) {
    // Get the empty link out of the way first.
    if (sig.empty())
        return LinkSigPrintable::encodeEmpty();

    try {
        // Both LinkSigBinary and second-generation LinkSigPrintable encode
        // exactly the same combinatorial information; it is just a matter of
        // converting between printable (6-bit) and byte-packed (8-bit) formats.
        BitDecoder dec(sig.begin(), sig.end());
        Base64BitEncoder enc;
        while (! dec.noMoreBits()) {
            // Re-encode one connected component of the link diagram at a time.
            size_t n = dec.decodeInt<size_t>(8);
            int intBits;
            if (n & 128) {
                intBits = static_cast<int>(n ^ 128);
                if (intBits == 0)
                    throw InvalidArgument(
                        "LinkSigBinary::asString(): invalid integer bitwidth");
                n = dec.decodeInt<size_t>(intBits);
            } else {
                intBits = bitsRequired(n + 1);
            }
            enc.encodeSize(n);

            if (n > 0) {
                enc.encodeBitmask(dec.decodeBitmask(4 * n), 4 * n);
                for (size_t i = 0; i < n; ++i)
                    enc.encodeInt(dec.decodeInt<size_t>(intBits), intBits);
            }

            dec.flushByte();
            enc.flushChar();
        }
        return std::move(enc).str();
    } catch (const InvalidInput&) {
        // Any exception caught here was thrown by BitDecoder.
        throw InvalidArgument("LinkSigBinary::asString(): "
            "incomplete or invalid byte sequence encoding");
    }
}

template <CharIterator Iterator>
requires std::bidirectional_iterator<Iterator>
void Link::fillDiagramComponentFromSig1(size_t size, int intWidth,
        Base64Decoder<Iterator>& decoder) {
    if (size == 0) {
        // Zero-crossing unknot.
        components_.emplace_back();
        return;
    }

    FixedArray<size_t> crossing(2 * size);
    FixedArray<int> sign(2 * size);
    FixedArray<int> strand(2 * size);

    // A connected _virtual_ diagram with n ≥ 1 crossings can have
    // up to n+1 link components.  Here compStart[i] is the index
    // into crossing[] at which component i begins, and we terminate
    // compStart[] with an extra value of 2n.
    FixedArray<size_t> compStart(size + 2);

    size_t i = 0;    // next index into crossing[] to read
    size_t comp = 0; // current component being read
    compStart[0] = 0;
    while (i < 2 * size) {
        crossing[i] = decoder.template decodeInt<size_t>(intWidth);
        if (crossing[i] < size)
            ++i;
        else if (crossing[i] == size /* sentinel for new link component */)
            compStart[++comp] = i;
        else
            throw InvalidArgument("fromSig(): invalid destination crossing");
    }
    compStart[++comp] = 2 * size;

    for (i = 0; i < 2 * size; i += 6) {
        unsigned bits = decoder.template decodeSingle<unsigned>();
        for (int j = 0; j < 6 && i + j < 2 * size; ++j) {
            strand[i + j] = (bits & 1);
            bits >>= 1;
        }
        if (bits)
            throw InvalidArgument("fromSig(): extraneous strand bits");
    }
    for (i = 0; i < 2 * size; i += 6) {
        unsigned bits = decoder.template decodeSingle<unsigned>();
        for (int j = 0; j < 6 && i + j < 2 * size; ++j) {
            sign[i + j] = ((bits & 1) ? 1 : -1);
            bits >>= 1;
        }
        if (bits)
            throw InvalidArgument("fromSig(): extraneous sign bits");
    }

    // At this point we are finished with our base64 decoder.

    size_t base = crossings_.size();
    for (i = 0; i < size; ++i)
        crossings_.push_back(new Crossing());

    comp = 0;
    for (i = 0; i < 2 * size; ++i) {
        Crossing* cr = crossings_[base + crossing[i]];
        if (cr->sign_ == 0)
            cr->sign_ = sign[i];
        else if (cr->sign_ != sign[i])
            throw InvalidArgument("fromSig(): inconsistent crossing signs");

        if (cr->next_[strand[i]].crossing_)
            throw InvalidArgument("fromSig(): invalid outgoing connection");

        size_t nextIdx;
        Crossing* next;
        if (i + 1 == compStart[comp + 1]) {
            nextIdx = compStart[comp];
            next = crossings_[base + crossing[nextIdx]];
            components_.push_back(next->strand(strand[nextIdx]));
            ++comp;
        } else {
            nextIdx = i + 1;
            next = crossings_[base + crossing[nextIdx]];
        }
        cr->next_[strand[i]].crossing_ = next;
        cr->next_[strand[i]].strand_ = strand[nextIdx];

        if (next->prev_[strand[nextIdx]])
            throw InvalidArgument("fromSig(): invalid incoming connection");
        next->prev_[strand[nextIdx]].crossing_ = cr;
        next->prev_[strand[nextIdx]].strand_ = strand[i];
    }
}

template <CharIterator Iterator>
requires std::bidirectional_iterator<Iterator>
void Link::fillDiagramComponentFromSig2(size_t size,
        Base64BitDecoder<Iterator>& decoder) {
    if (size == 0) {
        // Zero-crossing unknot.
        components_.emplace_back();
        return;
    }

    int intBits = bitsRequired(size + 1);

    Bitmask bits = decoder.decodeBitmask(4 * size);

    size_t base = crossings_.size();
    for (size_t i = 0; i < size; ++i)
        crossings_.push_back(new Crossing(bits.get(3 * size + i) ? 1 : -1));

    FixedArray<bool> seen(size, false);
    size_t nextIndex = 0;
    size_t nextCompStart = 0;
    StrandRef prev;
    Crossing* c;
    int strand;
    for (size_t i = 0; i < 2 * size; ++i) {
        bool firstVisit = bits.get(i);
        if (firstVisit) {
            // Since the diagram component is connected and the
            // labelling is canonical, this cannot be the start of
            // a new topological link component (other than the
            // very first).
            if (nextIndex == size)
                throw InvalidArgument("fromSig(): "
                    "too many first-time crossings");
            strand = bits.get(2 * size + nextIndex) ? 1 : 0;
            c = crossings_[base + nextIndex];
            ++nextIndex;
        } else {
            size_t index = decoder.template decodeInt<size_t>(intBits);
            if (index == size) {
                // We are starting a new topological link component.
                if (prev)
                    join(prev, components_.back());
                else
                    throw InvalidArgument("fromSig(): "
                        "missing topological link component");
                prev.reset();
                // The first crossing of the new component can be
                // deduced: by connectivity and minimality, it
                // should be the first crossing not yet revisited.
                for ( ; seen[nextCompStart] ; ++nextCompStart)
                    ;
                index = nextCompStart++;
            }
            if (index >= nextIndex)
                throw InvalidArgument("fromSig(): invalid revisited crossing");
            if (seen[index])
                throw InvalidArgument("fromSig(): multiply-revisited crossing");
            seen[index] = true;
            strand = bits.get(2 * size + index) ? 0 : 1;
            c = crossings_[base + index];
        }

        StrandRef s(c, strand);
        if (prev)
            join(prev, s);
        else
            components_.push_back(s);
        prev = s;
    }
    if (prev)
        join(prev, components_.back());
    else
        throw InvalidArgument("fromSig(): missing topological link component");

    decoder.flushChar();
}

Link Link::fromSig(const std::string& sig) {
    Link ans;

    Base64Decoder dec(sig.begin(), sig.end()); // strips whitespace

    // Get the empty link out of the way first.
    switch (dec.peek()) {
        case Base64Encoder::spare[0]:
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
        // Read the size of the first component.
        // Keep going until we find a component with non-zero size.
        std::pair<size_t, int> sizeAndWidth = dec.decodeSize();
        while (sizeAndWidth.first == 0) {
            ans.components_.emplace_back(); // zero-crossing unknot
            if (dec.done())
                return ans;
            sizeAndWidth = dec.decodeSize();
        }

        // We have landed on a component with non-zero size.  The next
        // character will tell us whether we have a first-generation or
        // second-generation signature.
        //
        // - A first-gen signature must follow with 'a' (which encodes 0),
        //   since in a canonical labelling the first crossing visited must
        //   be crossing 0.  This is true regardless of the integer width,
        //   since for larger widths we encode 0 as 'aa..a'.
        //
        // - A second-gen signature must follow with something not 'a'.
        //   This is because what follows is a bit pack with bit 0 set,
        //   since the first visited crossing must be previously-unseen.

        if (dec.peek() == 'a') {
            // This is a first-generation signature.
            while (true) {
                // Finish decoding this diagram component.
                ans.fillDiagramComponentFromSig1(sizeAndWidth.first,
                    sizeAndWidth.second, dec);

                if (dec.done())
                    return ans;

                // Prepare to read the next diagram component.
                sizeAndWidth = dec.decodeSize();
            }
        } else {
            // This is a second-generation signature.
            //
            // Note: peek() above might have returned 0 (i.e., we already
            // reached the end of the encoded string); however, this will be
            // picked up as an incomplete encoding in subsequent calls to the
            // decoder.

            // For second-generation signatures, we need to switch to a
            // bitwise decoder.
            auto bitwise = dec.unreadBitDecoder();
            while (true) {
                // Finish decoding this diagram component.
                ans.fillDiagramComponentFromSig2(sizeAndWidth.first, bitwise);

                if (bitwise.noMoreBits())
                    return ans;

                // Prepare to read the next diagram component.
                sizeAndWidth.first = bitwise.decodeSize();
            }
        }
    } catch (const InvalidInput&) {
        // Any exception caught here was thrown by the decoder.
        throw InvalidArgument(
            "fromSig(): incomplete or invalid base64 encoding");
    }
}

size_t Link::sigDiagramComponentSize(const std::string& sig) {
    Base64Decoder dec(sig.begin(), sig.end()); // strips whitespace

    // Get the empty link out of the way first.
    switch (dec.peek()) {
        case Base64Encoder::spare[0]:
            return 0; // empty link
        case 0:
            throw InvalidArgument("sigDiagramComponentSize(): empty signature");
        default:
            break;
    }

    try {
        return dec.decodeSize().first;
    } catch (const InvalidInput&) {
        throw InvalidArgument("sigDiagramComponentSize(): invalid signature");
    }
}

Link Link::fromSig(const ByteSequence& sig) {
    Link ans;

    BitDecoder dec(sig.begin(), sig.end());

    // Get the empty link out of the way first.
    if (dec.noMoreBits())
        return ans;

    try {
        while (! dec.noMoreBits()) {
            // Read one connected component of the link diagram at a time.

            size_t n = dec.decodeInt<size_t>(8);
            int intBits;
            if (n & 128) {
                intBits = static_cast<int>(n ^ 128);
                if (intBits == 0)
                    throw InvalidArgument(
                        "fromSig(): invalid integer bitwidth");
                n = dec.decodeInt<size_t>(intBits);
            } else if (n == 0) {
                // Zero-crossing unknot.
                // We are already at a byte boundary; no need to flushByte().
                ans.components_.emplace_back();
                continue;
            } else {
                intBits = bitsRequired(n + 1);
            }

            // We should have a bit-packed signature for a single connected
            // diagram component with a positive number of crossings.

            Bitmask bits = dec.decodeBitmask(4 * n);
            FixedArray<size_t> revisited(n);
            for (auto& c : revisited)
                c = dec.decodeInt<size_t>(intBits);

            size_t base = ans.crossings_.size();
            for (size_t i = 0; i < n; ++i)
                ans.crossings_.push_back(
                    new Crossing(bits.get(3 * n + i) ? 1 : -1));

            FixedArray<bool> seen(n, false);
            size_t nextIndex = 0;
            size_t nextCompStart = 0;
            auto nextRevisited = revisited.begin();
            StrandRef prev;
            Crossing* c;
            int strand;
            for (size_t i = 0; i < 2 * n; ++i) {
                bool firstVisit = bits.get(i);
                if (firstVisit) {
                    // Since the diagram component is connected and the
                    // labelling is canonical, this cannot be the start of a new
                    // topological link component (other than the very first).
                    if (nextIndex == n)
                        throw InvalidArgument("fromSig(): "
                            "too many first-time crossings");
                    strand = bits.get(2 * n + nextIndex) ? 1 : 0;
                    c = ans.crossings_[base + nextIndex];
                    ++nextIndex;
                } else {
                    size_t index = *nextRevisited++;
                    if (index == n) {
                        // We are starting a new topological link component.
                        if (prev)
                            ans.join(prev, ans.components_.back());
                        else
                            throw InvalidArgument("fromSig(): "
                                "missing topological link component");
                        prev.reset();
                        // The first crossing of the new component can be
                        // deduced: by connectivity and minimality, it should
                        // be the first crossing not yet revisited.
                        for ( ; seen[nextCompStart] ; ++nextCompStart)
                            ;
                        index = nextCompStart++;
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

            dec.flushByte();
        }

        return ans;
    } catch (const InvalidInput&) {
        // Any exception caught here was thrown by BitDecoder.
        throw InvalidArgument(
            "fromSig(): incomplete or invalid byte sequence encoding");
    }
}

template std::string Link::sig<1, LinkSigPrintable>(bool, bool, bool) const;
template std::string Link::sig<2, LinkSigPrintable>(bool, bool, bool) const;
template ByteSequence Link::sig<2, LinkSigBinary>(bool, bool, bool) const;

template std::string LinkSigPrintable::encode<1>(const LinkSigData&);
template std::string LinkSigPrintable::encode<2>(const LinkSigData&);
template ByteSequence LinkSigBinary::encode<2>(const LinkSigData&);

template size_t LinkSigPrintable::length<1>(const LinkSigData&);
template size_t LinkSigPrintable::length<2>(const LinkSigData&);
template size_t LinkSigBinary::length<2>(const LinkSigData&);

} // namespace regina

