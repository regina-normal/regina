
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

#include "config.h"
#ifdef __NO_INCLUDE_PATHS
    #include "sfproperties.h"
    #include "jnitools.h"
    #include "NJNISurfaceFilterProperties.h"
#else
    #include "engine/surfaces/sfproperties.h"
    #include "jni/jnitools.h"
    #include "jni/engine/surfaces/NJNISurfaceFilterProperties.h"
#endif

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties_addEC
        (JNIEnv *env, jobject me, jobject you) {
    GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->addEC(
        jBigIntegerToLarge(env, you));
}

JNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties__1getCompactness
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        getCompactness().getByteCode();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties_getEC
        (JNIEnv *env, jobject me, jlong index) {
    return jBigIntegerFromLarge(env,
        GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        getEC(index));
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties_getNumberOfECs
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        getNumberOfECs();
}

JNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties__1getOrientability
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        getOrientability().getByteCode();
}

JNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties__1getRealBoundary
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        getRealBoundary().getByteCode();
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties_newNSurfaceFilterProperties__
        (JNIEnv *env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NSurfaceFilterProperties(), me);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties_newNSurfaceFilterProperties__Lnormal_engine_surfaces_NSurfaceFilterProperties_2
        (JNIEnv *env, jobject me, jobject you) {
    ASSIGN_ENGINE_OBJECT(env, new NSurfaceFilterProperties(
        *GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, you)), me);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties_removeAllECs
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->removeAllECs();
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties_removeEC
        (JNIEnv *env, jobject me, jobject you) {
    GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->removeEC(
        jBigIntegerToLarge(env, you));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties__1setCompactness
        (JNIEnv *env, jobject me, jchar you) {
    GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        setCompactness(NBoolSet::fromByteCode(you));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties__1setOrientability
        (JNIEnv *env, jobject me, jchar you) {
    GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        setOrientability(NBoolSet::fromByteCode(you));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties__1setRealBoundary
        (JNIEnv *env, jobject me, jchar you) {
    GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        setRealBoundary(NBoolSet::fromByteCode(you));
}

