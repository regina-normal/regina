
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
}

std::string Link::knotSig(bool allowReflection, bool allowReversal) const {
    // Only defined for knots at present.
    if (components_.size() != 1)
        throw NotImplemented(
            "Knot signatures are only implemented for single-component links");

    // Minimise sequence: (crossing, strand, sign) ... (crossing, strand, sign)
    // Ordering: crossing by ID; strand upper first; sign positive first
    // Text: n c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]
    size_t n = crossings_.size();
    FixedArray<SigData> best(2 * n);
    FixedArray<SigData> curr(2 * n);

    // The image and preimage for each crossing.
    FixedArray<ssize_t> image(n);
    FixedArray<ssize_t> preimage(n);

    bool currBetter;
    StrandRef currStrand;
    size_t nextUnused;
    int reflectSign; /* + becomes - */
    int reflectStrands; /* upper becomes lower */
    bool begin = true;
    for (reflectSign = 0; reflectSign < 2; ++reflectSign) {
        for (reflectStrands = 0; reflectStrands < 2; ++reflectStrands) {
            for (int reverse = 0; reverse < 2; ++reverse) {
                for (auto start : crossings_) {
                    // If we can reflect, then we can guarantee to start with
                    // a positive crossing.
                    if (allowReflection &&
                            start->sign() == (reflectSign ? 1 : -1))
                        continue;

                    std::fill(image.begin(), image.end(), -1);
                    std::fill(preimage.begin(), preimage.end(), -1);

                    // Map crossing start -> 0.
                    image[start->index()] = 0;
                    preimage[0] = start->index();
                    nextUnused = 1;

                    currStrand = start->strand(reflectStrands ? 0 : 1);

                    curr[0].crossing = 0;
                    curr[0].strand = 1;
                    curr[0].sign = (reflectSign ?
                        - start->sign() : start->sign());

                    if (begin)
                        currBetter = true;
                    else if (curr[0] < best[0])
                        currBetter = true;
                    else if (best[0] < curr[0])
                        goto noncanonical;
                    else
                        currBetter = false;

                    for (size_t i = 1; i < 2 * n; ++i) {
                        if (reverse)
                            --currStrand;
                        else
                            ++currStrand;

                        if (image[currStrand.crossing()->index()] < 0) {
                            // This is a new crossing.
                            preimage[nextUnused] =
                                currStrand.crossing()->index();
                            image[currStrand.crossing()->index()] =
                                nextUnused++;
                        }

                        curr[i].crossing =
                            image[currStrand.crossing()->index()];
                        curr[i].strand = (reflectStrands ?
                            (currStrand.strand() ^ 1) :
                            currStrand.strand());
                        curr[i].sign = (reflectSign ?
                            - currStrand.crossing()->sign() :
                            currStrand.crossing()->sign());

                        if (! currBetter) {
                            if (curr[i] < best[i])
                                currBetter = true;
                            else if (best[i] < curr[i])
                                goto noncanonical;
                        }
                    }

                    if (currBetter) {
                        std::swap(curr, best);
                        begin = false;
                    }

                    noncanonical:
                        ;
                }

                if (! allowReversal)
                    break;
            }

            // We always allow reflectStrands, since this is just a
            // rotation of the knot.
        }

        if (! allowReflection)
            break;
    }

    // Text: n c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]
    Base64SigEncoder enc;

    // Output crossings in order.
    int charsPerInt = enc.encodeSize(n);
    for (const auto& dat: best)
        enc.encodeInt(dat.crossing, charsPerInt);

    // Output strands and signs, each as a packed sequence of bits.
    for (size_t i = 0; i < 2 * n; i += 6) {
        int write = 0;
        for (int j = 0; j < 6 && i + j < 2 * n; ++j)
            if (best[i + j].strand)
                write |= (1 << j);
        enc.encodeSingle(write);
    }
    for (size_t i = 0; i < 2 * n; i += 6) {
        int write = 0;
        for (int j = 0; j < 6 && i + j < 2 * n; ++j)
            if (best[i + j].sign > 0)
                write |= (1 << j);
        enc.encodeSingle(write);
    }

    return std::move(enc).str();
}

Link Link::fromKnotSig(const std::string& sig) {
    Link ans;

    Base64SigDecoder dec(sig.begin(), sig.end()); // skips leading whitespace
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

        size_t i, j;

        for (i = 0; i < 2 * n; ++i) {
            crossing[i] = dec.decodeInt<size_t>(charsPerInt);
            if (/* crossing[i] < 0 || */ crossing[i] >= n) {
                throw InvalidArgument("fromKnotSig(): "
                    "invalid destination crossing");
            }
        }

        for (i = 0; i < 2 * n; i += 6) {
            unsigned bits = dec.decodeSingle<unsigned>();
            for (j = 0; j < 6 && i + j < 2 * n; ++j) {
                strand[i + j] = (bits & 1);
                bits >>= 1;
            }
            if (bits) {
                throw InvalidArgument("fromKnotSig(): extraneous strand bits");
            }
        }
        for (i = 0; i < 2 * n; i += 6) {
            unsigned bits = dec.decodeSingle<unsigned>();
            for (j = 0; j < 6 && i + j < 2 * n; ++j) {
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
            size_t nextIdx = (i == 2 * n - 1 ? 0 : i + 1);
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

        ans.components_.push_back(
            ans.crossings_[crossing[0]]->strand(strand[0]));

        return ans;
    } catch (const InvalidInput&) {
        // Any exception caught here was thrown by Base64SigDecoder.
        throw InvalidArgument(
            "fromKnotSig(): incomplete or invalid base64 encoding");
    }
}

} // namespace regina

