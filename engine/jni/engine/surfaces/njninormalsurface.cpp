
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
    #include "nnormalsurface.h"
    #include "ntriangulation.h"
    #include "jnitools.h"
    #include "NJNINormalSurface.h"
#else
    #include "engine/surfaces/nnormalsurface.h"
    #include "engine/triangulation/ntriangulation.h"
    #include "jni/jnitools.h"
    #include "jni/engine/surfaces/NJNINormalSurface.h"
#endif

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
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_isOrientable
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurface, me)->isOrientable();
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurface_writeRawVector
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NNormalSurface, me)->writeRawVector(cout);
}

