
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

/***************************
 *
 *   CORBA Normal Surface Set Registry
 *   ---------------------------------
 *
 *
 *    THIS FILE SHOULD BE EDITED EACH TIME A SURFACE SET CLASS IS MIRRORED
 *        IN CORBA!
 *
 *    For each surface set class mirrored in CORBA there should be a line of
 *        the form:
 *
 *        REGISTER_CORBA_SURFACE_SET(cppClass, IDLClass, ptrClass)
 *
 *    where:
 *        cppClass = C++ class representing this surface set type;
 *        IDLClass = corresponding CORBA interface;
 *        ptrClass = corresponding CORBA pointer class.
 *
 *    The appropriate include files should also be placed in the include
 *        section below.
 *
 *    To use the CORBA surface set registry, simply #include this file.
 *        If you have not defined __CORBA_SURFACE_SET_REGISTRY_BODY,
 *        only the include section will be brought in.
 *        If you have defined __CORBA_SURFACE_SET_REGISTRY_BODY, the include
 *        section will be skipped and the REGISTER_CORBA_SURFACE_SET
 *        lines will be brought in instead.  By suitably defining the macro
 *        REGISTER_CORBA_SURFACE_SET before including this file, you can have
 *        these lines do whatever you wish.
 */

#ifndef __CORBA_SURFACE_SET_REGISTRY_BODY
	#include "NNormalSurfaceListIDL.h"
	#include "NSurfaceSubsetIDL.h"
#else
    REGISTER_CORBA_SURFACE_SET(NNormalSurfaceList,
		Regina::Surfaces::NNormalSurfaceList,
		Regina::Surfaces::NNormalSurfaceList_ptr)
    REGISTER_CORBA_SURFACE_SET(NSurfaceSubset,
		Regina::Surfaces::NSurfaceSubset,
		Regina::Surfaces::NSurfaceSubset_ptr)
#endif

/*! \file corbasurfacesetregistry.h
 *  \brief Contains a registry of normal surface set classes
 *  (subclasses of NSurfaceSet) mirrored in CORBA.
 *
 *  Each time a new surface set class is mirrored in CORBA, this CORBA
 *  surface set registry should be updated.  Instructions regarding how to
 *  do this are included in <i>corbasurfacesetregistry.h</i>, which also
 *  contains instructions regarding how to actually use the CORBA surface
 *  set registry.
 *
 *  See NSurfaceSet for further details.
 */
