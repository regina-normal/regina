
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

#include "config.h"
#ifdef __NO_INCLUDE_PATHS
    #include "ntext.h"
    #include "jnitools.h"
    #include "NJNIText.h"
#else
    #include "engine/packet/ntext.h"
    #include "jni/jnitools.h"
    #include "jni/engine/packet/NJNIText.h"
#endif

JNIEXPORT jstring JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIText_getText
        (JNIEnv *env, jobject me) {
    return jstringFromNString(env,
        GET_ENGINE_OBJECT(env, NText, me)->getText());
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIText_newNText__
        (JNIEnv *env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NText(), me);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIText_newNText__Ljava_lang_String_2
        (JNIEnv *env, jobject me, jstring newText) {
    ASSIGN_ENGINE_OBJECT(env, new NText(jstringToNString(env, newText)), me);
}

JNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_packet_NJNIText_setText
        (JNIEnv *env, jobject me, jstring newText) {
    GET_ENGINE_OBJECT(env, NText, me)->setText(jstringToNString(env, newText));
}

