
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
#include "manifold/ngraphpair.h"
#include "manifold/nsfs.h"
#include "maths/nmatrixint.h"

namespace regina {

NGraphPair::~NGraphPair() {
    delete sfs_[0];
    delete sfs_[1];
}

NAbelianGroup* NGraphPair::getHomologyH1() const {
    // Just for safety (this should always be true anyway):
    if (sfs_[0]->punctures(false) != 1 || sfs_[0]->punctures(true) != 0)
        return 0;
    if (sfs_[1]->punctures(false) != 1 || sfs_[1]->punctures(true) != 0)
        return 0;

    // Construct a matrix.
    // Generators: fibre 0, base curves 0, base boundary 0,
    //             exceptional fibre boundaries 0, obstruction 0,
    //             reflector boundaries 0, reflector half-fibres 0,
    //             fibre 1, base curves 1, base boundary 1,
    //             exceptional fibre boundaries 1, obstruction 1,
    //             reflector boundaries 0, reflector half-fibres 1.
    // Relations: base curve relation 0, exceptional fibre relations 0,
    //            obstruction relation 0, reflector relations 0,
    //            fibre constraint 0,
    //            base curve relation 1, exceptional fibre relations 1,
    //            obstruction relation 1, reflector relations 1,
    //            fibre constraint 1,
    //            joining of boundaries.
    unsigned long genus0 = sfs_[0]->baseGenus();
    unsigned long fibres0 = sfs_[0]->fibreCount();
    unsigned long ref0 = sfs_[0]->reflectors();
    unsigned long all0 = 3 + genus0 + fibres0 + 2 * ref0;
    unsigned long genus1 = sfs_[1]->baseGenus();
    unsigned long fibres1 = sfs_[1]->fibreCount();
    unsigned long ref1 = sfs_[1]->reflectors();

    // If we have an orientable base space, we get two curves per genus.
    // The easiest thing to do is just to double each genus now.
    if (sfs_[0]->baseOrientable())
        genus0 *= 2;
    if (sfs_[1]->baseOrientable())
        genus1 *= 2;

    NMatrixInt m(fibres0 + fibres1 + ref0 + ref1 + 8,
        genus0 + fibres0 + 2 * ref0 + genus1 + fibres1 + 2 * ref1 + 6);

    unsigned long i, f;
    // The relation for each base orbifold:
    for (i = 1 + genus0; i < 1 + genus0 + 1 + fibres0 + 1 + ref0; i++)
        m.entry(0, i) = 1;
    if (! sfs_[0]->baseOrientable())
        for (i = 1; i < 1 + genus0; i++)
            m.entry(0, i) = 2;

    for (i = 1 + genus1; i < 1 + genus1 + 1 + fibres1 + 1 + ref1; i++)
        m.entry(1, all0 + i) = 1;
    if (! sfs_[1]->baseOrientable())
        for (i = 1; i < 1 + genus1; i++)
            m.entry(1, all0 + i) = 2;

    // A relation for each exceptional fibre and obstruction constant:
    NSFSFibre fibre;

    for (f = 0; f < fibres0; f++) {
        fibre = sfs_[0]->fibre(f);
        m.entry(2 + f, 1 + genus0 + 1 + f) = fibre.alpha;
        m.entry(2 + f, 0) = fibre.beta;
    }
    m.entry(2 + fibres0, 1 + genus0 + 1 + fibres0) = 1;
    m.entry(2 + fibres0, 0) = sfs_[0]->obstruction();

    for (f = 0; f < fibres1; f++) {
        fibre = sfs_[1]->fibre(f);
        m.entry(3 + fibres0 + f, all0 + 1 + genus1 + 1 + f) = fibre.alpha;
        m.entry(3 + fibres0 + f, all0) = fibre.beta;
    }
    m.entry(3 + fibres0 + fibres1, all0 + 1 + genus1 + 1 + fibres1) = 1;
    m.entry(3 + fibres0 + fibres1, all0) = sfs_[1]->obstruction();

    // A relation for each reflector boundary:
    for (i = 0; i < ref0; i++) {
        m.entry(4 + fibres0 + fibres1 + i, 0) = -1;
        m.entry(4 + fibres0 + fibres1 + i,
            1 + genus0 + 1 + fibres0 + 1 + ref0 + i) = 2;
    }

    for (i = 0; i < ref1; i++) {
        m.entry(4 + fibres0 + fibres1 + ref0 + i, all0) = -1;
        m.entry(4 + fibres0 + fibres1 + ref0 + i,
            all0 + 1 + genus1 + 1 + fibres1 + 1 + ref1 + i) = 2;
    }

    // A relation contraining each fibre type.  This relationship only
    // appears in some cases; otherwise we will just have a (harmless)
    // zero row in the matrix.
    if (sfs_[0]->reflectors(true))
        m.entry(4 + fibres0 + fibres1 + ref0 + ref1, 0) = 1;
    else if (sfs_[0]->fibreReversing())
        m.entry(4 + fibres0 + fibres1 + ref0 + ref1, 0) = 2;

    if (sfs_[1]->reflectors(true))
        m.entry(5 + fibres0 + fibres1 + ref0 + ref1, all0) = 1;
    else if (sfs_[1]->fibreReversing())
        m.entry(5 + fibres0 + fibres1 + ref0 + ref1, all0) = 2;

    // Finally, two relations for the joining of boundaries:
    m.entry(6 + fibres0 + fibres1 + ref0 + ref1, all0) = -1;
    m.entry(6 + fibres0 + fibres1 + ref0 + ref1, 0) = matchingReln_[0][0];
    m.entry(6 + fibres0 + fibres1 + ref0 + ref1, 1 + genus0) =
        matchingReln_[0][1];
    m.entry(7 + fibres0 + fibres1 + ref0 + ref1, all0 + 1 + genus1) = -1;
    m.entry(7 + fibres0 + fibres1 + ref0 + ref1, 0) = matchingReln_[1][0];
    m.entry(7 + fibres0 + fibres1 + ref0 + ref1, 1 + genus0) =
        matchingReln_[1][1];

    NAbelianGroup* ans = new NAbelianGroup();
    ans->addGroup(m);
    return ans;
}

std::ostream& NGraphPair::writeName(std::ostream& out) const {
    sfs_[0]->writeName(out);
    out << " U/m ";
    sfs_[1]->writeName(out);
    return out << ", m = [ " <<
        matchingReln_[0][0] << ',' << matchingReln_[0][1] << " | " <<
        matchingReln_[1][0] << ',' << matchingReln_[1][1] << " ]";
}

std::ostream& NGraphPair::writeTeXName(std::ostream& out) const {
    sfs_[0]->writeTeXName(out);
    out << " \\bigcup_{\\homtwo{" <<
        matchingReln_[0][0] << "}{" << matchingReln_[0][1] << "}{" <<
        matchingReln_[1][0] << "}{" << matchingReln_[1][1] << "}} ";
    return sfs_[1]->writeTeXName(out);
}

void NGraphPair::reduce() {
    /**
     * Things to observe:
     *
     * 1. If we add a (1,1) twist to sfs_[0] we can compensate by setting
     *    col 1 -> col 1 - col 2.
     *
     * 2. If we add a (1,1) twist to sfs_[1] we can compensate by setting
     *    row 2 -> row 2 + row 1.
     *
     * 3. We can negate the entire matrix without problems (this
     *    corresponds to rotating one space by 180 degrees).
     *
     * 4. If we negate all fibres in sfs_[0] we can compensate by
     *    negating col 1, though note that this negates the determinant
     *    of the matrix.
     *
     * 5. If we negate all fibres in sfs_[1] we can compensate by
     *    negating row 1, though again note that this negates the
     *    determinant of the matrix.
     *
     * 6. If we wish to swap the two spaces, we invert M.
     */
    sfs_[0]->reduce(false);
    sfs_[1]->reduce(false);

    // Bring the obstruction constant for each SFS down to zero.
    long b;

    b = sfs_[0]->obstruction();
    if (b != 0) {
        sfs_[0]->insertFibre(1, -b);
        matchingReln_[0][0] += b * matchingReln_[0][1];
        matchingReln_[1][0] += b * matchingReln_[1][1];
    }

    b = sfs_[1]->obstruction();
    if (b != 0) {
        sfs_[1]->insertFibre(1, -b);
        matchingReln_[1][0] -= b * matchingReln_[0][0];
        matchingReln_[1][1] -= b * matchingReln_[0][1];
    }

    /**
     * If one of the spaces is M/n2, we can replace it with D:(2,1)(2,-1)
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
    unsigned i, j;
    for (i = 0; i < 2; i++)
        if (sfs_[i]->baseClass() == NSFSpace::bn2 &&
                sfs_[i]->baseGenus() == 1 &&
                (! sfs_[i]->baseOrientable()) &&
                sfs_[i]->punctures(false) == 1 &&
                sfs_[i]->punctures(true) == 0 &&
                sfs_[i]->reflectors() == 0 &&
                sfs_[i]->fibreCount() == 0 &&
                sfs_[i]->obstruction() == 0) {
            delete sfs_[i];

            sfs_[i] = new NSFSpace(NSFSpace::bo1, 0 /* genus */,
                1 /* punctures */, 0 /* twisted */,
                0 /* reflectors */, 0 /* twisted */);
            sfs_[i]->insertFibre(2, 1);
            sfs_[i]->insertFibre(2, 1);

