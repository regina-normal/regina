
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

#ifndef __NNORMALSURFACEI_H
#define __NNORMALSURFACEI_H

#include "surfaces/nnormalsurface.h"

#include "NNormalSurfaceIDL.h"
#include "ShareableObjectI.h"

class NNormalSurface_i : public virtual POA_Regina::Surfaces::NNormalSurface,
        public ShareableObject_i {
    STANDARD_ENGINE_TYPEDEFS(NNormalSurface_i, NNormalSurface,
            Regina::Surfaces::NNormalSurface)

    protected:
        NNormalSurface_i(::NNormalSurface* newCppPtr) :
                ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER
        
        virtual char* getTriangleCoord(CORBA::Long tetIndex,
            CORBA::Long vertex);
        virtual char* getQuadCoord(CORBA::Long tetIndex, CORBA::Long quadType);
        virtual char* getOctCoord(CORBA::Long tetIndex, CORBA::Long octType);
        virtual char* getEdgeWeight(CORBA::Long edgeIndex);
        virtual char* getFaceArcs(CORBA::Long faceIndex,
            CORBA::Long faceVertex);
        virtual CORBA::Long getNumberOfCoords();
        virtual Regina::Triangulation::NTriangulation_ptr getTriangulation();
        virtual CORBA::Boolean isCompact();
        virtual char* getEulerCharacteristic();
        virtual CORBA::Long isOrientable();
        virtual CORBA::Long isTwoSided();
        virtual CORBA::Long isConnected();
        virtual CORBA::Boolean hasRealBoundary();
        virtual CORBA::Boolean isVertexLinking();
        virtual CORBA::Boolean isSplitting();
        virtual Regina::Triangulation::NTriangulation_ptr cutAlong();
        virtual Regina::Triangulation::NTriangulation_ptr crush();
};

#endif

