
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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
    #include "naugtrisolidtorus.h"
    #include "jnitools.h"
    #include "NJNIAugTriSolidTorus.h"
#else
    #include "engine/subcomplex/naugtrisolidtorus.h"
    #include "jni/jnitools.h"
    #include "jni/engine/subcomplex/NJNIAugTriSolidTorus.h"
#endif

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus_cloneMe
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->clone(),
        "normal/engine/implementation/jni/subcomplex/NJNIAugTriSolidTorus");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus_getAugTorus
        (JNIEnv *env, jobject me, jint i) {
    return CREATE_WRAPPER_OBJECT(env, (NLayeredSolidTorus*)
        GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->getAugTorus(i),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredSolidTorus");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus_getCore
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env, (NTriSolidTorus*)
        &GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->getCore(),
        "normal/engine/implementation/jni/subcomplex/NJNITriSolidTorus");
}

JNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus__1getEdgeGroupRoles
        (JNIEnv *env, jobject me, jint i) {
    return GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->
        getEdgeGroupRoles(i).getPermCode();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus_getSeifertStructure
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env, (NSFS*)
        &GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->getSeifertStructure(),
        "normal/engine/implementation/jni/subcomplex/NJNISFS");
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus_getChainLength
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->getChainLength();
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus_getTorusAnnulus
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->getTorusAnnulus();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus_hasLayeredChain
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->hasLayeredChain();
}

