
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

#ifndef __NABELIANGROUPI_H
#define __NABELIANGROUPI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nabeliangroup.h"
    #include "corbatools.h"
#else
    #include "engine/algebra/nabeliangroup.h"
    #include "corba/corbatools.h"
#endif

#include "NAbelianGroupIDL.h"
#include "ShareableObjectI.h"

class NAbelianGroup_i : public virtual Regina::Algebra::_sk_NAbelianGroup,
        public ShareableObject_i {
    protected:
        NAbelianGroup_i(::NAbelianGroup* newCppPtr) :
                ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER(NAbelianGroup, NAbelianGroup_i,
            Regina::Algebra::NAbelianGroup_ptr)

        virtual void addRank(CORBA::Long extraRank);
        virtual void addTorsionElement_bigInt(const char* degree,
            CORBA::Long mult);
        virtual void addTorsionElement_long(CORBA::Long degree,
            CORBA::Long mult);
        virtual void addGroup_NMatrixInt(Regina::Maths::NMatrixInt_ptr pres);
        virtual void addGroup_NAbelianGroup(Regina::Algebra::NAbelianGroup_ptr
            group);

        virtual CORBA::Long getRank();
        virtual CORBA::Long getTorsionRank_bigInt(const char* degree);
        virtual CORBA::Long getTorsionRank_long(CORBA::Long degree);
        virtual CORBA::Long getNumberOfInvariantFactors();
        virtual char* getInvariantFactor(CORBA::Long index);
};

#endif

