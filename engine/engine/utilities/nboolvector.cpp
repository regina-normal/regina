
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

#include "utilities/nboolvector.h"

#include <iostream.h>

NBoolVector::NBoolVector(const NBoolVector& cloneMe) : nBits(cloneMe.nBits),
        nInts(cloneMe.nInts),
        unusedBits(cloneMe.unusedBits) {
    array = new unsigned[nInts];
    for (unsigned long i=0; i<nInts; i++)
        array[i] = cloneMe.array[i];
}

NBoolVector::NBoolVector(unsigned long size, bool value) : nBits(size),
        nInts(((size-1)/bitsPerInt) + 1),
        unusedBits((unsigned(1) <<
            ((bitsPerInt - 1) - ((size-1) % bitsPerInt))) - 1) {
    array = new unsigned[nInts];
    if (value)
        for (unsigned long i=0; i<nInts; i++) {
            array[i] = 0;
            array[i] = ~array[i];
        }
    else
        for (unsigned long i=0; i<nInts; i++)
            array[i] = 0;
}

void NBoolVector::clear() {
    for (unsigned long i=0; i<nInts; i++)
        array[i] = 0;
}

void NBoolVector::setElement(unsigned long index, bool value) {
    if (value)
        array[index / bitsPerInt] |=
            (unsigned(1) << (bitsPerInt - 1 - (index % bitsPerInt)));
    else
        array[index / bitsPerInt] &=
            ~ (unsigned(1) << (bitsPerInt - 1 - (index % bitsPerInt)));
}

void NBoolVector::operator &= (const NBoolVector& other) {
    for (unsigned long i=0; i<nInts; i++)
        array[i] &= other.array[i];
}

void NBoolVector::operator |= (const NBoolVector& other) {
    for (unsigned long i=0; i<nInts; i++)
        array[i] |= other.array[i];
}

void NBoolVector::negate() {
    for (unsigned long i=0; i<nInts; i++)
        array[i] = ~array[i];
}

bool NBoolVector::contains(const NBoolVector& subvector) const {
    for (unsigned long i=0; i<nInts-1; i++)
        if (array[i] & subvector.array[i] != subvector.array[i])
            return false;
    if (((array[nInts-1] & subvector.array[nInts-1]) | unusedBits)
            != (subvector.array[nInts-1] | unusedBits))
        return false;
    return true;
}

bool NBoolVector::operator < (const NBoolVector& other) const {
    for (unsigned long i=0; i<nInts-1; i++) {
        if (array[i] < other.array[i])
            return true;
        if (array[i] > other.array[i])
            return false;
    }
    return ((array[nInts-1] | unusedBits) <
        (other.array[nInts-1] | unusedBits));
}

bool NBoolVector::operator == (const NBoolVector& other) const {
    for (unsigned long i=0; i<nInts-1; i++)
        if (array[i] != other.array[i])
            return false;
    return ((array[nInts-1] | unusedBits) ==
        (other.array[nInts-1] | unusedBits));
}

bool NBoolVector::isAllFalse() const {
    for (unsigned long i=0; i<nInts-1; i++)
        if (array[i])
            return false;
    return ((array[nInts-1] | unusedBits) == unusedBits);
}

bool NBoolVector::isAllTrue() const {
    for (unsigned long i=0; i<nInts-1; i++)
        if (~array[i])
            return false;
    return !(~(array[nInts-1] | unusedBits));
}

ostream& operator << (ostream& out, const NBoolVector& vector) {
    for (unsigned long i=0; i<vector.nBits; i++)
        out << (vector[i] ? '1' : '0');
    return out;
}                                                                               

