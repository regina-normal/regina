
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

#include "surfaces/nsurfacesubset.h"
#include "surfaces/nnormalsurface.h"
#include "triangulation/ntriangulation.h"
#include "jnitools.h"
#include "registry/javasurfacesetregistry.h"
#include "engine/surfaces/NJNISurfaceSubset.h"

#define __JAVA_SURFACE_SET_REGISTRY_BODY

JNIEXPORT jboolean JNICALL 
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceSubset_allowsAlmostNormal
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NSurfaceSubset, me)->allowsAlmostNormal();
}

JNIEXPORT jint JNICALL 
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceSubset_getFlavour
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NSurfaceSubset, me)->getFlavour();
}

JNIEXPORT jlong JNICALL 
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceSubset_getNumberOfSurfaces
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NSurfaceSubset, me)->getNumberOfSurfaces();
}

JNIEXPORT jobject JNICALL 
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceSubset_getSurface
        (JNIEnv *env, jobject me, jlong index) {
    return CREATE_WRAPPER_OBJECT(env,
        (NNormalSurface*)GET_ENGINE_OBJECT(env, NSurfaceSubset,
        me)->getSurface(index),
        "normal/engine/implementation/jni/surfaces/NJNINormalSurface");
}

JNIEXPORT jobject JNICALL 
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceSubset_getTriangulation
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env, GET_ENGINE_OBJECT(env, NSurfaceSubset,
        me)->getTriangulation(),
        "normal/engine/implementation/jni/triangulation/NJNITriangulation");
}

JNIEXPORT jboolean JNICALL 
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceSubset_isEmbeddedOnly
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NSurfaceSubset, me)->isEmbeddedOnly();
}

#define REGISTER_JAVA_SURFACE_SET(cppClass, javaInterface, javaWrapper) \
    else if (env->IsInstanceOf(set, env->FindClass(javaInterface))) \
        realSet = GET_ENGINE_OBJECT(env, cppClass, set);

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceSubset_newNSurfaceSubset
        (JNIEnv *env, jobject me, jobject set, jobject filter) {
    NSurfaceSet* realSet = 0;
    if (0)
        realSet = 0;
    #include "registry/javasurfacesetregistry.h"

    ASSIGN_ENGINE_OBJECT(env, new NSurfaceSubset(*realSet,
        *GET_ENGINE_OBJECT(env, NSurfaceFilter, filter)), me);
}

