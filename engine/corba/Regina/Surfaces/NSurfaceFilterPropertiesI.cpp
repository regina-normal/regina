
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

#include "NSurfaceFilterPropertiesI.h"

CORBA::Long NSurfaceFilterProperties_i::getNumberOfECs() {
    return MY_ENGINE_OBJECT->getNumberOfECs();
}
char* NSurfaceFilterProperties_i::getEC(CORBA::Long index) {
    return stringFromLarge(MY_ENGINE_OBJECT->getEC(index));
}
CORBA::Char NSurfaceFilterProperties_i::getOrientability() {
    return MY_ENGINE_OBJECT->getOrientability().getByteCode();
}
CORBA::Char NSurfaceFilterProperties_i::getCompactness() {
    return MY_ENGINE_OBJECT->getCompactness().getByteCode();
}
CORBA::Char NSurfaceFilterProperties_i::getRealBoundary() {
    return MY_ENGINE_OBJECT->getRealBoundary().getByteCode();
}
void NSurfaceFilterProperties_i::addEC(const char* ec) {
    MY_ENGINE_OBJECT->addEC(stringToLarge(ec));
}
void NSurfaceFilterProperties_i::removeEC(const char* ec) {
    MY_ENGINE_OBJECT->removeEC(stringToLarge(ec));
}
void NSurfaceFilterProperties_i::removeAllECs() {
    MY_ENGINE_OBJECT->removeAllECs();
}
void NSurfaceFilterProperties_i::setOrientability(CORBA::Char value) {
    MY_ENGINE_OBJECT->setOrientability(NBoolSet::fromByteCode(value));
}
void NSurfaceFilterProperties_i::setCompactness(CORBA::Char value) {
    MY_ENGINE_OBJECT->setCompactness(NBoolSet::fromByteCode(value));
}
void NSurfaceFilterProperties_i::setRealBoundary(CORBA::Char value) {
    MY_ENGINE_OBJECT->setRealBoundary(NBoolSet::fromByteCode(value));
}

