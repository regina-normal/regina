
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
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
    GET_ENGINE_OBJECT(NMatrixInt, this)->initialise(
        stringToLarge(value));
}
void NMatrixInt_i::makeIdentity() {
    GET_ENGINE_OBJECT(NMatrixInt, this)->makeIdentity();
}
CORBA::Long NMatrixInt_i::rows() {
    return GET_ENGINE_OBJECT(NMatrixInt, this)->rows();
}
CORBA::Long NMatrixInt_i::columns() {
    return GET_ENGINE_OBJECT(NMatrixInt, this)->columns();
}
char* NMatrixInt_i::getEntry(CORBA::Long row, CORBA::Long column) {
    return stringFromLarge(GET_ENGINE_OBJECT(NMatrixInt, this)->
        entry(row, column));
}
void NMatrixInt_i::setEntry(CORBA::Long row, CORBA::Long column,
        const char* value) {
    GET_ENGINE_OBJECT(NMatrixInt, this)->entry(row, column) =
        stringToLarge(value);
}

void NMatrixInt_i::swapRows(CORBA::Long first, CORBA::Long second) {
    GET_ENGINE_OBJECT(NMatrixInt, this)->swapRows(first, second);
}
void NMatrixInt_i::swapColumns(CORBA::Long first, CORBA::Long second) {
    GET_ENGINE_OBJECT(NMatrixInt, this)->swapColumns(first, second);
}
void NMatrixInt_i::addRow_long_long(CORBA::Long source, CORBA::Long dest) {
    GET_ENGINE_OBJECT(NMatrixInt, this)->addRow(source, dest);
}
void NMatrixInt_i::addRow_long_long_bigInt(CORBA::Long source,
        CORBA::Long dest, const char* copies) {
    GET_ENGINE_OBJECT(NMatrixInt, this)->addRow(source, dest,
        stringToLarge(copies));
}
void NMatrixInt_i::addCol_long_long(CORBA::Long source, CORBA::Long dest) {
    GET_ENGINE_OBJECT(NMatrixInt, this)->addCol(source, dest);
}
void NMatrixInt_i::addCol_long_long_bigInt(CORBA::Long source,
        CORBA::Long dest, const char* copies) {
    GET_ENGINE_OBJECT(NMatrixInt, this)->addCol(source, dest,
        stringToLarge(copies));
}
void NMatrixInt_i::multRow(CORBA::Long row, const char* factor) {
    GET_ENGINE_OBJECT(NMatrixInt, this)->multRow(row, stringToLarge(factor));
}
void NMatrixInt_i::multCol(CORBA::Long col, const char* factor) {
    GET_ENGINE_OBJECT(NMatrixInt, this)->multCol(col, stringToLarge(factor));
}

