
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

#include "NComponentI.h"
#include "NBoundaryComponentI.h"
#include "NVertexI.h"
#include "NEdgeI.h"
#include "NFaceI.h"
#include "NTetrahedronI.h"

CORBA::Boolean NComponent_i::isIdeal() {
    return MY_ENGINE_OBJECT->isIdeal();
}
CORBA::Boolean NComponent_i::isOrientable() {
    return MY_ENGINE_OBJECT->isOrientable();
}
CORBA::Boolean NComponent_i::isClosed() {
    return MY_ENGINE_OBJECT->isClosed();
}
CORBA::Long NComponent_i::getNumberOfTetrahedra() {
    return MY_ENGINE_OBJECT->getNumberOfTetrahedra();
}
CORBA::Long NComponent_i::getNumberOfFaces() {
    return MY_ENGINE_OBJECT->getNumberOfFaces();
}
CORBA::Long NComponent_i::getNumberOfEdges() {
    return MY_ENGINE_OBJECT->getNumberOfEdges();
}
CORBA::Long NComponent_i::getNumberOfVertices() {
    return MY_ENGINE_OBJECT->getNumberOfVertices();
}
CORBA::Long NComponent_i::getNumberOfBoundaryComponents() {
    return MY_ENGINE_OBJECT->getNumberOfBoundaryComponents();
}
Regina::Triangulation::NTetrahedron_ptr NComponent_i::getTetrahedron(
        CORBA::Long index) {
    return NTetrahedron_i::newWrapper(MY_ENGINE_OBJECT->getTetrahedron(index));
}
Regina::Triangulation::NFace_ptr NComponent_i::getFace(CORBA::Long index) {
    return NFace_i::newWrapper(MY_ENGINE_OBJECT->getFace(index));
}
Regina::Triangulation::NEdge_ptr NComponent_i::getEdge(CORBA::Long index) {
    return NEdge_i::newWrapper(MY_ENGINE_OBJECT->getEdge(index));
}
Regina::Triangulation::NVertex_ptr NComponent_i::getVertex(CORBA::Long index) {
    return NVertex_i::newWrapper(MY_ENGINE_OBJECT->getVertex(index));
}
Regina::Triangulation::NBoundaryComponent_ptr
        NComponent_i::getBoundaryComponent(CORBA::Long index) {
    return NBoundaryComponent_i::newWrapper(
        MY_ENGINE_OBJECT->getBoundaryComponent(index));
}

