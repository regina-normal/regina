
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2021, Ben Burton                                        *
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

#include "algebra/grouppresentation.h"
#include "maths/perm.h"

namespace regina {

template <int index>
size_t GroupPresentation::enumerateCoversInternal(
        std::function<void(GroupPresentation&)>&& action) {
    // This is an expensive operation.
    // Have one last crack at simplifying the group first.
    intelligentSimplify();

    if (nGenerators_ == 0) {
        // We have the trivial group.
        // There is only one trivial representation, and it is not transitive.
        return 0;
    }

    if (nGenerators_ == 1) {
        // To be transitive, the representation of the one generator must be
        // a cycle, and all such representations are conjugate.
        //
        // To satisfy the relations:
        // - If we are Z, then there are no non-trivial relations and so
        //   they are vacuously satisfied.  The resulting subgroup is also Z.
        // - If we are Z_n, then n must be a multiple of index.  The resulting
        //   subgroup is Z_(n/index).
        //
        // TODO: Hard-code this result and return.
    }

    // Prepare to choose an S(index) representative for each generator.
    // The representative for generator i will be Perm<index>::Sn[rep[i]].
    size_t nReps = 0;
    typedef typename Perm<index>::Index Index;
    Index* rep = new Index[nGenerators_];
    std::fill(rep, rep + nGenerators_, 0);

    size_t pos = 0; // The generator whose current rep we are about to try.
    while (true) {
        bool backtrack = false;

        // TODO: Analyse the current partial reps to prune the search tree.

        // Move on to the next generator.
        if (! backtrack) {
            ++pos;
            if (pos == nGenerators_) {
                // We have a candidate representation.

                // Is it consistent with the group relations?
                for (const auto& r : relations_) {
                    Perm<index> p, comb;
                    for (const auto& t : r.terms()) {
                        p = Perm<index>::Sn[rep[t.generator]];
                        if (t.exponent > 0) {
                            for (long i = 0; i < t.exponent; ++i)
                                comb = p * comb;
                        } else if (t.exponent < 0) {
                            p = p.inverse();
                            for (long i = 0; i > t.exponent; --i)
                                comb = p * comb;
                        }
                    }
                    if (! comb.isIdentity())
                        goto candidateDone;
                }

                // Is it conjugacy minimal?
                // TODO: We can make this *much* faster.
                for (Index idx = 0; idx < Perm<index>::nPerms; ++idx) {
                    Perm<index> p = Perm<index>::Sn[idx];
                    for (unsigned long g = 0; g < nGenerators_; ++g) {
                        Index alt = (p * Perm<index>::Sn[rep[g]] * p.inverse()).
                            SnIndex();
                        if (alt < rep[g]) {
                            // Not conjugacy minimal.
                            goto candidateDone;
                        }
                        if (alt > rep[g]) {
                            // Move on to test the next permutation.
                            g = nGenerators_;
                            break;
                        }
                    }
                }

                // Put this next block inside braces because it declares
                // local variables but we have gotos that jump over it.
                {
                    // Is it transitive?
                    //
                    // Use a depth-first search to see if we can reach
                    // every sheet using the chosen reps.
                    //
                    // We also record *which* routes we found to reach
                    // all of the sheets, since together these give us a
                    // "spanning tree" of subgroup generators that should all
                    // be replaced with the identity in the subgroup.

                    // Note: we are guaranteed nGenerators_ >= 1.

                    bool seen[index];
                    std::fill(seen, seen + index, false);
                    seen[0] = true;

                    int nFound = 1;

                    int stack[index];
                    int stackSize = 1;
                    stack[0] = 0;

                    unsigned long spanningTree[index - 1];

                    while (nFound < index && stackSize > 0) {
                        int from = stack[--stackSize];
                        for (unsigned long i = 0; i < nGenerators_; ++i) {
                            int to = Perm<index>::Sn[rep[i]][from];
                            if (! seen[to]) {
                                seen[to] = true;
                                stack[stackSize++] = to;

                                // Add (generator i, sheet from) to the
                                // spanning tree.
                                spanningTree[nFound - 1] = i * index + from;

                                ++nFound;
                            }
                        }
                    }

                    if (nFound == index) {
                        // The representation is transitive!
                        // Build the subgroup representation and act on it.

                        GroupPresentation sub;
                        sub.nGenerators_ = index * nGenerators_;

                        sub.relations_.reserve(index * relations_.size() +
                            nGenerators_ - 1);

                        for (const auto& r : relations_) {
                            for (int start = 0; start < index; ++start) {
                                GroupExpression e;
                                int sheet = start;
                                Perm<index> p;
                                for (const auto& t : r.terms()) {
                                    p = Perm<index>::Sn[rep[t.generator]];
                                    if (t.exponent > 0) {
                                        for (long i = 0; i < t.exponent; ++i) {
                                            e.addTermLast(
                                                t.generator * index + sheet, 1);
                                            sheet = p[sheet];
                                        }
                                    } else if (t.exponent < 0) {
                                        p = p.inverse();
                                        for (long i = 0; i > t.exponent; --i) {
                                            sheet = p[sheet];
                                            e.addTermLast(
                                                t.generator * index + sheet,
                                                -1);
                                        }
                                    }
                                }
                                sub.relations_.push_back(std::move(e));
                            }
                        }

                        // TODO: Instead of adding trivial relations,
                        // just strip these generators out of the
                        // original relations.  Don't forget to reindex
                        // the surviving generators also.
                        for (int i = 0; i < index - 1; ++i)
                            sub.relations_.push_back(GroupExpression(
                                spanningTree[i], 1));

#if 0
                        std::cerr << "CANDIDATE:";
                        for (size_t i = 0; i < nGenerators_; ++i)
                            std::cerr << ' ' << rep[i];
                        std::cerr << std::endl;
#endif

                        sub.intelligentSimplify();
                        action(sub);
                    }
                }

candidateDone:

                --pos;
                backtrack = true;
            } else {
                continue;
            }
        }

        if (backtrack) {
            while (true) {
                if (++rep[pos] < Perm<index>::nPerms)
                    break;
                if (pos == 0)
                    goto finished;
                rep[pos--] = 0;
            }
        }
    }

finished:

    delete[] rep;
    return nReps;
}

// Instantiate templates for all valid indices.
template size_t GroupPresentation::enumerateCoversInternal<2>(
        std::function<void(GroupPresentation&)>&& action);
template size_t GroupPresentation::enumerateCoversInternal<3>(
        std::function<void(GroupPresentation&)>&& action);
template size_t GroupPresentation::enumerateCoversInternal<4>(
        std::function<void(GroupPresentation&)>&& action);
template size_t GroupPresentation::enumerateCoversInternal<5>(
        std::function<void(GroupPresentation&)>&& action);

} // namespace regina

