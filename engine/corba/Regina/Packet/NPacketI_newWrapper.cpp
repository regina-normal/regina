
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
    #include "corbapacketregistry.h"
#else
    #include "corba/registry/corbapacketregistry.h"
#endif

#include "NPacketI.h"

Regina::Packet::NPacket_ptr NPacket_i::newWrapper(::NPacket* newCppPtr) {
    if (! newCppPtr)
        return NPacket_i::_nil();

    int type = newCppPtr->getPacketType();

    #ifdef __MUTE_WARNINGS
        #pragma warn -ccc
        #pragma warn -rch
    #endif

    if (0)
        return 0;

    // Define something for REGISTER_CORBA_PACKET to do.
    #define REGISTER_CORBA_PACKET(cppClass, CORBAClass) \
    else if (type == cppClass::packetType) \
        return CORBAClass::newWrapper((cppClass*)newCppPtr);

    // Import the REGISTER_CORBA_PACKET lines.
    #define __CORBA_PACKET_REGISTRY_BODY
    #ifdef __NO_INCLUDE_PATHS
        #include "corbapacketregistry.h"
    #else
        #include "corba/registry/corbapacketregistry.h"
    #endif

    else
        return (new NPacket_i(newCppPtr))->_this();

    #ifdef __MUTE_WARNINGS
        #pragma warn .ccc
        #pragma warn .rch
    #endif
}

