
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

#ifndef __NNORMALSURFACEI_H
#define __NNORMALSURFACEI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
	#include "nnormalsurface.h"
	#include "corbatools.h"
#else
	#include "engine/surfaces/nnormalsurface.h"
	#include "corba/corbatools.h"
#endif

#include "NNormalSurfaceIDL.h"
#include "ShareableObjectI.h"

class NNormalSurface_i : public virtual Regina::Surfaces::_sk_NNormalSurface,
		public ShareableObject_i {
	protected:
		NNormalSurface_i(::NNormalSurface* newCppPtr) :
				ShareableObject_i(newCppPtr) {
		}
	public:
		STANDARD_NEW_WRAPPER(NNormalSurface, NNormalSurface_i,
			Regina::Surfaces::NNormalSurface_ptr)
		
		virtual char* getTriangleCoord(CORBA::Long tetIndex,
			CORBA::Long vertex);
		virtual char* getQuadCoord(CORBA::Long tetIndex, CORBA::Long quadType);
		virtual char* getOctCoord(CORBA::Long tetIndex, CORBA::Long octType);
		virtual char* getEdgeWeight(CORBA::Long edgeIndex);
		virtual char* getFaceArcs(CORBA::Long faceIndex,
			CORBA::Long faceVertex);
		virtual CORBA::Long getNumberOfCoords();
		virtual Regina::Triangulation::NTriangulation_ptr getTriangulation();
		virtual CORBA::Boolean isCompact();
		virtual char* getEulerCharacteristic();
		virtual CORBA::Long isOrientable();
		virtual CORBA::Boolean hasRealBoundary();
};

#endif

