
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
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
    #include "ntetrahedron.h"
    #include "ntriangulation.h"
    #include "jnitools.h"
    #include "NJNITetrahedron.h"
#else
    #include "engine/triangulation/ntetrahedron.h"
    #include "engine/triangulation/ntriangulation.h"
    #include "jni/jnitools.h"
    #include "jni/engine/triangulation/NJNITetrahedron.h"
#endif

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_getAdjacentFace
        (JNIEnv *env, jobject me, jint face) {
    return (jint)GET_ENGINE_OBJECT(env, NTetrahedron, me)->
        getAdjacentFace(face);
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_getAdjacentTetrahedron
        (JNIEnv *env, jobject me, jint index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTetrahedron, me)->getAdjacentTetrahedron(index),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

JNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron__1getAdjacentTetrahedronGluing
        (JNIEnv *env, jobject me, jint index) {
    return GET_ENGINE_OBJECT(env, NTetrahedron, me)->
        getAdjacentTetrahedronGluing(index).getPermCode();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_getComponent
        (JNIEnv *env, jobject me, jint index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTetrahedron, me)->getComponent(),
        "normal/engine/implementation/jni/triangulation/NJNIComponent");
}

JNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_getDescription
        (JNIEnv *env, jobject me) {
    return jstringFromNString(env, GET_ENGINE_OBJECT(env, NTetrahedron, me)->
        getDescription());
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_getEdge
        (JNIEnv *env, jobject me, jint index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTetrahedron, me)->getEdge(index),
        "normal/engine/implementation/jni/triangulation/NJNIEdge");
}

JNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron__1getEdgeMapping
        (JNIEnv *env, jobject me, jint index) {
    return GET_ENGINE_OBJECT(env, NTetrahedron, me)->getEdgeMapping(index).
        getPermCode();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_getFace
        (JNIEnv *env, jobject me, jint index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTetrahedron, me)->getFace(index),
        "normal/engine/implementation/jni/triangulation/NJNIFace");
}

JNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron__1getFaceMapping
        (JNIEnv *env, jobject me, jint index) {
    return GET_ENGINE_OBJECT(env, NTetrahedron, me)->getFaceMapping(index).
        getPermCode();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_getVertex
        (JNIEnv *env, jobject me, jint index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTetrahedron, me)->getVertex(index),
        "normal/engine/implementation/jni/triangulation/NJNIVertex");
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_hasBoundary
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NTetrahedron, me)->hasBoundary();
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_isolate
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NTetrahedron, me)->isolate();
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron__1joinTo
        (JNIEnv *env, jobject me, jint myFace, jobject you, jchar gluing) {
    GET_ENGINE_OBJECT(env, NTetrahedron, me)->joinTo(myFace,
        GET_ENGINE_OBJECT(env, NTetrahedron, you), NPerm(gluing));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_newNTetrahedron__
        (JNIEnv *env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NTetrahedron(), me);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_newNTetrahedron__Ljava_lang_String_2
        (JNIEnv *env, jobject me, jstring desc) {
    ASSIGN_ENGINE_OBJECT(env,
        new NTetrahedron(jstringToNString(env, desc)), me);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_setDescription
        (JNIEnv *env, jobject me, jstring newDesc) {
    GET_ENGINE_OBJECT(env, NTetrahedron, me)->setDescription(
        jstringToNString(env, newDesc));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNITetrahedron_unjoin
        (JNIEnv *env, jobject me, jint face) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTetrahedron, me)->unjoin(face),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

