
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

#ifndef __NPROGRESSMANAGERI_H
#define __NPROGRESSMANAGERI_H

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nprogressmanager.h"
#else
    #include "engine/progress/nprogressmanager.h"
#endif

#include "NProgressManagerIDL.h"
#include "ShareableObjectI.h"

class NProgressManager_i :
        public virtual POA_Regina::Progress::NProgressManager,
        public ShareableObject_i {
    STANDARD_ENGINE_TYPEDEFS(NProgressManager_i, NProgressManager,
            Regina::Progress::NProgressManager)

    protected:
        NProgressManager_i(::NProgressManager* newCppPtr) :
                ShareableObject_i(newCppPtr) {
        }
    public:
        STANDARD_NEW_WRAPPER

        virtual CORBA::Boolean isStarted();
        virtual CORBA::Boolean isFinished();
        virtual Regina::Progress::NProgress_ptr getProgress();
};

#endif

