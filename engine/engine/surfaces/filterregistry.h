
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

/*********************
 *
 *  Normal Surface Filter Registry
 *  ------------------------------
 *
 *
 *    THIS FILE SHOULD BE EDITED EACH TIME A NEW FILTERING CLASS
 *        IS CREATED!
 *
 *    Note that each normal surface filtering class should be a subclass
 *        of NSurfaceFilter.
 *
 *    For each such filtering class there should be a line of the form:
 *
 *        REGISTER_FILTER(id, class, name)
 *
 *    where:
 *        id = the integer ID of the new filtering class.
 *        class = the C++ subclass of NSurfaceFilter being registered.
 *        name = the string (English) name of the corresponding
 *            filtering method.
 *
 *    The appropriate include files should also be placed with full path
 *        and without full path in the appropriate include sections below.
 *
 *    To use the filter registry, simply #include this file.  If you
 *        have not defined __FILTER_REGISTRY_BODY, only the include
 *        sections will be brought in.  If you have defined
 *        __FILTER_REGISTRY_BODY, the include sections will be skipped
 *        and the REGISTER_FILTER lines will be brought in instead.
 *        By suitably defining the macro REGISTER_FILTER before including
 *        this file, you can have these lines do whatever you wish.
 *
 *    Filter IDs:
 *    ----------
 *    When selecting an integer ID for your new filtering class, the
 *        following guidelines should be adhered to:
 *
 *    0-999:      Reserved for use with the official program distribution.
 *    1000-9999:  Reserved for future use.
 *    10000-:     Unreserved.
 */

#ifndef __FILTER_REGISTRY_BODY
    #ifdef __NO_INCLUDE_PATHS
        #include "nsurfacefilter.h"
        #include "sfproperties.h"
        #include "sfcombination.h"
    #else
        #include "engine/surfaces/nsurfacefilter.h"
        #include "engine/surfaces/sfproperties.h"
        #include "engine/surfaces/sfcombination.h"
    #endif
#else
    REGISTER_FILTER(0, NSurfaceFilter, "Default filter")
    REGISTER_FILTER(1, NSurfaceFilterProperties,
        "Filter by basic properties")
    REGISTER_FILTER(2, NSurfaceFilterCombination,
        "Combination filter")
#endif

/*! \file filterregistry.h
 *  \brief Contains a registry of normal surface filtering classes known to
 *  the engine.  Each such class should be a subclass of NSurfaceFilter.
 *
 *  Each time a new filtering class is created, this filter registry should be
 *  updated.  Instructions regarding how to do this are included in
 *  <i>filterregistry.h</i>, which also contains instructions regarding
 *  how to actually use the filter registry.
 *
 *  See NSurfaceFilter for further details.
 */

