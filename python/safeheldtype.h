
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/*! \file python/safeheldtype.h
 *  \brief Provides held type and policies for wrapping C++ classes in Python.
 */

#ifndef __SAFEHELDTYPE_H
#ifndef __DOXYGEN
#define __SAFEHELDTYPE_H
#endif

#include "utilities/safeptr.h"
#include "Python.h"

#include <typeinfo>
#include <type_traits>

// We also need to include boost/python/object.hpp, but this must come *after*
// the declaration of SafeHeldType and friends.  Otherwise boost::get_pointer()
// falls back to the implementation for SafePtr<T>, not the override for
// SafeHeldType<T>.
//
// The extra include appears a little further down in this file.

namespace boost {
namespace python {

template<class T> struct pointee;

struct default_call_policies;

} } // namespace boost::python

namespace regina {
namespace python {

/**
 * A subclass of \c SafePtr that throws a python exception when trying
 * to dereference an expired smart pointer (i.e., after some C++ class called
 * the destructor on the pointee).
 */ 
template<class T> class SafeHeldType :
    public regina::SafePtr<T> {

public:
    SafeHeldType(T*);
    template<class Y> SafeHeldType(const SafeHeldType<Y>&);
    T* get() const;
};

} } // namespace regina::python

namespace boost {

/**
 * Let boost infrastructure know that \c SafeHeldType is dereferencable.
 * Note that get() is not virtual, so we need to overwrite the earlier
 * get_pointer for the base class \c SafePtr.
 */
template<class T>
T* get_pointer(regina::python::SafeHeldType<T> const &ptr) {
    return ptr.get();
}

namespace python {

/**
 * Let boost::python infrastructure know.
 */
template<class T> struct pointee<regina::python::SafeHeldType<T> > {
    typedef T type;
};

} } // namespace boost::python

// All includes that use boost.python must appear *after* the declaration
// of get_pointer(SafeHeldType const&).  See the discussion at the top of
// this header.

#include <boost/python/object.hpp>

namespace regina {
namespace python {

/**
 * An implementation of boost::python's ResultConverter concept that takes a
 * raw pointer of an object derived from \c SafePointeeBase and casts it to the
 * HeldType before converting it to a PyObject using the Base result converter.
 *
 * \tparam HeldType e.g., a smart pointer to the class we try to wrap
 * \tparam T a raw pointer to the class we try to wrap
 * \tparam Base an existing result converter taking the HeldType
 */
template<class HeldType, typename T, class Base>
struct to_held_type_result_converter : Base {
    PyObject* operator()(const T* t) const {
        if (t == 0) {
            // If we get a null-pointer, return None
            ::boost::python::object pyNone;
            return ::boost::python::incref(pyNone.ptr());
        }
        return Base()(HeldType(const_cast<T*>(t)));
    }
};

/**
 * An implementation of boost::python's ResultConverterGenerator that uses the
 * \c to_held_type_result_converter.
 *
 * \tparam U the class we try to wrap
 * \tparam HeldType e.g., a smart pointer to that class
 * \tparam Base a call policy. This used to get a result converter which is
 * used by to_held_type_result_converter:
 * to_held_type_result_converter first converts a raw pointer to the
 * HeldType and then applies the Base's result converter to obtain a
 * PyObject.
 */
template<template<class U> class HeldType = regina::python::SafeHeldType,
         class Base = boost::python::default_call_policies>
struct to_held_type {
    // Result converter generator from Base
    typedef typename Base::result_converter base_generator;
    template<class T> struct apply {
        // - T is a raw pointer to the class we try to wrap.

        // The potentially const type of the class we try to wrap.
        typedef typename boost::python::pointee<T>::type pointee_type;
        // The non-const type of the class we trt to wrap.
        typedef typename std::remove_const<pointee_type>::type
            non_const_pointee_type;
        // The type for holding that class
        typedef HeldType<non_const_pointee_type> HeldTypeT;
        // The result converter from the Base call policy.
        typedef typename base_generator::template apply<HeldTypeT>::type
            base_converter;
        // And finally, our result converter.
        typedef to_held_type_result_converter<
                              HeldTypeT, non_const_pointee_type, base_converter>
            type;
    };
};

// A helper to raise a python exception indicating what type of object we tried
// to dereference.
void raiseExpiredException(const std::type_info&);

} } // namespace regina::python

namespace regina {
namespace python {

template<class T>
inline SafeHeldType<T>::SafeHeldType(T* t) : regina::SafePtr<T>(t) {
}

template<class T>
template<class Y>
inline SafeHeldType<T>::SafeHeldType(const SafeHeldType<Y> &other)
    : regina::SafePtr<T>(other) {
}

template<class T>
inline T* SafeHeldType<T>::get() const {
    T* t = regina::SafePtr<T>::get();
    if (t == 0) {
        raiseExpiredException(typeid(T));
    }
    return t;
}

} } // namespace regina::python

#endif
