
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "corbasurfacesetregistry.h"
#else
    #include "corba/registry/corbasurfacesetregistry.h"
#endif

#include "EngineI.h"
#include "NNormalSurfaceListI.h"
#include "NSurfaceFilterI.h"
#include "NSurfaceFilterCombinationI.h"
#include "NSurfaceFilterPropertiesI.h"
#include "NSurfaceSubsetI.h"

Regina::Surfaces::NNormalSurfaceList_ptr Engine_i::newNNormalSurfaceList(
        Regina::Triangulation::NTriangulation_ptr owner,
        CORBA::Long flavour, CORBA::Boolean isEmbeddedOnly) {
    return NNormalSurfaceList_i::newWrapper(new ::NNormalSurfaceList(
        GET_ENGINE_OBJECT(NTriangulation, owner), flavour, isEmbeddedOnly));
}
Regina::Surfaces::NSurfaceFilter_ptr Engine_i::newNSurfaceFilter_() {
    return NSurfaceFilter_i::newWrapper(new ::NSurfaceFilter());
}
Regina::Surfaces::NSurfaceFilter_ptr
        Engine_i::newNSurfaceFilter_NSurfaceFilter(
        Regina::Surfaces::NSurfaceFilter_ptr cloneMe) {
    return NSurfaceFilter_i::newWrapper(new ::NSurfaceFilter(
        *GET_ENGINE_OBJECT(NSurfaceFilter, cloneMe)));
}
Regina::Surfaces::NSurfaceFilterCombination_ptr
        Engine_i::newNSurfaceFilterCombination_() {
    return NSurfaceFilterCombination_i::newWrapper(
        new ::NSurfaceFilterCombination());
}
Regina::Surfaces::NSurfaceFilterCombination_ptr
        Engine_i::newNSurfaceFilterCombination_NSurfaceFilterCombination(
        Regina::Surfaces::NSurfaceFilterCombination_ptr cloneMe) {
    return NSurfaceFilterCombination_i::newWrapper(
        new ::NSurfaceFilterCombination(
        *GET_ENGINE_OBJECT(NSurfaceFilterCombination, cloneMe)));
}
Regina::Surfaces::NSurfaceFilterProperties_ptr
        Engine_i::newNSurfaceFilterProperties_() {
    return NSurfaceFilterProperties_i::newWrapper(
        new ::NSurfaceFilterProperties());
}
Regina::Surfaces::NSurfaceFilterProperties_ptr
        Engine_i::newNSurfaceFilterProperties_NSurfaceFilterProperties(
        Regina::Surfaces::NSurfaceFilterProperties_ptr cloneMe) {
    return NSurfaceFilterProperties_i::newWrapper(
        new ::NSurfaceFilterProperties(
        *GET_ENGINE_OBJECT(NSurfaceFilterProperties, cloneMe)));
}
Regina::Surfaces::NSurfaceSubset_ptr Engine_i::newNSurfaceSubset(
        Regina::Surfaces::NSurfaceSet_ptr set,
        Regina::Surfaces::NSurfaceFilter_ptr filter) {
    ::NSurfaceSet* realSet = 0;

    #ifdef __MUTE_WARNINGS
        #pragma warn -ccc
        #pragma warn -rch
    #endif

    // Define something for REGISTER_CORBA_SURFACE_SET to do.
    #define REGISTER_CORBA_SURFACE_SET(cppClass, IDLClass, ptrClass) \
        { \
            ptrClass p = IDLClass::_narrow(set); \
            if (! CORBA::is_nil(p)) \
                realSet = GET_ENGINE_OBJECT(cppClass, p); \
        }

    // Import the REGISTER_CORBA_SURFACE_SET lines.
    #define __CORBA_SURFACE_SET_REGISTRY_BODY
    #ifdef __NO_INCLUDE_PATHS
        #include "corbasurfacesetregistry.h"
    #else
        #include "corba/registry/corbasurfacesetregistry.h"
    #endif

    #ifdef __MUTE_WARNINGS
        #pragma warn .ccc
        #pragma warn .rch
    #endif

    if (realSet == 0)
        return NSurfaceSubset_i::_nil();
    else
        return NSurfaceSubset_i::newWrapper(new ::NSurfaceSubset(
            *realSet, *GET_ENGINE_OBJECT(NSurfaceFilter, filter)));
}

