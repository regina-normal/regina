
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

#include "NPillowTwoSphereI.h"
#include "NFaceI.h"
#include "NTriangulationI.h"

Regina::Subcomplex::NPillowTwoSphere_ptr NPillowTwoSphere_i::cloneMe() {
	return NPillowTwoSphere_i::newWrapper(
		GET_ENGINE_OBJECT(NPillowTwoSphere, this)->clone());
}
Regina::Triangulation::NFace_ptr NPillowTwoSphere_i::getFace(
		CORBA::Long index) {
	return NFace_i::newWrapper(
		GET_ENGINE_OBJECT(NPillowTwoSphere, this)->getFace(index));
}
CORBA::Char NPillowTwoSphere_i::getFaceMapping() {
	return GET_ENGINE_OBJECT(NPillowTwoSphere, this)->getFaceMapping().
		getPermCode();
}
void NPillowTwoSphere_i::reduceTriangulation() {
	GET_ENGINE_OBJECT(NPillowTwoSphere, this)->reduceTriangulation();
}
Regina::Triangulation::NTriangulation_ptr
		NPillowTwoSphere_i::getReducedTriangulation(
		Regina::Triangulation::NTriangulation_ptr original) {
	return NTriangulation_i::newWrapper(
		GET_ENGINE_OBJECT(NPillowTwoSphere, this)->getReducedTriangulation(
		GET_ENGINE_OBJECT(NTriangulation, original)));
}
