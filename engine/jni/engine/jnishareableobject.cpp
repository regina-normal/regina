
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

#include "shareableobject.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_JNIShareableObject_destroy
        (JNIEnv *env, jobject me) {
    delete GET_ENGINE_OBJECT(env, ShareableObject, me);
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_JNIShareableObject_sameCppPtr
        (JNIEnv *, jclass, jlong ptr1, jlong ptr2) {
    return (javaLongToPtr(ptr1) == javaLongToPtr(ptr2));
}

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_JNIShareableObject_toString
        (JNIEnv *env, jobject me) {
    return jstringFromCString(env,
        GET_ENGINE_OBJECT(env, ShareableObject, me)->toString());
}

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_JNIShareableObject_toStringLong
        (JNIEnv *env, jobject me) {
    return jstringFromCString(env,
        GET_ENGINE_OBJECT(env, ShareableObject, me)->toStringLong());
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_JNIShareableObject_writeTextLong
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, ShareableObject, me)->writeTextLong(std::cout);
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_JNIShareableObject_writeTextShort
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, ShareableObject, me)->writeTextShort(std::cout);
}

