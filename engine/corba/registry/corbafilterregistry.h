
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

/***************************
 *
 *   CORBA Normal Surface Filter Registry
 *   ------------------------------------
 *
 *
 *    THIS FILE SHOULD BE EDITED EACH TIME A FILTERING CLASS IS MIRRORED
 *        IN CORBA!
 *
 *    For each filtering class mirrored in CORBA there should be a line of
 *        the form:
 *
 *        REGISTER_CORBA_FILTER(cppClass, CORBAClass)
 *
 *    where:
 *        cppClass = C++ class representing this surface filter type;
 *        CORBAClass = CORBA class used to wrap objects of this C++ class.
 *
 *    The appropriate include files should also be placed in the include
 *        section below.
 *
 *    To use the CORBA filter registry, simply #include this file.  If you have
 *        not defined __CORBA_FILTER_REGISTRY_BODY, only the include section
 *        will be brought in.  If you have defined __CORBA_FILTER_REGISTRY_BODY,
 *        the include section will be skipped and the REGISTER_CORBA_FILTER
 *        lines will be brought in instead.  By suitably defining the macro
 *        REGISTER_CORBA_FILTER before including this file, you can have these
 *        lines do whatever you wish.
 */

#ifndef __CORBA_FILTER_REGISTRY_BODY
    #include "NSurfaceFilterI.h"
    #include "NSurfaceFilterPropertiesI.h"
    #include "NSurfaceFilterCombinationI.h"
#else
    REGISTER_CORBA_FILTER(NSurfaceFilter, NSurfaceFilter_i)
    REGISTER_CORBA_FILTER(NSurfaceFilterProperties,
        NSurfaceFilterProperties_i)
    REGISTER_CORBA_FILTER(NSurfaceFilterCombination,
        NSurfaceFilterCombination_i)
#endif

/*! \file corbafilterregistry.h
 *  \brief Contains a registry of normal surface filtering classes
 *  mirrored in CORBA.
 *
 *  Each time a new filtering class is mirrored in CORBA, this CORBA filter
 *  registry should be updated.  Instructions regarding how to do this
 *  are included in <i>corbafilterregistry.h</i>, which also contains
 *  instructions regarding how to actually use the CORBA filter registry.
 *
 *  See NSurfaceFilter for further details.
 */
