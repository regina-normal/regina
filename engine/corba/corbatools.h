
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

#ifndef __CORBATOOLS_H
#ifndef __DOXYGEN
#define __CORBATOOLS_H
#endif

/*! \file corbatools.h
 *  \brief Contains routines to aid interaction between CORBA objects
 *  and native engine objects.
 */

#include "config.h"
#ifdef __NO_INCLUDE_PATHS
    #include "corbacast.h"
    #include "shareableobject.h"
#else
    #include "corba/corbacast.h"
    #include "engine/shareableobject.h"
#endif

/**
 * \hideinitializer
 *
 * Returns the underlying C++ object wrapped by the given CORBA wrapper
 * object.
 *
 * \ifaces Not present.
 *
 * @param cpp_class the C++ class of the underlying C++ object.
 * @param corba_object the CORBA wrapper to be examined, of type
 * <tt>Regina::ShareableObject_ptr</tt>.
 * @return the C++ object wrapped by the given CORBA wrapper, of type
 * <tt>(<i>cpp_class</i>*)</tt>.
 */
#define GET_ENGINE_OBJECT(cpp_class, corba_object) \
    ((::cpp_class*)CORBALongToPtr((corba_object)->getCppPtr()))

/**
 * \hideinitializer
 */
#define STANDARD_ENGINE_TYPEDEFS(servant_class, engine_class, interface_class) \
	public: \
		typedef servant_class ServantClass; \
		typedef ::engine_class EngineClass; \
		typedef ::interface_class InterfaceClass;

/**
 * \hideinitializer
 *
 * Declares and implements public static routine <tt>newWrapper()</tt> as
 * required by every CORBA server wrapper class (see the HOWTO for
 * \ref howToNewClass "adding a new class" for further details).
 * This macro should be
 * placed in the public section of the CORBA server wrapper class
 * declaration.  It will simultaneously declare the new routine and
 * implement it inline.
 *
 * The implementation of <tt>newWrapper()</tt> that this macro provides
 * will return <tt>_nil()</tt> if <tt>null</tt> is passed and will
 * otherwise create a new wrapper of class \a wrapper_class and return
 * <tt>wrapper->_this()</tt>.
 *
 * This macro should not be followed by a semicolon.
 *
 * @param engine_class the underlying engine class that we are wrapping
 * (for instance, NFile).
 * @param wrapper_class the corresponding CORBA server wrapper class
 * (for instance, <tt>NFile_i</tt>).
 * @param ptr_class the corresponding CORBA wrapper pointer type (for
 * instance, <tt>Regina::File::NFile_ptr</tt>).
 */
#define STANDARD_NEW_WRAPPER \
    static InterfaceClass::_ptr_type newWrapper(EngineClass* newCppPtr) { \
        if (newCppPtr) \
            return (new ServantClass(newCppPtr))->_this(); \
        else \
            return InterfaceClass::_nil(); \
    }
        
/**
 * Converts the given string to a C++ NLargeInteger.
 *
 * \ifaces Not present.
 *
 * @param value the string to convert.
 * @return the corresponding C++ NLargeInteger.
 */
#ifdef __DOXYGEN
    NLargeInteger stringToLarge(const char* value);
#else
    #define stringToLarge(value) (::NLargeInteger(value))
#endif
/**
 * Converts the given C++ NLargeInteger to a string.
 * The string returned will be as described by NLargeInteger::stringValue().
 *
 * \ifaces Not present.
 *
 * @param value the C++ NLargeInteger to convert.
 * @return the corresponding newly allocated string.  The deallocation
 * of this string is the responsibility of the caller of this routine.
 */
#ifdef __DOXYGEN
    char* stringFromLarge(const NLargeInteger& value);
#else
    #define stringFromLarge(value) ((value).stringValue())
#endif
        
#endif

