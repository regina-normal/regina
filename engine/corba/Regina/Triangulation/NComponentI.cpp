
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

#include "NComponentI.h"
#include "NBoundaryComponentI.h"
#include "NVertexI.h"
#include "NEdgeI.h"
#include "NFaceI.h"
#include "NTetrahedronI.h"

CORBA::Boolean NComponent_i::isIdeal() {
	return GET_ENGINE_OBJECT(NComponent, this)->isIdeal();
}
CORBA::Boolean NComponent_i::isOrientable() {
	return GET_ENGINE_OBJECT(NComponent, this)->isOrientable();
}
CORBA::Boolean NComponent_i::isClosed() {
	return GET_ENGINE_OBJECT(NComponent, this)->isClosed();
}
CORBA::Long NComponent_i::getNumberOfTetrahedra() {
	return GET_ENGINE_OBJECT(NComponent, this)->getNumberOfTetrahedra();
}
CORBA::Long NComponent_i::getNumberOfFaces() {
	return GET_ENGINE_OBJECT(NComponent, this)->getNumberOfFaces();
}
CORBA::Long NComponent_i::getNumberOfEdges() {
	return GET_ENGINE_OBJECT(NComponent, this)->getNumberOfEdges();
}
CORBA::Long NComponent_i::getNumberOfVertices() {
	return GET_ENGINE_OBJECT(NComponent, this)->getNumberOfVertices();
}
CORBA::Long NComponent_i::getNumberOfBoundaryComponents() {
	return GET_ENGINE_OBJECT(NComponent, this)->
		getNumberOfBoundaryComponents();
}
Regina::Triangulation::NTetrahedron_ptr NComponent_i::getTetrahedron(
		CORBA::Long index) {
	return NTetrahedron_i::newWrapper(GET_ENGINE_OBJECT(NComponent, this)->
		getTetrahedron(index));
}
Regina::Triangulation::NFace_ptr NComponent_i::getFace(CORBA::Long index) {
	return NFace_i::newWrapper(GET_ENGINE_OBJECT(NComponent, this)->
		getFace(index));
}
Regina::Triangulation::NEdge_ptr NComponent_i::getEdge(CORBA::Long index) {
	return NEdge_i::newWrapper(GET_ENGINE_OBJECT(NComponent, this)->
		getEdge(index));
}
Regina::Triangulation::NVertex_ptr NComponent_i::getVertex(CORBA::Long index) {
	return NVertex_i::newWrapper(GET_ENGINE_OBJECT(NComponent, this)->
		getVertex(index));
}
Regina::Triangulation::NBoundaryComponent_ptr
		NComponent_i::getBoundaryComponent(CORBA::Long index) {
	return NBoundaryComponent_i::newWrapper(
		GET_ENGINE_OBJECT(NComponent, this)->getBoundaryComponent(index));
}

