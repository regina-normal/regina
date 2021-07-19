
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
#include "maths/matrix.h"

namespace regina {

void GroupPresentation::minimaxGenerators(unsigned long* genRange) {
    if (relations_.size() == 0) {
        // Nothing to relabel, and the genRange array has size zero.
        return;
    }

    if (nGenerators_ == 0) {
        // Nothing to relabel.
        // We do need to note that every relation uses no generators at all.
        if (genRange)
            std::fill(genRange, genRange + relations_.size(), 0);
        return;
    }

    // Build a table of which relations contain which generators.
    // Rows = relations; columns = generators.
    Matrix<bool> inc = incidence();

    // Note how we plan to relabel the generators.
    unsigned long* relabel = new unsigned long[nGenerators_];
    unsigned long* relabelInv = new unsigned long[nGenerators_];
    for (unsigned long i = 0; i < nGenerators_; ++i)
        relabel[i] = relabelInv[i] = i;

    size_t gensUsed = 0;
    for (size_t rowsUsed = 0; rowsUsed < inc.rows(); ++rowsUsed) {
        // Find the row in [r, #relns) that uses the fewest generators
        // not yet seen (i.e., from [gensUsed, #gens) after relabelling).
        size_t useRow = rowsUsed;

        size_t best = 0;
        for (size_t g = gensUsed; g < nGenerators_; ++g)
            if (inc.entry(rowsUsed, relabelInv[g]))
                ++best;

        for (size_t r = rowsUsed + 1; r < inc.rows(); ++r) {
            size_t curr = 0;
            for (size_t g = gensUsed; g < nGenerators_; ++g)
                if (inc.entry(r, relabelInv[g]))
                    ++curr;
            if (curr < best) {
                best = curr;
                useRow = r;
            }
        }

        if (useRow != rowsUsed) {
            inc.swapRows(useRow, rowsUsed);
            relations_[useRow].swap(relations_[rowsUsed]);
        }

        // Make plans to relabel those previously-unseen generators that
        // appear in useRow to use the next available generator labels.
        for (size_t g = gensUsed; g < nGenerators_; ++g)
            if (inc.entry(rowsUsed, relabelInv[g])) {
                // Whatever was being relabelled to g should now be
                // relabelled to gensUsed.
                if (g != gensUsed) {
                    std::swap(relabelInv[g], relabelInv[gensUsed]);
                    std::swap(relabel[relabelInv[g]],
                        relabel[relabelInv[gensUsed]]);
                }
                ++gensUsed;
            }

        if (genRange)
            genRange[rowsUsed] = gensUsed;
    }

    // Now do the actual relabelling.
    for (auto& r : relations_)
        for (auto& t : r.terms())
            t.generator = relabel[t.generator];

    delete[] relabelInv;
    delete[] relabel;
}

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

    // Relabel and reorder generators and relations so that we can check
    // relations as early as possible and backtrack if they break.
    //
    // The minimaxGenerators function will fill the genRange array with
    // information on how many generators are used after each relation
    // is brought into the overall collection.
    unsigned long* genRange = (relations_.empty() ? nullptr :
        new unsigned long[relations_.size()]);
    minimaxGenerators(genRange);

    // Work out how many of the initial relations contain only generators
    // 0..g inclusive, for each g.
    size_t* relnRange = new size_t[nGenerators_];
    if (relations_.empty())
        std::fill(relnRange, relnRange + nGenerators_, 0);
    else {
        size_t r = 0;
        for (unsigned long g = 0; g < nGenerators_; ++g) {
            while (r < relations_.size() && genRange[r] <= g + 1)
                ++r;
            relnRange[g] = r;
        }
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

        // Check that the reps are conjugacy minimal, so far.
        // Note: for index 2, *everything* is conjugacy minimal.
        if constexpr (index > 2) {
            if (! backtrack) {
                if (pos == 0) {
                    if (! Perm<index>::Sn[rep[0]].isConjugacyMinimal())
                        backtrack = true;
                } else {
                    // TODO: We can make this faster.
                    for (Index idx = 0; idx < Perm<index>::nPerms; ++idx) {
                        Perm<index> p = Perm<index>::Sn[idx];
                        unsigned long g;
                        for (g = 0; g < pos; ++g) {
                            if (p * Perm<index>::Sn[rep[g]] * p.inverse() !=
                                    Perm<index>::Sn[rep[g]]) {
                                // This does not preserve the previous reps.
                                break;
                            }
                        }
                        if (g < pos) {
                            // Move on to the next permutation.
                            continue;
                        }
                        // Now see how this permutation affects the
                        // current rep that we have just chosen.
                        Index alt =
                            (p * Perm<index>::Sn[rep[pos]] * p.inverse()).
                            SnIndex();
                        if (alt < rep[pos]) {
                            // Not conjugacy minimal.
                            backtrack = true;
                            break;
                        }
                    }
                }
            }
        }

        // Check consistency with the group relations that containly only
        // generators whose reps have been chosen so far.
        if (! backtrack) {
            for (size_t r = (pos == 0 ? 0 : relnRange[pos - 1]);
                    r < relnRange[pos]; ++r) {
                Perm<index> p, comb;
                for (const auto& t : relations_[r].terms()) {
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
                if (! comb.isIdentity()) {
                    backtrack = true;
                    break;
                }
            }
        }

        // Move on to the next generator.
        if (! backtrack) {
            ++pos;
            if (pos == nGenerators_) {
                // We have a candidate representation.

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

                    sub.intelligentSimplify();
                    action(sub);
                }

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
    delete[] relnRange;
    delete[] genRange;
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

