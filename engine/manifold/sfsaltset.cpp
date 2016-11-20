
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "manifold/sfs.h"
#include "manifold/sfsaltset.h"

namespace regina {

SFSAltSet::SFSAltSet(const SFSpace* sfs) {
    /**
     * Note that whenever we add a (1,1) twist, we compensate by setting
     * row 2 -> row 2 + row 1 in our conversion matrix.
     */

    // Start with the original, reduced to give obstruction constant zero.
    data_[0] = new SFSpace(*sfs);
    data_[0]->reduce(false);

    long b = data_[0]->obstruction();
    if (b)
        data_[0]->insertFibre(1, -b);

    conversion_[0] = Matrix2(1, 0, -b, 1);
    reflected_[0] = false;

    /**
     * If the space is M/n2, we can replace it with D:(2,1)(2,-1)
     * with fibre and orbifold curves switched.  To preserve the
     * determinant of the matching matrix we will actually use a
     * [0,1,-1,0] switch instead of a [0,1,1,0] switch.
     *
     * In fact we will use D:(2,1)(2,1) instead, which means:
     *
     * M_basis = [  0 1 ] [  1 0 ] D_basis = [ -1 1 ] D_basis;
     *           [ -1 0 ] [ -1 1 ]           [ -1 0 ]
     *
     * D_basis = [ 1 0 ] [  0 -1 ] M_basis = [ 0 -1 ] M_basis.
     *           [ 1 1 ] [  1  0 ]           [ 1 -1 ]
     */
    if (data_[0]->baseClass() == SFSpace::bn2 &&
            data_[0]->baseGenus() == 1 &&
            (! data_[0]->baseOrientable()) &&
            data_[0]->punctures(false) == 1 &&
            data_[0]->punctures(true) == 0 &&
            data_[0]->reflectors() == 0 &&
            data_[0]->fibreCount() == 0 &&
            data_[0]->obstruction() == 0) {
        delete data_[0];

        data_[0] = new SFSpace(SFSpace::bo1, 0 /* genus */,
            1 /* punctures */, 0 /* twisted */,
            0 /* reflectors */, 0 /* twisted */);
        data_[0]->insertFibre(2, 1);
        data_[0]->insertFibre(2, 1);

        conversion_[0] = Matrix2(0, -1, 1, -1) * conversion_[0];
    }

    // Using data_[0] as a foundation, try now for a reflection.
    data_[1] = new SFSpace(*data_[0]);
    data_[1]->reflect();
    data_[1]->reduce(false);

    b = data_[1]->obstruction();
    data_[1]->insertFibre(1, -b);
    conversion_[1] = Matrix2(1, 0, -b, -1) * conversion_[0];
    reflected_[1] = true;

    size_ = 2;

    // In the vanilla case, this is all.  However, we can occasionally
    // do a little more.

    // Can we negate all fibres without reflecting?
    // Note that (1,2) == (1,0) in this case, so this is only
    // interesting if we have an odd number of exceptional fibres.
    if (data_[0]->fibreNegating() && (data_[0]->fibreCount() % 2 != 0)) {
        // Do it by adding a single (1,1).  The subsequent reduce() will
        // negate fibres to bring the obstruction constant back down to
        // zero, giving the desired effect.
        data_[2] = new SFSpace(*data_[0]);
        data_[2]->insertFibre(1, 1);
        data_[2]->reduce(false);

        b = data_[2]->obstruction();
        data_[2]->insertFibre(1, -b);
        conversion_[2] = Matrix2(1, 0, -b + 1, 1) * conversion_[0];
        reflected_[2] = false;

        // And do it again with an added reflection.
        data_[3] = new SFSpace(*data_[0]);
        data_[3]->insertFibre(1, 1);
        data_[3]->reflect();
        data_[3]->reduce(false);

        b = data_[3]->obstruction();
        data_[3]->insertFibre(1, -b);
        conversion_[3] = Matrix2(1, 0, -b - 1, -1) * conversion_[0];
        reflected_[3] = true;

        size_ = 4;
    }
}

void SFSAltSet::deleteAll() {
    for (unsigned i = 0; i < size_; i++)
        delete data_[i];
}

void SFSAltSet::deleteAll(SFSpace* exception) {
    for (unsigned i = 0; i < size_; i++)
        if (data_[i] != exception)
            delete data_[i];
}

void SFSAltSet::deleteAll(SFSpace* exception1, SFSpace* exception2) {
    for (unsigned i = 0; i < size_; i++)
        if (data_[i] != exception1 && data_[i] != exception2)
            delete data_[i];
}

} // namespace regina

