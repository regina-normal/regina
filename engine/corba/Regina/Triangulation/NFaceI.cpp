
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

#include "NVertexI.h"
#include "NEdgeI.h"
#include "NFaceI.h"
#include "NComponentI.h"
#include "NBoundaryComponentI.h"
#include "NTetrahedronI.h"

Regina::Triangulation::NComponent_ptr NFace_i::getComponent() {
    return NComponent_i::newWrapper(MY_ENGINE_OBJECT->getComponent());
}
Regina::Triangulation::NBoundaryComponent_ptr
        NFace_i::getBoundaryComponent() {
    return NBoundaryComponent_i::newWrapper(MY_ENGINE_OBJECT->
        getBoundaryComponent());
}
Regina::Triangulation::NVertex_ptr NFace_i::getVertex(CORBA::Long index) {
    return NVertex_i::newWrapper(MY_ENGINE_OBJECT->getVertex((int)index));
}
Regina::Triangulation::NEdge_ptr NFace_i::getEdge(CORBA::Long index) {
    return NEdge_i::newWrapper(MY_ENGINE_OBJECT->getEdge(index));
}
CORBA::Char NFace_i::getEdgeMapping(CORBA::Long index) {
    return MY_ENGINE_OBJECT->getEdgeMapping(index).getPermCode();
}
CORBA::Boolean NFace_i::isBoundary() {
    return MY_ENGINE_OBJECT->isBoundary();
}
CORBA::Long NFace_i::getNumberOfEmbeddings() {
    return MY_ENGINE_OBJECT->getNumberOfEmbeddings();
}
void NFace_i::getEmbedding(Regina::Triangulation::NTetrahedron_out tet,
        CORBA::Long& face, CORBA::Long index) {
    const NFaceEmbedding& emb = MY_ENGINE_OBJECT->getEmbedding(index);
    tet = NTetrahedron_i::newWrapper(emb.getTetrahedron());
    face = emb.getFace();
}

