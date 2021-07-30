
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

namespace {
    // The S_n indices representing conjugacy minimal permutations,
    // currently computed for n <= 6.
    constexpr int allMinimalPerms[] = {
        0, 1, 2, 6, 9, 27, 32, 127, 128, 146, 153
    };
    constexpr int nMinimalPerms[] = { 1, 1, 2, 3, 5, 7, 11 };

    // The maximum size of an automorphism group for a conjugacy minimal
    // permutation, excluding the case where the automorphism group is
    // all of S_n.
    constexpr int maxMinimalAutGroup[] = { 0, 0, 0, 3, 8, 12, 48 };

    // The (-1)-terminated automorphism group corresponding to each
    // conjugacy minimal permutation, or an empty list if the automorphism
    // group is all of S_n.
    // The code that generated these arrays can be found in aut.py, in the same
    // directory as this source file.
    template <int n> constexpr int
        minimalAutGroup[nMinimalPerms[n]][maxMinimalAutGroup[n] + 1];
    template <> constexpr int minimalAutGroup<2>[][1] = {
        /* 0 */ { -1 },
        /* 1 */ { -1 }
    };
    template <> constexpr int minimalAutGroup<3>[][4] = {
        /* 0 */ { -1, 0, 0, 0 },
        /* 1 */ { 0, 1, -1, 0 },
        /* 2 */ { 0, 2, 4, -1 }
    };
    template <> constexpr int minimalAutGroup<4>[][9] = {
        /* 0 */ { -1, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* 1 */ { 0, 1, 6, 7, -1, 0, 0, 0, 0 },
        /* 2 */ { 0, 2, 4, -1, 0, 0, 0, 0, 0 },
        /* 6 */ { 0, 1, 6, 7, 16, 17, 22, 23, -1 },
        /* 9 */ { 0, 9, 16, 19, -1, 0, 0, 0, 0 }
    };
    template <> constexpr int minimalAutGroup<5>[][13] = {
        /* 0 */ { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* 1 */ { 0, 1, 6, 7, 24, 25, 30, 31, 48, 49, 54, 55, -1 },
        /* 2 */ { 0, 2, 4, 25, 27, 29, -1, 0, 0, 0, 0, 0, 0 },
        /* 6 */ { 0, 1, 6, 7, 16, 17, 22, 23, -1, 0, 0, 0, 0 },
        /* 9 */ { 0, 9, 16, 19, -1, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* 27 */ { 0, 2, 4, 25, 27, 29, -1, 0, 0, 0, 0, 0, 0 },
        /* 32 */ { 0, 32, 64, 90, 96, -1, 0, 0, 0, 0, 0, 0, 0 }
    };
    template <> constexpr int minimalAutGroup<6>[][49] = {
        /* 0 */ { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* 1 */ { 0, 1, 6, 7, 24, 25, 30, 31, 48, 49, 54, 55, 120, 121, 126,
                  127, 144, 145, 150, 151, 168, 169, 174, 175, 240, 241, 246,
                  247, 264, 265, 270, 271, 288, 289, 294, 295, 360, 361, 366,
                  367, 384, 385, 390, 391, 408, 409, 414, 415, -1 },
        /* 2 */ { 0, 2, 4, 25, 27, 29, 121, 123, 125, 144, 146, 148, 240, 242,
                  244, 265, 267, 269, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* 6 */ { 0, 1, 6, 7, 16, 17, 22, 23, 120, 121, 126, 127, 136, 137,
                  142, 143, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* 9 */ { 0, 9, 16, 19, 121, 128, 137, 138, -1, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* 27 */ { 0, 2, 4, 25, 27, 29, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* 32 */ { 0, 32, 64, 90, 96, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* 127 */ { 0, 1, 6, 7, 16, 17, 22, 23, 120, 121, 126, 127, 136, 137,
                    142, 143, 288, 289, 294, 295, 304, 305, 310, 311, 408, 409,
                    414, 415, 424, 425, 430, 431, 576, 577, 582, 583, 592, 593,
                    598, 599, 696, 697, 702, 703, 712, 713, 718, 719, -1 },
        /* 128 */ { 0, 9, 16, 19, 121, 128, 137, 138, -1, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* 146 */ { 0, 2, 4, 144, 146, 148, 240, 242, 244, 451, 453, 455, 595,
                    597, 599, 691, 693, 695, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        /* 153 */ { 0, 153, 304, 451, 576, 601, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    struct Formula {
        std::vector<GroupExpressionTerm> terms;
        bool isRelation;

        Formula() = default;
        Formula(const Formula&) = default;
        Formula(Formula&&) = default;
        Formula& operator = (const Formula&) = default;
        Formula& operator = (Formula&&) = default;

        struct Compare {
            bool operator ()(const std::vector<GroupExpressionTerm>& a,
                    const std::vector<GroupExpressionTerm>& b) const {
                return a.size() < b.size() || (a.size() == b.size() && a < b);
            }
        };
    };

    template <int index>
    struct RelationScheme {
        size_t nGen;
        std::vector<Formula> formulae;
        size_t* compCount; // length nGenerators + 1
        Perm<index>* rep;
        Perm<index>* computed;

        // Do we want to use precomputed product tables that are generated
        // at runtime?
        //
        // Note that for index <= 5 the Perm<index> class already uses lookup
        // tables out-of-the-box and so there is no need for us to manage this
        // ourselves here.  For index >= 7 the Perm<index> class does not (yet)
        // have a runtime precomputation facility built in.
        static constexpr bool cacheProducts = (index == 6);

        RelationScheme(const GroupPresentation& g) {
            if constexpr (cacheProducts) {
                Perm<index>::precompute();
            }

            nGen = g.countGenerators();
            compCount = new size_t[nGen + 1];

            long nSeen = nGen;

            // Work out all the pieces we need to compute.
            // Initially we will give these temporary indices;
            // everything will be reindexed later when we have decided
            // on an order of computation.
            //
            // As we walk through each relation, currExp[i] holds the
            // maximum length sub-expression ending at the current
            // position, using only generators of index <= i, and
            // *excluding* all trailing terms with generators of index < i.
            //
            std::vector<GroupExpressionTerm>* currExp =
                new std::vector<GroupExpressionTerm>[nGen];

            typedef std::map<std::vector<GroupExpressionTerm>,
                             std::pair<long, bool>, Formula::Compare>
                             ExpressionMap;
            ExpressionMap* foundExp = new ExpressionMap[nGen];

            unsigned long depth;
            for (const auto& r : g.relations()) {
                depth = nGen; // the last generator seen
                unsigned long prev;

                for (const auto& t : r.terms()) {
                    if (t.generator < depth) {
                        depth = t.generator;
                        currExp[depth].push_back({ depth, t.exponent });
                    } else {
                        while (depth < t.generator) {
                            if (currExp[depth].size() == 1 &&
                                    currExp[depth].front().exponent == 1) {
                                // This expression is just a single symbol.
                                // Reuse that symbol.
                                prev = currExp[depth].front().generator;
                                currExp[depth].clear();
                            } else {
                                // We use a swap and a move to avoid a
                                // deep copy of currExp[depth].
                                // A side-effect is that this clears out
                                // currExp[depth] (which we want to do).
                                std::vector<GroupExpressionTerm> tmp;
                                tmp.swap(currExp[depth]);
                                auto result = foundExp[depth].emplace(
                                    std::move(tmp),
                                    std::make_pair(nSeen, false));
                                if (result.second) {
                                    // This was a new term that we
                                    // hadn't seen before.
                                    prev = nSeen++;
                                } else {
                                    // We already have plans to compute
                                    // this same expression.
                                    // Fetch its corresponding index.
                                    prev = result.first->second.first;
                                }
                            }
                            ++depth;
                            currExp[depth].push_back({ prev, 1 });
                        }
                        currExp[depth].push_back({ depth, t.exponent });
                    }
                }

                // We are guaranteed that the last term in the relation uses
                // the highest generator index that appears in the relation.
                //
                // This means that currExp[depth] is the entire relation.
                //
                // Again we use a swap and a move to avoid a deep copy
                // of currExp[depth], and this also clears currExp[depth].
                std::vector<GroupExpressionTerm> tmp;
                tmp.swap(currExp[depth]);
                auto result = foundExp[depth].emplace(std::move(tmp),
                    std::make_pair(nSeen, true));
                if (result.second)
                    ++nSeen;
                else {
                    // We already have plans to compute this same expression.
                    // Mark it as a relation.
                    result.first->second.second = true;
                }
            }

            // Reindex all the expressions, using the order that has been
            // generated by the maps, in order of increasing depth.
            // As an exception, at each level we put all relations first
            // (so we can backtrack sooner if the relation does not hold).
            //
            // Note that, by construction, each expression only uses other
            // expressions at a lower depth, which means with smaller indices.
            unsigned long* reindex = new unsigned long[nSeen];
            long newIndex = nGen;
            for (depth = 0; depth < nGen; ++depth) {
                // First relations...
                for (const auto& exp : foundExp[depth])
                    if (exp.second.second)
                        reindex[exp.second.first] = newIndex++;
                // ... and then non-relations.
                for (const auto& exp : foundExp[depth])
                    if (! exp.second.second)
                        reindex[exp.second.first] = newIndex++;
            }
            for (depth = 0; depth < nGen; ++depth) {
                // Again: relations first.
                for (auto& exp : foundExp[depth])
                    if (exp.second.second) {
                        Formula f;
                        f.terms.reserve(exp.first.size());
                        for (auto& t : exp.first)
                            if (t.generator < nGen)
                                f.terms.push_back(t);
                            else
                                f.terms.push_back(
                                    { reindex[t.generator], t.exponent });
                        f.isRelation = true;
                        formulae.push_back(std::move(f));
                    }
                for (auto& exp : foundExp[depth])
                    if (! exp.second.second) {
                        Formula f;
                        f.terms.reserve(exp.first.size());
                        for (auto& t : exp.first)
                            if (t.generator < nGen)
                                f.terms.push_back(t);
                            else
                                f.terms.push_back(
                                    { reindex[t.generator], t.exponent });
                        f.isRelation = false;
                        formulae.push_back(std::move(f));
                    }
            }

            compCount[0] = 0;
            for (depth = 0; depth < nGen; ++depth)
                compCount[depth + 1] = compCount[depth] +
                    foundExp[depth].size();

            delete[] reindex;
            delete[] foundExp;
            delete[] currExp;

            rep = new Perm<index>[nGen];
            computed = new Perm<index>[compCount[nGen]];
        }
        ~RelationScheme() {
            delete[] compCount;
            delete[] rep;
            delete[] computed;
        }
        bool computePiece(size_t piece) {
            // 0 <= piece < compCount[nGen]
            // Compute and cache one individual piece.
            // Return false if it is a relation and not identity.
            Perm<index> comb;
            for (const auto& t : formulae[piece].terms) {
                Perm<index> gen = (t.generator < nGen ?
                    rep[t.generator] : computed[t.generator - nGen]);
                // Pull out exponents +/-1, since in practice these are
                // common and we can avoid the (small) overhead of pow().
                if constexpr (cacheProducts) {
                    switch (t.exponent) {
                        case 1:
                            comb = gen.cachedComp(comb);
                            break;
                        case -1:
                            comb = gen.inverse().cachedComp(comb);
                            break;
                        default:
                            comb = gen.cachedPow(t.exponent).cachedComp(comb);
                            break;
                    }
                } else {
                    switch (t.exponent) {
                        case 1:
                            comb = gen * comb;
                            break;
                        case -1:
                            comb = gen.inverse() * comb;
                            break;
                        default:
                            comb = gen.pow(t.exponent) * comb;
                            break;
                    }
                }
            }
            if (formulae[piece].isRelation && ! comb.isIdentity())
                return false;
            else {
                computed[piece] = comb;
                return true;
            }
        }

        bool computeFor(size_t gen) {
            // 0 <= n < nGen
            for (size_t i = compCount[gen]; i < compCount[gen + 1]; ++i)
                if (! computePiece(i))
                    return false;
            return true;
        }
        void dump(std::ostream& out) {
            // If the total number of generators and computed pieces exceeds 26,
            // this routine will (in its current form) output junk (since it
            // outputs alphabetic generator symbols).  Since this is a private
            // routine that is never called, we will leave it like this for now.
            out << "#gen: " << nGen << std::endl;
            out << "compCount:";
            for (int i = 0; i <= nGen; ++i)
                out << ' ' << compCount[i];
            out << std::endl;

            out << "Formulae:" << std::endl;
            for (int i = 0; i < compCount[nGen]; ++i) {
                out << char('a' + nGen + i);
                if (formulae[i].isRelation)
                    out << "[*]";
                out << " :=";
                for (const auto& t : formulae[i].terms) {
                    out << ' ';
                    if (t.exponent == 1)
                        out << char('a' + t.generator);
                    else
                        out << char('a' + t.generator) << '^' << t.exponent;
                }
                out << std::endl;
            }
        }
    };
}

void GroupPresentation::minimaxGenerators() {
    if (relations_.size() == 0 || nGenerators_ == 0) {
        // Nothing to relabel.
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
            // TOOD: Make this test quicker by precomputing word lengths.
            if (curr < best ||
                    (curr == best &&
                     relations_[r].wordLength() <
                     relations_[useRow].wordLength())) {
                best = curr;
                useRow = r;
            }
        }

        if (useRow != rowsUsed) {
            inc.swapRows(useRow, rowsUsed);
            relations_[useRow].swap(relations_[rowsUsed]);
        }

        if (gensUsed == 0 && best == 0) {
            // This relation is empty (as are all those above it).
            continue;
        }

        // This relation is non-empty (as are all those below it).
        if (best > 0) {
            // This relation brings in new, previously unseen generator(s).
            // Make plans to relabel those new generators to use the
            // next available generator labels.
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
        }

        // The highest numbered generator that relation #rowsUsed uses is
        // now precisely (gensUsed-1).
        // Cycle the relation around so that its last term uses its
        // highest numbered generator.
        while (relations_[rowsUsed].terms().back().generator !=
                relabelInv[gensUsed - 1])
            relations_[rowsUsed].cycleLeft();
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
    if (nGenerators_ == 0) {
        // We have the trivial group.
        // There is only one trivial representation, and it is not transitive.
        return 0;
    }

    if (nGenerators_ == 1) {
        // To be transitive, the representation of the unique generator must
        // be a cycle, and all such representations are conjugate (so there
        // is at most one cover to generate).
        //
        // To satisfy the relations:
        //
        // - If we are Z, then there are no non-trivial relations and so
        //   they are vacuously satisfied.  The resulting subgroup is also Z.
        //
        // - If we are Z_n, then n must be a multiple of index.  The resulting
        //   subgroup is Z_(n/index).
        //
        // TODO: Hard-code this result and return.
    }

    // Relabel and reorder generators and relations so that we can check
    // relations as early as possible and backtrack if they break.
    minimaxGenerators();

    // Make a plan for how we will incrementally test consistency with
    // the group relations.
    RelationScheme<index> scheme(*this);

    // Prepare to choose an S(index) representative for each generator.
    // The representative for generator i will be scheme.rep[i].
    // All representatives will be initialised to the identity.
    size_t nReps = 0;

    size_t* nAut = new size_t[nGenerators_];
    Perm<index> (*aut)[maxMinimalAutGroup[index] + 1] =
        new Perm<index>[nGenerators_][maxMinimalAutGroup[index] + 1];

    size_t pos = 0; // The generator whose current rep we are about to try.
    while (true) {
        bool backtrack = false;

        // Check consistency with the group relations that we haven't
        // yet checked, and that containly only generators whose reps
        // have been chosen so far.
        if (! backtrack) {
            if (! scheme.computeFor(pos))
                backtrack = true;
        }

        // Check that the reps are conjugacy minimal, so far.
        // Note: for index 2, *everything* is conjugacy minimal.
        if constexpr (index > 2) {
            if (! backtrack) {
                if (pos == 0 || nAut[pos - 1] == 0) {
                    // Currently the automorphism group for the entire
                    // set of reps chosen before now is all of S_index.
                    // This means that rep[pos] needs to be conjugacy minimal.
                    if (scheme.rep[pos].isConjugacyMinimal()) {
                        if (scheme.rep[pos].isIdentity()) {
                            // The automorphism group remains all of S_index.
                            nAut[pos] = 0;
                        } else {
                            // Set up the automorphism group for this rep
                            // by explicitly listing the automorphisms.
                            int idx = 0;
                            while (allMinimalPerms[idx] !=
                                    scheme.rep[pos].SnIndex())
                                ++idx;

                            nAut[pos] = 0;
                            while (minimalAutGroup<index>[idx][nAut[pos]]
                                    >= 0) {
                                aut[pos][nAut[pos]] = Perm<index>::Sn[
                                    minimalAutGroup<index>[idx][nAut[pos]]];
                                ++nAut[pos];
                            }
                        }
                    } else {
                        backtrack = true;
                    }
                } else {
                    // The previous reps are together conjugacy minimal,
                    // and we have their automorphism group stored.
                    nAut[pos] = 0;
                    Perm<index> conj;
                    for (int a = 0; a < nAut[pos - 1]; ++a) {
                        Perm<index> p = aut[pos - 1][a];
                        if constexpr (RelationScheme<index>::cacheProducts) {
                            conj = p.cachedComp(scheme.rep[pos], p.inverse());
                        } else {
                            conj = p * scheme.rep[pos] * p.inverse();
                        }
                        if (conj < scheme.rep[pos]) {
                            // Not conjugacy minimal.
                            backtrack = true;
                            break;
                        } else if (conj == scheme.rep[pos]) {
                            // This remains part of our automorphism
                            // group going forwards.
                            aut[pos][nAut[pos]++] = p;
                        }
                    }
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
                        int to = scheme.rep[i][from];
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
                    sub.relations_.reserve(index * relations_.size());

                    std::sort(spanningTree, spanningTree + index - 1);

                    unsigned long* rewrite =
                        new unsigned long[sub.nGenerators_];

                    // Work out how the subgroup generators will be relabelled
                    // once the spanning tree is removed.
                    unsigned long treeIdx = 0;
                    for (unsigned long i = 0; i < sub.nGenerators_; ++i) {
                        if (treeIdx < index - 1 && spanningTree[treeIdx] == i) {
                            // This generator will be removed from the subgroup.
                            rewrite[i] = sub.nGenerators_;
                            ++treeIdx;
                        } else
                            rewrite[i] = i - treeIdx;
                    }
                    sub.nGenerators_ -= (index - 1);

                    for (const auto& r : relations_) {
                        for (int start = 0; start < index; ++start) {
                            GroupExpression e;
                            int sheet = start;
                            Perm<index> p;
                            unsigned long gen;
                            for (const auto& t : r.terms()) {
                                if (t.exponent > 0) {
                                    for (long i = 0; i < t.exponent; ++i) {
                                        gen = rewrite[
                                            t.generator * index + sheet];
                                        if (gen < sub.nGenerators_)
                                            e.addTermLast(gen, 1);
                                        sheet = scheme.rep[t.generator][sheet];
                                    }
                                } else if (t.exponent < 0) {
                                    for (long i = 0; i > t.exponent; --i) {
                                        sheet = scheme.rep[t.generator]
                                            .preImageOf(sheet);
                                        gen = rewrite[
                                            t.generator * index + sheet];
                                        if (gen < sub.nGenerators_)
                                            e.addTermLast(gen, -1);
                                    }
                                }
                            }
                            if (! e.terms().empty())
                                sub.relations_.push_back(std::move(e));
                        }
                    }

                    delete[] rewrite;

                    ++nReps;
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
                ++scheme.rep[pos];
                if (! scheme.rep[pos].isIdentity())
                    break;
                if (pos == 0)
                    goto finished;
                --pos;
            }
        }
    }

finished:

    delete[] aut;
    delete[] nAut;
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
template size_t GroupPresentation::enumerateCoversInternal<6>(
        std::function<void(GroupPresentation&)>&& action);

} // namespace regina

