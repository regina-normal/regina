
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

#ifndef __JNITOOLS_H
#ifndef __DOXYGEN
#define __JNITOOLS_H
#endif

/*! \file jnitools.h
 *  \brief Contains routines to aid JNI interaction between Java and C++
 *  code.
 */

#include <jni.h>

#include "javacast.h"
#include "shareableobject.h"
#include "utilities/nmpi.h"

/**
 * Specifies the field of a Java wrapper object that will contain the
 * C++ pointer to the underlying object in the engine.
 */
#define CPP_PTR_FIELD "cppPtr"

/**
 * \hideinitializer
 *
 * Modifies the given Java wrapper object to wrap the given underlying
 * C++ object.
 *
 * The arguments \a java_object and \a jni_env will be evaluated
 * multiple times in this macro.
 *
 * \ifaces Not present.
 *
 * @param jni_env the working JNI environment, of type <tt>(JNIEnv*)</tt>.
 * @param cpp_object the underlying C++ object to be wrapped, of type
 * <tt>(ShareableObject*)</tt>.
 * @param java_object the Java wrapper object to be modified, of type
 * <tt>jobject</tt>.
 */
#define ASSIGN_ENGINE_OBJECT(jni_env, cpp_object, java_object) \
    ((jni_env)->SetLongField(java_object, \
    (jni_env)->GetFieldID((jni_env)->GetObjectClass(java_object), \
    CPP_PTR_FIELD, "J"), javaPtrToLong(cpp_object)))

/**
 * \hideinitializer
 *
 * Returns the underlying C++ object wrapped by the given Java wrapper
 * object.
 *
 * The arguments \a java_object and \a jni_env will be evaluated
 * multiple times in this macro.
 *
 * \ifaces Not present.
 *
 * @param jni_env the working JNI environment, of type <tt>(JNIEnv*)</tt>.
 * @param cpp_class the C++ class of the underlying C++ object.
 * @param java_object the Java wrapper to be examined, of type
 * <tt>jobject</tt>.  This may be \c null.
 * @return the C++ object wrapped by the given Java wrapper, of type
 * <tt>(<i>cpp_class</i>*)</tt>.
 */
#define GET_ENGINE_OBJECT(jni_env, cpp_class, java_object) \
    ((java_object) == 0 ? ((cpp_class*)0) : \
    ((cpp_class*)javaLongToPtr((jni_env)->GetLongField(java_object, \
    (jni_env)->GetFieldID((jni_env)->GetObjectClass(java_object), \
    CPP_PTR_FIELD, "J")))))

/**
 * \hideinitializer
 *
 * Returns the Java class of the given Java object.
 *
 * \ifaces Not present.
 *
 * @param jni_env the working JNI environment, of type <tt>(JNIEnv*)</tt>.
 * @param java_object the Java object to be examined, of type <tt>jobject</tt>.
 * @return the Java class of the given object, of type <tt>jclass</tt>.
 */
#define GET_JAVA_CLASS(jni_env, java_object) \
    ((jni_env)->GetObjectClass(java_object))

/**
 * \hideinitializer
 *
 * Retrieves the value of a static int belonging to the given Java
 * class.
 *
 * The arguments \a java_class and \a jni_env will be evaluated
 * multiple times in this macro.
 *
 * \ifaces Not present.
 *
 * @param jni_env the working JNI environment, of type <tt>(JNIEnv*)</tt>.
 * @param java_class the Java class to be examined, of type <tt>jclass</tt>.
 * @param field_name the name of the static int field to be retrieved,
 * of type <tt>(char*)</tt>.
 * @return the value of the requested static int field, of type <tt>jint</tt>.
 */
#define GET_STATIC_INT(jni_env, java_class, field_name) \
    ((jni_env)->GetStaticIntField(java_class, \
    (jni_env)->GetStaticFieldID(java_class, field_name, "I")))

/**
 * Creates a new Java wrapper object that wraps the given C++ object in
 * the underlying engine.
 *
 * \ifaces Not present.
 *
 * @param jni_env the working JNI environment.
 * @param cpp_object the C++ object in the underlying engine that is to
 * be wrapped.
 * @param java_class a string representation of the Java class of the
 * new wrapper object to be created.  This should be a subclass of
 * <tt>normal.engine.implementation.jni.JNIShareableObject</tt>.
 * The string representation of the class should use forward slashes
 * instead of dots after package names.
 * @return the newly created Java wrapper object.
 */
jobject CREATE_WRAPPER_OBJECT(JNIEnv* jni_env, ShareableObject* cpp_object,
    const char* java_class);

/**
 * Converts the given Java string to a C++ string.
 *
 * \ifaces Not present.
 *
 * @param jni_env the working JNI environment.
 * @param str the Java string to convert.
 * @return the corresponding C++ string.
 */
NString jstringToNString(JNIEnv* jni_env, jstring str);
/**
 * Converts the given C++ string to a Java string.
 *
 * \ifaces Not present.
 *
 * @param jni_env the working JNI environment.
 * @param str the C++ string to convert.
 * @return the corresponding Java string.
 */
jstring jstringFromNString(JNIEnv* jni_env, const NString& str);

/**
 * Converts the given Java BigInteger to a C++ NLargeInteger.
 *
 * \ifaces Not present.
 *
 * @param jni_env the working JNI environment.
 * @param value the Java BigInteger to convert; this must be of type
 * <tt>java.math.BigInteger</tt>.
 * @return the corresponding C++ NLargeInteger.
 */
NLargeInteger jBigIntegerToLarge(JNIEnv* jni_env, jobject value);
/**
 * Converts the given C++ NLargeInteger to a Java BigInteger.
 * If the C++ large integer is infinite, the Java object returned will
 * be \c null.
 *
 * \ifaces Not present.
 *
 * @param jni_env the working JNI environment.
 * @param value the C++ NLargeInteger to convert.
 * @return the corresponding Java BigInteger; this will be of type
 * <tt>java.math.BigInteger</tt>.  If \a value is infinite, \c null will
 * be returned.
 */
jobject jBigIntegerFromLarge(JNIEnv* jni_env, const NLargeInteger& value);
        
#endif

