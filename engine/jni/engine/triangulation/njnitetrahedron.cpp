
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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

#include "triangulation/ntetrahedron.h"
#include "triangulation/ntriangulation.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_getAdjacentFace
        (JNIEnv *env, jobject me, jint face) {
    return (jint)GET_ENGINE_OBJECT(env, NTetrahedron, me)->
        getAdjacentFace(face);
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_getAdjacentTetrahedron
        (JNIEnv *env, jobject me, jint index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTetrahedron, me)->getAdjacentTetrahedron(index),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

REGJNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron__1getAdjacentTetrahedronGluing
        (JNIEnv *env, jobject me, jint index) {
    return GET_ENGINE_OBJECT(env, NTetrahedron, me)->
        getAdjacentTetrahedronGluing(index).getPermCode();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_getComponent
        (JNIEnv *env, jobject me, jint index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTetrahedron, me)->getComponent(),
        "normal/engine/implementation/jni/triangulation/NJNIComponent");
}

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_getDescription
        (JNIEnv *env, jobject me) {
    return jstringFromCString(env, GET_ENGINE_OBJECT(env, NTetrahedron, me)->
        getDescription());
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_getEdge
        (JNIEnv *env, jobject me, jint index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTetrahedron, me)->getEdge(index),
        "normal/engine/implementation/jni/triangulation/NJNIEdge");
}

REGJNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron__1getEdgeMapping
        (JNIEnv *env, jobject me, jint index) {
    return GET_ENGINE_OBJECT(env, NTetrahedron, me)->getEdgeMapping(index).
        getPermCode();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_getFace
        (JNIEnv *env, jobject me, jint index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTetrahedron, me)->getFace(index),
        "normal/engine/implementation/jni/triangulation/NJNIFace");
}

REGJNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron__1getFaceMapping
        (JNIEnv *env, jobject me, jint index) {
    return GET_ENGINE_OBJECT(env, NTetrahedron, me)->getFaceMapping(index).
        getPermCode();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_getVertex
        (JNIEnv *env, jobject me, jint index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTetrahedron, me)->getVertex(index),
        "normal/engine/implementation/jni/triangulation/NJNIVertex");
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_hasBoundary
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTetrahedron, me)->hasBoundary();
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_isolate
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NTetrahedron, me)->isolate();
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron__1joinTo
        (JNIEnv *env, jobject me, jint myFace, jobject you, jchar gluing) {
    GET_ENGINE_OBJECT(env, NTetrahedron, me)->joinTo(myFace,
        GET_ENGINE_OBJECT(env, NTetrahedron, you), NPerm(gluing));
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_newNTetrahedron__
        (JNIEnv *env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NTetrahedron(), me);
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_newNTetrahedron__Ljava_lang_String_2
        (JNIEnv *env, jobject me, jstring desc) {
    ASSIGN_ENGINE_OBJECT(env,
        new NTetrahedron(jstringToCString(env, desc)), me);
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_setDescription
        (JNIEnv *env, jobject me, jstring newDesc) {
    GET_ENGINE_OBJECT(env, NTetrahedron, me)->setDescription(
        jstringToCString(env, newDesc));
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_unjoin
        (JNIEnv *env, jobject me, jint face) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTetrahedron, me)->unjoin(face),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

