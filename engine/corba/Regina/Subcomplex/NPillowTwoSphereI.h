
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

#ifndef __NPILLOWTWOSPHEREI_H
#define __NPILLOWTWOSPHEREI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "npillowtwosphere.h"
#else
    #include "engine/subcomplex/npillowtwosphere.h"
#endif

#include "NPillowTwoSphereIDL.h"
#include "ShareableObjectI.h"

class NPillowTwoSphere_i:
        public virtual POA_Regina::Subcomplex::NPillowTwoSphere,
        public ShareableObject_i {
    STANDARD_ENGINE_TYPEDEFS(NPillowTwoSphere_i, NPillowTwoSphere,
        Regina::Subcomplex::NPillowTwoSphere)

    protected:
        NPillowTwoSphere_i(::NPillowTwoSphere* newCppPtr) :
                ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

        virtual Regina::Subcomplex::NPillowTwoSphere_ptr cloneMe();
        virtual Regina::Triangulation::NFace_ptr getFace(CORBA::Long index);
        virtual CORBA::Char getFaceMapping();
        virtual void reduceTriangulation();
        virtual Regina::Triangulation::NTriangulation_ptr
            getReducedTriangulation(
            Regina::Triangulation::NTriangulation_ptr original);
};

#endif

