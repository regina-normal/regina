
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
    #include "ngrouppresentation.h"
    #include "jnitools.h"
    #include "NJNIGroupExpression.h"
#else
    #include "engine/algebra/ngrouppresentation.h"
    #include "jni/jnitools.h"
    #include "jni/engine/algebra/NJNIGroupExpression.h"
#endif

JNIEXPORT void JNICALL
		Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_addTermFirst
		(JNIEnv *env, jobject me, jlong gen, jlong exp) {
	GET_ENGINE_OBJECT(env, NGroupExpression, me)->addTermFirst(gen, exp);
}

JNIEXPORT void JNICALL
		Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_addTermLast
		(JNIEnv *env, jobject me, jlong gen, jlong exp) {
	GET_ENGINE_OBJECT(env, NGroupExpression, me)->addTermLast(gen, exp);
}

JNIEXPORT jlong JNICALL
		Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_getExponent
		(JNIEnv *env, jobject me, jlong index) {
	return GET_ENGINE_OBJECT(env, NGroupExpression, me)->getExponent(index);
}

JNIEXPORT jlong JNICALL
		Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_getGenerator
		(JNIEnv *env, jobject me, jlong index) {
	return GET_ENGINE_OBJECT(env, NGroupExpression, me)->getGenerator(index);
}

JNIEXPORT jlong JNICALL
		Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_getNumberOfTerms
		(JNIEnv *env, jobject me) {
	return GET_ENGINE_OBJECT(env, NGroupExpression, me)->getNumberOfTerms();
}

JNIEXPORT jobject JNICALL
		Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_inverse
		(JNIEnv *env, jobject me) {
	return CREATE_WRAPPER_OBJECT(env, GET_ENGINE_OBJECT(env,
		NGroupExpression, me)->inverse(),
		"normal/engine/implementation/jni/algebra/NJNIGroupExpression");
}

JNIEXPORT void JNICALL
		Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_newNGroupExpression__
		(JNIEnv *env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NGroupExpression(), me);
}

JNIEXPORT void JNICALL
		Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_newNGroupExpression__Lnormal_engine_algebra_NGroupExpression_2
		(JNIEnv *env, jobject me, jobject you) {
    ASSIGN_ENGINE_OBJECT(env,
        new NGroupExpression(*GET_ENGINE_OBJECT(env, NGroupExpression, you)),
		me);
}

JNIEXPORT jobject JNICALL
		Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_power
		(JNIEnv *env, jobject me, jlong exp) {
	return CREATE_WRAPPER_OBJECT(env, GET_ENGINE_OBJECT(env,
		NGroupExpression, me)->power(exp),
		"normal/engine/implementation/jni/algebra/NJNIGroupExpression");
}

JNIEXPORT jboolean JNICALL
		Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_simplify
		(JNIEnv *env, jobject me, jboolean cyclic) {
	return GET_ENGINE_OBJECT(env, NGroupExpression, me)->simplify(cyclic);
}

JNIEXPORT jboolean JNICALL
		Java_normal_engine_implementation_jni_algebra_NJNIGroupExpression_substitute
		(JNIEnv *env, jobject me, jlong gen, jobject expansion,
		jboolean cyclic) {
	return GET_ENGINE_OBJECT(env, NGroupExpression, me)->substitute(
		gen, *GET_ENGINE_OBJECT(env, NGroupExpression, expansion), cyclic);
}

