
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

#include "NNormalSurfaceListI.h"
#include "NNormalSurfaceI.h"
#include "NTriangulationI.h"
#include "NMatrixIntI.h"

CORBA::Long NNormalSurfaceList_i::getFlavour() {
    return GET_ENGINE_OBJECT(NNormalSurfaceList, this)->getFlavour();
}
CORBA::Boolean NNormalSurfaceList_i::allowsAlmostNormal() {
    return GET_ENGINE_OBJECT(NNormalSurfaceList, this)->allowsAlmostNormal();
}
CORBA::Boolean NNormalSurfaceList_i::isEmbeddedOnly() {
    return GET_ENGINE_OBJECT(NNormalSurfaceList, this)->isEmbeddedOnly();
}
Regina::Triangulation::NTriangulation_ptr
        NNormalSurfaceList_i::getTriangulation() {
    return NTriangulation_i::newWrapper(
        GET_ENGINE_OBJECT(NNormalSurfaceList, this)->getTriangulation());
}
CORBA::Long NNormalSurfaceList_i::getNumberOfSurfaces() {
    return GET_ENGINE_OBJECT(NNormalSurfaceList, this)->getNumberOfSurfaces();
}
Regina::Surfaces::NNormalSurface_ptr NNormalSurfaceList_i::getSurface(
        CORBA::Long index) {
    return NNormalSurface_i::newWrapper((NNormalSurface*)
        (GET_ENGINE_OBJECT(NNormalSurfaceList, this)->getSurface(index)));
}
Regina::ShareableObject_ptr NNormalSurfaceList_i::getShareableObject() {
    return _this();
}

Regina::Maths::NMatrixInt_ptr
        NNormalSurfaceList_i::recreateMatchingEquations() {
    return NMatrixInt_i::newWrapper(GET_ENGINE_OBJECT(NNormalSurfaceList,
        this)->recreateMatchingEquations());
}

