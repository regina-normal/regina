
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

#include "triangulation/nvertex.h"
#include "triangulation/ncomponent.h"
#include "triangulation/nboundarycomponent.h"
#include "triangulation/ntetrahedron.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_getBoundaryComponent
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NVertex, me)->getBoundaryComponent(),
        "normal/engine/implementation/jni/triangulation/NJNIBoundaryComponent");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_getComponent
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NVertex, me)->getComponent(),
        "normal/engine/implementation/jni/triangulation/NJNIComponent");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_getEmbedding
        (JNIEnv *env, jobject me, jlong index) {
    NVertex* vertex = GET_ENGINE_OBJECT(env, NVertex, me);
    const NVertexEmbedding& embedding = vertex->getEmbedding(index);
    jobject tet = CREATE_WRAPPER_OBJECT(env, embedding.getTetrahedron(),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");

    jclass embClass =
        env->FindClass("normal/engine/triangulation/NVertexEmbedding");
    if (! embClass)
        return 0;
    jmethodID constructor = env->GetMethodID(embClass, "<init>",
        "(Lnormal/engine/triangulation/NTetrahedron;I)V");
    if (! constructor)
        return 0;
    return env->NewObject(embClass, constructor, tet, embedding.getVertex());
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_getLink
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->getLink();
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_getLinkEulerCharacteristic
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->getLinkEulerCharacteristic();
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_getNumberOfEmbeddings
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->getNumberOfEmbeddings();
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_isBoundary
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->isBoundary();
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_isIdeal
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->isIdeal();
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_isLinkClosed
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->isLinkClosed();
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_isLinkOrientable
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->isLinkOrientable();
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_triangulation_NJNIVertex_isStandard
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NVertex, me)->isStandard();
}

