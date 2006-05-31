
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
#include "maths/nmatrixint.h"
#include "manifold/ntorusbundle.h"

namespace regina {

NAbelianGroup* NTorusBundle::getHomologyH1() const {
    NMatrixInt relns(2, 2);

    relns.entry(0, 0) = monodromy[0][0] - 1;
    relns.entry(0, 1) = monodromy[0][1];
    relns.entry(1, 0) = monodromy[1][0];
    relns.entry(1, 1) = monodromy[1][1] - 1;

    NAbelianGroup* ans = new NAbelianGroup();
    ans->addGroup(relns);
    ans->addRank();

    return ans;
}

std::ostream& NTorusBundle::writeName(std::ostream& out) const {
    if (monodromy.isIdentity())
        return out << "T x I";
    else
        return out << "T x I / [ " << monodromy[0][0] << ','
            << monodromy[0][1] << " | " << monodromy[1][0] << ','
            << monodromy[1][1] << " ]";
}

std::ostream& NTorusBundle::writeTeXName(std::ostream& out) const {
    if (monodromy.isIdentity())
        return out << "T^2 \\times I";
    else
        return out << "T^2 \\times I / \\homtwo{" << monodromy[0][0] << "}{"
            << monodromy[0][1] << "}{" << monodromy[1][0] << "}{"
            << monodromy[1][1] << "}";
}

void NTorusBundle::reduce() {
    // Make the monodromy prettier.
    // In general we are allowed to:
    //
    // - Replace M with A M A^-1
    // - Replace M with M^-1
    //
    // Some specific tricks we can pull include:
    //
    // - Rotate the matrix 180 degrees (A = [ 0 1 | 1 0 ])
    // - Negate the off-diagonal (A = [ 1 0 | 0 -1 ])
    //
    // If det == +1, we can also:
    //
    // - Swap either diagonal individually (invert, then negate the
    //   off-diagonal, then optionally rotate by 180 degrees)
    //
    // If det == -1, we can also:
    //
    // - Simultaneously swap and negate the main diagonal (invert)

    // The determinant should be +/-1 according to our preconditions,
    // but we'd better check that anyway.
    long det = monodromy.determinant();
    if (det != 1 && det != -1) {
        // Something is very wrong.  Don't touch it.
        std::cerr << "ERROR: NTorusBundle monodromy does not have "
            "determinant +/-1.\n";
        return;
    }

    // Deal with the case where the main diagonal has strictly opposite
    // signs.
    long x;
    if (monodromy[0][0] < 0 && monodromy[1][1] > 0) {
        // Rotate 180 degrees to put the positive element up top.
        rotate();
    }
    while (monodromy[0][0] > 0 && monodromy[1][1] < 0) {
        // Set x to the greatest absolute value of any main diagonal element.
        if (monodromy[0][0] >= - monodromy[1][1])
            x = monodromy[0][0];
        else
            x = - monodromy[1][1];

        // If we catch any of the following four cases, the main diagonal
        // will either no longer have opposite signs, or it will have a
        // strictly smaller maximum absolute value.
        if (0 < monodromy[0][1] && monodromy[0][1] <= x) {
            addRCDown();
            continue;
        } else if (0 < - monodromy[0][1] && - monodromy[0][1] <= x) {
            subtractRCDown();
            continue;
        } else if (0 < monodromy[1][0] && monodromy[1][0] <= x) {
            subtractRCUp();
            continue;
        } else if (0 < - monodromy[1][0] && - monodromy[1][0] <= x) {
            addRCUp();
            continue;
        }

        // Since the determinant is +/-1 and neither element of the
        // main diagonal is zero, we cannot have both elements of the
        // off-diagonal with absolute value strictly greater than x.

        // The only remaining possibility is that some element of the
        // off-diagonal is zero (and therefore the main diagonal
        // contains +1 and -1).

        // The non-zero off-diagonal element (if any) can be reduced
        // modulo 2.  This leaves us with the following possibilities:
        //   [ 1 0 | 0 -1 ] ,   [ 1 1 | 0 -1 ],   [ 1 0 | 1 -1 ].
        // The final two possibilities are both equivalent to [ 0 1 | 1 0 ].
        if ((monodromy[0][1] % 2) || (monodromy[1][0] % 2)) {
            monodromy[0][0] = monodromy[1][1] = 0;
            monodromy[0][1] = monodromy[1][0] = 1;
        } else {
            monodromy[0][1] = monodromy[1][0] = 0;
            // The main diagonal elements stay as they are (1, -1).
        }

        // In these cases we are completely finished.
        return;
    }

    // We are now guaranteed that the main diagonal does not have
    // strictly opposite signs.

    // Time to arrange the same for the off-diagonal.

    // If the off-diagonal has strictly opposite signs, the elements
    // must be +1 and -1, and the main diagonal must contain a zero.
    // Otherwise there is no way we can get determinant +/-1.
    if (monodromy[0][1] < 0 && monodromy[1][0] > 0) {
        // We have [ a -1 | 1 d ].
        // Move the -1 to the bottom left corner by negating the off-diagonal.
        monodromy[0][1] = 1;
        monodromy[1][0] = -1;
    }
    if (monodromy[0][1] > 0 && monodromy[1][0] < 0) {
        // We have [ a 1 | -1 d ], where one of a or d is zero.
        // Rotate by 180 degrees to move the 0 to the bottom right
        // corner, negating the off-diagonal if necessary to preserve
        // the 1/-1 positions.
        if (monodromy[1][1]) {
            monodromy[0][0] = monodromy[1][1];
            monodromy[1][1] = 0;
        }

        // Now we have [ a 1 | -1 0 ].
        if (monodromy[0][0] > 1) {
            addRCDown();
            // Everything becomes non-negative.
        } else if (monodromy[0][0] < -1) {
            subtractRCUp();
            // Everything becomes non-positive.
        } else {
            // We have [ 1 1 | -1 0 ], [ 0 1 | -1 0 ] or [ -1 1 | -1 0 ].
            // All of these are canonical.
            return;
        }
    }

    // Neither diagonal has strictly opposite signs.
    // Time to give all elements of the matrix the same sign (or zero).
    bool allNegative = false;
    if (det == 1) {
        // Either all non-negative or all non-positive, as determined by
        // the main diagonal.
        // If it's going to end up negative, just switch the signs for
        // now and remember this fact for later on.
        if (monodromy[0][0] < 0 || monodromy[1][1] < 0) {
            allNegative = true;
            monodromy[0][0] = - monodromy[0][0];
            monodromy[1][1] = - monodromy[1][1];
        }
        if (monodromy[0][1] < 0 || monodromy[1][0] < 0) {
            // We're always allowed to do this.
            monodromy[0][1] = - monodromy[0][1];
            monodromy[1][0] = - monodromy[1][0];
        }
    } else {
        // The determinant is -1.
        // The entire matrix can be made non-negative.
        if (monodromy[0][0] < 0 || monodromy[1][1] < 0) {
            // Invert (swap and negate the main diagonal).
            x = monodromy[0][0];
            monodromy[0][0] = - monodromy[1][1];
            monodromy[1][1] = -x;
        }
        if (monodromy[0][1] < 0 || monodromy[1][0] < 0) {
            // Negate the off-diagonal as usual.
            monodromy[0][1] = - monodromy[0][1];
            monodromy[1][0] = - monodromy[1][0];
        }
    }

    // We now have a matrix whose entries are all non-negative.
    // Run through a cycle of equivalent matrices, and choose the nicest.
    // I'm pretty sure I can prove that this is a cycle, but the proof
    // really should be written down.
    NMatrix2 start = monodromy;
    NMatrix2 best = monodromy;
    while (1) {
        // INV: monodromy has all non-negative entries.
        // INV: best contains the best seen matrix, including the current one.

        // It can be proven (via det = +/-1) that one row must dominate
        // another, unless we have [ 1 0 | 0 1 ] or [ 0 1 | 1 0 ].
        if (monodromy.isIdentity()) {
            if (allNegative)
                monodromy.negate();
            return;
        }
        if (monodromy[0][0] == 0 && monodromy[0][1] == 1 &&
                monodromy[1][0] == 1 && monodromy[1][1] == 0) {
            if (allNegative)
                monodromy.negate();
            return;
        }

        // We know at this point that one row dominates the other.
        if (monodromy[0][0] >= monodromy[1][0] &&
                monodromy[0][1] >= monodromy[1][1])
            subtractRCUp();
        else
            subtractRCDown();

        // Looking at a new matrix.
        if (monodromy == start)
            break;

        if (NTorusBundle::simplerNonNeg(monodromy, best))
            best = monodromy;
    }

    // In the orientable case, run this all again for the rotated matrix.
    // This is not necessary in the non-orientable case since the
    // rotated matrix belongs to the same cycle as the original.
    if (det > 0) {
        rotate();
        if (NTorusBundle::simplerNonNeg(monodromy, best))
            best = monodromy;

        start = monodromy;
        while (1) {
            if (monodromy.isIdentity()) {
                if (allNegative)
                    monodromy.negate();
                return;
            }
            if (monodromy[0][0] == 0 && monodromy[0][1] == 1 &&
                    monodromy[1][0] == 1 && monodromy[1][1] == 0) {
                if (allNegative)
                    monodromy.negate();
                return;
            }

            // We know at this point that one row dominates the other.
            if (monodromy[0][0] >= monodromy[1][0] &&
                    monodromy[0][1] >= monodromy[1][1])
                subtractRCUp();
            else
                subtractRCDown();

            // Looking at a new matrix.
            if (monodromy == start)
                break;

            if (NTorusBundle::simplerNonNeg(monodromy, best))
                best = monodromy;
        }
    }

    monodromy = best;

    // Don't forget that negative case.
    if (allNegative)
        monodromy.negate();
}

bool NTorusBundle::simplerNonNeg(const NMatrix2& m1, const NMatrix2& m2) {
    // Value symmetric matrices above all else.
    if (m1[0][1] == m1[1][0] && m2[0][1] != m2[1][0])
        return true;
    if (m1[0][1] != m1[1][0] && m2[0][1] == m2[1][0])
        return false;

    // Go for the smallest possible bottom-right element, then so on
    // working our way up.
    if (m1[1][1] < m2[1][1])
        return true;
    if (m1[1][1] > m2[1][1])
        return false;

    if (m1[1][0] < m2[1][0])
        return true;
    if (m1[1][0] > m2[1][0])
        return false;

    if (m1[0][1] < m2[0][1])
        return true;
    if (m1[0][1] > m2[0][1])
        return false;

    if (m1[0][0] < m2[0][0])
        return true;
    return false;
}

} // namespace regina

