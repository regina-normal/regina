
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

#include "packet/npacket.h"
#include "jnitools.h"
#include "registry/makejnpacket.h"
#include "registry/javapacketregistry.h"
#include "registry/javafilterregistry.h"

namespace regina {
namespace jni {

jobject makeJNPacket(JNIEnv *env, regina::NPacket* packet) {
    if (! packet)
        return 0;

    int type = packet->getPacketType();
    std::string classPath;

    if (type == regina::NSurfaceFilter::packetType) {
        // Pull out the individual cases for different filter types.
        int id = ((regina::NSurfaceFilter*)packet)->getFilterID();

        if (0)
            return 0;

        // Define something for REGISTER_JAVA_FILTER to do.
        #define REGISTER_JAVA_FILTER(cppClass, javaClass) \
        else if (id == regina::cppClass::filterID) \
            classPath = javaClass;

        // Import the REGISTER_JAVA_FILTER lines.
        #define __JAVA_FILTER_REGISTRY_BODY
        #include "registry/javafilterregistry.h"

        else
            classPath = DEFAULT_PACKET_CLASS;
    }

    // Define something for REGISTER_JAVA_PACKET to do.
    #define REGISTER_JAVA_PACKET(cppClass, javaClass) \
    else if (type == regina::cppClass::packetType) \
        classPath = javaClass;

    // Import the REGISTER_JAVA_PACKET lines.
    #define __JAVA_PACKET_REGISTRY_BODY
    #include "registry/javapacketregistry.h"

    else
        classPath = DEFAULT_PACKET_CLASS;

    // Make the new Java wrapper.
    return CREATE_WRAPPER_OBJECT(env, packet, classPath.c_str());
}

} } // namespace regina::jni

