
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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
    #include "nsnappedtwosphere.h"
    #include "ntetrahedron.h"
    #include "ntriangulation.h"
    #include "jnitools.h"
    #include "NJNISnappedTwoSphere.h"
#else
    #include "engine/subcomplex/nsnappedtwosphere.h"
    #include "engine/triangulation/ntetrahedron.h"
    #include "engine/triangulation/ntriangulation.h"
    #include "jni/jnitools.h"
    #include "jni/engine/subcomplex/NJNISnappedTwoSphere.h"
#endif

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNISnappedTwoSphere_cloneMe
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NSnappedTwoSphere, me)->clone(),
        "normal/engine/implementation/jni/subcomplex/NJNISnappedTwoSphere");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNISnappedTwoSphere_getReducedTriangulation
        (JNIEnv *env, jobject me, jobject you) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NSnappedTwoSphere, me)->getReducedTriangulation(
        GET_ENGINE_OBJECT(env, NTriangulation, you)),
        "normal/engine/implementation/jni/triangulation/NJNITriangulation");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNISnappedTwoSphere_getSnappedBall
        (JNIEnv *env, jobject me, jint index) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NSnappedTwoSphere, me)->getSnappedBall(index),
        "normal/engine/implementation/jni/subcomplex/NJNISnappedBall");
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_subcomplex_NJNISnappedTwoSphere_reduceTriangulation
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, NSnappedTwoSphere, me)->reduceTriangulation();
}

