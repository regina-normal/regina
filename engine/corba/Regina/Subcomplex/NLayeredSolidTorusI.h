
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

#ifndef __NLAYEREDSOLIDTORUSI_H
#define __NLAYEREDSOLIDTORUSI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nlayeredsolidtorus.h"
#else
    #include "engine/subcomplex/nlayeredsolidtorus.h"
#endif

#include "NLayeredSolidTorusIDL.h"
#include "ShareableObjectI.h"

class NLayeredSolidTorus_i :
		public virtual POA_Regina::Subcomplex::NLayeredSolidTorus,
        public ShareableObject_i {
	STANDARD_ENGINE_TYPEDEFS(NLayeredSolidTorus_i, NLayeredSolidTorus,
		Regina::Subcomplex::NLayeredSolidTorus)

    protected:
        NLayeredSolidTorus_i(::NLayeredSolidTorus* newCppPtr) :
				ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

		virtual Regina::Subcomplex::NLayeredSolidTorus_ptr cloneMe();
		virtual CORBA::Long getNumberOfTetrahedra();
		virtual Regina::Triangulation::NTetrahedron_ptr getBase();
		virtual CORBA::Long getBaseEdge(CORBA::Long group, CORBA::Long index);
		virtual CORBA::Long getBaseEdgeGroup(CORBA::Long edge);
		virtual CORBA::Long getBaseFace(CORBA::Long index);
		virtual Regina::Triangulation::NTetrahedron_ptr getTopLevel();
		virtual CORBA::Long getMeridinalCuts(CORBA::Long group);
		virtual CORBA::Long getTopEdge(CORBA::Long group, CORBA::Long index);
		virtual CORBA::Long getTopEdgeGroup(CORBA::Long edge);
		virtual CORBA::Long getTopFace(CORBA::Long index);
};

#endif

