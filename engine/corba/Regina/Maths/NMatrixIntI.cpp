
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

#include "NMatrixIntI.h"

void NMatrixInt_i::initialise(const char* value) {
    MY_ENGINE_OBJECT->initialise(stringToLarge(value));
}
void NMatrixInt_i::makeIdentity() {
    MY_ENGINE_OBJECT->makeIdentity();
}
CORBA::Long NMatrixInt_i::rows() {
    return MY_ENGINE_OBJECT->rows();
}
CORBA::Long NMatrixInt_i::columns() {
    return MY_ENGINE_OBJECT->columns();
}
char* NMatrixInt_i::getEntry(CORBA::Long row, CORBA::Long column) {
    return stringFromLarge(MY_ENGINE_OBJECT->entry(row, column));
}
void NMatrixInt_i::setEntry(CORBA::Long row, CORBA::Long column,
        const char* value) {
    MY_ENGINE_OBJECT->entry(row, column) = stringToLarge(value);
}

void NMatrixInt_i::swapRows(CORBA::Long first, CORBA::Long second) {
    MY_ENGINE_OBJECT->swapRows(first, second);
}
void NMatrixInt_i::swapColumns(CORBA::Long first, CORBA::Long second) {
    MY_ENGINE_OBJECT->swapColumns(first, second);
}
void NMatrixInt_i::addRow_long_long(CORBA::Long source, CORBA::Long dest) {
    MY_ENGINE_OBJECT->addRow(source, dest);
}
void NMatrixInt_i::addRow_long_long_bigInt(CORBA::Long source,
        CORBA::Long dest, const char* copies) {
    MY_ENGINE_OBJECT->addRow(source, dest, stringToLarge(copies));
}
void NMatrixInt_i::addCol_long_long(CORBA::Long source, CORBA::Long dest) {
    MY_ENGINE_OBJECT->addCol(source, dest);
}
void NMatrixInt_i::addCol_long_long_bigInt(CORBA::Long source,
        CORBA::Long dest, const char* copies) {
    MY_ENGINE_OBJECT->addCol(source, dest, stringToLarge(copies));
}
void NMatrixInt_i::multRow(CORBA::Long row, const char* factor) {
    MY_ENGINE_OBJECT->multRow(row, stringToLarge(factor));
}
void NMatrixInt_i::multCol(CORBA::Long col, const char* factor) {
    MY_ENGINE_OBJECT->multCol(col, stringToLarge(factor));
}

