
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/***************************
 *
 *   Java Normal Surface Set Registry
 *   --------------------------------
 *
 *
 *    THIS FILE SHOULD BE EDITED EACH TIME A SURFACE SET CLASS IS MIRRORED
 *        IN JAVA!
 *
 *    For each surface set class mirrored in Java there should be a line of
 *        the form:
 *
 *        REGISTER_JAVA_SURFACE_SET(cppClass, javaInterface, javaClass)
 *
 *    where:
 *        cppClass = C++ class representing this surface set type;
 *        javaInterface = string containing the generic Java interface
 *            associated with this surface set type, with Java packages
 *            separated by forward slashes instead of dots.
 *        javaWrapper = string containing the actual JNI Java wrapper
 *            class associated with this surface set type, with Java
 *            packages separated by forward slashes instead of dots.
 *
 *    The appropriate include files should also be placed with full path
 *        and without full path in the appropriate include sections below.
 *
 *    To use the Java surface set registry, simply #include this file.
 *        If you have not defined __JAVA_SURFACE_SET_REGISTRY_BODY,
 *        only the include sections will be brought in.
 *        If you have defined __JAVA_SURFACE_SET_REGISTRY_BODY, the include
 *        sections will be skipped and the REGISTER_JAVA_SURFACE_SET
 *        lines will be brought in instead.  By suitably defining the macro
 *        REGISTER_JAVA_SURFACE_SET before including this file, you can have
 *        these lines do whatever you wish.
 */

#ifndef __JAVA_SURFACE_SET_REGISTRY_BODY
    #include "surfaces/nnormalsurfacelist.h"
    #include "surfaces/nsurfacesubset.h"
#else
    REGISTER_JAVA_SURFACE_SET(NNormalSurfaceList,
        "normal/engine/surfaces/NNormalSurfaceList",
        "normal/engine/implementation/jni/surfaces/NJNINormalSurfaceList")
    REGISTER_JAVA_SURFACE_SET(NSurfaceSubset,
        "normal/engine/surfaces/NSurfaceSubset",
        "normal/engine/implementation/jni/surfaces/NJNISurfaceSubset")
#endif

/*! \file javasurfacesetregistry.h
 *  \brief Contains a registry of normal surface set classes
 *  (subclasses of NSurfaceSet) mirrored in Java.
 *
 *  Each time a new surface set class is mirrored in Java, this Java
 *  surface set registry should be updated.  Instructions regarding how to
 *  do this are included in <i>javasurfacesetregistry.h</i>, which also
 *  contains instructions regarding how to actually use the Java surface
 *  set registry.
 *
 *  See NSurfaceSet for further details.
 */
