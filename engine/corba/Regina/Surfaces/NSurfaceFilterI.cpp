
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "corbafilterregistry.h"
    #include "corbatools.h"
#else
    #include "corba/registry/corbafilterregistry.h"
    #include "corba/corbatools.h"
#endif

#include "NSurfaceFilterI.h"

Regina::Surfaces::NSurfaceFilter_ptr NSurfaceFilter_i::newWrapper(
        ::NSurfaceFilter* newCppPtr) {
    if (! newCppPtr)
        return Regina::Surfaces::NSurfaceFilter::_nil();
    
    int type = newCppPtr->getFilterID();

    #ifdef __MUTE_WARNINGS
        #pragma warn -ccc
        #pragma warn -rch
    #endif

    // Check for NSurfaceFilter first so we don't recursively call
    // NSurfaceFilter_i::newWrapper().
    if (type == ::NSurfaceFilter::filterID) {
        NSurfaceFilter_i* f = new NSurfaceFilter_i(newCppPtr);
        f->activateObject();
        return f->_this();
    }
    
    // Define something for REGISTER_CORBA_FILTER to do.
    #define REGISTER_CORBA_FILTER(cppClass, CORBAClass) \
    else if (type == ::cppClass::filterID) \
        return CORBAClass::newWrapper((::cppClass*)newCppPtr);
    
    // Import the REGISTER_CORBA_FILTER lines.
    #define __CORBA_FILTER_REGISTRY_BODY
    #ifdef __NO_INCLUDE_PATHS
        #include "corbafilterregistry.h"
    #else
        #include "corba/registry/corbafilterregistry.h"
    #endif

    else {
        NSurfaceFilter_i* f = new NSurfaceFilter_i(newCppPtr);
        f->activateObject();
        return f->_this();
    }
    
    #ifdef __MUTE_WARNINGS
        #pragma warn .ccc
        #pragma warn .rch
    #endif
}
        
CORBA::Boolean NSurfaceFilter_i::accept(Regina::Surfaces::NNormalSurface_ptr
        surface) {
    return MY_ENGINE_OBJECT->accept(
        *GET_ENGINE_OBJECT(NNormalSurface, surface));
}
CORBA::Long NSurfaceFilter_i::getFilterID() {
    return MY_ENGINE_OBJECT->getFilterID();
}
char* NSurfaceFilter_i::getFilterName() {
    return MY_ENGINE_OBJECT->getFilterName().dupe();
}

