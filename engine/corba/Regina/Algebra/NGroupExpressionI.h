
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

#ifndef __NGROUPEXPRESSIONI_H
#define __NGROUPEXPRESSIONI_H

#include "algebra/ngrouppresentation.h"

#include "NGroupPresentationIDL.h"
#include "ShareableObjectI.h"

class ::NGroupExpression;

class NGroupExpression_i :
        public virtual POA_Regina::Algebra::NGroupExpression,
        public ShareableObject_i {
    STANDARD_ENGINE_TYPEDEFS(NGroupExpression_i, NGroupExpression,
            Regina::Algebra::NGroupExpression)

    protected:
        NGroupExpression_i(::NGroupExpression* newCppPtr) :
                ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

        virtual CORBA::Long getNumberOfTerms();
        virtual CORBA::Long getGenerator(CORBA::Long index);
        virtual CORBA::Long getExponent(CORBA::Long index);
        virtual void addTermFirst(CORBA::Long generator, CORBA::Long exponent);
        virtual void addTermLast(CORBA::Long generator, CORBA::Long exponent);
        virtual Regina::Algebra::NGroupExpression_ptr inverse();
        virtual Regina::Algebra::NGroupExpression_ptr power(CORBA::Long exp);
        virtual CORBA::Boolean simplify(CORBA::Boolean cyclic);
        virtual CORBA::Boolean substitute(CORBA::Long generator,
            Regina::Algebra::NGroupExpression_ptr exp, CORBA::Boolean cyclic);
};

#endif

