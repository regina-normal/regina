
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

#include "surfaces/sfproperties.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties_addEC
        (JNIEnv *env, jobject me, jobject you) {
    GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->addEC(
        jBigIntegerToLarge(env, you));
}

REGJNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties__1getCompactness
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        getCompactness().getByteCode();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties_getEC
        (JNIEnv *env, jobject me, jlong index) {
    return jBigIntegerFromLarge(env,
        GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        getEC(index));
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties_getNumberOfECs
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        getNumberOfECs();
}

REGJNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties__1getOrientability
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        getOrientability().getByteCode();
}

REGJNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties__1getRealBoundary
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        getRealBoundary().getByteCode();
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties_newNSurfaceFilterProperties__
        (JNIEnv *env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NSurfaceFilterProperties(), me);
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties_newNSurfaceFilterProperties__Lnormal_engine_surfaces_NSurfaceFilterProperties_2
        (JNIEnv *env, jobject me, jobject you) {
    ASSIGN_ENGINE_OBJECT(env, new NSurfaceFilterProperties(
        *GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, you)), me);
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties_removeAllECs
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->removeAllECs();
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties_removeEC
        (JNIEnv *env, jobject me, jobject you) {
    GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->removeEC(
        jBigIntegerToLarge(env, you));
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties__1setCompactness
        (JNIEnv *env, jobject me, jchar you) {
    GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        setCompactness(NBoolSet::fromByteCode(you));
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties__1setOrientability
        (JNIEnv *env, jobject me, jchar you) {
    GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        setOrientability(NBoolSet::fromByteCode(you));
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterProperties__1setRealBoundary
        (JNIEnv *env, jobject me, jchar you) {
    GET_ENGINE_OBJECT(env, NSurfaceFilterProperties, me)->
        setRealBoundary(NBoolSet::fromByteCode(you));
}

