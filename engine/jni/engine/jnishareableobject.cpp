
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

#include "shareableobject.h"
#include "jnitools.h"
#include "engine/JNIShareableObject.h"

JNIEXPORT void JNICALL Java_normal_engine_implementation_jni_JNIShareableObject_destroy
        (JNIEnv *env, jobject me) {
    delete GET_ENGINE_OBJECT(env, ShareableObject, me);
}

JNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_JNIShareableObject_toString
        (JNIEnv *env, jobject me) {
    return jstringFromNString(env,
        GET_ENGINE_OBJECT(env, ShareableObject, me)->toString());
}

JNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_JNIShareableObject_toStringLong
        (JNIEnv *env, jobject me) {
    return jstringFromNString(env,
        GET_ENGINE_OBJECT(env, ShareableObject, me)->toStringLong());
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_JNIShareableObject_writeTextLong
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, ShareableObject, me)->writeTextLong(cout);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_JNIShareableObject_writeTextShort
        (JNIEnv *env, jobject me) {
    GET_ENGINE_OBJECT(env, ShareableObject, me)->writeTextShort(cout);
}