            if (i == 0)
                matchingReln_ = matchingReln_ * NMatrix2(-1, 1, -1, 0);
            else
                matchingReln_ = NMatrix2(0, -1, 1, -1) * matchingReln_;
        }

    // Try all possible variants of reflection and space swapping.
    // Note that candidate 0 for each space _must_ be the original.
    unsigned nCandidates[2];
    NSFSpace* candidate[2][4];
    NMatrix2 origToCandidate[2][4];

    for (i = 0; i < 2; i++) {
        candidate[i][0] = sfs_[i];
        origToCandidate[i][0] = NMatrix2(1, 0, 0, 1);

        candidate[i][1] = new NSFSpace(*sfs_[i]);
        candidate[i][1]->reflect();
        candidate[i][1]->reduce(false);
        b = candidate[i][1]->obstruction();
        candidate[i][1]->insertFibre(1, -b);
        origToCandidate[i][1] = NMatrix2(1, 0, -b, -1);

        nCandidates[i] = 2;

        // Can we negate all fibres without reflecting?
        // Note that (1,2) == (1,0) in this case, so this is only
        // interesting if we have an odd number of exceptional fibres.
        if (sfs_[i]->fibreNegating() && (sfs_[i]->fibreCount() % 2 != 0)) {
            // Do it by adding a single (1,1).  The subsequent reduce() will
            // negate fibres to bring the obstruction constant back down to
            // zero, giving the desired effect.
            candidate[i][2] = new NSFSpace(*sfs_[i]);
            candidate[i][2]->insertFibre(1, 1);
            candidate[i][2]->reduce(false);
            b = candidate[i][2]->obstruction();
            candidate[i][2]->insertFibre(1, -b);
            origToCandidate[i][2] = NMatrix2(1, 0, -b + 1, 1);

            // And reflect also.
            candidate[i][3] = new NSFSpace(*sfs_[i]);
            candidate[i][3]->insertFibre(1, 1);
            candidate[i][3]->reflect();
            candidate[i][3]->reduce(false);
            b = candidate[i][3]->obstruction();
            candidate[i][3]->insertFibre(1, -b);
            origToCandidate[i][3] = NMatrix2(1, 0, -b - 1, -1);

            nCandidates[i] = 4;
        }
    }

    NSFSpace* use0 = 0;
    NSFSpace* use1 = 0;
    NMatrix2 useReln;

    NMatrix2 tryReln;
    for (i = 0; i < nCandidates[0]; i++)
        for (j = 0; j < nCandidates[1]; j++) {
            // Insist on the leftmost space being at least as simple as
            // the rightmost.

            // See if the (i,j) combination is better than what we've
            // seen so far.
            tryReln = origToCandidate[1][j] * matchingReln_ *
                origToCandidate[0][i].inverse();
            reduceSign(tryReln);

            // Try without space swapping.
            if (! (*candidate[1][j] < *candidate[0][i])) {
                if ((! use0) || simpler(tryReln, useReln)) {
                    use0 = candidate[0][i];
                    use1 = candidate[1][j];
                    useReln = tryReln;
                } else if (! simpler(useReln, tryReln)) {
                    // The matrix is the same as our best.  Compare spaces.
                    if (*candidate[0][i] < *use0 ||
                            (*candidate[0][i] == *use0 &&
                            *candidate[1][j] < *use1)) {
                        use0 = candidate[0][i];
                        use1 = candidate[1][j];
                        useReln = tryReln;
                    }
                }
            }

            // Now try with space swapping.
            if (! (*candidate[0][i] < *candidate[1][j])) {
                tryReln = tryReln.inverse();
                reduceSign(tryReln);

                if ((! use0) || simpler(tryReln, useReln)) {
                    use0 = candidate[1][j];
                    use1 = candidate[0][i];
                    useReln = tryReln;
                } else if (! simpler(useReln, tryReln)) {
                    // The matrix is the same as our best.  Compare spaces.
                    if (*candidate[1][j] < *use0 ||
                            (*candidate[1][j] == *use0 &&
                            *candidate[0][i] < *use1)) {
                        use0 = candidate[1][j];
                        use1 = candidate[0][i];
                        useReln = tryReln;
                    }
                }
            }
        }

    // This should never happen, but just in case... let's not crash.
    if (! (use0 && use1)) {
        use0 = sfs_[0];
        use1 = sfs_[1];
        useReln = matchingReln_;
    }

    // Use what we found.
    sfs_[0] = use0;
    sfs_[1] = use1;
    matchingReln_ = useReln;

    // And what we don't use, delete.
    // Note that the comparisons below are pointer comparisons, not
    // NSFSpace comparisons.
    for (i = 0; i < 2; i++)
        for (j = 0; j < nCandidates[i]; j++)
            if (candidate[i][j] != use0 && candidate[i][j] != use1)
                delete candidate[i][j];

    // TODO: Exploit the (1,2) = (1,0) and (1,1) = (1,0) relations in
    // the relevant non-orientable cases.
}

