
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

#ifndef __NSNAPPEDBALLI_H
#define __NSNAPPEDBALLI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nsnappedball.h"
#else
    #include "engine/subcomplex/nsnappedball.h"
#endif

#include "NSnappedBallIDL.h"
#include "ShareableObjectI.h"

class NSnappedBall_i:
		public virtual POA_Regina::Subcomplex::NSnappedBall,
        public ShareableObject_i {
	STANDARD_ENGINE_TYPEDEFS(NSnappedBall_i, NSnappedBall,
		Regina::Subcomplex::NSnappedBall)

    protected:
        NSnappedBall_i(::NSnappedBall* newCppPtr) :
				ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

		virtual Regina::Triangulation::NTetrahedron_ptr getTetrahedron();
		virtual CORBA::Long getBoundaryFace(CORBA::Long index);
		virtual CORBA::Long getInternalFace(CORBA::Long index);
		virtual CORBA::Long getEquatorEdge();
		virtual CORBA::Long getInternalEdge();
};

#endif

