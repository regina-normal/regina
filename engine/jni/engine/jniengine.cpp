
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
#include "regina.h"

#ifdef __NO_INCLUDE_PATHS
    #include "JNIEngine.h"
    #include "jnitools.h"
    #include "makejnpacket.h"
    #include "ntriangulation.h"
    #include "nsnappea.h"
    #include "nfile.h"
    #include "ncensus.h"
	#include "nlayeredlensspace.h"
	#include "nsnappedball.h"
    #include "nnormalsurfacelist.h"
    #include "matrixops.h"
#else
    #include "jni/engine/JNIEngine.h"
    #include "jni/jnitools.h"
    #include "jni/registry/makejnpacket.h"
    #include "engine/triangulation/ntriangulation.h"
    #include "engine/imports/nsnappea.h"
    #include "engine/file/nfile.h"
    #include "engine/census/ncensus.h"
    #include "engine/subcomplex/nlayeredlensspace.h"
    #include "engine/subcomplex/nsnappedball.h"
    #include "engine/surfaces/nnormalsurfacelist.h"
    #include "engine/maths/matrixops.h"
#endif

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_enterTextTriangulation
        (JNIEnv *env, jobject) {
    return CREATE_WRAPPER_OBJECT(env,
        NTriangulation::enterTextTriangulation(cin, cout),
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
		Java_normal_engine_implementation_jni_JNIEngine_isLayeredLensSpace
		(JNIEnv *env, jobject me, jobject you) {
    return CREATE_WRAPPER_OBJECT(env,
		NLayeredLensSpace::isLayeredLensSpace(
        GET_ENGINE_OBJECT(env, NComponent, you)),
        "normal/engine/implementation/jni/subcomplex/NJNILayeredLensSpace");
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
        Java_normal_engine_implementation_jni_JNIEngine_makeMatchingEquations
        (JNIEnv *env, jobject, jobject source, jint flavour) {
    return CREATE_WRAPPER_OBJECT(env, makeMatchingEquations(
        GET_ENGINE_OBJECT(env, NTriangulation, source), flavour),
        "normal/engine/implementation/jni/triangulation/NJNIMatrixInt");
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_readFromFile
        (JNIEnv *env, jobject me, jstring file) {
    const char* textChars = env->GetStringUTFChars(file, 0);
    NPacket *ans = readFromFile(textChars);
    env->ReleaseStringUTFChars(file, textChars);
    return makeJNPacket(env, ans);
}

JNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_JNIEngine_readSnapPea
        (JNIEnv *env, jobject me, jstring file) {
    const char* textChars = env->GetStringUTFChars(file, 0);
    NTriangulation *triang = readSnapPea(textChars);
    env->ReleaseStringUTFChars(file, textChars);
    return CREATE_WRAPPER_OBJECT(env, triang,
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
        Java_normal_engine_implementation_jni_JNIEngine_writeToFile
        (JNIEnv *env, jobject me, jstring file, jobject tree) {
    const char* textChars = env->GetStringUTFChars(file, 0);
    bool ans = writeToFile(textChars, GET_ENGINE_OBJECT(env, NPacket, tree));
    env->ReleaseStringUTFChars(file, textChars);
    return ans;
}

