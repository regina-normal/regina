
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

#include "NSFSI.h"
#include "NLensSpaceI.h"

CORBA::Long NSFS_i::getOrbitGenus() {
    return MY_ENGINE_OBJECT->getOrbitGenus();
}
CORBA::Boolean NSFS_i::isOrbitOrientable() {
    return MY_ENGINE_OBJECT->isOrbitOrientable();
}
CORBA::Long NSFS_i::getOrbitPunctures() {
    return MY_ENGINE_OBJECT->getOrbitPunctures();
}
CORBA::Long NSFS_i::getFibreCount() {
    return MY_ENGINE_OBJECT->getFibreCount();
}
CORBA::Long NSFS_i::getAlpha(CORBA::Long which) {
    return MY_ENGINE_OBJECT->getFibre(which).alpha;
}
CORBA::Long NSFS_i::getBeta(CORBA::Long which) {
    return MY_ENGINE_OBJECT->getFibre(which).beta;
}
void NSFS_i::insertFibre(CORBA::Long alpha, CORBA::Long beta) {
    MY_ENGINE_OBJECT->insertFibre(NExceptionalFibre(alpha, beta));
}
void NSFS_i::reduce() {
    MY_ENGINE_OBJECT->reduce();
}
Regina::Subcomplex::NLensSpace_ptr NSFS_i::isLensSpace() {
    return NLensSpace_i::newWrapper(MY_ENGINE_OBJECT->isLensSpace());
}
