
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

#ifndef __NTRISOLIDTORUSI_H
#define __NTRISOLIDTORUSI_H

#include "subcomplex/ntrisolidtorus.h"

#include "NTriSolidTorusIDL.h"
#include "ShareableObjectI.h"

class NTriSolidTorus_i :
        public virtual POA_Regina::Subcomplex::NTriSolidTorus,
        public ShareableObject_i {
    STANDARD_ENGINE_TYPEDEFS(NTriSolidTorus_i, NTriSolidTorus,
        Regina::Subcomplex::NTriSolidTorus)

    protected:
        NTriSolidTorus_i(::NTriSolidTorus* newCppPtr) :
                ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

        virtual Regina::Subcomplex::NTriSolidTorus_ptr cloneMe();
        virtual Regina::Triangulation::NTetrahedron_ptr getTetrahedron(
            CORBA::Long index);
        virtual CORBA::Char getVertexRoles(CORBA::Long index);
};

#endif

