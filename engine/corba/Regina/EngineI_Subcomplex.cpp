
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

#include "regina.h"

#include "EngineI.h"
#include "NAugTriSolidTorusI.h"
#include "NLayeredChainI.h"
#include "NLayeredLensSpaceI.h"
#include "NLayeredLoopI.h"
#include "NLayeredSolidTorusI.h"
#include "NLensSpaceI.h"
#include "NPillowTwoSphereI.h"
#include "NSFSI.h"
#include "NSnappedBallI.h"
#include "NSnappedTwoSphereI.h"
#include "NSpiralSolidTorusI.h"
#include "NTriSolidTorusI.h"

Regina::Subcomplex::NLayeredChain_ptr
        Engine_i::newNLayeredChain_NTetrahedron_NPerm(
        Regina::Triangulation::NTetrahedron_ptr tet, CORBA::Char roles) {
    return NLayeredChain_i::newWrapper(new ::NLayeredChain(
        GET_ENGINE_OBJECT(NTetrahedron, tet), NPerm(roles)));
}

Regina::Subcomplex::NLayeredChain_ptr
        Engine_i::newNLayeredChain_NLayeredChain(
        Regina::Subcomplex::NLayeredChain_ptr cloneMe) {
    return NLayeredChain_i::newWrapper(new ::NLayeredChain(
        *GET_ENGINE_OBJECT(NLayeredChain, cloneMe)));
}

Regina::Subcomplex::NLensSpace_ptr Engine_i::newNLensSpace_long_long(
        CORBA::Long p, CORBA::Long q) {
    return NLensSpace_i::newWrapper(new ::NLensSpace(p, q));
}

Regina::Subcomplex::NLensSpace_ptr Engine_i::newNLensSpace_NLensSpace(
        Regina::Subcomplex::NLensSpace_ptr cloneMe) {
    return NLensSpace_i::newWrapper(new ::NLensSpace(
        *GET_ENGINE_OBJECT(NLensSpace, cloneMe)));
}

Regina::Subcomplex::NSFS_ptr Engine_i::newNSFS_() {
    return NSFS_i::newWrapper(new ::NSFS());
}

Regina::Subcomplex::NSFS_ptr Engine_i::newNSFS_long_boolean_long(
        CORBA::Long genus, CORBA::Boolean orient, CORBA::Long punctures) {
    return NSFS_i::newWrapper(new ::NSFS(genus, orient, punctures));
}

Regina::Subcomplex::NSFS_ptr Engine_i::newNSFS_NSFS(
        Regina::Subcomplex::NSFS_ptr cloneMe) {
    return NSFS_i::newWrapper(new ::NSFS(*GET_ENGINE_OBJECT(NSFS, cloneMe)));
}

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
Regina::Subcomplex::NAugTriSolidTorus_ptr Engine_i::isAugTriSolidTorus(
        Regina::Triangulation::NComponent_ptr comp) {
    return NAugTriSolidTorus_i::newWrapper(
        ::NAugTriSolidTorus::isAugTriSolidTorus(
        GET_ENGINE_OBJECT(NComponent, comp)));
}
Regina::Subcomplex::NLayeredLensSpace_ptr Engine_i::isLayeredLensSpace(
        Regina::Triangulation::NComponent_ptr comp) {
    return NLayeredLensSpace_i::newWrapper(
        ::NLayeredLensSpace::isLayeredLensSpace(
        GET_ENGINE_OBJECT(NComponent, comp)));
}
Regina::Subcomplex::NLayeredLoop_ptr Engine_i::isLayeredLoop(
        Regina::Triangulation::NComponent_ptr comp) {
    return NLayeredLoop_i::newWrapper(
        ::NLayeredLoop::isLayeredLoop(
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
Regina::Subcomplex::NSpiralSolidTorus_ptr Engine_i::isSpiralSolidTorus(
        Regina::Triangulation::NTetrahedron_ptr tet, CORBA::Char vertexRoles) {
    return NSpiralSolidTorus_i::newWrapper(
        ::NSpiralSolidTorus::isSpiralSolidTorus(
        GET_ENGINE_OBJECT(NTetrahedron, tet), NPerm(vertexRoles)));
}
Regina::Subcomplex::NTriSolidTorus_ptr Engine_i::isTriSolidTorus(
        Regina::Triangulation::NTetrahedron_ptr tet, CORBA::Char vertexRoles) {
    return NTriSolidTorus_i::newWrapper(
        ::NTriSolidTorus::isTriSolidTorus(GET_ENGINE_OBJECT(NTetrahedron, tet),
        NPerm(vertexRoles)));
}

