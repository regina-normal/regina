
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
#include "njnienumeration.h"

using namespace regina;
using namespace regina::jni;

typedef NJNIEnumeration<std::set<std::string> > NPacketTagEnumerator;

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacketTagEnumerator_hasMoreElements
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NPacketTagEnumerator, me)->hasMoreElements();
}

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacketTagEnumerator_nextString
        (JNIEnv *env, jobject me) {
    return jstringFromCString(env, GET_ENGINE_OBJECT(env, NPacketTagEnumerator,
        me)->nextElement());
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacketTagEnumerator_newPacketTagEnumerator
        (JNIEnv *env, jobject me, jobject packet) {
    ASSIGN_ENGINE_OBJECT(env, new NPacketTagEnumerator(
        GET_ENGINE_OBJECT(env, NPacket, packet)->getTags()), me);
}

