
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

#include "subcomplex/nlensspace.h"
#include "jnitools.h"
#include "engine/subcomplex/NJNILensSpace.h"

using namespace regina;
using namespace regina::jni;

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILensSpace_getP
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLensSpace, me)->getP();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILensSpace_getQ
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLensSpace, me)->getQ();
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILensSpace_isEquivalentTo
        (JNIEnv *env, jobject me, jobject you) {
    return (*GET_ENGINE_OBJECT(env, NLensSpace, me) ==
        *GET_ENGINE_OBJECT(env, NLensSpace, you));
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILensSpace_newNJNILensSpace__JJ
        (JNIEnv *env, jobject me, jlong p, jlong q) {
    ASSIGN_ENGINE_OBJECT(env, new NLensSpace(p, q), me);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILensSpace_newNJNILensSpace__Lnormal_engine_subcomplex_NLensSpace_2
        (JNIEnv *env, jobject me, jobject you) {
    ASSIGN_ENGINE_OBJECT(env, new NLensSpace(
        *GET_ENGINE_OBJECT(env, NLensSpace, you)), me);
}

