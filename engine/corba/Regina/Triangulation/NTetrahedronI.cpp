
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

#include "NTetrahedronI.h"
#include "NVertexI.h"
#include "NEdgeI.h"
#include "NFaceI.h"
#include "NComponentI.h"
#include "NBoundaryComponentI.h"

Regina::Triangulation::NTetrahedron_ptr
        NTetrahedron_i::getAdjacentTetrahedron(CORBA::Long face) {
    return NTetrahedron_i::newWrapper(MY_ENGINE_OBJECT->
        getAdjacentTetrahedron(face));
}
CORBA::Char NTetrahedron_i::getAdjacentTetrahedronGluing(CORBA::Long face) {
    return MY_ENGINE_OBJECT->getAdjacentTetrahedronGluing(face).getPermCode();
}
CORBA::Long NTetrahedron_i::getAdjacentFace(CORBA::Long face) {
    return MY_ENGINE_OBJECT->getAdjacentFace(face);
}
CORBA::Boolean NTetrahedron_i::hasBoundary() {
    return MY_ENGINE_OBJECT->hasBoundary();
}
Regina::Triangulation::NComponent_ptr NTetrahedron_i::getComponent() {
    return NComponent_i::newWrapper(MY_ENGINE_OBJECT->getComponent());
}
Regina::Triangulation::NVertex_ptr NTetrahedron_i::getVertex(
        CORBA::Long vertex) {
    return NVertex_i::newWrapper(MY_ENGINE_OBJECT->getVertex(vertex));
}
Regina::Triangulation::NEdge_ptr NTetrahedron_i::getEdge(CORBA::Long edge) {
    return NEdge_i::newWrapper(MY_ENGINE_OBJECT->getEdge(edge));
}
Regina::Triangulation::NFace_ptr NTetrahedron_i::getFace(CORBA::Long face) {
    return NFace_i::newWrapper(MY_ENGINE_OBJECT->getFace(face));
}
CORBA::Char NTetrahedron_i::getEdgeMapping(CORBA::Long edge) {
    return MY_ENGINE_OBJECT->getEdgeMapping(edge).getPermCode();
}
CORBA::Char NTetrahedron_i::getFaceMapping(CORBA::Long face) {
    return MY_ENGINE_OBJECT->getFaceMapping(face).getPermCode();
}
void NTetrahedron_i::joinTo(CORBA::Long myFace,
        Regina::Triangulation::NTetrahedron_ptr you, CORBA::Char gluing) {
    MY_ENGINE_OBJECT->joinTo(myFace,
        GET_ENGINE_OBJECT(NTetrahedron, you), NPerm(gluing));
}
Regina::Triangulation::NTetrahedron_ptr NTetrahedron_i::unjoin(
        CORBA::Long myFace) {
    return NTetrahedron_i::newWrapper(MY_ENGINE_OBJECT->unjoin(myFace));
}
void NTetrahedron_i::isolate() {
    MY_ENGINE_OBJECT->isolate();
}
char* NTetrahedron_i::getDescription() {
    return MY_ENGINE_OBJECT->getDescription().dupe();
}
void NTetrahedron_i::setDescription(const char* description) {
    MY_ENGINE_OBJECT->setDescription(description);
}

