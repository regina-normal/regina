
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
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

#include "NAbelianGroupI.h"

void NAbelianGroup_i::addRank(CORBA::Long extraRank) {
    MY_ENGINE_OBJECT->addRank(extraRank);
}
void NAbelianGroup_i::addTorsionElement_bigInt(const char* degree,
        CORBA::Long mult) {
    MY_ENGINE_OBJECT->addTorsionElement(stringToLarge(degree), mult);
}
void NAbelianGroup_i::addTorsionElement_long(CORBA::Long degree,
        CORBA::Long mult) {
    MY_ENGINE_OBJECT->addTorsionElement(degree, mult);
}
void NAbelianGroup_i::addGroup_NMatrixInt(Regina::Maths::NMatrixInt_ptr pres) {
    MY_ENGINE_OBJECT->addGroup(*GET_ENGINE_OBJECT(NMatrixInt, pres));
}
void NAbelianGroup_i::addGroup_NAbelianGroup(Regina::Algebra::NAbelianGroup_ptr
        group) {
    MY_ENGINE_OBJECT->addGroup(*GET_ENGINE_OBJECT(NAbelianGroup, group));
}

CORBA::Long NAbelianGroup_i::getRank() {
    return MY_ENGINE_OBJECT->getRank();
}
CORBA::Long NAbelianGroup_i::getTorsionRank_bigInt(const char* degree) {
    return MY_ENGINE_OBJECT->getTorsionRank(stringToLarge(degree));
}
CORBA::Long NAbelianGroup_i::getTorsionRank_long(CORBA::Long degree) {
    return MY_ENGINE_OBJECT->getTorsionRank(degree);
}
CORBA::Long NAbelianGroup_i::getNumberOfInvariantFactors() {
    return MY_ENGINE_OBJECT->getNumberOfInvariantFactors();
}
char* NAbelianGroup_i::getInvariantFactor(CORBA::Long index) {
    return stringFromLarge(MY_ENGINE_OBJECT->getInvariantFactor(index));
}

