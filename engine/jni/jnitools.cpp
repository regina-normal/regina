
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

#include "jnitools.h"

namespace regina {
namespace jni {

jobject CREATE_WRAPPER_OBJECT(JNIEnv* jni_env,
        regina::ShareableObject* cpp_object, const char* java_class) {
    if (! cpp_object)
        return 0;

    // Get the new class.
    jclass newClass = jni_env->FindClass(java_class);
    if (! newClass)
        return 0;

    // Get the constructor for the new class: <init>(Sentry)
    jmethodID constructor = jni_env->GetMethodID(newClass, "<init>",
        "(Lnormal/engine/implementation/jni/Sentry;)V");
    if (! constructor)
        return 0;

    // We don't need a sentry object to pass to the constructor; null
    // will do, since Java already knows we are calling the sentry constructor.
    
    // Create and initialise the new Java object.
    jobject ans = jni_env->NewObject(newClass, constructor, 0);
    ASSIGN_ENGINE_OBJECT(jni_env, cpp_object, ans);
    return ans;
}

std::string jstringToCString(JNIEnv* jni_env, jstring str) {
    const char* strChars = jni_env->GetStringUTFChars(str, 0);
    std::string ans(strChars);
    jni_env->ReleaseStringUTFChars(str, strChars);
    return ans;
}

regina::NLargeInteger jBigIntegerToLarge(JNIEnv* jni_env, jobject value) {
    jclass c = jni_env->GetObjectClass(value);
    jmethodID m = jni_env->GetMethodID(c, "toString", "()Ljava/lang/String;");
    return jstringToCString(jni_env,
        (jstring)jni_env->CallObjectMethod(value, m)).c_str();
}

jobject jBigIntegerFromLarge(JNIEnv* jni_env,
        const regina::NLargeInteger& value) {
    if (value.isInfinite())
        return 0;

    jclass newClass =
        jni_env->FindClass("normal/engine/utilities/NLargeInteger");
    if (! newClass)
        return 0;
    
    jmethodID constructor = jni_env->GetMethodID(newClass, "<init>",
        "(Ljava/lang/String;)V");
    if (! constructor)
        return 0;

    char* decimal = value.stringValue();
    jobject ans = jni_env->NewObject(newClass, constructor,
        jni_env->NewStringUTF(decimal));
    delete[] decimal;
    return ans;
}

} } // namespace regina::jni

