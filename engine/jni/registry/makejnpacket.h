
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

#ifndef __MAKEJNPACKET_H
#ifndef __DOXYGEN
#define __MAKEJNPACKET_H
#endif

/*! \file makejnpacket.h
 *  \brief Provides a routine to wrap a C++ NPacket with a Java wrapper
 *  of the correct type.
 */

#include <jni.h>

/**
 * The Java packet wrapper class to be used if no other suitable wrapper
 * class can be determined.
 */
#define DEFAULT_PACKET_CLASS \
    "normal/engine/implementation/jni/packet/NJNIPacket"

namespace regina {

class NPacket;

namespace jni {

/**
 * Takes a C++ packet and creates a new Java object of the correct class
 * that wraps this packet.
 * If the given C++ packet is of a particular subclass of NPacket, the
 * new Java wrapper will be of the corresponding subclass of NJNIPacket.
 *
 * The list of all known correspondences between C++ and Java classes is
 * stored in javapacketregistry.h.  If the given C++ packet type is not
 * listed in this registry, the new wrapper object will simply be of type
 * NJNIPacket.
 *
 * If the given C++ pointer is 0, 0 will be returned.
 *
 * \ifaces Not present.
 *
 * @param env the current working JNI environment.
 * @param packet the C++ packet to wrap in a java object.
 * @return a newly created Java wrapper object, or 0 if \a packet was 0.
 */
jobject makeJNPacket(JNIEnv *env, regina::NPacket* packet);

} } // namespace regina::jni

#endif

