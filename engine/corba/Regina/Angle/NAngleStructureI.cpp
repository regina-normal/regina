
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

#include "NAngleStructureI.h"
#include "NTriangulationI.h"

char* NAngleStructure_i::getAngleNum(CORBA::Long tetIndex,
        CORBA::Long edgePair) {
    return stringFromLarge(MY_ENGINE_OBJECT->
        getAngle(tetIndex, edgePair).getNumerator());
}
char* NAngleStructure_i::getAngleDen(CORBA::Long tetIndex,
        CORBA::Long edgePair) {
    return stringFromLarge(MY_ENGINE_OBJECT->
        getAngle(tetIndex, edgePair).getDenominator());
}
Regina::Triangulation::NTriangulation_ptr
        NAngleStructure_i::getTriangulation() {
    return NTriangulation_i::newWrapper(MY_ENGINE_OBJECT->getTriangulation());
}
CORBA::Boolean NAngleStructure_i::isStrict() {
    return MY_ENGINE_OBJECT->isStrict();
}
CORBA::Boolean NAngleStructure_i::isTaut() {
    return MY_ENGINE_OBJECT->isTaut();
}

