
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
#include "engine/algebra/NJNIGroupPresentation.h"

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_addGenerator
        (JNIEnv *env, jobject me, jlong num) {
    return GET_ENGINE_OBJECT(env, NGroupPresentation, me)->addGenerator(num);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_addRelation
        (JNIEnv *env, jobject me, jobject rel) {
    GET_ENGINE_OBJECT(env, NGroupPresentation, me)->addRelation(
        GET_ENGINE_OBJECT(env, NGroupExpression, rel));
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_getNumberOfGenerators
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NGroupPresentation, me)->
        getNumberOfGenerators();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_getNumberOfRelations
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NGroupPresentation, me)->
        getNumberOfRelations();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_getRelation
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        (NGroupExpression*)&GET_ENGINE_OBJECT(env, NGroupPresentation, me)->
        getRelation(index),
        "normal/engine/implementation/jni/algebra/NJNIGroupExpression");
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_intelligentSimplify
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NGroupPresentation, me)->
        intelligentSimplify();
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_newNGroupPresentation__
        (JNIEnv *env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NGroupPresentation(), me);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_newNGroupPresentation__Lnormal_engine_algebra_NGroupPresentation_2
        (JNIEnv *env, jobject me, jobject you) {
    ASSIGN_ENGINE_OBJECT(env,
        new NGroupPresentation(*GET_ENGINE_OBJECT(
        env, NGroupPresentation, you)), me);
}

JNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_recogniseGroup
        (JNIEnv *env, jobject me) {
    return jstringFromNString(env,
        GET_ENGINE_OBJECT(env, NGroupPresentation, me)->recogniseGroup());
}

