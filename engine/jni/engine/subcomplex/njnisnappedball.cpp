
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
    #include "nsnappedball.h"
    #include "ntetrahedron.h"
    #include "jnitools.h"
    #include "NJNISnappedBall.h"
#else
    #include "engine/subcomplex/nsnappedball.h"
    #include "engine/triangulation/ntetrahedron.h"
    #include "jni/jnitools.h"
    #include "jni/engine/subcomplex/NJNISnappedBall.h"
#endif

JNIEXPORT jint JNICALL
		Java_normal_engine_implementation_jni_subcomplex_NJNISnappedBall_getBoundaryFace
		(JNIEnv *env, jobject me, jint index) {
	return GET_ENGINE_OBJECT(env, NSnappedBall, me)->getBoundaryFace(index);
}

JNIEXPORT jint JNICALL
		Java_normal_engine_implementation_jni_subcomplex_NJNISnappedBall_getEquatorEdge
		(JNIEnv *env, jobject me) {
	return GET_ENGINE_OBJECT(env, NSnappedBall, me)->getEquatorEdge();
}

JNIEXPORT jint JNICALL
		Java_normal_engine_implementation_jni_subcomplex_NJNISnappedBall_getInternalEdge
		(JNIEnv *env, jobject me) {
	return GET_ENGINE_OBJECT(env, NSnappedBall, me)->getInternalEdge();
}

JNIEXPORT jint JNICALL
		Java_normal_engine_implementation_jni_subcomplex_NJNISnappedBall_getInternalFace
		(JNIEnv *env, jobject me, jint index) {
	return GET_ENGINE_OBJECT(env, NSnappedBall, me)->getInternalFace(index);
}

JNIEXPORT jobject JNICALL
		Java_normal_engine_implementation_jni_subcomplex_NJNISnappedBall_getTetrahedron
		(JNIEnv *env, jobject me) {
    return CREATE_WRAPPER_OBJECT(env,
        GET_ENGINE_OBJECT(env, NSnappedBall, me)->getTetrahedron(),
        "normal/engine/implementation/jni/triangulation/NJNITetrahedron");
}

