
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

#include "NSnappedBallI.h"
#include "NSnappedTwoSphereI.h"
#include "NTriangulationI.h"

Regina::Subcomplex::NSnappedTwoSphere_ptr NSnappedTwoSphere_i::cloneMe() {
    return NSnappedTwoSphere_i::newWrapper(MY_ENGINE_OBJECT->clone());
}
Regina::Subcomplex::NSnappedBall_ptr NSnappedTwoSphere_i::getSnappedBall(
        CORBA::Long index) {
    return NSnappedBall_i::newWrapper(MY_ENGINE_OBJECT->getSnappedBall(index));
}
void NSnappedTwoSphere_i::reduceTriangulation() {
    MY_ENGINE_OBJECT->reduceTriangulation();
}
Regina::Triangulation::NTriangulation_ptr
        NSnappedTwoSphere_i::getReducedTriangulation(
        Regina::Triangulation::NTriangulation_ptr original) {
    return NTriangulation_i::newWrapper(
        MY_ENGINE_OBJECT->getReducedTriangulation(
        GET_ENGINE_OBJECT(NTriangulation, original)));
}
