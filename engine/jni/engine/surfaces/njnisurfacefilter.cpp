
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

#include "surfaces/nsurfacefilter.h"
#include "jnitools.h"
#include "engine/surfaces/NJNISurfaceFilter.h"

JNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilter_accept
        (JNIEnv *env, jobject me, jobject you) {
    return GET_ENGINE_OBJECT(env, NSurfaceFilter, me)->accept(
        *GET_ENGINE_OBJECT(env, NNormalSurface, you));
}

JNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilter_getFilterID
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NSurfaceFilter, me)->getFilterID();
}


JNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilter_getFilterName
        (JNIEnv *env, jobject me) {
    return jstringFromNString(env, GET_ENGINE_OBJECT(env, NSurfaceFilter, me)->
        getFilterName());
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilter_newNSurfaceFilter__
        (JNIEnv *env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NSurfaceFilter(), me);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilter_newNSurfaceFilter__Lnormal_engine_surfaces_NSurfaceFilter_2
        (JNIEnv *env, jobject me, jobject you) {
    ASSIGN_ENGINE_OBJECT(env, new NSurfaceFilter(
        *GET_ENGINE_OBJECT(env, NSurfaceFilter, you)), me);
}

