
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

#ifndef __NAUGTRISOLIDTORUSI_H
#define __NAUGTRISOLIDTORUSI_H

#include "subcomplex/naugtrisolidtorus.h"

#include "NAugTriSolidTorusIDL.h"
#include "ShareableObjectI.h"

class NAugTriSolidTorus_i :
        public virtual POA_Regina::Subcomplex::NAugTriSolidTorus,
        public ShareableObject_i {
    STANDARD_ENGINE_TYPEDEFS(NAugTriSolidTorus_i, NAugTriSolidTorus,
        Regina::Subcomplex::NAugTriSolidTorus)

    protected:
        NAugTriSolidTorus_i(::NAugTriSolidTorus* newCppPtr) :
                ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

        virtual Regina::Subcomplex::NAugTriSolidTorus_ptr cloneMe();
        virtual Regina::Subcomplex::NTriSolidTorus_ptr getCore();
        virtual Regina::Subcomplex::NLayeredSolidTorus_ptr getAugTorus(
                CORBA::Long annulus);
        virtual CORBA::Char getEdgeGroupRoles(CORBA::Long annulus);
        virtual CORBA::Long getChainLength();
        virtual CORBA::Long getTorusAnnulus();
        virtual CORBA::Boolean hasLayeredChain();
        virtual Regina::Subcomplex::NSFS_ptr getSeifertStructure();
};

#endif

