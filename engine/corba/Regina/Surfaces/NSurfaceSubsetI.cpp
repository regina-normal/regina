
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

#include "NSurfaceSubsetI.h"
#include "NNormalSurfaceI.h"
#include "NTriangulationI.h"

CORBA::Long NSurfaceSubset_i::getFlavour() {
    return MY_ENGINE_OBJECT->getFlavour();
}
CORBA::Boolean NSurfaceSubset_i::allowsAlmostNormal() {
    return MY_ENGINE_OBJECT->allowsAlmostNormal();
}
CORBA::Boolean NSurfaceSubset_i::isEmbeddedOnly() {
    return MY_ENGINE_OBJECT->isEmbeddedOnly();
}
Regina::Triangulation::NTriangulation_ptr
        NSurfaceSubset_i::getTriangulation() {
    return NTriangulation_i::newWrapper(MY_ENGINE_OBJECT->getTriangulation());
}
CORBA::Long NSurfaceSubset_i::getNumberOfSurfaces() {
    return MY_ENGINE_OBJECT->getNumberOfSurfaces();
}
Regina::Surfaces::NNormalSurface_ptr NSurfaceSubset_i::getSurface(
        CORBA::Long index) {
    return NNormalSurface_i::newWrapper((NNormalSurface*)
        (MY_ENGINE_OBJECT->getSurface(index)));
}
Regina::ShareableObject_ptr NSurfaceSubset_i::getShareableObject() {
    return _this();
}

