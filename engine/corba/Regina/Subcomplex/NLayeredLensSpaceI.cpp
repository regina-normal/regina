
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

#include "NLayeredLensSpaceI.h"
#include "NLayeredSolidTorusI.h"

Regina::Subcomplex::NLayeredLensSpace_ptr NLayeredLensSpace_i::cloneMe() {
    return NLayeredLensSpace_i::newWrapper(
        GET_ENGINE_OBJECT(NLayeredLensSpace, this)->clone());
}
CORBA::Long NLayeredLensSpace_i::getP() {
    return GET_ENGINE_OBJECT(NLayeredLensSpace, this)->getP();
}
CORBA::Long NLayeredLensSpace_i::getQ() {
    return GET_ENGINE_OBJECT(NLayeredLensSpace, this)->getQ();
}
Regina::Subcomplex::NLayeredSolidTorus_ptr NLayeredLensSpace_i::getTorus() {
    return NLayeredSolidTorus_i::newWrapper((NLayeredSolidTorus*)
        &GET_ENGINE_OBJECT(NLayeredLensSpace, this)->getTorus());
}
CORBA::Long NLayeredLensSpace_i::getMobiusBoundaryGroup() {
    return GET_ENGINE_OBJECT(NLayeredLensSpace, this)->
        getMobiusBoundaryGroup();
}
CORBA::Boolean NLayeredLensSpace_i::isSnapped() {
    return GET_ENGINE_OBJECT(NLayeredLensSpace, this)->isSnapped();
}
CORBA::Boolean NLayeredLensSpace_i::isTwisted() {
    return GET_ENGINE_OBJECT(NLayeredLensSpace, this)->isTwisted();
}

