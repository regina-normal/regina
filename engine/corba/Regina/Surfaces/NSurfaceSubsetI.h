
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

#ifndef __NSURFACESUBSETI_H
#define __NSURFACESUBSETI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nsurfacesubset.h"
    #include "corbatools.h"
#else
    #include "engine/surfaces/nsurfacesubset.h"
    #include "corba/corbatools.h"
#endif

#include "NSurfaceSubsetIDL.h"
#include "ShareableObjectI.h"

class NSurfaceSubset_i : public virtual Regina::Surfaces::_sk_NSurfaceSubset,
        public ShareableObject_i {
    protected:
        NSurfaceSubset_i(::NSurfaceSubset* newCppPtr) :
                ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER(NSurfaceSubset, NSurfaceSubset_i,
            Regina::Surfaces::NSurfaceSubset_ptr)
        
        virtual CORBA::Long getFlavour();
        virtual CORBA::Boolean allowsAlmostNormal();
        virtual CORBA::Boolean isEmbeddedOnly();
        virtual Regina::Triangulation::NTriangulation_ptr getTriangulation();
        virtual CORBA::Long getNumberOfSurfaces();
        virtual Regina::Surfaces::NNormalSurface_ptr getSurface(
            CORBA::Long index);
        virtual Regina::ShareableObject_ptr getShareableObject();
};

#endif

