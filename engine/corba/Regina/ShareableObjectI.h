
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

#ifndef __SHAREABLEOBJECTI_H
#define __SHAREABLEOBJECTI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "shareableobject.h"
    #include "corbacast.h"
    #include "corbatools.h"
#else
    #include "engine/shareableobject.h"
    #include "corba/corbacast.h"
    #include "corba/corbatools.h"
#endif

#include "ShareableObjectIDL.h"

class ShareableObject_i : public virtual POA_Regina::ShareableObject,
        public PortableServer::RefCountServantBase {
    STANDARD_ENGINE_TYPEDEFS(ShareableObject_i, ShareableObject,
            Regina::ShareableObject)

    private:
        ::ShareableObject* cppPtr;

    public:
        ShareableObject_i(::ShareableObject* newCppPtr);
        virtual ~ShareableObject_i() {
        }

        /**
         * Activates this object in the default CORBA POA.
         */
        void activateObject();

        virtual CORBA::Long getCppPtr();
        virtual CORBA::Boolean sameObject(Regina::ShareableObject_ptr other);
        virtual void destroy();
        virtual char* toString();
        virtual char* toStringLong();
};

#endif

