
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

#include "NNormalSurfaceI.h"
#include "NTriangulationI.h"

char* NNormalSurface_i::getTriangleCoord(CORBA::Long tetIndex,
        CORBA::Long vertex) {
    return stringFromLarge(GET_ENGINE_OBJECT(NNormalSurface, this)->
        getTriangleCoord(tetIndex, vertex));
}
char* NNormalSurface_i::getQuadCoord(CORBA::Long tetIndex,
        CORBA::Long quadType) {
    return stringFromLarge(GET_ENGINE_OBJECT(NNormalSurface, this)->
        getQuadCoord(tetIndex, quadType));
}
char* NNormalSurface_i::getOctCoord(CORBA::Long tetIndex,
        CORBA::Long octType) {
    return stringFromLarge(GET_ENGINE_OBJECT(NNormalSurface, this)->
        getOctCoord(tetIndex, octType));
}
char* NNormalSurface_i::getEdgeWeight(CORBA::Long edgeIndex) {
    return stringFromLarge(GET_ENGINE_OBJECT(NNormalSurface, this)->
        getEdgeWeight(edgeIndex));
}
char* NNormalSurface_i::getFaceArcs(CORBA::Long faceIndex,
        CORBA::Long faceVertex) {
    return stringFromLarge(GET_ENGINE_OBJECT(NNormalSurface, this)->
        getFaceArcs(faceIndex, faceVertex));
}
CORBA::Long NNormalSurface_i::getNumberOfCoords() {
    return GET_ENGINE_OBJECT(NNormalSurface, this)->getNumberOfCoords();
}
Regina::Triangulation::NTriangulation_ptr
        NNormalSurface_i::getTriangulation() {
    return NTriangulation_i::newWrapper(
        GET_ENGINE_OBJECT(NNormalSurface, this)->getTriangulation());
}
CORBA::Boolean NNormalSurface_i::isCompact() {
    return GET_ENGINE_OBJECT(NNormalSurface, this)->isCompact();
}
char* NNormalSurface_i::getEulerCharacteristic() {
    return stringFromLarge(GET_ENGINE_OBJECT(NNormalSurface, this)->
        getEulerCharacteristic());
}
CORBA::Long NNormalSurface_i::isOrientable() {
    return GET_ENGINE_OBJECT(NNormalSurface, this)->isOrientable();
}
CORBA::Boolean NNormalSurface_i::hasRealBoundary() {
    return GET_ENGINE_OBJECT(NNormalSurface, this)->hasRealBoundary();
}

