
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

#include "link/link.h"
#include "triangulation/detail/isosig-impl.h"

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

std::string Link::knotSig(bool useReflection, bool useReverse) const {
    // Only defined for knots at present.
    if (components_.size() != 1)
        return std::string();

    // Minimise sequence: (crossing, strand, sign) ... (crossing, strand, sign)
    // Ordering: crossing by ID; strand upper first; sign positive first
    // Text: n c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]
    size_t n = crossings_.size();
    SigData* best = new SigData[2 * n];
    SigData* curr = new SigData[2 * n];

    // The image and preimage for each crossing.
    ptrdiff_t* image = new ptrdiff_t[n];
    ptrdiff_t* preimage = new ptrdiff_t[n];

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
                    if (useReflection &&
                            start->sign() == (reflectSign ? 1 : -1))
                        continue;

                    std::fill(image, image + n, -1);
                    std::fill(preimage, preimage + n, -1);

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

                if (! useReverse)
                    break;
            }

            // We always allow reflectStrands, since this is just a
            // rotation of the knot.
        }

        if (! useReflection)
            break;
    }

    delete[] preimage;
    delete[] image;
    delete[] curr;

    // Text: n c_1 c_2 ... c_2n [packed strand bits] [packed sign bits]
    std::string ans;

    // Output chars per crossing index, if this is > 1.
    unsigned charsPerInt;
    if (n < 63)
        charsPerInt = 1;
    else {
        charsPerInt = 0;
        size_t tmp = n;
        while (tmp > 0) {
            tmp >>= 6;
            ++charsPerInt;
        }
        ans = regina::detail::IsoSigHelper::SCHAR(63);
        ans += regina::detail::IsoSigHelper::SCHAR(charsPerInt);
    }

    // Output crossings in order.
    regina::detail::IsoSigHelper::SAPPEND(ans, n, charsPerInt);
    for (auto dat = best; dat != best + 2*n; ++dat)
        regina::detail::IsoSigHelper::SAPPEND(ans, dat->crossing, charsPerInt);

    // Output strands and signs, each as a packed sequence of bits.
    unsigned i, j;
    unsigned write;
    for (i = 0; i < 2 * n; i += 6) {
        write = 0;
        for (j = 0; j < 6 && i + j < 2 * n; ++j)
            if (best[i + j].strand)
                write |= (1 << j);
        ans += regina::detail::IsoSigHelper::SCHAR(write);
    }
    for (i = 0; i < 2 * n; i += 6) {
        write = 0;
        for (j = 0; j < 6 && i + j < 2 * n; ++j)
            if (best[i + j].sign > 0)
                write |= (1 << j);
        ans += regina::detail::IsoSigHelper::SCHAR(write);
    }

    delete[] best;
    return ans;
}

Link* Link::fromKnotSig(const std::string& sig) {
    std::unique_ptr<Link> ans(new Link());
    Link::ChangeEventSpan span(ans.get());

    const char* c = sig.c_str();

    // Skip any leading whitespace.
    while (*c && ::isspace(*c))
        ++c;

    // Find the end of the string.
    const char* end = c;
    while (*end && ! ::isspace(*end))
        ++end;

    // Initial check for invalid characters.
    const char* d;
    for (d = c; d != end; ++d)
        if (! regina::detail::IsoSigHelper::SVALID(*d))
            return nullptr;
    for (d = end; *d; ++d)
        if (! ::isspace(*d))
            return nullptr;

    size_t charsPerInt;
    size_t n = regina::detail::IsoSigHelper::SVAL(*c++);
    if (n < 63)
        charsPerInt = 1;
    else {
        if (c == end)
            return nullptr;
        charsPerInt = regina::detail::IsoSigHelper::SVAL(*c++);
        if (c + charsPerInt > end)
            return nullptr;
        n = regina::detail::IsoSigHelper::SREAD<size_t>(c, charsPerInt);
        c += charsPerInt;
    }

    if (n == 0) {
        // Zero-crossing unknot.
        if (c != end)
            return nullptr;
        ans->components_.push_back(StrandRef());
        return ans.release();
    }

    if (end != c + 2 * n * charsPerInt + 2 * ((n + 2) / 3))
        return nullptr;

    size_t* crossing = new size_t[2 * n];
    int* sign = new int[2 * n];
    int* strand = new int[2 * n];

    size_t i, j;

    for (i = 0; i < 2 * n; ++i) {
        crossing[i] = regina::detail::IsoSigHelper::SREAD<size_t>(c,
            charsPerInt);
        if (/* crossing[i] < 0 || */ crossing[i] >= n) {
            delete[] crossing;
            delete[] sign;
            delete[] strand;
            return nullptr;
        }
        c += charsPerInt;
    }

    unsigned bits;
    for (i = 0; i < 2 * n; i += 6) {
        bits = regina::detail::IsoSigHelper::SVAL(*c++);
        for (j = 0; j < 6 && i + j < 2 * n; ++j) {
            strand[i + j] = (bits & 1);
            bits >>= 1;
        }
        if (bits) {
            delete[] crossing;
            delete[] sign;
            delete[] strand;
            return nullptr;
        }
    }
    for (i = 0; i < 2 * n; i += 6) {
        bits = regina::detail::IsoSigHelper::SVAL(*c++);
        for (j = 0; j < 6 && i + j < 2 * n; ++j) {
            sign[i + j] = ((bits & 1) ? 1 : -1);
            bits >>= 1;
        }
        if (bits) {
            delete[] crossing;
            delete[] sign;
            delete[] strand;
            return nullptr;
        }
    }

    for (i = 0; i < n; ++i)
        ans->crossings_.push_back(new Crossing());

    Crossing* cr;
    size_t nextIdx;
    for (i = 0; i < 2 * n; ++i) {
        cr = ans->crossings_[crossing[i]];
        if (cr->sign_ == 0)
            cr->sign_ = sign[i];
        else if (cr->sign_ != sign[i]) {
            delete[] crossing;
            delete[] sign;
            delete[] strand;
            return nullptr;
        }

        if (cr->next_[strand[i]].crossing_) {
            delete[] crossing;
            delete[] sign;
            delete[] strand;
            return nullptr;
        }
        nextIdx = (i == 2 * n - 1 ? 0 : i + 1);
        cr->next_[strand[i]].crossing_ = ans->crossings_[crossing[nextIdx]];
        cr->next_[strand[i]].strand_ = strand[nextIdx];

        if (ans->crossings_[crossing[nextIdx]]->prev_[strand[nextIdx]]) {
            delete[] crossing;
            delete[] sign;
            delete[] strand;
            return nullptr;
        }
        ans->crossings_[crossing[nextIdx]]->prev_[strand[nextIdx]].crossing_ =
            cr;
        ans->crossings_[crossing[nextIdx]]->prev_[strand[nextIdx]].strand_ =
            strand[i];
    }

    ans->components_.push_back(ans->crossings_[crossing[0]]->strand(strand[0]));

    delete[] crossing;
    delete[] sign;
    delete[] strand;
    return ans.release();
}

} // namespace regina

