
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

#include "packet/npacket.h"
#include "jnitools.h"
#include "registry/makejnpacket.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_addTag
        (JNIEnv *env, jobject me, jstring tag) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->addTag(
        jstringToCString(env, tag));
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_clonePacket
        (JNIEnv *env, jobject me, jboolean cloneDescendants, jboolean end) {
    return makeJNPacket(env, GET_ENGINE_OBJECT(env, NPacket, me)->
        clone(cloneDescendants, end));
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_dependsOnParent
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->dependsOnParent();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_findPacketLabel
        (JNIEnv *env, jobject me, jstring newLabel) {
    return makeJNPacket(env, GET_ENGINE_OBJECT(env, NPacket, me)->
        findPacketLabel(jstringToCString(env, newLabel)));
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_firstTreePacket
        (JNIEnv *env, jobject me, jstring newLabel) {
    return makeJNPacket(env, GET_ENGINE_OBJECT(env, NPacket, me)
        ->firstTreePacket(jstringToCString(env, newLabel)));
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getFirstTreeChild
        (JNIEnv *env, jobject me) {
    return makeJNPacket(env, 
        GET_ENGINE_OBJECT(env, NPacket, me)->getFirstTreeChild());
}

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getFullName
        (JNIEnv *env, jobject me) {
    return jstringFromCString(env,
        GET_ENGINE_OBJECT(env, NPacket, me)->getFullName());
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getLastTreeChild
        (JNIEnv *env, jobject me) {
    return makeJNPacket(env, 
        GET_ENGINE_OBJECT(env, NPacket, me)->getLastTreeChild());
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getNextTreeSibling
        (JNIEnv *env, jobject me) {
    return makeJNPacket(env, 
        GET_ENGINE_OBJECT(env, NPacket, me)->getNextTreeSibling());
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getNumberOfChildren
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->getNumberOfChildren();
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getNumberOfDescendants
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->getNumberOfDescendants();
}

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getPacketLabel
        (JNIEnv *env, jobject me) {
    return jstringFromCString(env,
        GET_ENGINE_OBJECT(env, NPacket, me)->getPacketLabel());
}

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getPacketTypeName
        (JNIEnv *env, jobject me) {
    return jstringFromCString(env,
        GET_ENGINE_OBJECT(env, NPacket, me)->getPacketTypeName());
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getPacketType
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->getPacketType();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getPrevTreeSibling
        (JNIEnv *env, jobject me) {
    return makeJNPacket(env, 
        GET_ENGINE_OBJECT(env, NPacket, me)->getPrevTreeSibling());
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getTotalTreeSize
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->getTotalTreeSize();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getTreeMatriarch
        (JNIEnv *env, jobject me) {
    return makeJNPacket(env,
        GET_ENGINE_OBJECT(env, NPacket, me)->getTreeMatriarch());
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_getTreeParent
        (JNIEnv *env, jobject me) {
    return makeJNPacket(env,
        GET_ENGINE_OBJECT(env, NPacket, me)->getTreeParent());
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_hasTag
        (JNIEnv *env, jobject me, jstring tag) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->hasTag(
        jstringToCString(env, tag));
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_hasTags
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->hasTags();
}


REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_insertChildAfter
        (JNIEnv *env, jobject me, jobject newChild, jobject prevChild) {
    GET_ENGINE_OBJECT(env, NPacket, me)->insertChildAfter(
        GET_ENGINE_OBJECT(env, NPacket, newChild),
        GET_ENGINE_OBJECT(env, NPacket, prevChild));
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_insertChildFirst
        (JNIEnv *env, jobject me, jobject you) {
    GET_ENGINE_OBJECT(env, NPacket, me)->
        insertChildFirst(GET_ENGINE_OBJECT(env, NPacket,you));
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_insertChildLast
        (JNIEnv *env, jobject me, jobject you) {
    GET_ENGINE_OBJECT(env, NPacket, me)->
        insertChildLast(GET_ENGINE_OBJECT(env, NPacket, you));
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_isGrandparentOf
        (JNIEnv *env, jobject me, jobject you) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->
            isGrandparentOf(GET_ENGINE_OBJECT(env, NPacket, you));
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_isPacketEditable
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->isPacketEditable();
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_levelsDownTo
        (JNIEnv *env, jobject me, jobject you) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->
        levelsDownTo(GET_ENGINE_OBJECT(env, NPacket, you));
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_levelsUpTo
        (JNIEnv *env, jobject me, jobject you) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->
        levelsUpTo(GET_ENGINE_OBJECT(env, NPacket, you));
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_makeOrphan
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NPacket, me)->makeOrphan();
}

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_makeUniqueLabel
        (JNIEnv *env, jobject me, jstring base) {
    return jstringFromCString(env, GET_ENGINE_OBJECT(env, NPacket, me)->
        makeUniqueLabel(jstringToCString(env, base)));
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_makeUniqueLabels
        (JNIEnv *env, jobject me, jobject ref) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->makeUniqueLabels(
        GET_ENGINE_OBJECT(env, NPacket, ref));
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_nextTreePacket__
        (JNIEnv *env, jobject me) {
    return makeJNPacket(env,
        GET_ENGINE_OBJECT(env, NPacket, me)->nextTreePacket());
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_nextTreePacket__Ljava_lang_String_2
        (JNIEnv *env, jobject me, jstring newLabel) {
    return makeJNPacket(env, GET_ENGINE_OBJECT(env, NPacket, me)->
        nextTreePacket(jstringToCString(env, newLabel)));
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_removeAllTags
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NPacket, me)->removeAllTags();
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_removeTag
        (JNIEnv *env, jobject me, jstring tag) {
    return GET_ENGINE_OBJECT(env, NPacket, me)->removeTag(
        jstringToCString(env, tag));
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_setPacketLabel
        (JNIEnv *env, jobject me, jstring newLabel) {
    GET_ENGINE_OBJECT(env, NPacket, me)->setPacketLabel(
        jstringToCString(env, newLabel));
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIPacket_swapWithNextSibling
        (JNIEnv* env, jobject me) {
    GET_ENGINE_OBJECT(env, NPacket, me)->swapWithNextSibling();
}

