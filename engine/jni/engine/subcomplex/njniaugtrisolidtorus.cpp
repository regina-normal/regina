
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

#include "subcomplex/naugtrisolidtorus.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus_cloneMe
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->clone(),
        "normal/engine/implementation/jni/subcomplex/NJNIAugTriSolidTorus");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus_getAugTorus
        (JNIEnv *env, jobject me, jint i) {
    return CREATE_WRAPPER_OBJECT(env, (NLayeredSolidTorus*)
        GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->getAugTorus(i),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredSolidTorus");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus_getCore
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env, (NTriSolidTorus*)
        &GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->getCore(),
        "normal/engine/implementation/jni/subcomplex/NJNITriSolidTorus");
}

REGJNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus__1getEdgeGroupRoles
        (JNIEnv *env, jobject me, jint i) {
    return GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->
        getEdgeGroupRoles(i).getPermCode();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus_getSeifertStructure
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env, (NSFS*)
        &GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->getSeifertStructure(),
        "normal/engine/implementation/jni/subcomplex/NJNISFS");
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus_getChainLength
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->getChainLength();
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus_getTorusAnnulus
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->getTorusAnnulus();
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIAugTriSolidTorus_hasLayeredChain
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NAugTriSolidTorus, me)->hasLayeredChain();
}

