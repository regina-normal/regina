
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

// Regina ≤ 7.3 - the original knot signatures:
//
// - Minimise: (crossing, strand, sign) ... (crossing, strand, sign)
// - Ordering: crossing by ID; strand upper first; sign positive first
// - Text: n c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]
//
// Regina ≥ 7.4 - extending to all link diagrams:
//
// - For a connected diagram with multiple components:
//
//   * In the sequence above, insert a sentinel (n, 0, 0) between different
//     link components (but not after the final component).
//   * In the text output, include sentinels in the list of crossings (but
//     not in the strand/sign bits).
//
// - For more than one connected component:
//
//   * Build the sequence for each connected component, with each sequence
//     treated as a standalong link diagram (so we reuse crossing numbers).
//   * Sort these sequences, then concatenate the corresponding signatures.
//     The ordering (which seems natural for describing a link diagram) is by:
//     + the number of crossings, descending;
//     + the length of the sequence (i.e., # link components), descending;
//     + lexicographical ordering on the sequences themselves, ascending.
//   * If we allow reflection of the entire diagram, then we do all of this
//     once without reflection and once with reflection, and take the first
//     "sequence of sequences" under the same ordering as above.
//
// - For the special case of the empty link:
//
//   * We cannot encode the sequence [ 0 ] since this already represents the
//     0-crossing unknot: instead we cheat and give the empty link a symbol
//     that is not part of our usual base64 set (Base64SigEncoder::spare[0]).
//
// Signature creation without allowing reversal of link components is
// polynomial time in the number of crossings.  If we do allow reversal then
// we must multiply this by an exponential in the number of link components.

namespace {
    /**
     * An individual term in the (crossing, strand, sign) ... sequence
     * that we are trying to minimise when creating a signature.
     */
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

