
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

#include "packet/nscript.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_addFirst
        (JNIEnv *env, jobject me, jstring str) {
    GET_ENGINE_OBJECT(env, NScript, me)->addFirst(jstringToCString(env, str));
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_addLast
        (JNIEnv *env, jobject me, jstring str) {
    GET_ENGINE_OBJECT(env, NScript, me)->addLast(jstringToCString(env, str));
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_addVariable
        (JNIEnv *env, jobject me, jstring name, jstring value) {
    return GET_ENGINE_OBJECT(env, NScript, me)->addVariable(
        jstringToCString(env, name), jstringToCString(env, value));
}


REGJNIEXPORT jstring JNICALL 
        Java_normal_engine_implementation_jni_packet_NJNIScript_getLine
        (JNIEnv *env, jobject me, jlong index) {
    return jstringFromCString(env, GET_ENGINE_OBJECT(env, NScript, me)->
        getLine(index));
}

REGJNIEXPORT jlong JNICALL Java_normal_engine_implementation_jni_packet_NJNIScript_getNumberOfLines
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NScript, me)->getNumberOfLines();
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_getNumberOfVariables
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NScript, me)->getNumberOfVariables();
}

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_getVariableName
        (JNIEnv *env, jobject me, jlong index) {
    return jstringFromCString(env, GET_ENGINE_OBJECT(env, NScript, me)->
        getVariableName(index));
}

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_getVariableValue__J
        (JNIEnv *env, jobject me, jlong index) {
    return jstringFromCString(env, GET_ENGINE_OBJECT(env, NScript, me)->
        getVariableValue(index));
}

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_getVariableValue__Ljava_lang_String_2
        (JNIEnv *env, jobject me, jstring name) {
    return jstringFromCString(env, GET_ENGINE_OBJECT(env, NScript, me)->
        getVariableValue(jstringToCString(env, name)));
}

REGJNIEXPORT void JNICALL 
        Java_normal_engine_implementation_jni_packet_NJNIScript_insertAtPosition
        (JNIEnv *env, jobject me, jstring str, jlong index) {
    GET_ENGINE_OBJECT(env, NScript, me)->insertAtPosition(
        jstringToCString(env, str), index);
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_newNScript
        (JNIEnv *env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NScript(), me);
}

REGJNIEXPORT void JNICALL 
        Java_normal_engine_implementation_jni_packet_NJNIScript_removeAllLines
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NScript, me)->removeAllLines();
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_removeAllVariables
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NScript, me)->removeAllVariables();
}

REGJNIEXPORT void JNICALL 
        Java_normal_engine_implementation_jni_packet_NJNIScript_removeLineAt
        (JNIEnv *env, jobject me, jlong index) {
    GET_ENGINE_OBJECT(env, NScript, me)->removeLineAt(index);
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_removeVariable
        (JNIEnv *env, jobject me, jstring name) {
    GET_ENGINE_OBJECT(env, NScript, me)->removeVariable(
        jstringToCString(env, name));
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_replaceAtPosition
        (JNIEnv *env, jobject me, jstring str, jlong index) {
    GET_ENGINE_OBJECT(env, NScript, me)->replaceAtPosition(
        jstringToCString(env, str), index);
}

