
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

#include "subcomplex/nlayeredsolidtorus.h"
#include "triangulation/ntetrahedron.h"
#include "jnitools.h"
#include "engine/subcomplex/NJNILayeredSolidTorus.h"

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_cloneMe
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->clone(),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredSolidTorus");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getBase
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->getBase(),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getBaseEdge
        (JNIEnv *env, jobject me, jint group, jint index) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->
        getBaseEdge(group, index);
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getBaseEdgeGroup
        (JNIEnv *env, jobject me, jint i) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->
        getBaseEdgeGroup(i);
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getBaseFace
        (JNIEnv *env, jobject me, jint index) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->getBaseFace(index);
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getMeridinalCuts
        (JNIEnv *env, jobject me, jint i) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->
        getMeridinalCuts(i);
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getTopEdge
        (JNIEnv *env, jobject me, jint group, jint index) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->
        getTopEdge(group, index);
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getTopEdgeGroup
        (JNIEnv *env, jobject me, jint i) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->
        getTopEdgeGroup(i);
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getTopFace
        (JNIEnv *env, jobject me, jint index) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->getTopFace(index);
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getTopLevel
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->getTopLevel(),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getNumberOfTetrahedra
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->
        getNumberOfTetrahedra();
}

