
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include "manifold/graphpair.h"
#include "manifold/sfs.h"
#include "manifold/sfsalt.h"
#include "maths/matrix.h"

namespace regina {

bool GraphPair::operator < (const GraphPair& compare) const {
    if (sfs_[0] < compare.sfs_[0])
        return true;
    if (compare.sfs_[0] < sfs_[0])
        return false;

    if (sfs_[1] < compare.sfs_[1])
        return true;
    if (compare.sfs_[1] < sfs_[1])
        return false;

    return simpler(matchingReln_, compare.matchingReln_);
}

AbelianGroup GraphPair::homology() const {
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
    unsigned long genus0 = sfs_[0].baseGenus();
    unsigned long fibres0 = sfs_[0].fibreCount();
    unsigned long ref0 = sfs_[0].reflectors();
    unsigned long all0 = 3 + genus0 + fibres0 + 2 * ref0;
    unsigned long genus1 = sfs_[1].baseGenus();
    unsigned long fibres1 = sfs_[1].fibreCount();
    unsigned long ref1 = sfs_[1].reflectors();

    // If we have an orientable base space, we get two curves per genus.
    // The easiest thing to do is just to double each genus now.
    if (sfs_[0].baseOrientable())
        genus0 *= 2;
    if (sfs_[1].baseOrientable())
        genus1 *= 2;

    MatrixInt m(fibres0 + fibres1 + ref0 + ref1 + 8,
        genus0 + fibres0 + 2 * ref0 + genus1 + fibres1 + 2 * ref1 + 6);

    unsigned long i, f;
    // The relation for each base orbifold:
    for (i = 1 + genus0; i < 1 + genus0 + 1 + fibres0 + 1 + ref0; i++)
        m.entry(0, i) = 1;
    if (! sfs_[0].baseOrientable())
        for (i = 1; i < 1 + genus0; i++)
            m.entry(0, i) = 2;

    for (i = 1 + genus1; i < 1 + genus1 + 1 + fibres1 + 1 + ref1; i++)
        m.entry(1, all0 + i) = 1;
    if (! sfs_[1].baseOrientable())
        for (i = 1; i < 1 + genus1; i++)
            m.entry(1, all0 + i) = 2;

    // A relation for each exceptional fibre and obstruction constant:
    SFSFibre fibre;

    for (f = 0; f < fibres0; f++) {
        fibre = sfs_[0].fibre(f);
        m.entry(2 + f, 1 + genus0 + 1 + f) = fibre.alpha;
        m.entry(2 + f, 0) = fibre.beta;
    }
    m.entry(2 + fibres0, 1 + genus0 + 1 + fibres0) = 1;
    m.entry(2 + fibres0, 0) = sfs_[0].obstruction();

    for (f = 0; f < fibres1; f++) {
        fibre = sfs_[1].fibre(f);
        m.entry(3 + fibres0 + f, all0 + 1 + genus1 + 1 + f) = fibre.alpha;
        m.entry(3 + fibres0 + f, all0) = fibre.beta;
    }
    m.entry(3 + fibres0 + fibres1, all0 + 1 + genus1 + 1 + fibres1) = 1;
    m.entry(3 + fibres0 + fibres1, all0) = sfs_[1].obstruction();

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
    if (sfs_[0].reflectors(true))
        m.entry(4 + fibres0 + fibres1 + ref0 + ref1, 0) = 1;
    else if (sfs_[0].fibreReversing())
        m.entry(4 + fibres0 + fibres1 + ref0 + ref1, 0) = 2;

    if (sfs_[1].reflectors(true))
        m.entry(5 + fibres0 + fibres1 + ref0 + ref1, all0) = 1;
    else if (sfs_[1].fibreReversing())
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

    return AbelianGroup(std::move(m));
}

std::ostream& GraphPair::writeName(std::ostream& out) const {
    sfs_[0].writeName(out);
    out << " U/m ";
    sfs_[1].writeName(out);
    return out << ", m = [ " <<
        matchingReln_[0][0] << ',' << matchingReln_[0][1] << " | " <<
        matchingReln_[1][0] << ',' << matchingReln_[1][1] << " ]";
}

std::ostream& GraphPair::writeTeXName(std::ostream& out) const {
    sfs_[0].writeTeXName(out);
    out << " \\bigcup_{\\homtwo{" <<
        matchingReln_[0][0] << "}{" << matchingReln_[0][1] << "}{" <<
        matchingReln_[1][0] << "}{" << matchingReln_[1][1] << "}} ";
    return sfs_[1].writeTeXName(out);
}

void GraphPair::reduce() {
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

    // Simplify each space and build a list of possible reflections and
    // other representations that we wish to experiment with using.
    std::vector<SFSAlt> alt0 = SFSAlt::altSet(sfs_[0]);
    std::vector<SFSAlt> alt1 = SFSAlt::altSet(sfs_[1]);

    // Decide which of these possible representations gives the nicest
    // matching relation.
    std::vector<SFSAlt>::iterator use0, use1;
    Matrix2 useReln;

    bool first = true;
    Matrix2 tryReln;
    for (auto it0 = alt0.begin(); it0 != alt0.end(); ++it0) {
        for (auto it1 = alt1.begin(); it1 != alt1.end(); ++it1) {
            // Insist on the leftmost space being at least as simple as
            // the rightmost.

            // See if the (it0, it1) combination is better than what we've
            // seen so far.
            tryReln = it1->conversion() * matchingReln_ *
                it0->conversion().inverse();
            reduceSign(tryReln);

            // Try without space swapping.
            if (! (it1->alt() < it0->alt())) {
                if (first || simpler(tryReln, useReln)) {
                    use0 = it0;
                    use1 = it1;
                    useReln = tryReln;
                    first = false;
                } else if (! simpler(useReln, tryReln)) {
                    // The matrix is the same as our best.  Compare spaces.
                    if (it0->alt() < use0->alt() ||
                            (it0->alt() == use0->alt() &&
                             it1->alt() < use1->alt())) {
                        use0 = it0;
                        use1 = it1;
                        useReln = tryReln;
                    }
                }
            }

            // Now try with space swapping.
            if (! (it0->alt() < it1->alt())) {
                tryReln = tryReln.inverse();
                reduceSign(tryReln);

                if (first || simpler(tryReln, useReln)) {
                    use0 = it1;
                    use1 = it0;
                    useReln = tryReln;
                    first = false;
                } else if (! simpler(useReln, tryReln)) {
                    // The matrix is the same as our best.  Compare spaces.
                    if (it1->alt() < use0->alt() ||
                            (it1->alt() == use0->alt() &&
                             it0->alt() < use1->alt())) {
                        use0 = it1;
                        use1 = it0;
                        useReln = tryReln;
                    }
                }
            }
        }
    }

    // Use what we found.
    sfs_[0] = std::move(use0->alt());
    sfs_[1] = std::move(use1->alt());
    matchingReln_ = useReln;

    // TODO: Exploit the (1,2) = (1,0) and (1,1) = (1,0) relations in
    // the relevant non-orientable cases.
}

void GraphPair::reduceSign(Matrix2& reln) {
    // All we can do is negate the entire matrix (180 degree rotation
    // along the join).
    if (simpler(- reln, reln))
        reln.negate();
}

} // namespace regina

