
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

#ifndef __NEDGEI_H
#define __NEDGEI_H

#include "triangulation/nedge.h"

#include "NTetrahedronIDL.h"
#include "ShareableObjectI.h"

class NEdge_i : public virtual POA_Regina::Triangulation::NEdge,
        public ShareableObject_i {
    STANDARD_ENGINE_TYPEDEFS(NEdge_i, NEdge, Regina::Triangulation::NEdge)

    protected:
        NEdge_i(::NEdge* newCppPtr) : ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

        virtual Regina::Triangulation::NComponent_ptr getComponent();
        virtual Regina::Triangulation::NBoundaryComponent_ptr
            getBoundaryComponent();
        virtual Regina::Triangulation::NVertex_ptr getVertex(CORBA::Long index);
        virtual CORBA::Boolean isBoundary();
        virtual CORBA::Boolean isValid();
        virtual CORBA::Long getNumberOfEmbeddings();
        virtual void getEmbedding(Regina::Triangulation::NTetrahedron_out tet,
            CORBA::Long& edge, CORBA::Long index);
};

#endif

