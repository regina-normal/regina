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

#include "subcomplex/doublesnappedball.h"

namespace regina {
namespace {

enum class PairType {
    Fold,
    Twist,
    Unknown
};

bool isFold(int source, int target, const Perm<5>& perm) {
    return perm == Perm<5>(source, target);
}

bool isTwist(int source, int target, const Perm<5>& perm) {
    if (perm[source] != target)
        return false;

    int fixed = -1;
    int nFixed = 0;
    for (int i = 0; i < 5; ++i)
        if (perm[i] == i) {
            fixed = i;
            ++nFixed;
        }

    if (nFixed != 1 || fixed == source || fixed == target)
        return false;

    bool seen[5] = { false, false, false, false, false };
    int length = 0;
    int curr = source;
    do {
        if (seen[curr])
            return false;
        seen[curr] = true;
        ++length;
        curr = perm[curr];
    } while (curr != source);

    return length == 4 && ! seen[fixed];
}

PairType classifyPair(int source, int target, const Perm<5>& perm) {
    if (isFold(source, target, perm))
        return PairType::Fold;
    if (isTwist(source, target, perm))
        return PairType::Twist;
    return PairType::Unknown;
}

} // anonymous namespace

std::unique_ptr<DoubleSnappedBall> DoubleSnappedBall::recognise(
        Pentachoron<4>* pent) {
    int facet[2][2];
    PairType pairType[2];
    bool used[5] = { false, false, false, false, false };
    int nPairs = 0;

    for (int f = 0; f < 5; ++f) {
        if (pent->adjacentPentachoron(f) != pent)
            continue;

        Perm<5> perm = pent->adjacentGluing(f);
        int g = perm[f];

        if (g < 0 || g >= 5 || g == f)
            return nullptr;
        if (pent->adjacentPentachoron(g) != pent)
            return nullptr;
        if (pent->adjacentGluing(g)[g] != f)
            return nullptr;

        if (f > g)
            continue;

        if (used[f] || used[g] || nPairs == 2)
            return nullptr;

        PairType type = classifyPair(f, g, perm);
        if (type == PairType::Unknown)
            return nullptr;

        facet[nPairs][0] = f;
        facet[nPairs][1] = g;
        pairType[nPairs] = type;
        used[f] = used[g] = true;
        ++nPairs;
    }

    if (nPairs != 2)
        return nullptr;

    int boundary = -1;
    for (int f = 0; f < 5; ++f)
        if (! used[f]) {
            if (boundary >= 0)
                return nullptr;
            boundary = f;
        }

    if (boundary < 0)
        return nullptr;

    int nFolds = 0;
    int nTwists = 0;
    for (int i = 0; i < 2; ++i) {
        if (pairType[i] == PairType::Fold)
            ++nFolds;
        else if (pairType[i] == PairType::Twist)
            ++nTwists;
    }

    if (nFolds == 2) {
        if (facet[1][0] < facet[0][0] ||
                (facet[1][0] == facet[0][0] && facet[1][1] < facet[0][1])) {
            std::swap(facet[0][0], facet[1][0]);
            std::swap(facet[0][1], facet[1][1]);
        }

        return std::unique_ptr<DoubleSnappedBall>(new DoubleSnappedBall(
            pent, facet[0][0], facet[0][1], facet[1][0], facet[1][1],
            boundary, Type::TypeI));
    }

    if (nFolds == 1 && nTwists == 1) {
        if (pairType[0] == PairType::Twist) {
            std::swap(facet[0][0], facet[1][0]);
            std::swap(facet[0][1], facet[1][1]);
        }

        return std::unique_ptr<DoubleSnappedBall>(new DoubleSnappedBall(
            pent, facet[0][0], facet[0][1], facet[1][0], facet[1][1],
            boundary, Type::TypeII));
    }

    return nullptr;
}

} // namespace regina
