
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

#include "maths/nmatrixint.h"
#include "jnitools.h"
#include "engine/maths/NJNIMatrixInt.h"

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_maths_NJNIMatrixInt_addCol__II
        (JNIEnv *env, jobject me, jint source, jint dest) {
    GET_ENGINE_OBJECT(env, NMatrixInt, me)->addCol(source, dest);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_maths_NJNIMatrixInt_addCol__IILjava_math_BigInteger_2
        (JNIEnv *env, jobject me, jint source, jint dest, jobject copies) {
    GET_ENGINE_OBJECT(env, NMatrixInt, me)->addCol(source, dest,
        jBigIntegerToLarge(env, copies));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_maths_NJNIMatrixInt_addRow__II
        (JNIEnv *env, jobject me, jint source, jint dest) {
    GET_ENGINE_OBJECT(env, NMatrixInt, me)->addRow(source, dest);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_maths_NJNIMatrixInt_addRow__IILjava_math_BigInteger_2
        (JNIEnv *env, jobject me, jint source, jint dest, jobject copies) {
    GET_ENGINE_OBJECT(env, NMatrixInt, me)->addRow(source, dest,
        jBigIntegerToLarge(env, copies));
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_maths_NJNIMatrixInt_columns
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NMatrixInt, me)->columns();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_maths_NJNIMatrixInt_getEntry
        (JNIEnv *env, jobject me, jint row, jint col) {
    return jBigIntegerFromLarge(env, GET_ENGINE_OBJECT(env, NMatrixInt, me)->
        entry(row, col));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_maths_NJNIMatrixInt_initialise
        (JNIEnv *env, jobject me, jobject value) {
    GET_ENGINE_OBJECT(env, NMatrixInt, me)->initialise(
        jBigIntegerToLarge(env, value));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_maths_NJNIMatrixInt_multCol
        (JNIEnv *env, jobject me, jint col, jobject factor) {
    GET_ENGINE_OBJECT(env, NMatrixInt, me)->multCol(col,
        jBigIntegerToLarge(env, factor));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_maths_NJNIMatrixInt_multRow
        (JNIEnv *env, jobject me, jint row, jobject factor) {
    GET_ENGINE_OBJECT(env, NMatrixInt, me)->multRow(row,
        jBigIntegerToLarge(env, factor));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_maths_NJNIMatrixInt_newNMatrixInt__II
        (JNIEnv *env, jobject me, jint rows, jint columns) {
    ASSIGN_ENGINE_OBJECT(env, new NMatrixInt(rows, columns), me);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_maths_NJNIMatrixInt_newNMatrixInt__Lnormal_engine_maths_NMatrixInt_2
        (JNIEnv *env, jobject me, jobject you) {
    ASSIGN_ENGINE_OBJECT(env, new NMatrixInt(
        *GET_ENGINE_OBJECT(env, NMatrixInt, you)), me);
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_maths_NJNIMatrixInt_rows
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NMatrixInt, me)->rows();
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_maths_NJNIMatrixInt_setEntry
        (JNIEnv *env, jobject me, jint row, jint col, jobject value) {
    GET_ENGINE_OBJECT(env, NMatrixInt, me)->entry(row, col) =
        jBigIntegerToLarge(env, value);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_maths_NJNIMatrixInt_swapColumns
        (JNIEnv *env, jobject me, jint first, jint second) {
    GET_ENGINE_OBJECT(env, NMatrixInt, me)->swapColumns(first, second);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_maths_NJNIMatrixInt_swapRows
        (JNIEnv *env, jobject me, jint first, jint second) {
    GET_ENGINE_OBJECT(env, NMatrixInt, me)->swapRows(first, second);
}

