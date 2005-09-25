
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
    // Options we will consider include:
    //
    // - Switching the generators (swap both diagonals)
    // - Negating one of the generators (negate off-diagonal)
    // - Inverting the matrix
    //       (det == +1 -> swap main diagonal, negate off-diagonal)
    //       (det == -1 -> swap & negate main diagonal)
    //
    // What this means is that:
    //
    // - If det == +1, we can independently swap the main diagonal,
    //   swap the off-diagonal, and/or negate the off-diagonal.
    //
    // - If det == -1, we can independently swap the main diagonal,
    //   negate the main diagonal, and/or negate the off-diagonal.
    //   After this, we must swap the off-diagonal iff we did one but
    //   not both of (swap main diagonal, negate main diagonal).
    //
    // The determinant should be +/-1 according to our preconditions,
    // but we'd better check that anyway.
    bool swapMain = false, swapOff = false, negMain = false, negOff = false;

    long det = monodromy.determinant();
    if (det == 1) {
        // First work out how we'd like the main diagonal.
        // Our only option here is to swap.
        swapMain = (monodromy[0][0] < monodromy[1][1]);

        // Now work out how we'd like the off diagonal.
        // We may swap or negate as we please.
        if (monodromy[0][1] >= 0 && monodromy[1][0] >= 0) {
            // >= 0 | >= 0
            swapOff = (monodromy[0][1] < monodromy[1][0]);
        } else if (monodromy[0][1] <= 0 && monodromy[1][0] <= 0) {
            // <= 0 | <= 0
            negOff = true;
            swapOff = (monodromy[0][1] > monodromy[1][0]);
        } else if (monodromy[0][1] < 0) {
            // < 0 | > 0
            if (- monodromy[0][1] < monodromy[1][0])
                swapOff = true;
            else
                negOff = true;
        } else {
            // > 0 | < 0
            if (monodromy[0][1] < - monodromy[1][0])
                swapOff = negOff = true;
        }
    } else if (det == -1) {
        // If the main diagonal elements are equal, swapping is a no-op.
        // This means our forced off-diagonal swap becomes optional.
        bool optionalSwapOff = (monodromy[0][0] == monodromy[1][1]);

        // First work out how we'd like the main diagonal.
        if (monodromy[0][0] >= 0 && monodromy[1][1] >= 0) {
            // >= 0 | >= 0
            swapMain = (monodromy[0][0] < monodromy[1][1]);
        } else if (monodromy[0][0] <= 0 && monodromy[1][1] <= 0) {
            // <= 0 | <= 0
            negMain = true;
            swapMain = (monodromy[0][0] > monodromy[1][1]);
        } else if (monodromy[0][0] < 0) {
            // < 0 | > 0
            if (- monodromy[0][0] < monodromy[1][1])
                swapMain = true;
            else
                negMain = true;
        } else {
            // > 0 | < 0
            if (monodromy[0][0] < - monodromy[1][1])
                swapMain = negMain = true;
        }

        // Now work out how we'd like the off diagonal.
        if (optionalSwapOff) {
            // We may swap or negate as we please.
            if (monodromy[0][1] >= 0 && monodromy[1][0] >= 0) {
                // >= 0 | >= 0
                swapOff = (monodromy[0][1] < monodromy[1][0]);
            } else if (monodromy[0][1] <= 0 && monodromy[1][0] <= 0) {
                // <= 0 | <= 0
                negOff = true;
                swapOff = (monodromy[0][1] > monodromy[1][0]);
            } else if (monodromy[0][1] < 0) {
                // < 0 | > 0
                if (- monodromy[0][1] < monodromy[1][0])
                    swapOff = true;
                else
                    negOff = true;
            } else {
                // > 0 | < 0
                if (monodromy[0][1] < - monodromy[1][0])
                    swapOff = negOff = true;
            }
        } else {
            // Our only choice is whether to negate.
            negOff = (monodromy[0][1] < 0 ||
                (monodromy[0][1] == 0 && monodromy[1][0] < 0));
        }
    } else {
        // Something is very wrong.  Don't touch it.
        std::cerr << "ERROR: NTorusBundle monodromy does not have "
            "determinant +/-1.\n";
        return;
    }

    // Actually do whatever it is we have planned.
    long tmp;

    if (negMain) {
        monodromy[0][0] = -monodromy[0][0];
        monodromy[1][1] = -monodromy[1][1];
    }
    if (negOff) {
        monodromy[0][1] = -monodromy[0][1];
        monodromy[1][0] = -monodromy[1][0];
    }
    if (swapMain) {
        tmp = monodromy[0][0];
        monodromy[0][0] = monodromy[1][1];
        monodromy[1][1] = tmp;
    }
    if (swapOff) {
        tmp = monodromy[0][1];
        monodromy[0][1] = monodromy[1][0];
        monodromy[1][0] = tmp;
    }
}

} // namespace regina

