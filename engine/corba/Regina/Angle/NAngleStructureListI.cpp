
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

#include "NAngleStructureListI.h"
#include "NAngleStructureI.h"
#include "NTriangulationI.h"

Regina::Triangulation::NTriangulation_ptr
        NAngleStructureList_i::getTriangulation() {
    return NTriangulation_i::newWrapper(MY_ENGINE_OBJECT->getTriangulation());
}
CORBA::Long NAngleStructureList_i::getNumberOfStructures() {
    return MY_ENGINE_OBJECT->getNumberOfStructures();
}
Regina::Angle::NAngleStructure_ptr NAngleStructureList_i::getStructure(
        CORBA::Long index) {
    return NAngleStructure_i::newWrapper((NAngleStructure*)
        (MY_ENGINE_OBJECT->getStructure(index)));
}
CORBA::Boolean NAngleStructureList_i::allowsStrict() {
    return MY_ENGINE_OBJECT->allowsStrict();
}
CORBA::Boolean NAngleStructureList_i::allowsTaut() {
    return MY_ENGINE_OBJECT->allowsTaut();
}

