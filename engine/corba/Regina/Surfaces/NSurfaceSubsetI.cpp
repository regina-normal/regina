
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

#include "NSurfaceSubsetI.h"
#include "NNormalSurfaceI.h"
#include "NTriangulationI.h"

CORBA::Long NSurfaceSubset_i::getFlavour() {
    return GET_ENGINE_OBJECT(NSurfaceSubset, this)->getFlavour();
}
CORBA::Boolean NSurfaceSubset_i::allowsAlmostNormal() {
    return GET_ENGINE_OBJECT(NSurfaceSubset, this)->allowsAlmostNormal();
}
CORBA::Boolean NSurfaceSubset_i::isEmbeddedOnly() {
    return GET_ENGINE_OBJECT(NSurfaceSubset, this)->isEmbeddedOnly();
}
Regina::Triangulation::NTriangulation_ptr
        NSurfaceSubset_i::getTriangulation() {
    return NTriangulation_i::newWrapper(
        GET_ENGINE_OBJECT(NSurfaceSubset, this)->getTriangulation());
}
CORBA::Long NSurfaceSubset_i::getNumberOfSurfaces() {
    return GET_ENGINE_OBJECT(NSurfaceSubset, this)->getNumberOfSurfaces();
}
Regina::Surfaces::NNormalSurface_ptr NSurfaceSubset_i::getSurface(
        CORBA::Long index) {
    return NNormalSurface_i::newWrapper((NNormalSurface*)
        (GET_ENGINE_OBJECT(NSurfaceSubset, this)->getSurface(index)));
}
Regina::ShareableObject_ptr NSurfaceSubset_i::getShareableObject() {
    return _this();
}

