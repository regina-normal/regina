
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

#include "subcomplex/nlayeredlensspace.h"
#include "jnitools.h"
#include "engine/subcomplex/NJNILayeredLensSpace.h"

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredLensSpace_cloneMe
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NLayeredLensSpace, me)->clone(),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredLensSpace");
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredLensSpace_getMobiusBoundaryGroup
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLayeredLensSpace, me)->
        getMobiusBoundaryGroup();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredLensSpace_getP
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLayeredLensSpace, me)->getP();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredLensSpace_getQ
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLayeredLensSpace, me)->getQ();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredLensSpace_getTorus
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env, (NLayeredSolidTorus*)
        &GET_ENGINE_OBJECT(env, NLayeredLensSpace, me)->getTorus(),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredSolidTorus");
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredLensSpace_isSnapped
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLayeredLensSpace, me)->isSnapped();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredLensSpace_isTwisted
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLayeredLensSpace, me)->isTwisted();
}

