
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
    #include "NJNIPacket.h"
#else
    #include "engine/packet/npacket.h"
    #include "jni/jnitools.h"
    #include "jni/registry/makejnpacket.h"
    #include "jni/engine/packet/NJNIPacket.h"
#endif

#include <strstream.h>

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_clonePacket
        (JNIEnv *env, jobject me, jboolean cloneDescendants, jboolean end) {
    return makeJNPacket(env, GET_ENGINE_OBJECT(env, NPacket, me)->
        clone(cloneDescendants, end));
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_dependsOnParent
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->dependsOnParent();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_findPacketLabel
        (JNIEnv *env, jobject me, jstring newLabel) {
    return makeJNPacket(env, GET_ENGINE_OBJECT(env, NPacket, me)->
        findPacketLabel(jstringToNString(env, newLabel)));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_firstTreePacket
        (JNIEnv *env, jobject me, jstring newLabel) {
    return makeJNPacket(env, GET_ENGINE_OBJECT(env, NPacket, me)
        ->firstTreePacket(jstringToNString(env, newLabel)));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getFirstTreeChild
        (JNIEnv *env, jobject me) {
    return makeJNPacket(env, 
        GET_ENGINE_OBJECT(env, NPacket, me)->getFirstTreeChild());
}

JNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getFullName
        (JNIEnv *env, jobject me) {
    return jstringFromNString(env,
        GET_ENGINE_OBJECT(env, NPacket, me)->getFullName());
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getLastTreeChild
        (JNIEnv *env, jobject me) {
    return makeJNPacket(env, 
        GET_ENGINE_OBJECT(env, NPacket, me)->getLastTreeChild());
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getNextTreeSibling
        (JNIEnv *env, jobject me) {
    return makeJNPacket(env, 
        GET_ENGINE_OBJECT(env, NPacket, me)->getNextTreeSibling());
}

JNIEXPORT jlong JNICALL
		Java_normal_engine_implementation_jni_packet_NJNIPacket_getNumberOfChildren
		(JNIEnv *env, jobject me) {
	return GET_ENGINE_OBJECT(env, NPacket, me)->getNumberOfChildren();
}

JNIEXPORT jlong JNICALL
		Java_normal_engine_implementation_jni_packet_NJNIPacket_getNumberOfDescendants
		(JNIEnv *env, jobject me) {
	return GET_ENGINE_OBJECT(env, NPacket, me)->getNumberOfDescendants();
}

JNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getPacketLabel
        (JNIEnv *env, jobject me) {
    return jstringFromNString(env,
        GET_ENGINE_OBJECT(env, NPacket, me)->getPacketLabel());
}

JNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getPacketName
        (JNIEnv *env, jobject me) {
    return jstringFromNString(env,
        GET_ENGINE_OBJECT(env, NPacket, me)->getPacketName());
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getPacketType
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->getPacketType();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getPrevTreeSibling
        (JNIEnv *env, jobject me) {
    return makeJNPacket(env, 
        GET_ENGINE_OBJECT(env, NPacket, me)->getPrevTreeSibling());
}

JNIEXPORT jlong JNICALL
		Java_normal_engine_implementation_jni_packet_NJNIPacket_getTotalTreeSize
		(JNIEnv *env, jobject me) {
	return GET_ENGINE_OBJECT(env, NPacket, me)->getTotalTreeSize();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getTreeMatriarch
        (JNIEnv *env, jobject me) {
    return makeJNPacket(env,
        GET_ENGINE_OBJECT(env, NPacket, me)->getTreeMatriarch());
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getTreeParent
        (JNIEnv *env, jobject me) {
    return makeJNPacket(env,
        GET_ENGINE_OBJECT(env, NPacket, me)->getTreeParent());
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_insertChildAfter
        (JNIEnv *env, jobject me, jobject newChild, jobject prevChild) {
    GET_ENGINE_OBJECT(env, NPacket, me)->insertChildAfter(
        GET_ENGINE_OBJECT(env, NPacket, newChild),
        GET_ENGINE_OBJECT(env, NPacket, prevChild));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_insertChildFirst
        (JNIEnv *env, jobject me, jobject you) {
    GET_ENGINE_OBJECT(env, NPacket, me)->
        insertChildFirst(GET_ENGINE_OBJECT(env, NPacket,you));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_insertChildLast
        (JNIEnv *env, jobject me, jobject you) {
    GET_ENGINE_OBJECT(env, NPacket, me)->
        insertChildLast(GET_ENGINE_OBJECT(env, NPacket, you));
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_isGrandparentOf
        (JNIEnv *env, jobject me, jobject you) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->
            isGrandparentOf(GET_ENGINE_OBJECT(env, NPacket, you));
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_isPacketEditable
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->isPacketEditable();
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_levelsDownTo
        (JNIEnv *env, jobject me, jobject you) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->
        levelsDownTo(GET_ENGINE_OBJECT(env, NPacket, you));
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_levelsUpTo
        (JNIEnv *env, jobject me, jobject you) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->
        levelsUpTo(GET_ENGINE_OBJECT(env, NPacket, you));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_makeOrphan
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NPacket, me)->makeOrphan();
}

JNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_makeUniqueLabel
        (JNIEnv *env, jobject me, jstring base) {
    return jstringFromNString(env, GET_ENGINE_OBJECT(env, NPacket, me)->
        makeUniqueLabel(jstringToNString(env, base)));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_nextTreePacket__
        (JNIEnv *env, jobject me) {
    return makeJNPacket(env,
        GET_ENGINE_OBJECT(env, NPacket, me)->nextTreePacket());
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_nextTreePacket__Ljava_lang_String_2
        (JNIEnv *env, jobject me, jstring newLabel) {
    return makeJNPacket(env, GET_ENGINE_OBJECT(env, NPacket, me)->
        nextTreePacket(jstringToNString(env, newLabel)));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_setPacketLabel
        (JNIEnv *env, jobject me, jstring newLabel) {
    GET_ENGINE_OBJECT(env, NPacket, me)->setPacketLabel(
        jstringToNString(env, newLabel));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_swapWithNextSibling
        (JNIEnv* env, jobject me) {
    GET_ENGINE_OBJECT(env, NPacket, me)->swapWithNextSibling();
}

