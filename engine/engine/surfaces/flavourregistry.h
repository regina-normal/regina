
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
 *  Coordinate System Flavour Registry
 *  ----------------------------------
 *
 *
 *    THIS FILE SHOULD BE EDITED EACH TIME A NEW FLAVOUR OF COORDINATE
 *        SYSTEM IS CREATED!
 *
 *    Note that this registry should only contain flavours of coordinate
 *        system that can actually be used to create and store normal
 *        surfaces.
 *
 *    For each such flavour there should be a line of the form:
 *
 *        REGISTER_FLAVOUR(id_name, class, name, almost_normal, pre_test)
 *
 *    where:
 *        id_name = the constant int member of NNormalSurfaceList that
 *            represents this flavour.
 *        class = the C++ subclass of NNormalSurfaceVector that uses this
 *            coordinate system.
 *        name = the string (English) name of the new flavour.
 *        almost_normal = true if this coordinate system allows almost
 *            normal surfaces or false otherwise.
 *        pre_test = test to be performed before enumerating surfaces;
 *            if this test returns false, it will be assumed there are
 *            no surfaces at all and thus no enumeration will be performed.
 *            It may be assumed that the variable [triang] represents the
 *            triangulation for which we are enumerating surfaces.
 *
 *    The appropriate include files should also be placed with full path
 *        and without full path in the appropriate include sections below.
 *
 *    To use the flavour registry, simply #include this file.  If you
 *        have not defined __FLAVOUR_REGISTRY_BODY, only the include
 *        sections will be brought in.  If you have defined
 *        __FLAVOUR_REGISTRY_BODY, the include sections will be skipped
 *        and the REGISTER_FLAVOUR lines will be brought in instead.
 *        By suitably defining the macro REGISTER_FLAVOUR before including
 *        this file, you can have these lines do whatever you wish.
 *
 *    If you do #include this file, be sure to #undef both
 *        REGISTER_FLAVOUR and __FLAVOUR_REGISTRY_BODY when you are
 *        finished, so that if --enable-final is being used then these
 *        macros are left undefined for the following files.
 */

#ifndef __FLAVOUR_REGISTRY_BODY
    #include "surfaces/nsstandard.h"
    #include "surfaces/nsanstandard.h"
    #include "surfaces/nsquad.h"
#else
    REGISTER_FLAVOUR(STANDARD, NNormalSurfaceVectorStandard,
        "Standard normal (tri-quad)", false, true)
    REGISTER_FLAVOUR(AN_STANDARD, NNormalSurfaceVectorANStandard,
        "Standard almost normal (tri-quad-oct)", true, true)
    REGISTER_FLAVOUR(QUAD, NNormalSurfaceVectorQuad,
        "Quad normal", false, true)
#endif

/*! \file flavourregistry.h
 *  \brief Contains a registry of coordinate system flavours known to the
 *  engine that can actually be used to create and store normal surfaces.
 *
 *  Each time a new flavour is created, this flavour registry should be
 *  updated.  Instructions regarding how to do this are included in
 *  <i>flavourregistry.h</i>, which also contains instructions regarding
 *  how to actually use the flavour registry.
 *
 *  See NNormalSurfaceList for further details.
 */

