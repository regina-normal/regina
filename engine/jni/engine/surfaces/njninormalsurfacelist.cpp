
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
    #include "nnormalsurfacelist.h"
    #include "ntriangulation.h"
    #include "nmatrixint.h"
    #include "jnitools.h"
    #include "NJNINormalSurfaceList.h"
#else
    #include "engine/surfaces/nnormalsurfacelist.h"
    #include "engine/triangulation/ntriangulation.h"
    #include "engine/maths/nmatrixint.h"
    #include "jni/jnitools.h"
    #include "jni/engine/surfaces/NJNINormalSurfaceList.h"
#endif

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_allowsAlmostNormal
        (JNIEnv* env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurfaceList, me)->
        allowsAlmostNormal();
}


JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_getFlavour
        (JNIEnv* env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurfaceList, me)->
        getFlavour();
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_getNumberOfSurfaces
        (JNIEnv* env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurfaceList, me)->
        getNumberOfSurfaces();
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_getSurface
        (JNIEnv* env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        (NNormalSurface*)GET_ENGINE_OBJECT(env, NNormalSurfaceList,
        me)->getSurface(index),
        "normal/engine/implementation/jni/surfaces/NJNINormalSurface");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_getTriangulation
        (JNIEnv* env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env, GET_ENGINE_OBJECT(env, NNormalSurfaceList,
        me)->getTriangulation(),
        "normal/engine/implementation/jni/triangulation/NJNITriangulation");
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_isEmbeddedOnly
        (JNIEnv* env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurfaceList, me)->
        isEmbeddedOnly();
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_newNNormalSurfaceList
        (JNIEnv* env, jobject me, jobject triangulation, jint flavour,
        jboolean embedded) {
    ASSIGN_ENGINE_OBJECT(env, new NNormalSurfaceList(
        GET_ENGINE_OBJECT(env, NTriangulation, triangulation), flavour,
        embedded), me);
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_recreateMatchingEquations
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env, GET_ENGINE_OBJECT(env, NNormalSurfaceList,
        me)->recreateMatchingEquations(),
        "normal/engine/implementation/jni/maths/NJNIMatrixInt");
}

