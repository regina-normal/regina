
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

#include "subcomplex/ntrisolidtorus.h"
#include "triangulation/ntetrahedron.h"
#include "jnitools.h"
#include "engine/subcomplex/NJNITriSolidTorus.h"

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNITriSolidTorus_cloneMe
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTriSolidTorus, me)->clone(),
        "normal/engine/implementation/jni/subcomplex/NJNITriSolidTorus");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNITriSolidTorus_getTetrahedron
        (JNIEnv *env, jobject me, jint index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NTriSolidTorus, me)->getTetrahedron(index),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

JNIEXPORT jchar JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNITriSolidTorus__1getVertexRoles
        (JNIEnv *env, jobject me, jint index) {
    return GET_ENGINE_OBJECT(env, NTriSolidTorus, me)->getVertexRoles(index).
        getPermCode();
}

