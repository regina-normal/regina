
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

#include "NSpiralSolidTorusI.h"
#include "NTetrahedronI.h"

Regina::Subcomplex::NSpiralSolidTorus_ptr NSpiralSolidTorus_i::cloneMe() {
    return NSpiralSolidTorus_i::newWrapper(MY_ENGINE_OBJECT->clone());
}
CORBA::Long NSpiralSolidTorus_i::getNumberOfTetrahedra() {
    return MY_ENGINE_OBJECT->getNumberOfTetrahedra();
}
Regina::Triangulation::NTetrahedron_ptr NSpiralSolidTorus_i::getTetrahedron(
        CORBA::Long index) {
    return NTetrahedron_i::newWrapper(MY_ENGINE_OBJECT->getTetrahedron(index));
}
CORBA::Char NSpiralSolidTorus_i::getVertexRoles(CORBA::Long index) {
    return MY_ENGINE_OBJECT->getVertexRoles(index).getPermCode();
}
void NSpiralSolidTorus_i::reverse() {
    MY_ENGINE_OBJECT->reverse();
}
void NSpiralSolidTorus_i::cycle(CORBA::Long k) {
    MY_ENGINE_OBJECT->cycle(k);
}
CORBA::Boolean NSpiralSolidTorus_i::makeCanonical(
        Regina::Triangulation::NTriangulation_ptr tri) {
    return MY_ENGINE_OBJECT->makeCanonical(
        GET_ENGINE_OBJECT(NTriangulation, tri));
}
CORBA::Boolean NSpiralSolidTorus_i::isCanonical(
        Regina::Triangulation::NTriangulation_ptr tri) {
    return MY_ENGINE_OBJECT->isCanonical(
        GET_ENGINE_OBJECT(NTriangulation, tri));
}

