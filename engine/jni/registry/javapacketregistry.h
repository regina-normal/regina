
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *   Java Packet Registry
 *   --------------------
 *
 *
 *    THIS FILE SHOULD BE EDITED EACH TIME A PACKET TYPE IS MIRRORED
 *        IN JAVA!
 *
 *    For each packet type mirrored in Java there should be a line of
 *        the form:
 *
 *        REGISTER_JAVA_PACKET(cppClass, javaClass)
 *
 *    where:
 *        cppClass = C++ class representing this packet type;
 *        javaClass = string containing the Java class associated with
 *            this packet type, with Java packages separated by forward
 *            slashes instead of dots.
 *
 *    The appropriate include files should also be placed with full path
 *        and without full path in the appropriate include sections below.
 *
 *    To use the java packet registry, simply #include this file.  If you have
 *        not defined __JAVA_PACKET_REGISTRY_BODY, only the include sections
 *        will be brought in.  If you have defined __JAVA_PACKET_REGISTRY_BODY,
 *        the include sections will be skipped and the REGISTER_JAVA_PACKET
 *        lines will be brought in instead.  By suitably defining the macro
 *        REGISTER_JAVA_PACKET before including this file, you can have these
 *        lines do whatever you wish.
 */

#ifndef __JAVA_PACKET_REGISTRY_BODY
    #include "packet/ncontainer.h"
    #include "packet/ntext.h"
    #include "packet/nscript.h"
    #include "triangulation/ntriangulation.h"
    #include "surfaces/nnormalsurfacelist.h"
    #include "surfaces/nsurfacefilter.h"
    #include "angle/nanglestructurelist.h"
#else
    REGISTER_JAVA_PACKET(NContainer,
        "normal/engine/implementation/jni/packet/NJNIContainer")
    REGISTER_JAVA_PACKET(NText,
        "normal/engine/implementation/jni/packet/NJNIText")
    REGISTER_JAVA_PACKET(NTriangulation,
        "normal/engine/implementation/jni/triangulation/NJNITriangulation")
    REGISTER_JAVA_PACKET(NNormalSurfaceList,
        "normal/engine/implementation/jni/surfaces/NJNINormalSurfaceList")
    REGISTER_JAVA_PACKET(NScript,
        "normal/engine/implementation/jni/packet/NJNIScript")
    REGISTER_JAVA_PACKET(NSurfaceFilter,
        "normal/engine/implementation/jni/surfaces/NJNISurfaceFilter")
    REGISTER_JAVA_PACKET(NAngleStructureList,
        "normal/engine/implementation/jni/angle/NJNIAngleStructureList")
#endif

/*! \file javapacketregistry.h
 *  \brief Contains a registry of packet types mirrored in Java.
 *
 *  Each time a new packet type is mirrored in Java, this Java packet
 *  registry should be updated.  Instructions regarding how to do this
 *  are included in <i>javapacketregistry.h</i>, which also contains
 *  instructions regarding how to actually use the Java packet registry.
 *
 *  See NPacket for further details.
 */