        void makeSentinel(size_t diagramSize) {
            crossing = diagramSize;
            strand = sign = 0;
        }
    };

    struct SigSequence {
        size_t crossings;
        FixedArray<SigData> data;

        SigSequence(const Link& link) :
                crossings(link.size()),
                data(2 * link.size() + link.countComponents() - 1) {
        }

        SigSequence(const SigSequence&) = default;
        SigSequence(SigSequence&&) noexcept = default;
        SigSequence& operator = (const SigSequence&) = delete;
        SigSequence& operator = (SigSequence&&) noexcept = default;

        void swap(SigSequence& other) noexcept {
            std::swap(crossings, other.crossings);
            data.swap(other.data);
        }

        bool operator < (const SigSequence& rhs) const {
            // Number of crossings, descending:
            if (crossings < rhs.crossings) return false;
            if (crossings > rhs.crossings) return true;

            // Length of the sequence, descending:
            if (data.size() < rhs.data.size()) return false;
            if (data.size() > rhs.data.size()) return true;

            // Lexicographical sequence data, ascending:
            return std::lexicographical_compare(data.begin(), data.end(),
                rhs.data.begin(), rhs.data.end());
        }
    };

    void swap(SigSequence& a, SigSequence& b) noexcept {
        a.swap(b);
    };

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

    /**
     * Computes the signature sequence for a single connected link diagram.
     *
     * \pre link is a non-empty connected diagram with at least one crossing
     * and fewer than 64 link components.
     */
    SigSequence sigSequenceConnected(const Link& link,
            BoolSet reflectionOptions, bool allowReversal) {
        const size_t n = link.size();
        Symmetries sym(link);

        // Details of the sequence we are trying to minimise,
        // including sentinels:
        SigSequence best(link);
        FixedArray<SigData> curr(best.data.size());
        bool firstAttempt = true;

        // The image and preimage for each crossing:
        FixedArray<ssize_t> image(n);
        FixedArray<ssize_t> preimage(n);

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
        FixedArray<int> seen(n);

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

                    for (sym.rotate = 0; sym.rotate < 2; ++sym.rotate) {
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
                        // need to test it against best.data[0].
                        bool currBetter = firstAttempt;

                        StrandRef s = compStart;
                        if (compReverse)
                            --s;
                        else
                            ++s;
                        for (size_t pos = 1; pos < curr.size(); ++pos) {
                            if (s == compStart && curr[pos-1].crossing != n) {
                                // We are at the start of the component, and it
                                // is not because we just started the component
                                // now.  We must have finished traversing this
                                // component.
                                curr[pos].makeSentinel(n);

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
                                if (curr[pos] < best.data[pos])
                                    currBetter = true;
                                else if (best.data[pos] < curr[pos])
                                    goto noncanonical;
                            }
                        }

                        if (currBetter) {
                            curr.swap(best.data);
                            firstAttempt = false;
                        }

                        noncanonical:
                            ;
                    }
                }
            }
        }

        return best;
    }

    /**
     * Encodes the signature sequence for a single connected link diagram.
     */
    void encodeSigSequence(Base64SigEncoder& enc, const SigSequence& seq) {
        // Text: n c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]

        // Output crossings in order.
        int charsPerInt = enc.encodeSize(seq.crossings);
        for (const auto& dat : seq.data)
            enc.encodeInt(dat.crossing, charsPerInt);

        // Output strands and signs, each as a packed sequence of bits.
        // Note: both the strands and the signs could be written using n bits
        // each, not 2n bits each (we are basically writing everything twice) -
        // however, the old knot signatured wrote 2n bits and it would be bad
        // to break compatibility with those.  Ah well.  An extra 2n bits ~ n/3
        // chars is not the end of the world: it only multiplies the length of
        // the signature by 7/6 (or less, if ints require more than one char).
        int val = 0, bit = 0;

        for (const auto& data : seq.data) {
            if (data.crossing == seq.crossings)
                continue; // this is a sentinel
            if (data.strand)
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

        for (const auto& data : seq.data) {
            if (data.crossing == seq.crossings)
                continue; // this is a sentinel
            if (data.sign > 0)
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
    }
}

std::string Link::knotSig(bool allowReflection, bool allowReversal) const {
    if (components_.size() >= 64)
        throw NotImplemented("Signatures are only implemented for "
            "fewer than 64 link components");

    // Get the zero-crossing cases out of the way first.
    if (size() == 0) {
        if (isEmpty()) {
            return { Base64SigEncoder::spare[0] };
        } else {
            // Since the diagram is connected, we must have a 0-crossing unlink.
            Base64SigEncoder enc;
            for (size_t i = 0; i < components_.size(); ++i)
                enc.encodeSize(0);
            return std::move(enc).str();
        }
    }

    // We have at least one crossing, and therefore at least one component.

    Base64SigEncoder enc;

    if (isConnected()) {
        // This is the easy case.
        encodeSigSequence(enc, sigSequenceConnected(*this,
            allowReflection ? BoolSet(true, true) /* both options */ :
                BoolSet(false) /* false only */,
            allowReversal));
    } else {
        // We need to build a sequence for each connected component.
        // For now we will not worry too much about overhead since people
        // should not be doing intense work with disconnected link diagrams
        // in practice (?).
        //
        // Do this first without reflection.
        auto components = diagramComponents();
        size_t nTrivial = 0;

        std::vector<SigSequence> bits;
        for (auto c : components) {
            if (c.size() == 0) {
                // This is a zero-crossing unknot component.
                ++nTrivial;
            } else {
                bits.push_back(sigSequenceConnected(c, { false },
                    allowReversal));
            }
        }
        std::sort(bits.begin(), bits.end());

        if (allowReflection) {
            // ... and again with reflection.
            std::vector<SigSequence> alt;
            for (auto c : components) {
                if (c.size() > 0)
                    alt.push_back(sigSequenceConnected(c, { true },
                        allowReversal));
            }
            std::sort(alt.begin(), alt.end());
            if (alt < bits)
                alt.swap(bits);
        }

        for (const auto& seq : bits)
            encodeSigSequence(enc, seq);
        for (size_t i = 0; i < nTrivial; ++i)
            enc.encodeSize(0);
    }

    return std::move(enc).str();
}

Link Link::fromKnotSig(const std::string& sig) {
    Link ans;

    Base64SigDecoder dec(sig.begin(), sig.end()); // skips leading whitespace

    // Get the empty link out of the way first.
    switch (dec.peek()) {
        case Base64SigEncoder::spare[0]:
            // This is the signature for the empty link.
            dec.skip();
            if (! dec.done())
                throw InvalidArgument("fromKnotSig(): "
                    "unexpected additional characters");
            return ans;
        case 0:
            // An empty string is _not_ the signature for the empty link.
            throw InvalidArgument("fromKnotSig(): signature is empty");
    }

    try {
        while (! dec.done()) {
            // Read one component of the link diagram at a time.
            // Note: the call to dec.done() ignores whitespace, but if there
            // _is_ internal whitespace between components then this will be
            // caught by decodeSize() below.
            auto [ n, charsPerInt ] = dec.decodeSize();
            if (n == 0) {
                // Zero-crossing unknot.
                ans.components_.emplace_back();
                continue;
            }

            FixedArray<size_t> crossing(2 * n);
            FixedArray<int> sign(2 * n);
            FixedArray<int> strand(2 * n);

            // A connected diagram with n ≥ 1 crossings can have at most n
            // link components, since each component uses ≥ 2 strands.  Here
            // compStart[i] is the index into crossing[] at which component i
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
                    // A sentinel indicating the start of a new link component.
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
                    throw InvalidArgument(
                        "fromKnotSig(): extraneous strand bits");
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
                        "fromKnotSig(): extraneous sign bits");
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
                        "fromKnotSig(): inconsistent crossing signs");
                }

                if (cr->next_[strand[i]].crossing_) {
                    throw InvalidArgument(
                        "fromKnotSig(): invalid outgoing connection");
                }

                size_t nextIdx;
                Crossing* next;
                if (i + 1 == compStart[comp + 1]) {
                    nextIdx = compStart[comp];
                    next = ans.crossings_[base + crossing[nextIdx]];
                    ans.components_.push_back(next->strand(strand[nextIdx]));
                    ++comp;
                } else {
                    nextIdx = i + 1;
                    next = ans.crossings_[base + crossing[nextIdx]];
                }
                cr->next_[strand[i]].crossing_ = next;
                cr->next_[strand[i]].strand_ = strand[nextIdx];

                if (next->prev_[strand[nextIdx]])
                    throw InvalidArgument(
                        "fromKnotSig(): invalid incoming connection");
                next->prev_[strand[nextIdx]].crossing_ = cr;
                next->prev_[strand[nextIdx]].strand_ = strand[i];
            }
        }

        return ans;
    } catch (const InvalidInput&) {
        // Any exception caught here was thrown by Base64SigDecoder.
        throw InvalidArgument(
            "fromKnotSig(): incomplete or invalid base64 encoding");
    }
}

} // namespace regina

