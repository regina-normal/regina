
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include "surfaces/nnormalsurface.h"
#include "triangulation/ntriangulation.h"
#include "jnitools.h"
#include "engine/surfaces/NJNINormalSurface.h"

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_crush
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NNormalSurface, me)->crush(),
        "normal/engine/implementation/jni/triangulation/NJNITriangulation");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_cutAlong
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NNormalSurface, me)->cutAlong(),
        "normal/engine/implementation/jni/triangulation/NJNITriangulation");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_getEdgeWeight
        (JNIEnv *env, jobject me, jlong index) {
    return jBigIntegerFromLarge(env, GET_ENGINE_OBJECT(env,
        NNormalSurface, me)->getEdgeWeight(index));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_getEulerCharacteristic
        (JNIEnv *env, jobject me) {
    return jBigIntegerFromLarge(env, GET_ENGINE_OBJECT(env, NNormalSurface,
        me)->getEulerCharacteristic());
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_getFaceArcs
        (JNIEnv *env, jobject me, jlong index, jint type) {
    return jBigIntegerFromLarge(env, GET_ENGINE_OBJECT(env,
        NNormalSurface, me)->getFaceArcs(index, type));
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_getNumberOfCoords
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurface, me)->getNumberOfCoords();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_getOctCoord
        (JNIEnv *env, jobject me, jlong index, jint type) {
    return jBigIntegerFromLarge(env, GET_ENGINE_OBJECT(env,
        NNormalSurface, me)->getOctCoord(index, type));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_getQuadCoord
        (JNIEnv *env, jobject me, jlong index, jint type) {
    return jBigIntegerFromLarge(env, GET_ENGINE_OBJECT(env,
        NNormalSurface, me)->getQuadCoord(index, type));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_getTriangleCoord
        (JNIEnv *env, jobject me, jlong index, jint type) {
    return jBigIntegerFromLarge(env, GET_ENGINE_OBJECT(env,
        NNormalSurface, me)->getTriangleCoord(index, type));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_getTriangulation
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NNormalSurface, me)->getTriangulation(),
        "normal/engine/implementation/jni/triangulation/NJNITriangulation");
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_hasRealBoundary
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurface, me)->hasRealBoundary();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_isCompact
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurface, me)->isCompact();
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_isConnected
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurface, me)->isConnected();
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_isOrientable
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurface, me)->isOrientable();
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_isTwoSided
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurface, me)->isTwoSided();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_isVertexLinking
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurface, me)->isVertexLinking();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_isSplitting
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurface, me)->isSplitting();
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_writeRawVector
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NNormalSurface, me)->writeRawVector(cout);
}

