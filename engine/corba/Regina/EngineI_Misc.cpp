
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

#include "config.h"
#include "regina.h"

#ifdef __NO_INCLUDE_PATHS
	#include "ngrouppresentation.h"
    #include "ncensus.h"
    #include "matrixops.h"
    #include "nlayeredsolidtorus.h"
    #include "nnormalsurfacelist.h"
#else
    #include "engine/algebra/ngrouppresentation.h"
    #include "engine/census/ncensus.h"
    #include "engine/maths/matrixops.h"
    #include "engine/subcomplex/nlayeredsolidtorus.h"
    #include "engine/surfaces/nnormalsurfacelist.h"
#endif

#include "EngineI.h"
#include "NAbelianGroupI.h"
#include "NFileI.h"
#include "NGroupExpressionI.h"
#include "NGroupPresentationI.h"
#include "NLayeredSolidTorusI.h"
#include "NMatrixIntI.h"
#include "NProgressManagerI.h"

Regina::Algebra::NAbelianGroup_ptr Engine_i::newNAbelianGroup_() {
    return NAbelianGroup_i::newWrapper(new ::NAbelianGroup());
}
Regina::Algebra::NAbelianGroup_ptr Engine_i::newNAbelianGroup_NAbelianGroup(
        Regina::Algebra::NAbelianGroup_ptr cloneMe) {
    return NAbelianGroup_i::newWrapper(new ::NAbelianGroup(
        *GET_ENGINE_OBJECT(NAbelianGroup, cloneMe)));
}
Regina::Algebra::NGroupExpression_ptr Engine_i::newNGroupExpression_() {
    return NGroupExpression_i::newWrapper(new ::NGroupExpression());
}
Regina::Algebra::NGroupExpression_ptr
		Engine_i::newNGroupExpression_NGroupExpression(
        Regina::Algebra::NGroupExpression_ptr cloneMe) {
    return NGroupExpression_i::newWrapper(new ::NGroupExpression(
        *GET_ENGINE_OBJECT(NGroupExpression, cloneMe)));
}
Regina::Algebra::NGroupPresentation_ptr Engine_i::newNGroupPresentation_() {
    return NGroupPresentation_i::newWrapper(new ::NGroupPresentation());
}
Regina::Algebra::NGroupPresentation_ptr
		Engine_i::newNGroupPresentation_NGroupPresentation(
        Regina::Algebra::NGroupPresentation_ptr cloneMe) {
    return NGroupPresentation_i::newWrapper(new ::NGroupPresentation(
        *GET_ENGINE_OBJECT(NGroupPresentation, cloneMe)));
}

Regina::File::NFile_ptr Engine_i::newNFile() {
    return NFile_i::newWrapper(new ::NFile());
}

Regina::Maths::NMatrixInt_ptr Engine_i::newNMatrixInt_long_long(
        CORBA::Long rows, CORBA::Long columns) {
    return NMatrixInt_i::newWrapper(new ::NMatrixInt(rows, columns));
}
Regina::Maths::NMatrixInt_ptr Engine_i::newNMatrixInt_NMatrixInt(
        Regina::Maths::NMatrixInt_ptr cloneMe) {
    return NMatrixInt_i::newWrapper(new ::NMatrixInt(
        *GET_ENGINE_OBJECT(NMatrixInt, cloneMe)));
}

Regina::Progress::NProgressManager_ptr Engine_i::newNProgressManager() {
    return NProgressManager_i::newWrapper(new ::NProgressManager());
}

CORBA::Long Engine_i::formCensus(Regina::Packet::NPacket_ptr parent,
        CORBA::Long nTetrahedra, CORBA::Char finiteness,
        CORBA::Char orientability, CORBA::Char boundary,
        CORBA::Long nBdryFaces,
        Regina::Progress::NProgressManager_ptr manager) {
    return ::formCensus(GET_ENGINE_OBJECT(NPacket, parent),
        nTetrahedra, NBoolSet::fromByteCode(finiteness),
        NBoolSet::fromByteCode(orientability),
        NBoolSet::fromByteCode(boundary), nBdryFaces,
        GET_ENGINE_OBJECT(NProgressManager, manager));
}
CORBA::Long Engine_i::getVersionMajor() {
    return ENGINE_VERSION_MAJOR;
}
CORBA::Long Engine_i::getVersionMinor() {
    return ENGINE_VERSION_MINOR;
}
char* Engine_i::getVersionString() {
    return NString(ENGINE_VERSION).dupe();
}
Regina::Subcomplex::NLayeredSolidTorus_ptr Engine_i::isLayeredSolidTorusBase(
		Regina::Triangulation::NTetrahedron_ptr tet) {
	return NLayeredSolidTorus_i::newWrapper(
		::NLayeredSolidTorus::isLayeredSolidTorusBase(
		GET_ENGINE_OBJECT(NTetrahedron, tet)));
}
Regina::Maths::NMatrixInt_ptr Engine_i::makeMatchingEquations(
        Regina::Triangulation::NTriangulation_ptr triangulation,
        CORBA::Long flavour) {
    return NMatrixInt_i::newWrapper(::makeMatchingEquations(
        GET_ENGINE_OBJECT(NTriangulation, triangulation), flavour));
}
void Engine_i::smithNormalForm(Regina::Maths::NMatrixInt_ptr matrix) {
    ::smithNormalForm(*GET_ENGINE_OBJECT(NMatrixInt, matrix));
}
CORBA::Long Engine_i::testEngine(CORBA::Long value) {
    return value;
}

