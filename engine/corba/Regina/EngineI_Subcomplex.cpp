
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

#include "config.h"
#include "regina.h"

#include "EngineI.h"
#include "NLayeredLensSpaceI.h"
#include "NLayeredSolidTorusI.h"
#include "NPillowTwoSphereI.h"
#include "NSnappedBallI.h"
#include "NSnappedTwoSphereI.h"

Regina::Subcomplex::NPillowTwoSphere_ptr
        Engine_i::formsPillowTwoSphere(Regina::Triangulation::NFace_ptr face1,
        Regina::Triangulation::NFace_ptr face2) {
    return NPillowTwoSphere_i::newWrapper(
        ::NPillowTwoSphere::formsPillowTwoSphere(
        GET_ENGINE_OBJECT(NFace, face1),
        GET_ENGINE_OBJECT(NFace, face2)));
}

Regina::Subcomplex::NSnappedTwoSphere_ptr
        Engine_i::formsSnappedTwoSphere_NSnappedBall(
        Regina::Subcomplex::NSnappedBall_ptr p1,
        Regina::Subcomplex::NSnappedBall_ptr p2) {
    return NSnappedTwoSphere_i::newWrapper(
        ::NSnappedTwoSphere::formsSnappedTwoSphere(
        GET_ENGINE_OBJECT(NSnappedBall, p1),
        GET_ENGINE_OBJECT(NSnappedBall, p2)));
}
Regina::Subcomplex::NSnappedTwoSphere_ptr
        Engine_i::formsSnappedTwoSphere_NTetrahedron(
        Regina::Triangulation::NTetrahedron_ptr p1,
        Regina::Triangulation::NTetrahedron_ptr p2) {
    return NSnappedTwoSphere_i::newWrapper(
        ::NSnappedTwoSphere::formsSnappedTwoSphere(
        GET_ENGINE_OBJECT(NTetrahedron, p1),
        GET_ENGINE_OBJECT(NTetrahedron, p2)));
}
Regina::Subcomplex::NLayeredLensSpace_ptr Engine_i::isLayeredLensSpace(
        Regina::Triangulation::NComponent_ptr comp) {
    return NLayeredLensSpace_i::newWrapper(
        ::NLayeredLensSpace::isLayeredLensSpace(
        GET_ENGINE_OBJECT(NComponent, comp)));
}
Regina::Subcomplex::NLayeredSolidTorus_ptr Engine_i::isLayeredSolidTorusBase(
        Regina::Triangulation::NTetrahedron_ptr tet) {
    return NLayeredSolidTorus_i::newWrapper(
        ::NLayeredSolidTorus::isLayeredSolidTorusBase(
        GET_ENGINE_OBJECT(NTetrahedron, tet)));
}
Regina::Subcomplex::NSnappedBall_ptr Engine_i::isSnappedBall(
        Regina::Triangulation::NTetrahedron_ptr tet) {
    return NSnappedBall_i::newWrapper(
        ::NSnappedBall::isSnappedBall(GET_ENGINE_OBJECT(NTetrahedron, tet)));
}

