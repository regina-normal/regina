
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
#include "maths/laurent.h"
#include "utilities/bitmanip.h"
#include "utilities/sequence.h"

namespace regina {

const char* Link::jonesVar = "\u221At"; // \u221A = square root

size_t Link::resolutionLoops(unsigned long mask, size_t* loopIDs,
        size_t* loopLengths) const {
    size_t n = crossings_.size();

    size_t pos;
    int dirInit, dir;
    StrandRef s;

    // found[0..n-1] : seen the half of the upper strand that exits the crossing
    // found[n..2n-1] : seen the half of the upper strand that enters the crossing
    bool* found = new bool[2 * n];
    std::fill(found, found + 2 * n, false);

    size_t loops = 0;
    size_t len;

    // The following two loops iterate through indices of found[] in
    // increasing order.
    for (dirInit = 0; dirInit < 2; ++dirInit) {
        for (pos = 0; pos < n; ++pos) {
            // dirInit: 1 = with arrows, 0 = against arrows.
            // This refers to the direction along the strand as you
            // approach the crossing (before you jump to the other strand).
            if (! found[pos + (dirInit ? n : 0)]) {
                //std::cerr << "LOOP\n";
                if (loopIDs)
                    loopIDs[loops] = pos + (dirInit ? n : 0);

                s = crossings_[pos]->upper();
                dir = dirInit;
                len = 0;

                do {
                    //std::cerr << "At: " << s <<
                    //    (dir == 1 ? " ->" : " <-") << std::endl;

                    if (    ((mask & (1 << s.crossing()->index())) &&
                                s.crossing()->sign() < 0) ||
                            ((mask & (1 << s.crossing()->index())) == 0 &&
                                s.crossing()->sign() > 0)) {
                        // Turn in a way that is consistent with the arrows.
                        if (dir == 1) {
                            found[s.crossing()->index() +
                                (s.strand() ? n : 0)] = true;
                            s = s.crossing()->next(s.strand() ^ 1);
                        } else {
                            found[s.crossing()->index() +
                                (s.strand() ? 0 : n)] = true;
                            s = s.crossing()->prev(s.strand() ^ 1);
                        }
                    } else {
                        // Turn in a way that is inconsistent with the arrows.
                        if (dir == 1) {
                            found[s.crossing()->index() + n] = true;
                            s = s.crossing()->prev(s.strand() ^ 1);
                        } else {
                            found[s.crossing()->index()] = true;
                            s = s.crossing()->next(s.strand() ^ 1);
                        }
                        dir ^= 1;
                    }

                    ++len;
                } while (! (dir == dirInit &&
                    s.crossing()->index() == pos && s.strand() == 1));

                if (loopLengths)
                    loopLengths[loops] = len;
                ++loops;
            }
        }
    }

    delete[] found;
    return loops;
}

Laurent<Integer>* Link::bracketNaive() const {
    /**
     * \ /         \ /            \_/
     *  /   ->   A | |   +   A^-1  _
     * / \         / \            / \
     *
     * O^k  ->  (-A^2 - A^-2)^(k-1)
     */

    if (components_.size() == 0)
        return new Laurent<Integer>();

    // It is guaranteed that we have at least one strand, though we
    // might have zero crossings.

    // How many zero-crossing components do we start with?
    size_t initLoops = 0;
    for (StrandRef s : components_)
        if (! s)
            ++initLoops;

    size_t n = crossings_.size();
    if (n >= sizeof(long) * 8)
        return nullptr;

    // In count[i-1], the coefficient of A^k reflects the number of
    // resolutions with i loops and multiplier A^k.
    // We will always have 1 <= i <= #components + #crossings.
    Laurent<Integer>* count = new Laurent<Integer>[n + components_.size()];

    size_t maxLoops = 0;

    static_assert(BitManipulator<unsigned long>::specialised,
        "BitManipulator is not specialised for the mask type.");

    size_t loops;
    long shift;
    for (unsigned long mask = 0; mask != (1 << n); ++mask) {
        //std::cerr << "Mask: " << mask << std::endl;

        loops = initLoops + resolutionLoops(mask);
        if (loops > maxLoops)
            maxLoops = loops;

        --loops;

        // Set shift = #(0 bits) - #(1 bits) in mask.
        shift = n - 2 * BitManipulator<unsigned long>::bits(mask);
        if (shift > count[loops].maxExp() || shift < count[loops].minExp())
            count[loops].set(shift, 1);
        else
            count[loops].set(shift, count[loops][shift] + 1);
    }

    Laurent<Integer>* ans = new Laurent<Integer>;

    Laurent<Integer> loopPoly;
    loopPoly.set(0, -1);
    loopPoly.set(4, -1);
    loopPoly.shift(-2);

    Laurent<Integer> loopPow(0); // Initialises to x^0 == 1.
    for (loops = 0; loops < maxLoops; ++loops) {
        // std::cerr << "count[" << loops << "] = " << count[loops] << std::endl;
        if (! count[loops].isZero()) {
            count[loops] *= loopPow;
            (*ans) += count[loops];
        }

        loopPow *= loopPoly;
    }

    delete[] count;
    return ans;
}

Laurent<Integer>* Link::bracketTreewidth() const {
    if (crossings_.empty())
        return bracketNaive();

    // We are guaranteed >= 1 crossing and >= 1 component.

    Laurent<Integer> loopPoly;
    loopPoly.set(0, -1);
    loopPoly.set(4, -1);
    loopPoly.shift(-2);

    // Build a nice tree decomposition.:
    const TreeDecomposition& d = niceTreeDecomposition();
    size_t nBags = d.size();

    const TreeBag *bag, *child, *sibling;
    int index;

    // Each partial solution is a key-value map.
    //
    // Each key pairs off strands that connect a crossing in the bag with a
    // crossing that has been forgotten. Strands are numbered 0..(2n-1),
    // where strand i of crossing c is numbered 2c+i.
    //
    // The key is stored as a sequence x[0 .. 2n-1], where
    // - if strand k is being paired off then x[k] is its partner strand;
    // - if strand k connects two forgotten crossings then x[k] = -1;
    // - otherwise x[k] = -2.
    //
    // Each value is a Laurent polynomial, which is essentially a
    // partially computed bracket polynomial that accounts for those
    // crossings that have already been forgotten.
    //
    // We ignore any 0-crossing unknot components throughout this
    // calculation, and only factor them in at the very end when we
    // extract the final bracket polynomial.

    size_t nStrands = 2 * size();
    size_t loops;

    typedef LightweightSequence<int> Key;
    typedef Laurent<Integer> Value;

    typedef std::map<Key*, Value*, Key::Less> SolnSet;

    SolnSet** partial = new SolnSet*[nBags];
    std::fill(partial, partial + nBags, nullptr);

    for (bag = d.first(); bag; bag = bag->next()) {
        index = bag->index();
        std::cerr << "Bag " << index << " [" << bag->size() << "] ";

        if (bag->isLeaf()) {
            // Leaf bag.
            std::cerr << "LEAF" << std::endl;
            partial[index] = new SolnSet;

            Key* k = new Key(nStrands);
            std::fill(k->begin(), k->end(), -2);

            Value* v = new Laurent<Integer>(0); // Initialised to x^0 = 1.

            partial[index]->insert(std::make_pair(k, v));
        } else if (bag->type() == NICE_INTRODUCE) {
            // Introduce bag.
            std::cerr << "INTRODUCE" << std::endl;
            child = bag->children();

            // When introducing a new crossing, all of its arcs must
            // lead to unseen crossings or crossings already in the bag.
            // Therefore the keys and values remain unchanged.

            partial[index] = partial[child->index()];
            partial[child->index()] = nullptr;
        } else if (bag->type() == NICE_FORGET) {
            // Forget bag.
            child = bag->children();
            std::cerr << "FORGET -> 2 x " <<
                partial[child->index()]->size() << std::endl;

            Crossing* forget = crossings_[child->element(bag->subtype())];

            // The A resolution connects strands conn[0][0][0-1], and
            // connects strands conn[0][1][0-1].
            // The A^{-1} resolution connects strands conn[1][0][0-1], and
            // connects strands conn[1][1][0-1].
            StrandRef conn[2][2][2];
            conn[0][0][0] = conn[1][0][0] = forget->upper().prev();
            if (forget->sign() > 0) {
                conn[0][0][1] = conn[1][1][0] = forget->lower();
                conn[0][1][0] = conn[1][0][1] = forget->lower().prev();
            } else {
                conn[0][0][1] = conn[1][1][0] = forget->lower().prev();
                conn[0][1][0] = conn[1][0][1] = forget->lower();
            }
            conn[0][1][1] = conn[1][1][1] = forget->upper();

            size_t connIdx[2][2][2];
            int i, j, k;
            for (i = 0; i < 2; ++i)
                for (j = 0; j < 2; ++j)
                    for (k = 0; k < 2; ++k)
                        connIdx[i][j][k] =
                            2 * conn[i][j][k].crossing()->index() +
                            conn[i][j][k].strand();

            partial[index] = new SolnSet;

            const Key *kChild;
            Key *kNew;
            const Value *vChild;
            Value *vNew;
            size_t newLoops;
            for (auto& soln : *(partial[child->index()])) {
                kChild = soln.first;
                vChild = soln.second;

                // Adjust the key and value to reflect the fact the newly
                // forgotten crossing, under both possible resolutions.
                for (i = 0; i < 2; ++i) {
                    // i = 0: A resolution
                    // i = 1: A^{-1} resolution

                    kNew = new Key(nStrands);
                    std::copy(kChild->begin(), kChild->end(), kNew->begin());

                    newLoops = 0;
                    for (j = 0; j < 2; ++j) {
                        // Connect strands conn[i][j][0-1].
                        if ((*kNew)[connIdx[i][j][0]] == -2 &&
                                (*kNew)[connIdx[i][j][1]] == -2) {
                            // Both strands stay in or above the bag.
                            if (connIdx[i][j][0] == connIdx[i][j][1]) {
                                // The two strands form a loop.
                                // Bury them in the forgotten region.
                                (*kNew)[connIdx[i][j][0]] = -1;
                                (*kNew)[connIdx[i][j][1]] = -1;
                                ++newLoops;
                            } else {
                                // The two strands go separate ways.
                                // Make them the endponts of a new path that
                                // enters and exits the forgotten region.
                                (*kNew)[connIdx[i][j][0]] = connIdx[i][j][1];
                                (*kNew)[connIdx[i][j][1]] = connIdx[i][j][0];
                            }
                        } else if ((*kNew)[connIdx[i][j][0]] == -2) {
                            // We cannot have one strand as -2 and the
                            // other as -1, since -2 means neither end
                            // of the strand is forgotten and -1 means
                            // both ends are forgotten.

                            // In this case we lengthen a section of the link
                            // that passes through the forgotten region.
                            (*kNew)[connIdx[i][j][0]] =
                                (*kNew)[connIdx[i][j][1]];
                            (*kNew)[(*kNew)[connIdx[i][j][1]]] =
                                connIdx[i][j][0];
                            (*kNew)[connIdx[i][j][1]] = -1;
                        } else if ((*kNew)[connIdx[i][j][1]] == -2) {
                            // As before, we lengthen a section of the link
                            // that passes through the forgotten region.
                            (*kNew)[connIdx[i][j][1]] =
                                (*kNew)[connIdx[i][j][0]];
                            (*kNew)[(*kNew)[connIdx[i][j][0]]] =
                                connIdx[i][j][1];
                            (*kNew)[connIdx[i][j][0]] = -1;
                        } else {
                            // Both strands head down into the forgotten region.
                            if ((*kNew)[connIdx[i][j][0]] ==
                                    connIdx[i][j][1]) {
                                // We have closed off a loop.
                                ++newLoops;
                            } else {
                                // We connect two sections of the link
                                // that pass through the forgotten region.
                                (*kNew)[(*kNew)[connIdx[i][j][0]]] =
                                    (*kNew)[connIdx[i][j][1]];
                                (*kNew)[(*kNew)[connIdx[i][j][1]]] =
                                    (*kNew)[connIdx[i][j][0]];
                            }
                            (*kNew)[connIdx[i][j][0]] = -1;
                            (*kNew)[connIdx[i][j][1]] = -1;
                        }
                    }

                    // We start at each leaf with the polynomial 1,
                    // which effectively adds one closed loop that we
                    // didn't have.  So in the very last iteration (which
                    // is guaranteed to close off at least one loop),
                    // subtract one closed loop to compensate.
                    if (index == nBags - 1)
                        --newLoops;

                    vNew = new Value(*vChild);
                    vNew->shift(i == 0 ? 1 : -1);
                    for (loops = 0; loops < newLoops; ++loops)
                        (*vNew) *= loopPoly;

                    // Insert the new key/value into our partial
                    // solution, aggregating if need be.
                    auto existingSoln = partial[index]->insert(
                        std::make_pair(kNew, vNew));
                    if (! existingSoln.second) {
                        *(existingSoln.first->second) += *vNew;
                        delete kNew;
                        delete vNew;
                    }
                }

                delete kChild;
                delete vChild;
            }

            delete partial[child->index()];
            partial[child->index()] = nullptr;
        } else {
            // Join bag.
            child = bag->children();
            sibling = child->sibling();
            std::cerr << "JOIN -> " <<
                partial[child->index()]->size() << " x " <<
                partial[sibling->index()]->size() << std::endl;

            partial[index] = new SolnSet;

            const Key *k1, *k2;
            const Value *v1, *v2;
            Key *kNew;
            Value *vNew;
            size_t strand;

            for (auto& soln1 : *(partial[child->index()])) {
                k1 = soln1.first;
                v1 = soln1.second;
                for (auto& soln2 : *(partial[sibling->index()])) {
                    k2 = soln2.first;
                    v2 = soln2.second;

                    // Combine the two child keys and values.
                    kNew = new Key(nStrands);
                    for (strand = 0; strand < nStrands; ++strand)
                        if ((*k1)[strand] == -2)
                            (*kNew)[strand] = (*k2)[strand];
                        else if ((*k2)[strand] == -2)
                            (*kNew)[strand] = (*k1)[strand];
                        else
                            std::cerr <<
                                "ERROR: Incompatible keys in join bag"
                                << std::endl;

                    vNew = new Value(*v1);
                    *vNew *= *v2;

                    if (! partial[index]->insert(
                            std::make_pair(kNew, vNew)).second)
                        std::cerr << "ERROR: Combined keys in join bag "
                            "are not unique" << std::endl;
                }
            }

            for (auto& soln : *(partial[child->index()])) {
                delete soln.first;
                delete soln.second;
            }
            for (auto& soln : *(partial[sibling->index()])) {
                delete soln.first;
                delete soln.second;
            }

            delete partial[child->index()];
            delete partial[sibling->index()];
            partial[child->index()] = partial[sibling->index()] = nullptr;
        }
    }

    // Collect the final answer from partial[nBags - 1].
    std::cerr << "FINISH" << std::endl;
    Value* ans = partial[nBags - 1]->begin()->second;

    for (auto& soln : *(partial[nBags - 1])) {
        delete soln.first;
        if (soln.second != ans)
            delete soln.second;
    }
    delete partial[nBags - 1];

    delete[] partial;

    // Finally, factor in any zero-crossing components.
    for (StrandRef s : components_)
        if (! s)
            (*ans) *= loopPoly;

    return ans;
}

const Laurent<Integer>& Link::bracket(Algorithm alg) const {
    if (bracket_.known())
        return *bracket_.value();

    switch (alg) {
        case ALG_TREEWIDTH:
            bracket_ = bracketTreewidth();
            break;
        default:
            bracket_ = bracketNaive();
            break;
    }
    return *bracket_.value();
}

const Laurent<Integer>& Link::jones(Algorithm alg) const {
    if (jones_.known())
        return *jones_.value();

    // (-A^3)^(-w) * bracket, then multiply all exponents by -1/4.
    Laurent<Integer>* ans = new Laurent<Integer>(bracket(alg));
    if (! ans)
        return *(jones_ = new Laurent<Integer>());

    long w = writhe();
    ans->shift(-3 * w);
    if (w % 2)
        ans->negate();

    // We only scale exponents by -1/2, since we are returning a Laurent
    // polynomial in sqrt(t).
    ans->scaleDown(-2);
    return *(jones_ = ans);
}

void Link::optimiseForJones(TreeDecomposition& td) const {
    td.compress();
    if (td.size() <= 1)
        return;

    int* costSame = new int[td.size()];
    int* costReverse = new int[td.size()];
    int* costRoot = new int[td.size()];

    // For a bag b:
    //
    // costRoot: Count strands from crossings in b to crossings not in b.
    //
    // costSame: Count strands from crossings in b to crossings not in b,
    // but in one of b's descendants.
    //
    // costReverse: Count strands from crossings in b->parent to crossings
    // not in b->parent and not in b or any of b's descendants.

    std::fill(costSame, costSame + td.size(), 0);
    std::fill(costReverse, costReverse + td.size(), 0);
    std::fill(costRoot, costRoot + td.size(), 0);

    int i;
    int p, q;
    Crossing *c;
    int adj;
    const TreeBag *b, *desc;
    for (b = td.first(); b; b = b->next()) {
        for (i = 0; i < b->size(); ++i) {
            c = crossings_[b->element(i)];
            for (p = 0; p < 2; ++p)
                for (q = 0; q < 2; ++q) {
                    adj = (p == 0 ? c->prev(q).crossing() :
                        c->next(q).crossing())->index();
                    if (! b->contains(adj)) {
                        // We have a strand from a crossing in b that
                        // leads to a crossing not in b.
                        ++costRoot[b->index()];

                        // Is adj buried within b's descendants?
                        // TODO: Make this faster.
                        for (desc = b; desc->children();
                                desc = desc->children())
                            ;
                        while (desc != b) {
                            if (desc->contains(adj)) {
                                ++costSame[b->index()];
                                break;
                            }
                            desc = desc->next();
                        }
                    }
                }
        }

        if (b->parent()) {
            for (i = 0; i < b->parent()->size(); ++i) {
                c = crossings_[b->parent()->element(i)];
                for (p = 0; p < 2; ++p)
                    for (q = 0; q < 2; ++q) {
                        adj = (p == 0 ? c->prev(q).crossing() :
                            c->next(q).crossing())->index();
                        if (! b->parent()->contains(adj)) {
                            // We have a strand from a crossing in b's parent
                            // that leads to a crossing not in b's parent.

                            // Is adj *not* buried within b or its descendants?
                            // TODO: Make this faster.
                            if (! b->contains(adj)) {
                                for (desc = b; desc->children();
                                        desc = desc->children())
                                    ;
                                while (desc != b) {
                                    if (desc->contains(adj)) {
                                        break;
                                    }
                                    desc = desc->next();
                                }
                                if (desc == b)
                                    ++costReverse[b->index()];
                            }
                        }
                    }
            }
        }
    }

    td.reroot(costSame, costReverse, costRoot);

    delete[] costSame;
    delete[] costReverse;
    delete[] costRoot;
}

} // namespace regina

