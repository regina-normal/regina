
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

#ifndef __NFACEI_H
#define __NFACEI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
	#include "nface.h"
	#include "corbatools.h"
#else
	#include "engine/triangulation/nface.h"
	#include "corba/corbatools.h"
#endif

#include "NTetrahedronIDL.h"
#include "ShareableObjectI.h"

class NFace_i : public virtual Regina::Triangulation::_sk_NFace,
		public ShareableObject_i {
	protected:
		NFace_i(::NFace* newCppPtr) : ShareableObject_i(newCppPtr) {
		}
	public:
		STANDARD_NEW_WRAPPER(NFace, NFace_i, Regina::Triangulation::NFace_ptr)

		virtual Regina::Triangulation::NComponent_ptr getComponent();
		virtual Regina::Triangulation::NBoundaryComponent_ptr
			getBoundaryComponent();
		virtual CORBA::Boolean isBoundary();
		virtual CORBA::Long getNumberOfEmbeddings();
		virtual void getEmbedding(Regina::Triangulation::NTetrahedron_ptr& tet,
			CORBA::Long& face, CORBA::Long index);
};

#endif

