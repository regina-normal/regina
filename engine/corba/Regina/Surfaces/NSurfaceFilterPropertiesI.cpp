
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

#include "NSurfaceFilterPropertiesI.h"

CORBA::Long NSurfaceFilterProperties_i::getNumberOfECs() {
    return GET_ENGINE_OBJECT(NSurfaceFilterProperties, this)->getNumberOfECs();
}
char* NSurfaceFilterProperties_i::getEC(CORBA::Long index) {
    return stringFromLarge(GET_ENGINE_OBJECT(NSurfaceFilterProperties, this)->
        getEC(index));
}
CORBA::Char NSurfaceFilterProperties_i::getOrientability() {
    return GET_ENGINE_OBJECT(NSurfaceFilterProperties, this)->
        getOrientability().getByteCode();
}
CORBA::Char NSurfaceFilterProperties_i::getCompactness() {
    return GET_ENGINE_OBJECT(NSurfaceFilterProperties, this)->
        getCompactness().getByteCode();
}
CORBA::Char NSurfaceFilterProperties_i::getRealBoundary() {
    return GET_ENGINE_OBJECT(NSurfaceFilterProperties, this)->
        getRealBoundary().getByteCode();
}
void NSurfaceFilterProperties_i::addEC(const char* ec) {
    GET_ENGINE_OBJECT(NSurfaceFilterProperties, this)->addEC(
        stringToLarge(ec));
}
void NSurfaceFilterProperties_i::removeEC(const char* ec) {
    GET_ENGINE_OBJECT(NSurfaceFilterProperties, this)->removeEC(
        stringToLarge(ec));
}
void NSurfaceFilterProperties_i::removeAllECs() {
    GET_ENGINE_OBJECT(NSurfaceFilterProperties, this)->removeAllECs();
}
void NSurfaceFilterProperties_i::setOrientability(CORBA::Char value) {
    GET_ENGINE_OBJECT(NSurfaceFilterProperties, this)->
        setOrientability(NBoolSet::fromByteCode(value));
}
void NSurfaceFilterProperties_i::setCompactness(CORBA::Char value) {
    GET_ENGINE_OBJECT(NSurfaceFilterProperties, this)->
        setCompactness(NBoolSet::fromByteCode(value));
}
void NSurfaceFilterProperties_i::setRealBoundary(CORBA::Char value) {
    GET_ENGINE_OBJECT(NSurfaceFilterProperties, this)->
        setRealBoundary(NBoolSet::fromByteCode(value));
}

