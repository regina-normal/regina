
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

#ifndef __NLAYEREDLENSSPACEI_H
#define __NLAYEREDLENSSPACEI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nlayeredlensspace.h"
#else
    #include "engine/subcomplex/nlayeredlensspace.h"
#endif

#include "NLayeredLensSpaceIDL.h"
#include "ShareableObjectI.h"

class NLayeredLensSpace_i :
        public virtual POA_Regina::Subcomplex::NLayeredLensSpace,
        public ShareableObject_i {
    STANDARD_ENGINE_TYPEDEFS(NLayeredLensSpace_i, NLayeredLensSpace,
        Regina::Subcomplex::NLayeredLensSpace)

    protected:
        NLayeredLensSpace_i(::NLayeredLensSpace* newCppPtr) :
                ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

        virtual Regina::Subcomplex::NLayeredLensSpace_ptr cloneMe();
        virtual CORBA::Long getP();
        virtual CORBA::Long getQ();
        virtual Regina::Subcomplex::NLayeredSolidTorus_ptr getTorus();
        virtual CORBA::Long getMobiusBoundaryGroup();
        virtual CORBA::Boolean isSnapped();
        virtual CORBA::Boolean isTwisted();
};

#endif

