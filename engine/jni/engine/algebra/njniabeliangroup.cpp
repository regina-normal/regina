
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

#include "algebra/nabeliangroup.h"
#include "jnitools.h"
#include "engine/algebra/NJNIAbelianGroup.h"

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIAbelianGroup_addGroup__Lnormal_engine_algebra_NAbelianGroup_2
        (JNIEnv* env, jobject me, jobject group) {
    GET_ENGINE_OBJECT(env, NAbelianGroup, me)->
        addGroup(*GET_ENGINE_OBJECT(env, NAbelianGroup, group));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIAbelianGroup_addGroup__Lnormal_engine_utilities_NMatrixInt_2
        (JNIEnv* env, jobject me, jobject presentation) {
    GET_ENGINE_OBJECT(env, NAbelianGroup, me)->
        addGroup(*GET_ENGINE_OBJECT(env, NMatrixInt, presentation));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIAbelianGroup_addRank
        (JNIEnv* env, jobject me, jint extraRank) {
    GET_ENGINE_OBJECT(env, NAbelianGroup, me)->addRank(extraRank);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIAbelianGroup_addTorsionElement__JI
        (JNIEnv* env, jobject me, jlong degree, jint mult) {
    GET_ENGINE_OBJECT(env, NAbelianGroup, me)->addTorsionElement(degree, mult);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIAbelianGroup_addTorsionElement__Ljava_math_BigInteger_2I
        (JNIEnv* env, jobject me, jobject degree, jint mult) {
    GET_ENGINE_OBJECT(env, NAbelianGroup, me)->
        addTorsionElement(jBigIntegerToLarge(env, degree), mult);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIAbelianGroup_addTorsionElements
        (JNIEnv* env, jobject me, jobject torsion) {
    NDoubleList<NLargeInteger> list;
    jclass c = env->GetObjectClass(torsion);
    jmethodID hasMoreElements = env->GetMethodID(c, "hasMoreElements",
        "()Z");
    jmethodID nextElement = env->GetMethodID(c, "nextElement",
        "()Ljava/lang/Object;");
    while (env->CallBooleanMethod(torsion, hasMoreElements)) {
        list.addLast(jBigIntegerToLarge(env,
            env->CallObjectMethod(torsion, nextElement)));
    }
    GET_ENGINE_OBJECT(env, NAbelianGroup, me)->addTorsionElements(list);
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIAbelianGroup_getNumberOfInvariantFactors
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NAbelianGroup, me)->
        getNumberOfInvariantFactors();
}
 
JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIAbelianGroup_getInvariantFactor
        (JNIEnv *env, jobject me, jlong index) {
    return jBigIntegerFromLarge(env,
        GET_ENGINE_OBJECT(env, NAbelianGroup, me)->getInvariantFactor(index));
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIAbelianGroup_getRank
        (JNIEnv* env, jobject me) {
    return (jint)GET_ENGINE_OBJECT(env, NAbelianGroup, me)->getRank();
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIAbelianGroup_getTorsionRank__J
        (JNIEnv* env, jobject me, jlong degree) {
    return (jint)GET_ENGINE_OBJECT(env, NAbelianGroup, me)->
        getTorsionRank(degree);
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIAbelianGroup_getTorsionRank__Ljava_math_BigInteger_2
        (JNIEnv* env, jobject me, jobject degree) {
    return (jint)GET_ENGINE_OBJECT(env, NAbelianGroup, me)->
        getTorsionRank(jBigIntegerToLarge(env, degree));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIAbelianGroup_newNAbelianGroup__
        (JNIEnv* env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NAbelianGroup(), me);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIAbelianGroup_newNAbelianGroup__Lnormal_engine_algebra_NAbelianGroup_2
        (JNIEnv* env, jobject me, jobject cloneMe) {
    ASSIGN_ENGINE_OBJECT(env,
        new NAbelianGroup(*GET_ENGINE_OBJECT(env, NAbelianGroup, cloneMe)), me);
}

