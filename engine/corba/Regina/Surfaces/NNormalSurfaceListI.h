
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

#ifndef __NNORMALSURFACELISTI_H
#define __NNORMALSURFACELISTI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nnormalsurfacelist.h"
#else
    #include "engine/surfaces/nnormalsurfacelist.h"
#endif

#include "NNormalSurfaceListIDL.h"
#include "NPacketI.h"

class NNormalSurfaceList_i :
        public virtual POA_Regina::Surfaces::NNormalSurfaceList,
        public NPacket_i {
    STANDARD_ENGINE_TYPEDEFS(NNormalSurfaceList_i, NNormalSurfaceList,
            Regina::Surfaces::NNormalSurfaceList)

    protected:
        NNormalSurfaceList_i(::NNormalSurfaceList* newCppPtr) :
                NPacket_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER
        
        virtual CORBA::Long getFlavour();
        virtual CORBA::Boolean allowsAlmostNormal();
        virtual CORBA::Boolean isEmbeddedOnly();
        virtual Regina::Triangulation::NTriangulation_ptr getTriangulation();
        virtual CORBA::Long getNumberOfSurfaces();
        virtual Regina::Surfaces::NNormalSurface_ptr getSurface(
            CORBA::Long index);
        virtual Regina::ShareableObject_ptr getShareableObject();

        virtual Regina::Maths::NMatrixInt_ptr recreateMatchingEquations();
};

#endif

