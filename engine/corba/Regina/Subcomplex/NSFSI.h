
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

#ifndef __NSFSI_H
#define __NSFSI_H

#include "subcomplex/nsfs.h"

#include "NSFSIDL.h"
#include "ShareableObjectI.h"

class NSFS_i :
        public virtual POA_Regina::Subcomplex::NSFS,
        public ShareableObject_i {
    STANDARD_ENGINE_TYPEDEFS(NSFS_i, NSFS,
        Regina::Subcomplex::NSFS)

    protected:
        NSFS_i(::NSFS* newCppPtr) :
                ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

        virtual CORBA::Long getOrbitGenus();
        virtual CORBA::Boolean isOrbitOrientable();
        virtual CORBA::Long getOrbitPunctures();
        virtual CORBA::Long getFibreCount();
        virtual CORBA::Long getAlpha(CORBA::Long which);
        virtual CORBA::Long getBeta(CORBA::Long which);
        virtual void insertFibre(CORBA::Long alpha, CORBA::Long beta);
        virtual void reduce();
        virtual Regina::Subcomplex::NLensSpace_ptr isLensSpace();
};

#endif

