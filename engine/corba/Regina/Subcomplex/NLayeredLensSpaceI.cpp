
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

#include "NLayeredLensSpaceI.h"
#include "NLayeredSolidTorusI.h"

Regina::Subcomplex::NLayeredLensSpace_ptr NLayeredLensSpace_i::cloneMe() {
    return NLayeredLensSpace_i::newWrapper(MY_ENGINE_OBJECT->clone());
}
CORBA::Long NLayeredLensSpace_i::getP() {
    return MY_ENGINE_OBJECT->getP();
}
CORBA::Long NLayeredLensSpace_i::getQ() {
    return MY_ENGINE_OBJECT->getQ();
}
Regina::Subcomplex::NLayeredSolidTorus_ptr NLayeredLensSpace_i::getTorus() {
    return NLayeredSolidTorus_i::newWrapper((NLayeredSolidTorus*)
        &MY_ENGINE_OBJECT->getTorus());
}
CORBA::Long NLayeredLensSpace_i::getMobiusBoundaryGroup() {
    return MY_ENGINE_OBJECT->getMobiusBoundaryGroup();
}
CORBA::Boolean NLayeredLensSpace_i::isSnapped() {
    return MY_ENGINE_OBJECT->isSnapped();
}
CORBA::Boolean NLayeredLensSpace_i::isTwisted() {
    return MY_ENGINE_OBJECT->isTwisted();
}

