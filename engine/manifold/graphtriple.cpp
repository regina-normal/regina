
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

#include "algebra/abeliangroup.h"
#include "manifold/graphtriple.h"
#include "manifold/sfs.h"
#include "manifold/sfsalt.h"
#include "maths/matrix.h"

namespace regina {

bool GraphTriple::operator < (const GraphTriple& compare) const {
    if (centre_ < compare.centre_)
        return true;
    if (compare.centre_ < centre_)
        return false;

    if (end_[0] < compare.end_[0])
        return true;
    if (compare.end_[0] < end_[0])
        return false;

    if (end_[1] < compare.end_[1])
        return true;
    if (compare.end_[1] < end_[1])
        return false;

    if (simpler(matchingReln_[0], compare.matchingReln_[0]))
        return true;
    if (simpler(compare.matchingReln_[0], matchingReln_[0]))
        return false;

    return simpler(matchingReln_[1], compare.matchingReln_[1]);
}

AbelianGroup GraphTriple::homology() const {
    // Construct a matrix.
    // Generators:
    //     - Spaces are ordered centre, end 0, end 1.
    //     - For each space, generators are:
    //           - fibre
    //           - base curves
    //           - base boundary
    //           - exceptional fibre boundaries
    //           - obstruction
    //           - reflector boundaries
    //           - reflector half-fibres
    // Relations:
    //     - For each space:
    //           - base curve relation
    //           - exceptional fibre relations
    //           - obstruction relation
    //           - reflector relations
    //           - fibre constraint
    //     - Plus two boundary joinings.
    const SFSpace* sfs[3] = { &centre_, &end_[0], &end_[1] };
    unsigned long genus[3], punc[3], fibres[3], ref[3], gens[3];
    unsigned long start[3];

    punc[0] = 2;
    punc[1] = 1;
    punc[2] = 1;

    int s;
    for (s = 0; s < 3; s++) {
        genus[s] = sfs[s]->baseGenus();
        fibres[s] = sfs[s]->fibreCount();
        ref[s] = sfs[s]->reflectors();

        // If we have an orientable base space, we get two curves per genus.
        // The easiest thing seems to be to just double the genus now.
        if (sfs[s]->baseOrientable())
            genus[s] *= 2;

        gens[s] = 1 + genus[s] + punc[s] + fibres[s] + 1 + ref[s] + ref[s];
    }

    start[0] = 0;
    start[1] = gens[0];
    start[2] = gens[0] + gens[1];

    MatrixInt m(fibres[0] + fibres[1] + fibres[2] +
        ref[0] + ref[1] + ref[2] + 13, gens[0] + gens[1] + gens[2]);

    unsigned long i, f;
    SFSFibre fibre;
    unsigned long reln = 0;

    // Relations internal to each space:
    for (s = 0; s < 3; s++) {
        // The relation for the base orbifold:
        for (i = 1 + genus[s];
                i < 1 + genus[s] + punc[s] + fibres[s] + 1 + ref[s]; i++)
            m.entry(reln, start[s] + i) = 1;
        if (! sfs[s]->baseOrientable())
            for (i = 1; i < 1 + genus[s]; i++)
                m.entry(reln, start[s] + i) = 2;
        reln++;

        // A relation for each exception fibre:
        for (f = 0; f < fibres[s]; f++) {
            fibre = sfs[s]->fibre(f);
            m.entry(reln, start[s] + 1 + genus[s] + punc[s] + f) = fibre.alpha;
            m.entry(reln, start[s]) = fibre.beta;
            reln++;
        }

        // The obstruction constant:
        m.entry(reln, start[s] + 1 + genus[s] + punc[s] + fibres[s]) = 1;
        m.entry(reln, start[s]) = sfs[s]->obstruction();
        reln++;

        // A relation for each reflector boundary:
        for (i = 0; i < ref[s]; i++) {
            m.entry(reln, start[s]) = -1;
            m.entry(reln, start[s] + 1 + genus[s] + punc[s] + fibres[s] +
                1 + ref[s] + i) = 2;
            reln++;
        }

        // A relation constraining the fibre.  This relation only
        // appears in some cases; otherwise we will just have a
        // (harmless) zero row in the matrix.
        if (sfs[s]->reflectors(true))
            m.entry(reln, start[s]) = 1;
        else if (sfs[s]->fibreReversing())
            m.entry(reln, start[s]) = 2;
        reln++;
    }

    // Joining of boundaries:
    m.entry(reln, start[1]) = -1;
    m.entry(reln, 0) = matchingReln_[0][0][0];
    m.entry(reln, 1 + genus[0]) = matchingReln_[0][0][1];
    reln++;
    m.entry(reln, start[1] + 1 + genus[1]) = -1;
    m.entry(reln, 0) = matchingReln_[0][1][0];
    m.entry(reln, 1 + genus[0]) = matchingReln_[0][1][1];
    reln++;

    m.entry(reln, start[2]) = -1;
    m.entry(reln, 0) = matchingReln_[1][0][0];
    m.entry(reln, 1 + genus[0] + 1) = matchingReln_[1][0][1];
    reln++;
    m.entry(reln, start[2] + 1 + genus[2]) = -1;
    m.entry(reln, 0) = matchingReln_[1][1][0];
    m.entry(reln, 1 + genus[0] + 1) = matchingReln_[1][1][1];
    reln++;

    // Phew.
    return AbelianGroup(std::move(m));
}

std::ostream& GraphTriple::writeName(std::ostream& out) const {
    end_[0].writeName(out);
    out << " U/m ";
    centre_.writeName(out);
    out << " U/n ";
    end_[1].writeName(out);

    Matrix2 m0 = matchingReln_[0].inverse();
    out << ", m = [ " <<
        m0[0][0] << ',' << m0[0][1] << " | " <<
        m0[1][0] << ',' << m0[1][1] << " ]";
    out << ", n = [ " <<
        matchingReln_[1][0][0] << ',' << matchingReln_[1][0][1] << " | " <<
        matchingReln_[1][1][0] << ',' << matchingReln_[1][1][1] << " ]";
    return out;
}

std::ostream& GraphTriple::writeTeXName(std::ostream& out) const {
    end_[0].writeTeXName(out);
    Matrix2 m0 = matchingReln_[0].inverse();
    out << " \\bigcup_{\\homtwo{" <<
        m0[0][0] << "}{" << m0[0][1] << "}{" <<
        m0[1][0] << "}{" << m0[1][1] << "}} ";
    centre_.writeTeXName(out);
    out << " \\bigcup_{\\homtwo{" <<
        matchingReln_[1][0][0] << "}{" << matchingReln_[1][0][1] << "}{" <<
        matchingReln_[1][1][0] << "}{" << matchingReln_[1][1][1] << "}} ";
    end_[1].writeTeXName(out);
    return out;
}

void GraphTriple::reduce() {
    /**
     * Things to observe:
     *
     * 1. If we add a (1,1) twist to centre_ we can compensate by setting
     *    col 1 -> col 1 - col 2 in one of the matching relations.
     *
     * 2. If we add a (1,1) twist to end_[i] we can compensate by setting
     *    row 2 -> row 2 + row 1 in matching relation i.
     *
     * 3. We can negate an entire matrix without problems (this
     *    corresponds to rotating some spaces by 180 degrees).
     *
     * 4. If we negate all fibres in centre_ we can compensate by
     *    negating col 1 of both matching relations, though note
     *    that this negates the determinant of each matrix.
     *
     * 5. If we negate all fibres in end_[i] we can compensate by
     *    negating row 1 of matching relation i, though again note that
     *    this negates the determinant of the matrix.
     *
     * 6. If we wish to swap the order of spaces, we swap both matrices.
     */

    // Simplify each space and build a list of possible reflections and
    // other representations that we wish to experiment with using.
    std::vector<SFSAlt> alt0 = SFSAlt::altSet(end_[0]);
    std::vector<SFSAlt> alt1 = SFSAlt::altSet(end_[1]);
    std::vector<SFSAlt> altCentre = SFSAlt::altSet(centre_);

    // Decide which of these possible representations gives the nicest
    // matching relations.
    std::vector<SFSAlt>::iterator use0, use1, useCentre;
    Matrix2 useReln[2];

    bool first = true;
    Matrix2 tryReln[2], tmpReln;
    for (auto it0 = alt0.begin(); it0 != alt0.end(); ++it0) {
        for (auto it1 = alt1.begin(); it1 != alt1.end(); ++it1) {
            for (auto itC = altCentre.begin(); itC != altCentre.end(); ++itC) {
                // See if (it0, it1, itC) gives us a combination better than
                // anything we've seen so far.
                tryReln[0] = it0->conversion() * matchingReln_[0] *
                    itC->conversion().inverse();

                if (itC->reflected())
                    tryReln[1] = it1->conversion() * matchingReln_[1] *
                        Matrix2(1, 0, 0, -1);
                else
                    tryReln[1] = it1->conversion() * matchingReln_[1];

                reduceBasis(tryReln[0], tryReln[1]);

                // Insist on the first end space being at least as
                // simple as the second.

                // First try without end space swapping.
                if (! (it1->alt() < it0->alt())) {
                    if (first || simpler(tryReln[0], tryReln[1],
                            useReln[0], useReln[1])) {
                        use0 = it0;
                        use1 = it1;
                        useCentre = itC;
                        useReln[0] = tryReln[0];
                        useReln[1] = tryReln[1];
                        first = false;
                    } else if (! simpler(useReln[0], useReln[1],
                            tryReln[0], tryReln[1])) {
                        // The matrices are the same as our best.
                        // Compare spaces.
                        if (itC->alt() < useCentre->alt() ||
                                (itC->alt() == useCentre->alt() &&
                                    it0->alt() < use0->alt()) ||
                                (itC->alt() == useCentre->alt() &&
                                    it0->alt() == use0->alt() &&
                                    it1->alt() < use1->alt())) {
                            use0 = it0;
                            use1 = it1;
                            useCentre = itC;
                            useReln[0] = tryReln[0];
                            useReln[1] = tryReln[1];
                        }
                    }
                }

                // Now try with end space swapping.
                if (! (it0->alt() < it1->alt())) {
                    reduceBasis(tryReln[1], tryReln[0]);

                    if (first || simpler(tryReln[1], tryReln[0],
                            useReln[0], useReln[1])) {
                        use0 = it1;
                        use1 = it0;
                        useCentre = itC;
                        useReln[0] = tryReln[1];
                        useReln[1] = tryReln[0];
                        first = false;
                    } else if (! simpler(useReln[0], useReln[1],
                            tryReln[1], tryReln[0])) {
                        // The matrices are the same as our best.
                        // Compare spaces.
                        if (itC->alt() < useCentre->alt() ||
                                (itC->alt() == useCentre->alt() &&
                                    it1->alt() < use0->alt()) ||
                                (itC->alt() == useCentre->alt() &&
                                    it1->alt() == use0->alt() &&
                                    it0->alt() < use1->alt())) {
                            use0 = it1;
                            use1 = it0;
                            useCentre = itC;
                            useReln[0] = tryReln[1];
                            useReln[1] = tryReln[0];
                        }
                    }
                }
            }
        }
    }

    // Use what we found.
    end_[0] = std::move(use0->alt());
    end_[1] = std::move(use1->alt());
    centre_ = std::move(useCentre->alt());
    matchingReln_[0] = useReln[0];
    matchingReln_[1] = useReln[1];

    // TODO: More reductions!
}

void GraphTriple::reduceBasis(Matrix2& reln0, Matrix2& reln1) {
    /**
     * The operation we allow here is to add a (1,1) / (1,-1) pair of
     * twists to centre_, which means:
     *
     *     col 1 -> col 1 + col 2 in one of the matching relations;
     *     col 1 -> col 1 - col 2 in the other.
     */

    // Start by making the first entry in each column 2 positive (for
    // consistency).
    if (reln0[0][1] < 0 || (reln0[0][1] == 0 && reln0[1][1] < 0))
        reln0.negate();
    if (reln1[0][1] < 0 || (reln1[0][1] == 0 && reln1[1][1] < 0))
        reln1.negate();

    // Go for the local minimum.
    // TODO: We can certainly do better than this (both in terms of being
    // faster [use division] and simpler matrices coming out the end).
    Matrix2 alt0, alt1;
    while (true) {
        alt0 = reln0 * Matrix2(1, 0, 1, 1);
        alt1 = reln1 * Matrix2(1, 0, -1, 1);
        if (simpler(alt0, alt1, reln0, reln1)) {
            reln0 = alt0;
            reln1 = alt1;
            continue;
        }

        alt0 = reln0 * Matrix2(1, 0, -1, 1);
        alt1 = reln1 * Matrix2(1, 0, 1, 1);
        if (simpler(alt0, alt1, reln0, reln1)) {
            reln0 = alt0;
            reln1 = alt1;
            continue;
        }

        // We're at a local minimum.  Call it enough for now.
        break;
    }

    // Final tidying up.
    reduceSign(reln0);
    reduceSign(reln1);
}

void GraphTriple::reduceSign(Matrix2& reln) {
    // Make the first non-zero entry positive.
    int i, j;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++) {
            if (reln[i][j] > 0)
                return;

            if (reln[i][j] < 0) {
                // Negate everything (180 degree rotation along the join)
                // and return.

                for (i = 0; i < 2; i++)
                    for (j = 0; j < 2; j++)
                        reln[i][j] = - reln[i][j];
                return;
            }
        }

    // The matrix is entirely zero (which, incidentally, should never
    // happen).  Do nothing.
}

} // namespace regina

