
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <strstream.h>

#include "algebra/nabeliangroup.h"
#include "maths/matrixops.h"
#include "file/nfile.h"

NAbelianGroup::NAbelianGroup(const NAbelianGroup& g) : rank(g.rank) {
    NDoubleListIterator<NLargeInteger> it(g.invariantFactors);
    while (!it.done()) {
        invariantFactors.addLast(*it);
        it++;
    }
}

void NAbelianGroup::addTorsionElement(const NLargeInteger& degree,
        unsigned mult) {
    // If there are no current torsion elements, just throw in the new
    // ones.
    if (invariantFactors.size() == 0) {
        for (unsigned j=0; j<mult; j++)
            invariantFactors.addLast(degree);
        return;
    }

    // Build a presentation matrix for the torsion.
    unsigned len = invariantFactors.size() + mult;
    NMatrixInt a(len, len);

    // Put our own invariant factors in the top.
    unsigned i=0;
    NDoubleListIterator<NLargeInteger> it(invariantFactors);
    while (! it.done()) {
        a.entry(i,i) = *it;
        i++;
        it++;
    }

    // Put the passed torsion elements beneath.
    for (unsigned j=0; j<mult; j++) {
        a.entry(i,i) = degree;
        i++;
    }

    // Go calculate!
    smithNormalForm(a);
    replaceTorsion(a);
}

void NAbelianGroup::addTorsionElements(const NDoubleList<NLargeInteger>&
        torsion) {
    // Build a presentation matrix for the torsion.
    unsigned len = invariantFactors.size() + torsion.size();
    NMatrixInt a(len, len);

    // Put our own invariant factors in the top.
    unsigned i=0;
    NDoubleListIterator<NLargeInteger> it(invariantFactors);
    while (! it.done()) {
        a.entry(i,i) = *it;
        i++;
        it++;
    }

    // Put the passed torsion elements beneath.
    it.init(torsion);
    while (! it.done()) {
        a.entry(i,i) = *it;
        i++;
        it++;
    }

    // Go calculate!
    smithNormalForm(a);
    replaceTorsion(a);
}

void NAbelianGroup::addGroup(const NMatrixInt& presentation) {
    // Prepare to calculate invariant factors.
    unsigned len = invariantFactors.size();
    NMatrixInt a(len + presentation.rows(), len + presentation.columns());

    // Fill in the complete presentation matrix.
    // Fill the bottom half of the matrix with the presentation.
    unsigned i,j;
    for (i=0; i<presentation.rows(); i++)
        for (j=0; j<presentation.columns(); j++)
            a.entry(len + i, len + j) = presentation.entry(i, j);
    
    // Fill in the invariant factors in the top.
    i = 0;
    NDoubleListIterator<NLargeInteger> it(invariantFactors);
    while (! it.done()) {
        a.entry(i, i) = *it;
        i++;
        it++;
    }

    // Go calculate!
    smithNormalForm(a);
    replaceTorsion(a);
}

void NAbelianGroup::addGroup(const NAbelianGroup& group) {
    rank += group.rank;

    // Work out the torsion elements.
    if (invariantFactors.size() == 0) {
        // Copy the other group's factors!
        NDoubleListIterator<NLargeInteger> it(group.invariantFactors);
        while (! it.done()) {
            invariantFactors.addLast(*it);
            it++;
        }
        return;
    }
    if (group.invariantFactors.size() == 0)
        return;

    // We will have to calculate the invariant factors ourselves.
    unsigned len = invariantFactors.size() + group.invariantFactors.size();
    NMatrixInt a(len, len);

    // Put our own invariant factors in the top.
    unsigned i = 0;
    NDoubleListIterator<NLargeInteger> it(invariantFactors);
    while (! it.done()) {
        a.entry(i,i) = *it;
        i++;
        it++;
    }

    // Put the other group's invariant factors beneath.
    it.init(group.invariantFactors);
    while (! it.done()) {
        a.entry(i,i) = *it;
        i++;
        it++;
    }

    // Go calculate!
    smithNormalForm(a);
    replaceTorsion(a);
}

unsigned NAbelianGroup::getTorsionRank(const NLargeInteger& degree) const {
    NDoubleListIterator<NLargeInteger> it;
    it.initEnd(invariantFactors);
    unsigned ans = 0;
    // Because we have SNF, we can bail as soon as we reach a factor
    // that is not divisible by degree.
    while (! it.done()) {
        if (((*it) % degree) == 0)
            ans++;
        else
            return ans;
        it--;
    }
    return ans;
}

void NAbelianGroup::writeTextShort(ostream& out) const {
    bool writtenSomething = false;

    if (rank > 0) {
        if (rank > 1)
            out << rank << ' ';
        out << 'Z';
        writtenSomething = true;
    }

    NDoubleListIterator<NLargeInteger> it(invariantFactors);
    NLargeInteger currDegree;
    unsigned currMult = 0;
    while(true) {
        if (! it.done()) {
            if ((*it) == currDegree) {
                currMult++;
                it++;
                continue;
            }
        }
        if (currMult > 0) {
            if (writtenSomething)
                out << " + ";
            if (currMult > 1)
                out << currMult << ' ';
            out << "Z_" << currDegree.stringValue();
            writtenSomething = true;
        }
        if (it.done())
            break;
        currDegree = *it;
        currMult = 1;
        it++;
    }

    if (! writtenSomething)
        out << '0';
}

void NAbelianGroup::replaceTorsion(const NMatrixInt& matrix) {
    // Delete any preexisting torsion.
    invariantFactors.flush();

    // Run up the diagonal until we hit 1.
    // Hopefully this will be faster than running down the diagonal
    // looking for 0 because the SNF calculation should end up with
    // many 1s for a unnecessarily large presentation matrix such as
    // is produced for instance by homology calculations.
    unsigned rows = matrix.rows();
    unsigned i = matrix.columns();
    if (i > rows) {
        rank += (i - rows);
        i = rows;
    }
    while (i > 0) {
        if (matrix.entry(i-1, i-1) == 0)
            rank++;
        else if (matrix.entry(i-1, i-1) == 1)
            return;
        else
            invariantFactors.addFirst(matrix.entry(i-1, i-1));
        i--;
    }
}

void NAbelianGroup::writeToFile(NFile& out) const {
    out.writeUInt(rank);
    out.writeULong(invariantFactors.size());
    NDoubleListIterator<NLargeInteger> it(invariantFactors);
    while (! it.done()) {
        out.writeLarge(*it);
        it++;
    }
}

NAbelianGroup* NAbelianGroup::readFromFile(NFile& in) {
    NAbelianGroup* ans = new NAbelianGroup();
    ans->rank = in.readUInt();
    unsigned long nFactors = in.readULong();
    for (unsigned long i=0; i<nFactors; i++)
        ans->invariantFactors.addLast(in.readLarge());
    return ans;
}

