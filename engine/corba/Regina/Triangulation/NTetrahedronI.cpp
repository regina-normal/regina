
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

#include "NTetrahedronI.h"
#include "NVertexI.h"
#include "NEdgeI.h"
#include "NFaceI.h"
#include "NComponentI.h"
#include "NBoundaryComponentI.h"

Regina::Triangulation::NTetrahedron_ptr
		NTetrahedron_i::getAdjacentTetrahedron(CORBA::Long face) {
	return NTetrahedron_i::newWrapper(GET_ENGINE_OBJECT(NTetrahedron, this)->
		getAdjacentTetrahedron(face));
}
CORBA::Char NTetrahedron_i::getAdjacentTetrahedronGluing(CORBA::Long face) {
	return GET_ENGINE_OBJECT(NTetrahedron, this)->
		getAdjacentTetrahedronGluing(face).getPermCode();
}
CORBA::Long NTetrahedron_i::getAdjacentFace(CORBA::Long face) {
	return GET_ENGINE_OBJECT(NTetrahedron, this)->getAdjacentFace(face);
}
CORBA::Boolean NTetrahedron_i::hasBoundary() {
	return GET_ENGINE_OBJECT(NTetrahedron, this)->hasBoundary();
}
Regina::Triangulation::NComponent_ptr NTetrahedron_i::getComponent() {
	return NComponent_i::newWrapper(GET_ENGINE_OBJECT(NTetrahedron, this)->
		getComponent());
}
Regina::Triangulation::NVertex_ptr NTetrahedron_i::getVertex(
		CORBA::Long vertex) {
	return NVertex_i::newWrapper(GET_ENGINE_OBJECT(NTetrahedron, this)->
		getVertex(vertex));
}
Regina::Triangulation::NEdge_ptr NTetrahedron_i::getEdge(CORBA::Long edge) {
	return NEdge_i::newWrapper(GET_ENGINE_OBJECT(NTetrahedron, this)->
		getEdge(edge));
}
Regina::Triangulation::NFace_ptr NTetrahedron_i::getFace(CORBA::Long face) {
	return NFace_i::newWrapper(GET_ENGINE_OBJECT(NTetrahedron, this)->
		getFace(face));
}
CORBA::Char NTetrahedron_i::getEdgeMapping(CORBA::Long edge) {
	return GET_ENGINE_OBJECT(NTetrahedron, this)->
		getEdgeMapping(edge).getPermCode();
}
CORBA::Char NTetrahedron_i::getFaceMapping(CORBA::Long face) {
	return GET_ENGINE_OBJECT(NTetrahedron, this)->
		getFaceMapping(face).getPermCode();
}
void NTetrahedron_i::joinTo(CORBA::Long myFace,
		Regina::Triangulation::NTetrahedron_ptr you, CORBA::Char gluing) {
	GET_ENGINE_OBJECT(NTetrahedron, this)->joinTo(
		myFace, GET_ENGINE_OBJECT(NTetrahedron, you), NPerm(gluing));
}
Regina::Triangulation::NTetrahedron_ptr NTetrahedron_i::unjoin(
		CORBA::Long myFace) {
	return NTetrahedron_i::newWrapper(GET_ENGINE_OBJECT(NTetrahedron, this)->
		unjoin(myFace));
}
void NTetrahedron_i::isolate() {
	GET_ENGINE_OBJECT(NTetrahedron, this)->isolate();
}
char* NTetrahedron_i::getDescription() {
	return GET_ENGINE_OBJECT(NTetrahedron, this)->getDescription().dupe();
}
void NTetrahedron_i::setDescription(const char* description) {
	GET_ENGINE_OBJECT(NTetrahedron, this)->setDescription(description);
}

