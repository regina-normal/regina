
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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

#include "pybind11/pybind11.h"
#include "utilities/safeptr.h"

#include <typeinfo>

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

// A helper to raise a python exception indicating what type of object we tried
// to dereference.
void raiseExpiredException(const std::type_info&);

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

} // namespace boost

// Inform pybind11 that this can be used as a holder type, and that it
// is safe to construct multiple holders from the same T*.
PYBIND11_DECLARE_HOLDER_TYPE(T, regina::python::SafeHeldType<T>, true);

// Inline implementations:

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
    if (t == nullptr) {
        raiseExpiredException(typeid(T));
    }
    return t;
}

} } // namespace regina::python

#endif
