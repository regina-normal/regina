
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

#include "regina.h"

#include "engine/JNIEngine.h"
#include "jnitools.h"
#include "registry/makejnpacket.h"
#include "triangulation/ntriangulation.h"
#include "foreign/nsnappea.h"
#include "file/nfile.h"
#include "file/nfileinfo.h"
#include "file/nxmlfile.h"
#include "census/ncensus.h"
#include "subcomplex/naugtrisolidtorus.h"
#include "subcomplex/nlayeredlensspace.h"
#include "subcomplex/nlayeredloop.h"
#include "subcomplex/npillowtwosphere.h"
#include "subcomplex/nsnappedtwosphere.h"
#include "subcomplex/nspiralsolidtorus.h"
#include "subcomplex/ntrisolidtorus.h"
#include "surfaces/nnormalsurfacelist.h"
#include "maths/matrixops.h"

using namespace regina;

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_enterTextTriangulation
        (JNIEnv *env, jobject) {
    return CREATE_WRAPPER_OBJECT(env,
        NTriangulation::enterTextTriangulation(std::cin, std::cout),
        "normal/engine/implementation/jni/triangulation/NJNITriangulation");
}

JNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_JNIEngine__1formCensus
        (JNIEnv *env, jobject, jobject parent, jint nTets,
        jchar flag1, jchar flag2, jchar flag3, jint nBdryFaces,
        jobject manager) {
    return formCensus(GET_ENGINE_OBJECT(env, NPacket, parent), nTets,
        NBoolSet::fromByteCode(flag1),
        NBoolSet::fromByteCode(flag2),
        NBoolSet::fromByteCode(flag3),
        nBdryFaces, GET_ENGINE_OBJECT(env, NProgressManager, manager));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_formsPillowTwoSphere
        (JNIEnv *env, jobject me, jobject f1, jobject f2) {
    return CREATE_WRAPPER_OBJECT(env,
        NPillowTwoSphere::formsPillowTwoSphere(
            GET_ENGINE_OBJECT(env, NFace, f1),
            GET_ENGINE_OBJECT(env, NFace, f2)),
        "normal/engine/implementation/jni/subcomplex/NJNIPillowTwoSphere");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_formsSnappedTwoSphere__Lnormal_engine_subcomplex_NSnappedBall_2Lnormal_engine_subcomplex_NSnappedBall_2
        (JNIEnv *env, jobject me, jobject p1, jobject p2) {
    return CREATE_WRAPPER_OBJECT(env,
        NSnappedTwoSphere::formsSnappedTwoSphere(
            GET_ENGINE_OBJECT(env, NSnappedBall, p1),
            GET_ENGINE_OBJECT(env, NSnappedBall, p2)),
        "normal/engine/implementation/jni/subcomplex/NJNISnappedTwoSphere");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_formsSnappedTwoSphere__Lnormal_engine_triangulation_NTetrahedron_2Lnormal_engine_triangulation_NTetrahedron_2
        (JNIEnv *env, jobject me, jobject p1, jobject p2) {
    return CREATE_WRAPPER_OBJECT(env,
        NSnappedTwoSphere::formsSnappedTwoSphere(
            GET_ENGINE_OBJECT(env, NTetrahedron, p1),
            GET_ENGINE_OBJECT(env, NTetrahedron, p2)),
        "normal/engine/implementation/jni/subcomplex/NJNISnappedTwoSphere");
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_getVersionMajor
        (JNIEnv *env, jobject me) {
    return ENGINE_VERSION_MAJOR;
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_getVersionMinor
        (JNIEnv *env, jobject me) {
    return ENGINE_VERSION_MINOR;
}

JNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_getVersionString
        (JNIEnv *env, jobject me) {
    return env->NewStringUTF(ENGINE_VERSION);
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_identifyFileInfo
        (JNIEnv *env, jobject me, jstring pathname) {
    return CREATE_WRAPPER_OBJECT(env,
        NFileInfo::identify(jstringToCString(env, pathname)),
        "normal/engine/implementation/jni/file/NJNIFileInfo");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_isAugTriSolidTorus
        (JNIEnv *env, jobject me, jobject you) {
    return CREATE_WRAPPER_OBJECT(env,
        NAugTriSolidTorus::isAugTriSolidTorus(
        GET_ENGINE_OBJECT(env, NComponent, you)),
        "normal/engine/implementation/jni/subcomplex/NJNIAugTriSolidTorus");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_isLayeredLensSpace
        (JNIEnv *env, jobject me, jobject you) {
    return CREATE_WRAPPER_OBJECT(env,
        NLayeredLensSpace::isLayeredLensSpace(
        GET_ENGINE_OBJECT(env, NComponent, you)),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredLensSpace");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_isLayeredLoop
        (JNIEnv *env, jobject me, jobject you) {
    return CREATE_WRAPPER_OBJECT(env,
        NLayeredLoop::isLayeredLoop(
        GET_ENGINE_OBJECT(env, NComponent, you)),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredLoop");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_isLayeredSolidTorusBase
        (JNIEnv* env, jobject me, jobject you) {
    return CREATE_WRAPPER_OBJECT(env,
        NLayeredSolidTorus::isLayeredSolidTorusBase(
        GET_ENGINE_OBJECT(env, NTetrahedron, you)),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredSolidTorus");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_isSnappedBall
        (JNIEnv* env, jobject me, jobject you) {
    return CREATE_WRAPPER_OBJECT(env,
        NSnappedBall::isSnappedBall(GET_ENGINE_OBJECT(env, NTetrahedron, you)),
        "normal/engine/implementation/jni/subcomplex/NJNISnappedBall");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine__1isSpiralSolidTorus
        (JNIEnv* env, jobject me, jobject tet, jchar perm) {
    return CREATE_WRAPPER_OBJECT(env,
        NSpiralSolidTorus::isSpiralSolidTorus(GET_ENGINE_OBJECT(
        env, NTetrahedron, tet), NPerm(perm)),
        "normal/engine/implementation/jni/subcomplex/NJNISpiralSolidTorus");
}


JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine__1isTriSolidTorus
        (JNIEnv* env, jobject me, jobject tet, jchar perm) {
    return CREATE_WRAPPER_OBJECT(env,
        NTriSolidTorus::isTriSolidTorus(GET_ENGINE_OBJECT(
        env, NTetrahedron, tet), NPerm(perm)),
        "normal/engine/implementation/jni/subcomplex/NJNITriSolidTorus");
}


JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_makeMatchingEquations
        (JNIEnv *env, jobject, jobject source, jint flavour) {
    return CREATE_WRAPPER_OBJECT(env, makeMatchingEquations(
        GET_ENGINE_OBJECT(env, NTriangulation, source), flavour),
        "normal/engine/implementation/jni/triangulation/NJNIMatrixInt");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_readFileMagic
        (JNIEnv *env, jobject me, jstring file) {
    return makeJNPacket(env, readFileMagic(jstringToCString(env, file)));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_readFromFile
        (JNIEnv *env, jobject me, jstring file) {
    return makeJNPacket(env, readFromFile(jstringToCString(env, file).c_str()));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_readXMLFile
        (JNIEnv *env, jobject me, jstring file) {
    return makeJNPacket(env, readXMLFile(jstringToCString(env, file).c_str()));
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_readSnapPea
        (JNIEnv *env, jobject me, jstring file) {
    return CREATE_WRAPPER_OBJECT(env,
        readSnapPea(jstringToCString(env, file).c_str()),
        "normal/engine/implementation/jni/triangulation/NJNITriangulation");
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_smithNormalForm
        (JNIEnv *env, jobject, jobject matrix) {
    smithNormalForm(*GET_ENGINE_OBJECT(env, NMatrixInt, matrix));
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_testEngine
        (JNIEnv*, jobject, jint value) {
    return value;
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_writeSnapPea
        (JNIEnv *env, jobject me, jstring file, jobject tri) {
    const char* textChars = env->GetStringUTFChars(file, 0);
    bool ans = writeSnapPea(textChars,
        *GET_ENGINE_OBJECT(env, NTriangulation, tri));
    env->ReleaseStringUTFChars(file, textChars);
    return ans;
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_writeToFile
        (JNIEnv *env, jobject me, jstring file, jobject tree) {
    const char* textChars = env->GetStringUTFChars(file, 0);
    bool ans = writeToFile(textChars, GET_ENGINE_OBJECT(env, NPacket, tree));
    env->ReleaseStringUTFChars(file, textChars);
    return ans;
}

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_writeXMLFile__Ljava_lang_String_2Lnormal_engine_packet_NPacket_2Z
        (JNIEnv *env, jobject me, jstring file, jobject tree, jboolean comp) {
    const char* textChars = env->GetStringUTFChars(file, 0);
    bool ans = writeXMLFile(textChars, GET_ENGINE_OBJECT(env, NPacket, tree),
        comp);
    env->ReleaseStringUTFChars(file, textChars);
    return ans;
}
