
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

#include "packet/nscript.h"
#include "jnitools.h"
#include "engine/packet/NJNIScript.h"

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_addFirst
        (JNIEnv *env, jobject me, jstring str) {
    GET_ENGINE_OBJECT(env, NScript, me)->addFirst(jstringToNString(env, str));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_addLast
        (JNIEnv *env, jobject me, jstring str) {
    GET_ENGINE_OBJECT(env, NScript, me)->addLast(jstringToNString(env, str));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_addVariable
        (JNIEnv *env, jobject me, jstring name, jstring value) {
    GET_ENGINE_OBJECT(env, NScript, me)->addVariable(
        jstringToNString(env, name), jstringToNString(env, value));
}


JNIEXPORT jstring JNICALL 
        Java_normal_engine_implementation_jni_packet_NJNIScript_getLine
        (JNIEnv *env, jobject me, jlong index) {
    return jstringFromNString(env, GET_ENGINE_OBJECT(env, NScript, me)->
        getLine(index));
}

JNIEXPORT jlong JNICALL Java_normal_engine_implementation_jni_packet_NJNIScript_getNumberOfLines
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NScript, me)->getNumberOfLines();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_getNumberOfVariables
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NScript, me)->getNumberOfVariables();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_getVariableIndex
        (JNIEnv *env, jobject me, jstring name) {
    return GET_ENGINE_OBJECT(env, NScript, me)->getVariableIndex(
        jstringToNString(env, name));
}

JNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_getVariableName
        (JNIEnv *env, jobject me, jlong index) {
    return jstringFromNString(env, GET_ENGINE_OBJECT(env, NScript, me)->
        getVariableName(index));
}

JNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_getVariableValue__J
        (JNIEnv *env, jobject me, jlong index) {
    return jstringFromNString(env, GET_ENGINE_OBJECT(env, NScript, me)->
        getVariableValue(index));
}

JNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_getVariableValue__Ljava_lang_String_2
        (JNIEnv *env, jobject me, jstring name) {
    return jstringFromNString(env, GET_ENGINE_OBJECT(env, NScript, me)->
        getVariableValue(jstringToNString(env, name)));
}

JNIEXPORT void JNICALL 
        Java_normal_engine_implementation_jni_packet_NJNIScript_insertAtPosition
        (JNIEnv *env, jobject me, jstring str, jlong index) {
    GET_ENGINE_OBJECT(env, NScript, me)->insertAtPosition(
        jstringToNString(env, str), index);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_newNScript
        (JNIEnv *env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NScript(), me);
}

JNIEXPORT void JNICALL 
        Java_normal_engine_implementation_jni_packet_NJNIScript_removeAllLines
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NScript, me)->removeAllLines();
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_removeAllVariables
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NScript, me)->removeAllVariables();
}

JNIEXPORT void JNICALL 
        Java_normal_engine_implementation_jni_packet_NJNIScript_removeLineAt
        (JNIEnv *env, jobject me, jlong index) {
    GET_ENGINE_OBJECT(env, NScript, me)->removeLineAt(index);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_removeVariable
        (JNIEnv *env, jobject me, jstring name) {
    GET_ENGINE_OBJECT(env, NScript, me)->removeVariable(
        jstringToNString(env, name));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_removeVariableAt
        (JNIEnv *env, jobject me, jlong index) {
    GET_ENGINE_OBJECT(env, NScript, me)->removeVariableAt(index);
}


JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIScript_replaceAtPosition
        (JNIEnv *env, jobject me, jstring str, jlong index) {
    GET_ENGINE_OBJECT(env, NScript, me)->replaceAtPosition(
        jstringToNString(env, str), index);
}