void NGraphPair::reduceSign(NMatrix2& reln) {
    // All we can do is negate the entire matrix (180 degree rotation
    // along the join).
    if (simpler(- reln, reln))
        reln.negate();
}

bool NGraphPair::simpler(const NMatrix2& m1, const NMatrix2& m2) {
    long maxAbs1 = 0, maxAbs2 = 0;
    unsigned nZeroes1 = 0, nZeroes2 = 0;
    unsigned nNeg1 = 0, nNeg2 = 0;

    int i, j;
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++) {
            if (m1[i][j] > maxAbs1)
                maxAbs1 = m1[i][j];
            if (m1[i][j] < -maxAbs1)
                maxAbs1 = -m1[i][j];
            if (m2[i][j] > maxAbs2)
                maxAbs2 = m2[i][j];
            if (m2[i][j] < -maxAbs2)
                maxAbs2 = -m2[i][j];

            if (m1[i][j] == 0)
                nZeroes1++;
            else if (m1[i][j] < 0)
                nNeg1++;
            if (m2[i][j] == 0)
                nZeroes2++;
            else if (m2[i][j] < 0)
                nNeg2++;
        }

    if (maxAbs1 < maxAbs2)
        return true;
    if (maxAbs1 > maxAbs2)
        return false;

    if (nZeroes1 > nZeroes2)
        return true;
    if (nZeroes1 < nZeroes2)
        return false;

    if (nNeg1 < nNeg2)
        return true;
    if (nNeg1 > nNeg2)
        return false;

    // Go lexicograhpic.
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++)
            if (m1[i][j] < m2[i][j])
                return true;
            else if (m1[i][j] > m2[i][j])
                return false;

    // They're the same.
    return false;
}

} // namespace regina

