
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
    #include "npacket.h"
    #include "jnitools.h"
    #include "makejnpacket.h"
    #include "javapacketregistry.h"
    #include "javafilterregistry.h"
#else
    #include "engine/packet/npacket.h"
    #include "jni/jnitools.h"
    #include "jni/registry/makejnpacket.h"
    #include "jni/registry/javapacketregistry.h"
    #include "jni/registry/javafilterregistry.h"
#endif

jobject makeJNPacket(JNIEnv *env, NPacket* packet) {
    if (! packet)
        return 0;

    int type = packet->getPacketType();
    NString classPath;

    #ifdef __MUTE_WARNINGS
        #pragma warn -ccc
        #pragma warn -rch
    #endif

    if (type == NSurfaceFilter::packetType) {
        // Pull out the individual cases for different filter types.
        int id = ((NSurfaceFilter*)packet)->getFilterID();

        if (0)
            return 0;

        // Define something for REGISTER_JAVA_FILTER to do.
        #define REGISTER_JAVA_FILTER(cppClass, javaClass) \
        else if (id == cppClass::filterID) \
            classPath = javaClass;

        // Import the REGISTER_JAVA_FILTER lines.
        #define __JAVA_FILTER_REGISTRY_BODY
        #ifdef __NO_INCLUDE_PATHS
            #include "javafilterregistry.h"
        #else
            #include "jni/registry/javafilterregistry.h"
        #endif

        else
            classPath = DEFAULT_PACKET_CLASS;
    }

    // Define something for REGISTER_JAVA_PACKET to do.
    #define REGISTER_JAVA_PACKET(cppClass, javaClass) \
    else if (type == cppClass::packetType) \
        classPath = javaClass;

    // Import the REGISTER_JAVA_PACKET lines.
    #define __JAVA_PACKET_REGISTRY_BODY
    #ifdef __NO_INCLUDE_PATHS
        #include "javapacketregistry.h"
    #else
        #include "jni/registry/javapacketregistry.h"
    #endif

    else
        classPath = DEFAULT_PACKET_CLASS;

    #ifdef __MUTE_WARNINGS
        #pragma warn .ccc
        #pragma warn .rch
    #endif

    // Make the new Java wrapper.
    char* classChars = classPath.dupe();
    jobject ans = CREATE_WRAPPER_OBJECT(env, packet, classChars);
    delete[] classChars;
    return ans;
}

