
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#ifndef __NLAYEREDLOOPI_H
#define __NLAYEREDLOOPI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nlayeredloop.h"
#else
    #include "engine/subcomplex/nlayeredloop.h"
#endif

#include "NLayeredLoopIDL.h"
#include "ShareableObjectI.h"

class NLayeredLoop_i :
        public virtual POA_Regina::Subcomplex::NLayeredLoop,
        public ShareableObject_i {
    STANDARD_ENGINE_TYPEDEFS(NLayeredLoop_i, NLayeredLoop,
        Regina::Subcomplex::NLayeredLoop)

    protected:
        NLayeredLoop_i(::NLayeredLoop* newCppPtr) :
                ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

        virtual Regina::Subcomplex::NLayeredLoop_ptr cloneMe();
        virtual CORBA::Long getIndex();
        virtual CORBA::Boolean isTwisted();
        virtual Regina::Triangulation::NEdge_ptr getHinge(CORBA::Long which);
};

#endif

