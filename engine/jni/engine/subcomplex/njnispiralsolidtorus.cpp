
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

#include "subcomplex/nspiralsolidtorus.h"
#include "triangulation/ntriangulation.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNISpiralSolidTorus_cloneMe
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NSpiralSolidTorus, me)->clone(),
        "normal/engine/implementation/jni/subcomplex/NJNISpiralSolidTorus");
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNISpiralSolidTorus_cycle
        (JNIEnv *env, jobject me, jlong index) {
    GET_ENGINE_OBJECT(env, NSpiralSolidTorus, me)->cycle(index);
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNISpiralSolidTorus_getNumberOfTetrahedra
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NSpiralSolidTorus, me)->
        getNumberOfTetrahedra();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNISpiralSolidTorus_getTetrahedron
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NSpiralSolidTorus, me)->getTetrahedron(index),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

REGJNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNISpiralSolidTorus__1getVertexRoles
        (JNIEnv *env, jobject me, jlong index) {
    return GET_ENGINE_OBJECT(env, NSpiralSolidTorus, me)->getVertexRoles(index).
        getPermCode();
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNISpiralSolidTorus_isCanonical
        (JNIEnv *env, jobject me, jobject tri) {
    return GET_ENGINE_OBJECT(env, NSpiralSolidTorus, me)->isCanonical(
        GET_ENGINE_OBJECT(env, NTriangulation, tri));
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNISpiralSolidTorus_makeCanonical
        (JNIEnv *env, jobject me, jobject tri) {
    return GET_ENGINE_OBJECT(env, NSpiralSolidTorus, me)->makeCanonical(
        GET_ENGINE_OBJECT(env, NTriangulation, tri));
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNISpiralSolidTorus_reverse
        (JNIEnv* env, jobject me) {
    GET_ENGINE_OBJECT(env, NSpiralSolidTorus, me)->reverse();
}

