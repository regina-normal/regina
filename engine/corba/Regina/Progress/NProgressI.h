
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

#ifndef __NPROGRESSI_H
#define __NPROGRESSI_H

#include "progress/nprogress.h"

#include "NProgressIDL.h"
#include "ShareableObjectI.h"

class NProgress_i : public virtual POA_Regina::Progress::NProgress,
        public ShareableObject_i {
    STANDARD_ENGINE_TYPEDEFS(NProgress_i, NProgress,
            Regina::Progress::NProgress)

    protected:
        NProgress_i(::NProgress* newCppPtr) : ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

        virtual CORBA::Boolean hasChanged();
        virtual CORBA::Boolean isFinished();
        virtual CORBA::Boolean isCancellable();
        virtual void cancel();
        virtual CORBA::Boolean isCancelled();
        virtual char* getDescription();
        virtual CORBA::Boolean isPercent();
        virtual CORBA::Double getPercent();
};

#endif

