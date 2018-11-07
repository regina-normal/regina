
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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
#include "maths/binom.h"
#include "maths/laurent.h"
#include "maths/matrixops.h"
#include "utilities/bitmanip.h"

namespace regina {

const char* Link::khovanovVarX = "q";
const char* Link::khovanovVarY = "t";

const Laurent2<Integer>& Link::khovanov() const {
    if (khovanovQ_.known())
        return *khovanovQ_.value();

    size_t nc = components_.size();
    if (nc == 0)
        return *(khovanovQ_ = new Laurent2<Integer>());

    // It is guaranteed that we have at least one strand, though we
    // might have zero crossings.

    // How many zero-crossing components do we start with?
    // Each of these simply tensors everything by the vector space V,
    // and therefore multiplies all graded dimensions by (q + q^-1).
    // We will handle this multiplication at the end.
    size_t initLoops = 0;
    for (StrandRef s : components_)
        if (! s)
            ++initLoops;

    size_t n = crossings_.size();
    if (n + nc > 29)
        return *(khovanovQ_ = new Laurent2<Integer>);

    // Since n + nc <= 29, we also have n < number of bits in a long.

    // Consider a resolution with height h (0 <= h <= n) giving k loops.
    // We always have 1 <= k <= n + nc.
    // The basis elements have dimensions in the range [ h-k, h+k ],
    //   which gives: -(n + nc) <= dim <= 2n + nc

    static_assert(BitManipulator<unsigned long>::specialised,
        "BitManipulator is not specialised for the mask type.");

    // For each dimension d, how many basis elements do we have in the
    // overall (summed) vector spaces at the old and new heights?
    // We store this in oldDim[n + nc + d - h_old] and
    // newDim[n + nc + d - h_new] respectively.
    size_t* oldDim = new size_t[2 * (n + nc) + 1];
    size_t* newDim = new size_t[2 * (n + nc) + 1];
    std::fill(oldDim, oldDim + 2 * (n + nc) + 1, 0);

    // For each dimension d, what is the boundary matrix from the old height
    // to the new height?
    // We store this in bdry[n + nc + d - h_new].
    MatrixInt** bdry = new MatrixInt*[2 * (n + nc) + 1];
    std::fill(bdry, bdry + 2 * (n + nc) + 1, nullptr);

    size_t rows, cols;

    // For each new height, newLoopIDs[m*(n+nc) + i] stores the ID of the
    // ith unknotted loop that comes from resolving according to the mth mask
    // at that height.  The array newLoopLen[] does likewise, but instead
    // stores the length of each unknotted loop.  Similarly, we have
    // oldLoopIDs[] and oldLoopLen[] for the previous height.
    const size_t maxMasksPerHeight = binomMedium(n, n/2);
    size_t* oldLoopIDs = new size_t[maxMasksPerHeight * (n + nc)];
    size_t* newLoopIDs = new size_t[maxMasksPerHeight * (n + nc)];
    size_t* oldLoopLen = new size_t[maxMasksPerHeight * (n + nc)];
    size_t* newLoopLen = new size_t[maxMasksPerHeight * (n + nc)];

    // For each mask, we store the index of the mask within its height class.
    // TODO: Reduce the footprint of this array by only storing indices
    // for current and previous heights.
    size_t* index = new size_t[(unsigned long)(1) << n];

    int h;
    unsigned long mask, submask;
    unsigned long maskIndex, submaskIndex;
    size_t loops;
    long i;

    int actType; // 0 = unknown; 1 = split; -1 = merge
    size_t actLower, actUpper;
    for (h = 0; h <= n; ++h) {
        std::cerr << "Height: " << h << std::endl;
        std::fill(newDim, newDim + 2 * (n + nc) + 1, 0);

        // Iterate through all masks at this height to establish the
        // final dimensions of each boundary matrix.
        maskIndex = 0;
        for (mask = ((unsigned long)(1) << h) - 1;
                mask < ((unsigned long)(1) << n);
                mask = BitManipulator<unsigned long>::nextPermutation(mask)) {
            std::cerr << "Mask: " << mask << std::endl;
            index[mask] = maskIndex;

            loops = resolutionLoops(mask,
                newLoopIDs + maskIndex * (n + nc),
                newLoopLen + maskIndex * (n + nc));
            std::cerr << "  Loops: " << loops << " -";
            for (size_t i = 0; i < loops; ++i)
                std::cerr << ' ' << newLoopIDs[maskIndex * (n + nc) + i]
                    << '(' << newLoopLen[maskIndex * (n + nc) + i] << ')';
            std::cerr << std::endl;

            for (i = 0; i <= loops; ++i) {
                // (loops choose i) basis vectors of dimension (2i - loops) + h.
                newDim[n + nc + 2*i - loops] += binomMedium(loops, i);
            }

            // In the special case of height 0, nextPermutation() will
            // never change mask.
            if (mask == 0)
                break;
            ++maskIndex;
        }

        std::cerr << "Old dim:";
        for (i = 0; i < 2 * (n + nc) + 1; ++i)
            std::cerr << ' ' << (i + (h - 1) - long(n + nc)) << ':' << oldDim[i];
        std::cerr << std::endl;

        std::cerr << "New dim:";
        for (i = 0; i < 2 * (n + nc) + 1; ++i)
            std::cerr << ' ' << (i + h - long(n + nc)) << ':' << newDim[i];
        std::cerr << std::endl;

        // Build the boundary matrices.
        for (i = 0; i < 2 * (n + nc); ++i)
            if (oldDim[i + 1] && newDim[i]) {
                bdry[i] = new MatrixInt(oldDim[i + 1], newDim[i]);
                std::cerr << "Matrix, dim " << (i + h - long(n + nc))
                    << ": " << bdry[i]->rows() << " x "
                    << bdry[i]->columns() << std::endl;
            }

        // Now iterate again through all masks at this height, this time
        // computing the boundary maps explicitly.
        if (h > 0) {
            maskIndex = 0;
            for (mask = ((unsigned long)(1) << h) - 1;
                    mask < ((unsigned long)(1) << n);
                    mask = BitManipulator<unsigned long>::nextPermutation(mask))
                    {
                std::cerr << "Mask: " << mask << std::endl;

                // TODO: Store this in the previous loop, don't recompute it.
                loops = resolutionLoops(mask);

                // Compute the boundary maps.
                // For this we iterate through submasks at the prevous height.
                // TODO: We could do this iteration faster, using the tools in
                // firstBit() / lastBit().
                for (int bit = 0; bit < n; ++bit) {
                    if (mask & ((unsigned long)(1) << bit)) {
                        submask = mask ^ ((unsigned long)(1) << bit);
                        submaskIndex = index[submask];

                        // Identify how the loops change.
                        for (i = 0;
                                newLoopLen[maskIndex * (n + nc) + i] ==
                                oldLoopLen[submaskIndex * (n + nc) + i];
                                ++i)
                            ;

                        // actLower = newLoopIDs[maskIndex * (n + nc) + i];
                        actLower = i;

                        if (newLoopLen[maskIndex * (n + nc) + i] >
                                oldLoopLen[submaskIndex * (n + nc) + i]) {
                            // Two loops have merged.
                            actType = -1;
                            for (++i;
                                    i < loops &&
                                    newLoopIDs[maskIndex * (n + nc) + i] ==
                                    oldLoopIDs[submaskIndex * (n + nc) + i];
                                    ++i)
                                ;
                            // actUpper = oldLoopIDs[submaskIndex * (n + nc) + i];
                            actUpper = i;
                        } else {
                            // One loop has split.
                            actType = 1;
                            for (++i;
                                    i < loops - 1 &&
                                    newLoopIDs[maskIndex * (n + nc) + i] ==
                                    oldLoopIDs[submaskIndex * (n + nc) + i];
                                    ++i)
                                ;
                            // actUpper = newLoopIDs[maskIndex * (n + nc) + i];
                            actUpper = i;
                        }

                        std::cerr << "  Map: " << submask << " -> " << mask
                            << ", " << (actType > 0 ? "split #" : "merge #")
                            << actLower << " #" << actUpper << std::endl;
                    }
                }

                ++maskIndex;
            }
        }

        // Destroy the boundary matrices.
        for (i = 0; i < 2 * (n + nc); ++i)
            if (bdry[i]) {
                delete bdry[i];
                bdry[i] = nullptr;
            }

        std::swap(oldDim, newDim);
        std::swap(oldLoopIDs, newLoopIDs);
        std::swap(oldLoopLen, newLoopLen);
    }

    delete[] index;
    delete[] oldLoopIDs;
    delete[] newLoopIDs;
    delete[] oldLoopLen;
    delete[] newLoopLen;
    delete[] bdry;
    delete[] oldDim;
    delete[] newDim;

    return *(khovanovQ_ = new Laurent2<Integer>());
}

} // namespace regina

