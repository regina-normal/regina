
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

#include "NNormalSurfaceI.h"
#include "NTriangulationI.h"

char* NNormalSurface_i::getTriangleCoord(CORBA::Long tetIndex,
        CORBA::Long vertex) {
    return stringFromLarge(MY_ENGINE_OBJECT->
        getTriangleCoord(tetIndex, vertex));
}
char* NNormalSurface_i::getQuadCoord(CORBA::Long tetIndex,
        CORBA::Long quadType) {
    return stringFromLarge(MY_ENGINE_OBJECT->getQuadCoord(tetIndex, quadType));
}
char* NNormalSurface_i::getOctCoord(CORBA::Long tetIndex,
        CORBA::Long octType) {
    return stringFromLarge(MY_ENGINE_OBJECT->getOctCoord(tetIndex, octType));
}
char* NNormalSurface_i::getEdgeWeight(CORBA::Long edgeIndex) {
    return stringFromLarge(MY_ENGINE_OBJECT->getEdgeWeight(edgeIndex));
}
char* NNormalSurface_i::getFaceArcs(CORBA::Long faceIndex,
        CORBA::Long faceVertex) {
    return stringFromLarge(MY_ENGINE_OBJECT->
        getFaceArcs(faceIndex, faceVertex));
}
CORBA::Long NNormalSurface_i::getNumberOfCoords() {
    return MY_ENGINE_OBJECT->getNumberOfCoords();
}
Regina::Triangulation::NTriangulation_ptr
        NNormalSurface_i::getTriangulation() {
    return NTriangulation_i::newWrapper(MY_ENGINE_OBJECT->getTriangulation());
}
char* NNormalSurface_i::getName() {
    return MY_ENGINE_OBJECT->getName().dupe();
}
void NNormalSurface_i::setName(const char* newName) {
    MY_ENGINE_OBJECT->setName(newName);
}
CORBA::Boolean NNormalSurface_i::isCompact() {
    return MY_ENGINE_OBJECT->isCompact();
}
char* NNormalSurface_i::getEulerCharacteristic() {
    return stringFromLarge(MY_ENGINE_OBJECT->getEulerCharacteristic());
}
CORBA::Long NNormalSurface_i::isOrientable() {
    return MY_ENGINE_OBJECT->isOrientable();
}
CORBA::Long NNormalSurface_i::isTwoSided() {
    return MY_ENGINE_OBJECT->isTwoSided();
}
CORBA::Long NNormalSurface_i::isConnected() {
    return MY_ENGINE_OBJECT->isConnected();
}
CORBA::Boolean NNormalSurface_i::hasRealBoundary() {
    return MY_ENGINE_OBJECT->hasRealBoundary();
}
CORBA::Boolean NNormalSurface_i::isVertexLinking() {
    return MY_ENGINE_OBJECT->isVertexLinking();
}
CORBA::Boolean NNormalSurface_i::isSplitting() {
    return MY_ENGINE_OBJECT->isSplitting();
}
Regina::Triangulation::NTriangulation_ptr NNormalSurface_i::cutAlong() {
    return NTriangulation_i::newWrapper(MY_ENGINE_OBJECT->cutAlong());
}
Regina::Triangulation::NTriangulation_ptr NNormalSurface_i::crush() {
    return NTriangulation_i::newWrapper(MY_ENGINE_OBJECT->crush());
}

