
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

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_cloneMe
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->clone(),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredSolidTorus");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getBase
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->getBase(),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getBaseEdge
        (JNIEnv *env, jobject me, jint group, jint index) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->
        getBaseEdge(group, index);
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getBaseEdgeGroup
        (JNIEnv *env, jobject me, jint i) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->
        getBaseEdgeGroup(i);
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getBaseFace
        (JNIEnv *env, jobject me, jint index) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->getBaseFace(index);
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getMeridinalCuts
        (JNIEnv *env, jobject me, jint i) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->
        getMeridinalCuts(i);
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getTopEdge
        (JNIEnv *env, jobject me, jint group, jint index) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->
        getTopEdge(group, index);
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getTopEdgeGroup
        (JNIEnv *env, jobject me, jint i) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->
        getTopEdgeGroup(i);
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getTopFace
        (JNIEnv *env, jobject me, jint index) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->getTopFace(index);
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getTopLevel
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->getTopLevel(),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredSolidTorus_getNumberOfTetrahedra
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLayeredSolidTorus, me)->
        getNumberOfTetrahedra();
}

