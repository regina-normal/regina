
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

#include "jnitools.h"
#include "registry/makejnpacket.h"
#include "triangulation/ntriangulation.h"
#include "foreign/nsnappea.h"
#include "file/nfile.h"
#include "file/nfileinfo.h"
#include "file/nxmlfile.h"
#include "census/ncensus.h"
#include "split/nsignature.h"
#include "subcomplex/naugtrisolidtorus.h"
#include "subcomplex/nlayeredchainpair.h"
#include "subcomplex/nlayeredlensspace.h"
#include "subcomplex/nlayeredloop.h"
#include "subcomplex/npillowtwosphere.h"
#include "subcomplex/nplugtrisolidtorus.h"
#include "subcomplex/nsnappedtwosphere.h"
#include "subcomplex/nspiralsolidtorus.h"
#include "subcomplex/ntrisolidtorus.h"
#include "surfaces/nnormalsurfacelist.h"
#include "maths/matrixops.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_enterTextTriangulation
        (JNIEnv *env, jobject) {
    return CREATE_WRAPPER_OBJECT(env,
        NTriangulation::enterTextTriangulation(std::cin, std::cout),
        "normal/engine/implementation/jni/triangulation/NJNITriangulation");
}

REGJNIEXPORT jlong JNICALL
        Java_normal_engine_implementation_jni_JNIEngine__1formCensus
        (JNIEnv *env, jobject, jobject parent, jint nTets,
        jchar flag1, jchar flag2, jchar flag3, jint nBdryFaces,
        jobject manager) {
    return NCensus::formCensus(GET_ENGINE_OBJECT(env, NPacket, parent), nTets,
        NBoolSet::fromByteCode(flag1),
        NBoolSet::fromByteCode(flag2),
        NBoolSet::fromByteCode(flag3),
        nBdryFaces, 0, 0, GET_ENGINE_OBJECT(env, NProgressManager, manager));
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_formsPillowTwoSphere
        (JNIEnv *env, jobject me, jobject f1, jobject f2) {
    return CREATE_WRAPPER_OBJECT(env,
        NPillowTwoSphere::formsPillowTwoSphere(
            GET_ENGINE_OBJECT(env, NFace, f1),
            GET_ENGINE_OBJECT(env, NFace, f2)),
        "normal/engine/implementation/jni/subcomplex/NJNIPillowTwoSphere");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_formsSnappedTwoSphere__Lnormal_engine_subcomplex_NSnappedBall_2Lnormal_engine_subcomplex_NSnappedBall_2
        (JNIEnv *env, jobject me, jobject p1, jobject p2) {
    return CREATE_WRAPPER_OBJECT(env,
        NSnappedTwoSphere::formsSnappedTwoSphere(
            GET_ENGINE_OBJECT(env, NSnappedBall, p1),
            GET_ENGINE_OBJECT(env, NSnappedBall, p2)),
        "normal/engine/implementation/jni/subcomplex/NJNISnappedTwoSphere");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_formsSnappedTwoSphere__Lnormal_engine_triangulation_NTetrahedron_2Lnormal_engine_triangulation_NTetrahedron_2
        (JNIEnv *env, jobject me, jobject p1, jobject p2) {
    return CREATE_WRAPPER_OBJECT(env,
        NSnappedTwoSphere::formsSnappedTwoSphere(
            GET_ENGINE_OBJECT(env, NTetrahedron, p1),
            GET_ENGINE_OBJECT(env, NTetrahedron, p2)),
        "normal/engine/implementation/jni/subcomplex/NJNISnappedTwoSphere");
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_getVersionMajor
        (JNIEnv *env, jobject me) {
    return ENGINE_VERSION_MAJOR;
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_getVersionMinor
        (JNIEnv *env, jobject me) {
    return ENGINE_VERSION_MINOR;
}

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_getVersionString
        (JNIEnv *env, jobject me) {
    return env->NewStringUTF(ENGINE_VERSION);
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_identifyFileInfo
        (JNIEnv *env, jobject me, jstring pathname) {
    return CREATE_WRAPPER_OBJECT(env,
        NFileInfo::identify(jstringToCString(env, pathname)),
        "normal/engine/implementation/jni/file/NJNIFileInfo");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_isAugTriSolidTorus
        (JNIEnv *env, jobject me, jobject you) {
    return CREATE_WRAPPER_OBJECT(env,
        NAugTriSolidTorus::isAugTriSolidTorus(
        GET_ENGINE_OBJECT(env, NComponent, you)),
        "normal/engine/implementation/jni/subcomplex/NJNIAugTriSolidTorus");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_isLayeredChainPair
        (JNIEnv *env, jobject me, jobject you) {
    return CREATE_WRAPPER_OBJECT(env,
        NLayeredChainPair::isLayeredChainPair(
        GET_ENGINE_OBJECT(env, NComponent, you)),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredChainPair");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_isLayeredLensSpace
        (JNIEnv *env, jobject me, jobject you) {
    return CREATE_WRAPPER_OBJECT(env,
        NLayeredLensSpace::isLayeredLensSpace(
        GET_ENGINE_OBJECT(env, NComponent, you)),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredLensSpace");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_isLayeredLoop
        (JNIEnv *env, jobject me, jobject you) {
    return CREATE_WRAPPER_OBJECT(env,
        NLayeredLoop::isLayeredLoop(
        GET_ENGINE_OBJECT(env, NComponent, you)),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredLoop");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_isLayeredSolidTorusBase
        (JNIEnv* env, jobject me, jobject you) {
    return CREATE_WRAPPER_OBJECT(env,
        NLayeredSolidTorus::isLayeredSolidTorusBase(
        GET_ENGINE_OBJECT(env, NTetrahedron, you)),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredSolidTorus");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_isPlugTriSolidTorus
        (JNIEnv* env, jobject me, jobject you) {
    return CREATE_WRAPPER_OBJECT(env,
        NPlugTriSolidTorus::isPlugTriSolidTorus(
        GET_ENGINE_OBJECT(env, NTriangulation, you)),
        "normal/engine/implementation/jni/subcomplex/NJNIPlugTriSolidTorus");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_isSnappedBall
        (JNIEnv* env, jobject me, jobject you) {
    return CREATE_WRAPPER_OBJECT(env,
        NSnappedBall::isSnappedBall(GET_ENGINE_OBJECT(env, NTetrahedron, you)),
        "normal/engine/implementation/jni/subcomplex/NJNISnappedBall");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine__1isSpiralSolidTorus
        (JNIEnv* env, jobject me, jobject tet, jchar perm) {
    return CREATE_WRAPPER_OBJECT(env,
        NSpiralSolidTorus::isSpiralSolidTorus(GET_ENGINE_OBJECT(
        env, NTetrahedron, tet), NPerm(perm)),
        "normal/engine/implementation/jni/subcomplex/NJNISpiralSolidTorus");
}


REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine__1isTriSolidTorus
        (JNIEnv* env, jobject me, jobject tet, jchar perm) {
    return CREATE_WRAPPER_OBJECT(env,
        NTriSolidTorus::isTriSolidTorus(GET_ENGINE_OBJECT(
        env, NTetrahedron, tet), NPerm(perm)),
        "normal/engine/implementation/jni/subcomplex/NJNITriSolidTorus");
}


REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_makeMatchingEquations
        (JNIEnv *env, jobject, jobject source, jint flavour) {
    return CREATE_WRAPPER_OBJECT(env, makeMatchingEquations(
        GET_ENGINE_OBJECT(env, NTriangulation, source), flavour),
        "normal/engine/implementation/jni/triangulation/NJNIMatrixInt");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_parseSignature
        (JNIEnv *env, jobject me, jstring sig) {
    return CREATE_WRAPPER_OBJECT(env,
        NSignature::parse(jstringToCString(env, sig)),
        "normal/engine/implementation/jni/split/NJNISignature");
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_readFileMagic
        (JNIEnv *env, jobject me, jstring file) {
    return makeJNPacket(env, readFileMagic(jstringToCString(env, file)));
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_readFromFile
        (JNIEnv *env, jobject me, jstring file) {
    return makeJNPacket(env, readFromFile(jstringToCString(env, file).c_str()));
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_readXMLFile
        (JNIEnv *env, jobject me, jstring file) {
    return makeJNPacket(env, readXMLFile(jstringToCString(env, file).c_str()));
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_readSnapPea
        (JNIEnv *env, jobject me, jstring file) {
    return CREATE_WRAPPER_OBJECT(env,
        readSnapPea(jstringToCString(env, file).c_str()),
        "normal/engine/implementation/jni/triangulation/NJNITriangulation");
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_smithNormalForm
        (JNIEnv *env, jobject, jobject matrix) {
    smithNormalForm(*GET_ENGINE_OBJECT(env, NMatrixInt, matrix));
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_testEngine
        (JNIEnv*, jobject, jint value) {
    return value;
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_writeSnapPea
        (JNIEnv *env, jobject me, jstring file, jobject tri) {
    const char* textChars = env->GetStringUTFChars(file, 0);
    bool ans = writeSnapPea(textChars,
        *GET_ENGINE_OBJECT(env, NTriangulation, tri));
    env->ReleaseStringUTFChars(file, textChars);
    return ans;
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_writeToFile
        (JNIEnv *env, jobject me, jstring file, jobject tree) {
    const char* textChars = env->GetStringUTFChars(file, 0);
    bool ans = writeToFile(textChars, GET_ENGINE_OBJECT(env, NPacket, tree));
    env->ReleaseStringUTFChars(file, textChars);
    return ans;
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_writeXMLFile
        (JNIEnv *env, jobject me, jstring file, jobject tree, jboolean comp) {
    const char* textChars = env->GetStringUTFChars(file, 0);
    bool ans = writeXMLFile(textChars, GET_ENGINE_OBJECT(env, NPacket, tree),
        comp);
    env->ReleaseStringUTFChars(file, textChars);
    return ans;
}
