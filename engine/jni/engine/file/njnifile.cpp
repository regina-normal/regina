
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

#include "file/nfile.h"
#include "jnitools.h"
#include "registry/makejnpacket.h"
#include "engine/file/NJNIFile.h"

using namespace regina;

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFile_close
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NFile, me)->close();
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFile_getMajorVersion
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NFile, me)->getMajorVersion();
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFile_getMinorVersion
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NFile, me)->getMinorVersion();
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFile_getOpenMode
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NFile, me)->getOpenMode();
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFile_newNFile
        (JNIEnv *env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NFile(), me);
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFile_open
        (JNIEnv *env, jobject me, jstring fileName, jint fileMode) {
    // Always fail.
    return JNI_FALSE;

    /* 
    bool ans = GET_ENGINE_OBJECT(env, NFile, me)->open(
        stringToCString(env, fileName).c_str(),
        (NRandomAccessResource::mode)fileMode);

    if (ans)
        return JNI_TRUE;
    else
        return JNI_FALSE;
    */
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFile_readPacketTree__
        (JNIEnv *env, jobject me) {
    return makeJNPacket(env,
        GET_ENGINE_OBJECT(env, NFile, me)->readPacketTree());
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFile_readPacketTree__Lnormal_engine_packet_NPacket_2
        (JNIEnv *env, jobject me, jobject parent) {
    return makeJNPacket(env,
        GET_ENGINE_OBJECT(env, NFile, me)->readPacketTree(
        GET_ENGINE_OBJECT(env, NPacket, parent)));
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFile_versionEarlierThan
        (JNIEnv *env, jobject me, jint major, jint minor) {
    return GET_ENGINE_OBJECT(env, NFile, me)->versionEarlierThan(major, minor);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFile_writePacketTree
        (JNIEnv *env, jobject me, jobject you) {
    GET_ENGINE_OBJECT(env, NFile, me)->writePacketTree(GET_ENGINE_OBJECT(env,
        NPacket, you));
}

