
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

#include "algebra/ngrouppresentation.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_addTermFirst
        (JNIEnv *env, jobject me, jlong gen, jlong exp) {
    GET_ENGINE_OBJECT(env, NGroupExpression, me)->addTermFirst(gen, exp);
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_addTermLast
        (JNIEnv *env, jobject me, jlong gen, jlong exp) {
    GET_ENGINE_OBJECT(env, NGroupExpression, me)->addTermLast(gen, exp);
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_getExponent
        (JNIEnv *env, jobject me, jlong index) {
    return GET_ENGINE_OBJECT(env, NGroupExpression, me)->getExponent(index);
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_getGenerator
        (JNIEnv *env, jobject me, jlong index) {
    return GET_ENGINE_OBJECT(env, NGroupExpression, me)->getGenerator(index);
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_getNumberOfTerms
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NGroupExpression, me)->getNumberOfTerms();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_inverse
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env, GET_ENGINE_OBJECT(env,
        NGroupExpression, me)->inverse(),
        "normal/engine/implementation/jni/algebra/NJNIGroupExpression");
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_newNGroupExpression__
        (JNIEnv *env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NGroupExpression(), me);
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_newNGroupExpression__Lnormal_engine_algebra_NGroupExpression_2
        (JNIEnv *env, jobject me, jobject you) {
    ASSIGN_ENGINE_OBJECT(env,
        new NGroupExpression(*GET_ENGINE_OBJECT(env, NGroupExpression, you)),
        me);
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_power
        (JNIEnv *env, jobject me, jlong exp) {
    return CREATE_WRAPPER_OBJECT(env, GET_ENGINE_OBJECT(env,
        NGroupExpression, me)->power(exp),
        "normal/engine/implementation/jni/algebra/NJNIGroupExpression");
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_simplify
        (JNIEnv *env, jobject me, jboolean cyclic) {
    return GET_ENGINE_OBJECT(env, NGroupExpression, me)->simplify(cyclic);
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_substitute
        (JNIEnv *env, jobject me, jlong gen, jobject expansion,
        jboolean cyclic) {
    return GET_ENGINE_OBJECT(env, NGroupExpression, me)->substitute(
        gen, *GET_ENGINE_OBJECT(env, NGroupExpression, expansion), cyclic);
}

