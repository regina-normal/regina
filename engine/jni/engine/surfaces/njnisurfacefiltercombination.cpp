
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

#include "surfaces/sfcombination.h"
#include "jnitools.h"

using namespace regina;
using namespace regina::jni;

REGJNIEXPORT jboolean JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterCombination_getUsesAnd
        (JNIEnv* env, jobject me) {
    return GET_ENGINE_OBJECT(env, NSurfaceFilterCombination, me)->getUsesAnd();
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterCombination_newNSurfaceFilterCombination__
        (JNIEnv *env, jobject me) {
    ASSIGN_ENGINE_OBJECT(env, new NSurfaceFilterCombination(), me);
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterCombination_newNSurfaceFilterCombination__Lnormal_engine_surfaces_NSurfaceFilterCombination_2
        (JNIEnv *env, jobject me, jobject you) {
    ASSIGN_ENGINE_OBJECT(env, new NSurfaceFilterCombination(
        *GET_ENGINE_OBJECT(env, NSurfaceFilterCombination, you)), me);
}

REGJNIEXPORT void JNICALL
        Java_normal_engine_implementation_jni_surfaces_NJNISurfaceFilterCombination_setUsesAnd
        (JNIEnv *env, jobject me, jboolean value) {
    GET_ENGINE_OBJECT(env, NSurfaceFilterCombination, me)->setUsesAnd(value);
}

