
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

#include "triangulation/nedge.h"
#include "triangulation/nface.h"
#include "triangulation/ncomponent.h"
#include "triangulation/nboundarycomponent.h"
#include "triangulation/ntetrahedron.h"
#include "jnitools.h"
#include "engine/triangulation/NJNIFace.h"

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIFace_getBoundaryComponent
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NFace, me)->getBoundaryComponent(),
        "normal/engine/implementation/jni/triangulation/NJNIBoundaryComponent");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIFace_getComponent
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NFace, me)->getComponent(),
        "normal/engine/implementation/jni/triangulation/NJNIComponent");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIFace_getEdge
        (JNIEnv *env, jobject me, jint index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NFace, me)->getEdge(index),
        "normal/engine/implementation/jni/triangulation/NJNIEdge");
}

JNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIFace__1getEdgeMapping
        (JNIEnv *env, jobject me, jint index) {
    return GET_ENGINE_OBJECT(env, NFace, me)->getEdgeMapping(index).
        getPermCode();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIFace_getEmbedding
        (JNIEnv *env, jobject me, jint index) {
    NFace* face = GET_ENGINE_OBJECT(env, NFace, me);
    const NFaceEmbedding& embedding = face->getEmbedding(index);
    jobject tet = CREATE_WRAPPER_OBJECT(env, embedding.getTetrahedron(),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");

    jclass embClass =
        env->FindClass("normal/engine/triangulation/NFaceEmbedding");
    if (! embClass)
        return 0;
    jmethodID constructor = env->GetMethodID(embClass, "<init>",
        "(Lnormal/engine/triangulation/NTetrahedron;I)V");
    if (! constructor)
        return 0;
    return env->NewObject(embClass, constructor, tet, embedding.getFace());
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIFace_getNumberOfEmbeddings
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NFace, me)->getNumberOfEmbeddings();
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIFace_getSubtype
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NFace, me)->getSubtype();
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIFace_getType
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NFace, me)->getType();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIFace_getVertex
        (JNIEnv *env, jobject me, jint index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NFace, me)->getVertex(index),
        "normal/engine/implementation/jni/triangulation/NJNIVertex");
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIFace_isBoundary
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NFace, me)->isBoundary();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIFace_isCone
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NFace, me)->isCone();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIFace_isMobiusBand
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NFace, me)->isMobiusBand();
}

