
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

#include "algebra/ngrouppresentation.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_addGenerator
        (JNIEnv *env, jobject me, jlong num) {
    return GET_ENGINE_OBJECT(env, NGroupPresentation, me)->addGenerator(num);
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_addRelation
        (JNIEnv *env, jobject me, jobject rel) {
    GET_ENGINE_OBJECT(env, NGroupPresentation, me)->addRelation(
        GET_ENGINE_OBJECT(env, NGroupExpression, rel));
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_getNumberOfGenerators
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NGroupPresentation, me)->
        getNumberOfGenerators();
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_getNumberOfRelations
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NGroupPresentation, me)->
        getNumberOfRelations();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_getRelation
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        (NGroupExpression*)&GET_ENGINE_OBJECT(env, NGroupPresentation, me)->
        getRelation(index),
        "normal/engine/implementation/jni/algebra/NJNIGroupExpression");
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_intelligentSimplify
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NGroupPresentation, me)->
        intelligentSimplify();
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_newNGroupPresentation__
        (JNIEnv *env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NGroupPresentation(), me);
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_newNGroupPresentation__Lnormal_engine_algebra_NGroupPresentation_2
        (JNIEnv *env, jobject me, jobject you) {
    ASSIGN_ENGINE_OBJECT(env,
        new NGroupPresentation(*GET_ENGINE_OBJECT(
        env, NGroupPresentation, you)), me);
}

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_algebra_NJNIGroupPresentation_recogniseGroup
        (JNIEnv *env, jobject me) {
    return jstringFromCString(env,
        GET_ENGINE_OBJECT(env, NGroupPresentation, me)->recogniseGroup());
}

