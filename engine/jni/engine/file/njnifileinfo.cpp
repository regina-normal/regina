
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

#include "file/nfileinfo.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFileInfo_getPathname
        (JNIEnv *env, jobject me) {
    return jstringFromCString(env, GET_ENGINE_OBJECT(env, NFileInfo, me)->
        getPathname());
}

REGJNIEXPORT jint JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFileInfo_getType
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NFileInfo, me)->getType();
}

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFileInfo_getTypeDescription
        (JNIEnv *env, jobject me) {
    return jstringFromCString(env, GET_ENGINE_OBJECT(env, NFileInfo, me)->
        getTypeDescription());
}

REGJNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFileInfo_getEngine
        (JNIEnv *env, jobject me) {
    return jstringFromCString(env, GET_ENGINE_OBJECT(env, NFileInfo, me)->
        getEngine());
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFileInfo_isCompressed
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NFileInfo, me)->isCompressed();
}

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_file_NJNIFileInfo_isInvalid
        (JNIEnv *env, jobject me) {
    return GET_ENGINE_OBJECT(env, NFileInfo, me)->isInvalid();
}

