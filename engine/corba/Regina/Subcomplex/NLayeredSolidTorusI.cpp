
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

#include "NLayeredSolidTorusI.h"
#include "NTetrahedronI.h"

Regina::Subcomplex::NLayeredSolidTorus_ptr NLayeredSolidTorus_i::cloneMe() {
    return NLayeredSolidTorus_i::newWrapper(MY_ENGINE_OBJECT->clone());
}
CORBA::Long NLayeredSolidTorus_i::getNumberOfTetrahedra() {
    return MY_ENGINE_OBJECT->getNumberOfTetrahedra();
}
Regina::Triangulation::NTetrahedron_ptr NLayeredSolidTorus_i::getBase() {
    return NTetrahedron_i::newWrapper(MY_ENGINE_OBJECT->getBase());
}
CORBA::Long NLayeredSolidTorus_i::getBaseEdge(CORBA::Long group,
        CORBA::Long index) {
    return MY_ENGINE_OBJECT->getBaseEdge(group, index);
}
CORBA::Long NLayeredSolidTorus_i::getBaseEdgeGroup(CORBA::Long edge) {
    return MY_ENGINE_OBJECT->getBaseEdgeGroup(edge);
}
CORBA::Long NLayeredSolidTorus_i::getBaseFace(CORBA::Long index) {
    return MY_ENGINE_OBJECT->getBaseFace(index);
}
Regina::Triangulation::NTetrahedron_ptr NLayeredSolidTorus_i::getTopLevel() {
    return NTetrahedron_i::newWrapper(MY_ENGINE_OBJECT->getTopLevel());
}
CORBA::Long NLayeredSolidTorus_i::getMeridinalCuts(CORBA::Long group) {
    return MY_ENGINE_OBJECT->getMeridinalCuts(group);
}
CORBA::Long NLayeredSolidTorus_i::getTopEdge(CORBA::Long group,
        CORBA::Long index) {
    return MY_ENGINE_OBJECT->getTopEdge(group, index);
}
CORBA::Long NLayeredSolidTorus_i::getTopEdgeGroup(CORBA::Long edge) {
    return MY_ENGINE_OBJECT->getTopEdgeGroup(edge);
}
CORBA::Long NLayeredSolidTorus_i::getTopFace(CORBA::Long index) {
    return MY_ENGINE_OBJECT->getTopFace(index);
}

