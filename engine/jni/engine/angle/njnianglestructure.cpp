
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

#include "angle/nanglestructure.h"
#include "triangulation/ntriangulation.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_angle_NJNIAngleStructure_getAngleDen
        (JNIEnv *env, jobject me, jlong tet, jint edges) {
    return jBigIntegerFromLarge(env, GET_ENGINE_OBJECT(env,
        NAngleStructure, me)->getAngle(tet, edges).getDenominator());
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_angle_NJNIAngleStructure_getAngleNum
        (JNIEnv *env, jobject me, jlong tet, jint edges) {
    return jBigIntegerFromLarge(env, GET_ENGINE_OBJECT(env,
        NAngleStructure, me)->getAngle(tet, edges).getNumerator());
}

REGJNIEXPORT jobject JNICALL
        Java_normal_engine_implementation_jni_angle_NJNIAngleStructure_getTriangulation
        (JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NAngleStructure, me)->getTriangulation(),
        "normal/engine/implementation/jni/triangulation/NJNITriangulation");
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_angle_NJNIAngleStructure_isStrict
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NAngleStructure, me)->isStrict();
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_angle_NJNIAngleStructure_isTaut
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NAngleStructure, me)->isTaut();
}

