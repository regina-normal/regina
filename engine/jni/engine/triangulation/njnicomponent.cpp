
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

#include "triangulation/ncomponent.h"
#include "triangulation/nboundarycomponent.h"
#include "triangulation/nface.h"
#include "triangulation/nedge.h"
#include "triangulation/nvertex.h"
#include "jnitools.h"
#include "engine/triangulation/NJNIComponent.h"

using namespace regina;

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIComponent_getBoundaryComponent
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NComponent, me)->getBoundaryComponent(index),
        "normal/engine/implementation/jni/triangulation/NJNIBoundaryComponent");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIComponent_getEdge
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NComponent, me)->getEdge(index),
        "normal/engine/implementation/jni/triangulation/NJNIEdge");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIComponent_getFace
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NComponent, me)->getFace(index),
        "normal/engine/implementation/jni/triangulation/NJNIFace");
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIComponent_getNumberOfBoundaryComponents
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NComponent, me)->
        getNumberOfBoundaryComponents();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIComponent_getNumberOfEdges
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NComponent, me)->getNumberOfEdges();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIComponent_getNumberOfFaces
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NComponent, me)->getNumberOfFaces();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIComponent_getNumberOfTetrahedra
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NComponent, me)->getNumberOfTetrahedra();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIComponent_getNumberOfVertices
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NComponent, me)->getNumberOfVertices();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIComponent_getTetrahedron
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NComponent, me)->getTetrahedron(index),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIComponent_getVertex
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NComponent, me)->getVertex(index),
        "normal/engine/implementation/jni/triangulation/NJNIVertex");
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIComponent_isClosed
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NComponent, me)->isClosed();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIComponent_isIdeal
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NComponent, me)->isIdeal();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIComponent_isOrientable
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NComponent, me)->isOrientable();
}

