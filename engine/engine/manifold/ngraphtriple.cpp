
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include "algebra/nabeliangroup.h"
#include "manifold/ngraphtriple.h"
#include "manifold/nsfs.h"
#include "maths/nmatrixint.h"

namespace regina {

NGraphTriple::~NGraphTriple() {
    delete end_[0];
    delete end_[1];
    delete centre_;
}

NAbelianGroup* NGraphTriple::getHomologyH1() const {
    // Just for safety (this should always be true anyway):
    if (end_[0]->punctures(false) != 1 || end_[0]->punctures(true) != 0)
        return 0;
    if (end_[1]->punctures(false) != 1 || end_[1]->punctures(true) != 0)
        return 0;
    if (centre_->punctures(false) != 2 || centre_->punctures(true) != 0)
        return 0;

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
    NSFSpace* sfs[3];
    unsigned long genus[3], punc[3], fibres[3], ref[3], gens[3];
    unsigned long start[3];

    sfs[0] = centre_;
    sfs[1] = end_[0];
    sfs[2] = end_[1];

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

    NMatrixInt m(fibres[0] + fibres[1] + fibres[2] +
        ref[0] + ref[1] + ref[2] + 13, gens[0] + gens[1] + gens[2]);

    unsigned long i, f;
    NSFSFibre fibre;
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
    NAbelianGroup* ans = new NAbelianGroup();
    ans->addGroup(m);
    return ans;
}

std::ostream& NGraphTriple::writeName(std::ostream& out) const {
    end_[0]->writeName(out);
    out << " U/m ";
    centre_->writeName(out);
    out << " U/n ";
    end_[1]->writeName(out);

    NMatrix2 m0 = matchingReln_[0].inverse();
    out << ", m = [ " <<
        m0[0][0] << ',' << m0[0][1] << " | " <<
        m0[1][0] << ',' << m0[1][1] << " ]";
    out << ", n = [ " <<
        matchingReln_[1][0][0] << ',' << matchingReln_[1][0][1] << " | " <<
        matchingReln_[1][1][0] << ',' << matchingReln_[1][1][1] << " ]";
    return out;
}

std::ostream& NGraphTriple::writeTeXName(std::ostream& out) const {
    end_[0]->writeTeXName(out);
    NMatrix2 m0 = matchingReln_[1].inverse();
    out << " \\bigcup_{\\homtwo{" <<
        m0[0][0] << "}{" << m0[0][1] << "}{" <<
        m0[1][0] << "}{" << m0[1][1] << "}} ";
    centre_->writeTeXName(out);
    out << " \\bigcup_{\\homtwo{" <<
        matchingReln_[1][0][0] << "}{" << matchingReln_[1][0][1] << "}{" <<
        matchingReln_[1][1][0] << "}{" << matchingReln_[1][1][1] << "}} ";
    end_[1]->writeTeXName(out);
    return out;
}

void NGraphTriple::reduce() {
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

    // Bring the obstruction constant for each SFS down to zero.
    long b;

    b = end_[0]->obstruction();
    if (b != 0) {
        end_[0]->insertFibre(1, -b);
        matchingReln_[0][1][0] -= b * matchingReln_[0][0][0];
        matchingReln_[0][1][1] -= b * matchingReln_[0][0][1];
    }

    b = end_[1]->obstruction();
    if (b != 0) {
        end_[1]->insertFibre(1, -b);
        matchingReln_[1][1][0] -= b * matchingReln_[1][0][0];
        matchingReln_[1][1][1] -= b * matchingReln_[1][0][1];
    }

    b = centre_->obstruction();
    if (b != 0) {
        centre_->insertFibre(1, -b);
        matchingReln_[0][0][0] += b * matchingReln_[0][0][1];
        matchingReln_[0][1][0] += b * matchingReln_[0][1][1];
    }

    /**
     * If one of the end spaces is M/n2, we can replace it with D:(2,1)(2,-1)
     * with fibre and orbifold curves switched.  To preserve the
     * determinant of the matching matrix we will actually use a [0,1,-1,0]
     * switch instead of a [0,1,1,0] switch.
     *
     * In fact we will use D:(2,1)(2,1) instead, which means:
     *
     * M_basis = [  0 1 ] [  1 0 ] D_basis = [ -1 1 ] D_basis;
     *           [ -1 0 ] [ -1 1 ]           [ -1 0 ]
     *
     * D_basis = [ 1 0 ] [  0 -1 ] M_basis = [ 0 -1 ] M_basis.
     *           [ 1 1 ] [  1  0 ]           [ 1 -1 ]
     */
    int i;
    for (i = 0; i < 2; i++)
        if (end_[i]->baseClass() == NSFSpace::bn2 &&
                end_[i]->baseGenus() == 1 &&
                (! end_[i]->baseOrientable()) &&
                end_[i]->punctures(false) == 1 &&
                end_[i]->punctures(true) == 0 &&
                end_[i]->reflectors() == 0 &&
                end_[i]->fibreCount() == 0 &&
                end_[i]->obstruction() == 0) {
            delete end_[i];

            end_[i] = new NSFSpace(NSFSpace::bo1, 0 /* genus */,
                1 /* punctures */, 0 /* twisted */,
                0 /* reflectors */, 0 /* twisted */);
            end_[i]->insertFibre(2, 1);
            end_[i]->insertFibre(2, 1);

            matchingReln_[i] = NMatrix2(0, -1, 1, -1) * matchingReln_[i];
        }

    // See whether each space is best reflected or left alone (or
    // whether it doesn't matter).
    bool mayRefEnd[2];
    NSFSpace* tmpSFS;
    for (i = 0; i < 2; i++) {
        mayRefEnd[i] = false;

        tmpSFS = new NSFSpace(*end_[i]);
        tmpSFS->reflect();
        tmpSFS->reduce(false);
        b = tmpSFS->obstruction();
        tmpSFS->insertFibre(1, -b);

        if (*tmpSFS < *end_[i]) {
            // Best to reflect.
            delete end_[i];
            end_[i] = tmpSFS;

            matchingReln_[i][1][0] = -matchingReln_[i][1][0]
                - b * matchingReln_[i][0][0];
            matchingReln_[i][1][1] = -matchingReln_[i][1][1]
                - b * matchingReln_[i][0][1];
        } else if (*end_[i] < *tmpSFS) {
            // Best not to reflect.
            delete tmpSFS;
        } else {
            // Doesn't matter.
            mayRefEnd[i] = true;
            delete tmpSFS;
        }
    }

    bool mayRefCentre = false;
    tmpSFS = new NSFSpace(*centre_);
    tmpSFS->reflect();
    tmpSFS->reduce(false);
    b = tmpSFS->obstruction();
    tmpSFS->insertFibre(1, -b);

    if (*tmpSFS < *centre_) {
        // Best to reflect.
        delete centre_;
        centre_ = tmpSFS;

        matchingReln_[0][0][0] = -matchingReln_[0][0][0]
            + b * matchingReln_[0][0][1];
        matchingReln_[0][1][0] = -matchingReln_[0][1][0]
            + b * matchingReln_[0][1][1];
        matchingReln_[1][0][0] = -matchingReln_[1][0][0];
        matchingReln_[1][1][0] = -matchingReln_[1][1][0];
    } else if (*centre_ < *tmpSFS) {
        // Best not to reflect.
        delete tmpSFS;
    } else {
        // Doesn't matter.
        mayRefCentre = true;
        delete tmpSFS;
    }

    // See whether we're better swapping the two end spaces or leaving them
    // alone (or whether it doesn't matter).
    bool maySwap = false;
    if (*end_[1] < *end_[0]) {
        // Swap them.
        tmpSFS = end_[0];
        end_[0] = end_[1];
        end_[1] = tmpSFS;

        bool tmpRef = mayRefEnd[0];
        mayRefEnd[0] = mayRefEnd[1];
        mayRefEnd[1] = tmpRef;

        NMatrix2 tmpReln = matchingReln_[0];
        matchingReln_[0] = matchingReln_[1];
        matchingReln_[1] = tmpReln;
    } else if (*end_[0] < *end_[1]) {
        // Don't swap them.
    } else {
        // Doesn't matter.
        maySwap = true;
    }

    // Consider replacing each space with its reflection.
    reduceReflect(matchingReln_[0], matchingReln_[1],
        end_[0]->fibreCount(), centre_->fibreCount(), end_[1]->fibreCount(),
        mayRefEnd[0], mayRefCentre, mayRefEnd[1]);

    // Consider swapping end spaces.
    if (maySwap) {
        NMatrix2 altReln0 = matchingReln_[1];
        NMatrix2 altReln1 = matchingReln_[0];

        reduceReflect(altReln0, altReln1,
            end_[1]->fibreCount(), centre_->fibreCount(), end_[0]->fibreCount(),
            mayRefEnd[1], mayRefCentre, mayRefEnd[0]);

        if (simpler(altReln0, altReln1, matchingReln_[0], matchingReln_[1])) {
            matchingReln_[0] = altReln0;
            matchingReln_[1] = altReln1;
        }
    }

    // TODO: More reductions!
}

void NGraphTriple::reduceReflect(NMatrix2& reln0, NMatrix2& reln1,
        unsigned long fibres0, unsigned long fibresCentre,
        unsigned long fibres1, bool mayRef0, bool mayRefCentre, bool mayRef1) {
    // Rotation can be done always.
    reduceBasis(reln0, reln1);

    if (! (mayRef0 || mayRef1 || mayRefCentre))
        return;

    NMatrix2 best0 = reln0;
    NMatrix2 best1 = reln1;

    NMatrix2 alt0, alt1;
    int do0, doCentre, do1;
    for (do0 = 0; do0 <= (mayRef0 ? 1 : 0); do0++)
        for (do1 = 0; do1 <= (mayRef1 ? 1 : 0); do1++)
            for (doCentre = 0; doCentre <= (mayRefCentre ? 1 : 0); doCentre++) {
                if (! (do0 || do1 || doCentre))
                    continue;

                alt0 = reln0;
                alt1 = reln1;

                if (do0)
                    alt0 = NMatrix2(1, 0, fibres0, -1) * alt0;
                if (do1)
                    alt1 = NMatrix2(1, 0, fibres1, -1) * alt1;
                if (doCentre) {
                    alt0 = alt0 * NMatrix2(1, 0, fibresCentre, -1);
                    alt1 = alt1 * NMatrix2(1, 0, 0, -1);
                }

                reduceBasis(alt0, alt1);
                if (simpler(alt0, alt1, best0, best1)) {
                    best0 = alt0;
                    best1 = alt1;
                }
            }

    reln0 = best0;
    reln1 = best1;
}

void NGraphTriple::reduceBasis(NMatrix2& reln0, NMatrix2& reln1) {
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
    NMatrix2 alt0, alt1;
    while (true) {
        alt0 = reln0 * NMatrix2(1, 0, 1, 1);
        alt1 = reln1 * NMatrix2(1, 0, -1, 1);
        if (simpler(alt0, alt1, reln0, reln1)) {
            reln0 = alt0;
            reln1 = alt1;
            continue;
        }

        alt0 = reln0 * NMatrix2(1, 0, -1, 1);
        alt1 = reln1 * NMatrix2(1, 0, 1, 1);
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

void NGraphTriple::reduceSign(NMatrix2& reln) {
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

bool NGraphTriple::simpler(
        const NMatrix2& pair0first, const NMatrix2& pair0second,
        const NMatrix2& pair1first, const NMatrix2& pair1second) {
    long maxAbs0 = 0, maxAbs1 = 0;
    unsigned nZeroes0 = 0, nZeroes1 = 0;

    int i, j;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++) {
            if (pair0first[i][j] > maxAbs0)
                maxAbs0 = pair0first[i][j];
            if (pair0first[i][j] < -maxAbs0)
                maxAbs0 = -pair0first[i][j];
            if (pair0second[i][j] > maxAbs0)
                maxAbs0 = pair0second[i][j];
            if (pair0second[i][j] < -maxAbs0)
                maxAbs0 = -pair0second[i][j];
            if (pair1first[i][j] > maxAbs1)
                maxAbs1 = pair1first[i][j];
            if (pair1first[i][j] < -maxAbs1)
                maxAbs1 = -pair1first[i][j];
            if (pair1second[i][j] > maxAbs1)
                maxAbs1 = pair1second[i][j];
            if (pair1second[i][j] < -maxAbs1)
                maxAbs1 = -pair1second[i][j];

            if (pair0first[i][j] == 0)
                nZeroes0++;
            if (pair0second[i][j] == 0)
                nZeroes0++;
            if (pair1first[i][j] == 0)
                nZeroes1++;
            if (pair1second[i][j] == 0)
                nZeroes1++;
        }

    if (maxAbs0 < maxAbs1)
        return true;
    if (maxAbs0 > maxAbs1)
        return false;

    if (nZeroes0 > nZeroes1)
        return true;
    if (nZeroes0 < nZeroes1)
        return false;

    // Go lexicograhpic.
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++)
            if (pair0first[i][j] < pair1first[i][j])
                return true;
            else if (pair0first[i][j] > pair1first[i][j])
                return false;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++)
            if (pair0second[i][j] < pair1second[i][j])
                return true;
            else if (pair0second[i][j] > pair1second[i][j])
                return false;

    // They're the same.
    return false;
}

} // namespace regina

