
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

#include "subcomplex/nlayeredchain.h"
#include "triangulation/ntetrahedron.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredChain_extendAbove
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLayeredChain, me)->extendAbove();
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredChain_extendBelow
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLayeredChain, me)->extendBelow();
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredChain_extendMaximal
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLayeredChain, me)->extendMaximal();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredChain_getBottom
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NLayeredChain, me)->getBottom(),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

REGJNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredChain__1getBottomVertexRoles
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLayeredChain, me)->getBottomVertexRoles().
        getPermCode();
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredChain_getIndex
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLayeredChain, me)->getIndex();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredChain_getTop
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NLayeredChain, me)->getTop(),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

REGJNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredChain__1getTopVertexRoles
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLayeredChain, me)->getTopVertexRoles().
        getPermCode();
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredChain_invert
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NLayeredChain, me)->invert();
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredChain_newNJNILayeredChain
        (JNIEnv* env, jobject me, jobject tet, jchar roles) {
    ASSIGN_ENGINE_OBJECT(env, new NLayeredChain(
        GET_ENGINE_OBJECT(env, NTetrahedron, tet), NPerm(roles)), me);
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredChain_newNJNILayeredChain__Lnormal_engine_subcomplex_NLayeredChain_2
        (JNIEnv* env, jobject me, jobject you) {
    ASSIGN_ENGINE_OBJECT(env, new NLayeredChain(
        *GET_ENGINE_OBJECT(env, NLayeredChain, you)), me);
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredChain_reverse
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NLayeredChain, me)->reverse();
}

