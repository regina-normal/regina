
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

#ifndef __NSURFACEFILTERCOMBINATIONI_H
#define __NSURFACEFILTERCOMBINATIONI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "sfcombination.h"
#else
    #include "engine/surfaces/sfcombination.h"
#endif

#include "NSurfaceFilterCombinationIDL.h"
#include "NSurfaceFilterI.h"

class NSurfaceFilterCombination_i :
        public virtual POA_Regina::Surfaces::NSurfaceFilterCombination,
        public NSurfaceFilter_i {
    STANDARD_ENGINE_TYPEDEFS(NSurfaceFilterCombination_i,
            NSurfaceFilterCombination,
            Regina::Surfaces::NSurfaceFilterCombination)

    protected:
        NSurfaceFilterCombination_i(::NSurfaceFilterCombination* newCppPtr) :
                NSurfaceFilter_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER
        
        virtual CORBA::Boolean getUsesAnd();
        virtual void setUsesAnd(CORBA::Boolean value);
};

#endif

