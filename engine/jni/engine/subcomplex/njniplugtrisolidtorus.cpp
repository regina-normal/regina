
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

#include "subcomplex/nplugtrisolidtorus.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIPlugTriSolidTorus_cloneMe
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NPlugTriSolidTorus, me)->clone(),
        "normal/engine/implementation/jni/subcomplex/NJNIPlugTriSolidTorus");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIPlugTriSolidTorus_getChain
        (JNIEnv *env, jobject me, jint which) {
    return CREATE_WRAPPER_OBJECT(env, (NLayeredChain*)
        GET_ENGINE_OBJECT(env, NPlugTriSolidTorus, me)->getChain(which),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredChain");
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIPlugTriSolidTorus_getChainType
        (JNIEnv *env, jobject me, jint which) {
    return GET_ENGINE_OBJECT(env, NPlugTriSolidTorus, me)->getChainType(which);
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIPlugTriSolidTorus_getCore
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env, (NTriSolidTorus*)
        &GET_ENGINE_OBJECT(env, NPlugTriSolidTorus, me)->getCore(),
        "normal/engine/implementation/jni/subcomplex/NJNITriSolidTorus");
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIPlugTriSolidTorus_getEquatorType
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NPlugTriSolidTorus, me)->getEquatorType();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNIPlugTriSolidTorus_getSeifertStructure
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env, (NSFS*)
        &GET_ENGINE_OBJECT(env, NPlugTriSolidTorus, me)->getSeifertStructure(),
        "normal/engine/implementation/jni/subcomplex/NJNISFS");
}

