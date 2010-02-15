
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*********************
 *
 *  Normal Hypersurface Coordinate System Flavour Registry
 *  ------------------------------------------------------
 *
 *
 *    THIS FILE SHOULD BE EDITED EACH TIME A NEW FLAVOUR OF COORDINATE
 *        SYSTEM IS CREATED!
 *
 *    Note that this registry should only contain flavours of coordinate
 *        system that can actually be used to create and store normal
 *        hypersurfaces.
 *
 *    For each such flavour there should be a line of the form:
 *
 *        REGISTER_HSFLAVOUR(id_name, class, name)
 *
 *    where:
 *        id_name = the constant int member of NNormalHypersurfaceList that
 *            represents this flavour.
 *        class = the C++ subclass of NNormalHypersurfaceVector that uses this
 *            coordinate system.
 *        name = the string (English) name of the new flavour.
 *
 *    The appropriate include files should also be placed in the
 *        appropriate include sections below.
 *
 *    To use the flavour registry, simply #include this file.  If you
 *        have not defined __HSFLAVOUR_REGISTRY_BODY, only the include
 *        sections will be brought in.  If you have defined
 *        __HSFLAVOUR_REGISTRY_BODY, the include sections will be skipped
 *        and the REGISTER_HSFLAVOUR lines will be brought in instead.
 *        By suitably defining the macro REGISTER_HSFLAVOUR before including
 *        this file, you can have these lines do whatever you wish.
 *
 *    If you do #include this file, be sure to #undef both
 *        REGISTER_HSFLAVOUR and __HSFLAVOUR_REGISTRY_BODY when you are
 *        finished, so that if --enable-final is being used then these
 *        macros are left undefined for the following files.
 */

#ifndef __HSFLAVOUR_REGISTRY_BODY
    #include "hypersurface/nhsstandard.h"
#else
    REGISTER_HSFLAVOUR(STANDARD, NNormalHypersurfaceVectorStandard,
        "Standard normal (tet-prism)")
#endif

/*! \file hypersurface/hsflavourregistry.h
 *  \brief Contains a registry of coordinate system flavours known to the
 *  engine that can actually be used to create and store normal hypersurfaces.
 *  in 4-manifold triangulations
 *
 *  Each time a new flavour is created, this flavour registry should be
 *  updated.  Instructions regarding how to do this are included in
 *  <i>hsflavourregistry.h</i>, which also contains instructions regarding
 *  how to actually use the flavour registry.
 *
 *  See NNormalHypersurfaceList for further details.
 */

