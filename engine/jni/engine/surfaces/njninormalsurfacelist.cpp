
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

#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"
#include "maths/nmatrixint.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_allowsAlmostNormal
        (JNIEnv* env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurfaceList, me)->
        allowsAlmostNormal();
}


REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_getFlavour
        (JNIEnv* env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurfaceList, me)->
        getFlavour();
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_getNumberOfSurfaces
        (JNIEnv* env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurfaceList, me)->
        getNumberOfSurfaces();
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_getSurface
        (JNIEnv* env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        (NNormalSurface*)GET_ENGINE_OBJECT(env, NNormalSurfaceList,
        me)->getSurface(index),
        "normal/engine/implementation/jni/surfaces/NJNINormalSurface");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_getTriangulation
        (JNIEnv* env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env, GET_ENGINE_OBJECT(env, NNormalSurfaceList,
        me)->getTriangulation(),
        "normal/engine/implementation/jni/triangulation/NJNITriangulation");
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_isEmbeddedOnly
        (JNIEnv* env, jobject me) {
    return GET_ENGINE_OBJECT(env, NNormalSurfaceList, me)->
        isEmbeddedOnly();
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_newNNormalSurfaceList
        (JNIEnv* env, jobject me, jobject triangulation, jint flavour,
        jboolean embedded) {
    ASSIGN_ENGINE_OBJECT(env, new NNormalSurfaceList(
        GET_ENGINE_OBJECT(env, NTriangulation, triangulation), flavour,
        embedded), me);
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNINormalSurfaceList_recreateMatchingEquations
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env, GET_ENGINE_OBJECT(env, NNormalSurfaceList,
        me)->recreateMatchingEquations(),
        "normal/engine/implementation/jni/maths/NJNIMatrixInt");
}

