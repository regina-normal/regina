
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

#include "subcomplex/nlayeredloop.h"
#include "triangulation/nedge.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredLoop_cloneMe
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NLayeredLoop, me)->clone(),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredLoop");
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredLoop_getIndex
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLayeredLoop, me)->getIndex();
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredLoop_isTwisted
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NLayeredLoop, me)->isTwisted();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredLoop_getHinge
        (JNIEnv *env, jobject me, jint which) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NLayeredLoop, me)->getHinge(which),
        "normal/engine/implementation/jni/triangulation/NJNIEdge");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNILayeredLoop_getSeifertStructure
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env, (NSFS*)
        &GET_ENGINE_OBJECT(env, NLayeredLoop, me)->getSeifertStructure(),
        "normal/engine/implementation/jni/subcomplex/NJNISFS");
}
