
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

#ifndef __NBOUNDARYCOMPONENTI_H
#define __NBOUNDARYCOMPONENTI_H

#include "triangulation/nboundarycomponent.h"

#include "NTetrahedronIDL.h"
#include "ShareableObjectI.h"

class NBoundaryComponent_i :
        public virtual POA_Regina::Triangulation::NBoundaryComponent,
        public ShareableObject_i {
    STANDARD_ENGINE_TYPEDEFS(NBoundaryComponent_i, NBoundaryComponent,
            Regina::Triangulation::NBoundaryComponent)

    protected:
        NBoundaryComponent_i(::NBoundaryComponent* newCppPtr) :
                ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

        virtual CORBA::Long getEulerCharacteristic();
        virtual CORBA::Boolean isIdeal();
        virtual CORBA::Boolean isOrientable();
        virtual CORBA::Long getNumberOfFaces();
        virtual CORBA::Long getNumberOfEdges();
        virtual CORBA::Long getNumberOfVertices();
        virtual Regina::Triangulation::NFace_ptr getFace(CORBA::Long index);
        virtual Regina::Triangulation::NEdge_ptr getEdge(CORBA::Long index);
        virtual Regina::Triangulation::NVertex_ptr getVertex(
            CORBA::Long index);
};

#endif

