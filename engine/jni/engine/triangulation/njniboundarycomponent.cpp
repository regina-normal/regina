
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

#include "triangulation/nboundarycomponent.h"
#include "triangulation/nface.h"
#include "triangulation/nedge.h"
#include "triangulation/nvertex.h"
#include "jnitools.h"
#include "engine/triangulation/NJNIBoundaryComponent.h"

using namespace regina;

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIBoundaryComponent_getEdge
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NBoundaryComponent, me)->getEdge(index),
        "normal/engine/implementation/jni/triangulation/NJNIEdge");
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIBoundaryComponent_getEulerCharacteristic
        (JNIEnv* env, jobject me) {
    return GET_ENGINE_OBJECT(env, NBoundaryComponent, me)->
        getEulerCharacteristic();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIBoundaryComponent_getFace
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NBoundaryComponent, me)->getFace(index),
        "normal/engine/implementation/jni/triangulation/NJNIFace");
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIBoundaryComponent_getNumberOfEdges
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NBoundaryComponent, me)->getNumberOfEdges();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIBoundaryComponent_getNumberOfFaces
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NBoundaryComponent, me)->getNumberOfFaces();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIBoundaryComponent_getNumberOfVertices
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NBoundaryComponent, me)->
        getNumberOfVertices();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIBoundaryComponent_getVertex
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NBoundaryComponent, me)->getVertex(index),
        "normal/engine/implementation/jni/triangulation/NJNIVertex");
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIBoundaryComponent_isIdeal
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NBoundaryComponent, me)->isIdeal();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIBoundaryComponent_isOrientable
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NBoundaryComponent, me)->isOrientable();
}

