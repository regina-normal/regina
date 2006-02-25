
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

    for (int i = 0; i < 2; i++)
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

    bool ref;

    reduceReflectEnd(matchingReln_[0], end_[0]->fibreCount(), ref);
    if (ref)
        end_[0]->complementAllFibres();

    reduceReflectEnd(matchingReln_[1], end_[1]->fibreCount(), ref);
    if (ref)
        end_[1]->complementAllFibres();

    // TODO: More reductions!  More reductions!
}

void NGraphTriple::reduceReflectEnd(NMatrix2& reln, unsigned long fibres,
        bool& ref) {
    // Consider replacing the end space with its reflection.
    // Note that we have b=0 for all spaces at this stage.
    NMatrix2 alt = NMatrix2(1, 0, fibres, -1) * reln;

    reduceSign(reln);
    reduceSign(alt);

    if (simpler(alt, reln)) {
        reln = alt;
        ref = true;
    } else {
        ref = false;
    }
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

bool NGraphTriple::simpler(const NMatrix2& m1, const NMatrix2& m2) {
    long maxAbs1 = 0, maxAbs2 = 0;
    unsigned nZeroes1 = 0, nZeroes2 = 0;

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
            if (m2[i][j] == 0)
                nZeroes2++;
        }

    if (maxAbs1 < maxAbs2)
        return true;
    if (maxAbs1 > maxAbs2)
        return false;

    if (nZeroes1 > nZeroes2)
        return true;
    if (nZeroes1 < nZeroes2)
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

