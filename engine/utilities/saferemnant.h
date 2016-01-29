
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2015, Ben Burton                                   *
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

/* end stub */

/*! \file utilities/saferemnant.h
 *  \brief Provides a helpers used in implemenation of safe pointers.
 */

#ifndef __SAFEREMNANT_H
#ifndef __DOXYGEN
#define __SAFEREMNANT_H
#endif

#include <boost/version.hpp>
#if BOOST_VERSION >= 105500
#define USE_BOOST_INTRUSIVE_REF_COUNTER
#endif

#ifdef USE_BOOST_INTRUSIVE_REF_COUNTER
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#else
#include "utilities/nintrusiverefcounter.h"
#endif

namespace regina {

/**
 * A helper class for \c SafePtr. The dereferencing of a \c SafePtr
 * is indirected through this class, i.e., \c SafePtr is pointing to this
 * class, which is pointing to the pointee, an object deriving from
 * \c SafePointeeBase.
 * The life time of an object of this class is tied to the existence of
 * \c SafePtr's pointing to it (through reference counting), not to that of
 * the corresponding pointee.
 */

template <class T>
class SafeRemnant :
#ifdef USE_BOOST_INTRUSIVE_REF_COUNTER
    public boost::intrusive_ref_counter<SafeRemnant<T>> {
#else
    public regina::temporary::NIntrusiveRefCounter<SafeRemnant<T>> {
#endif

public:
    /**
     * Given an \p object deriving from \c SafePointeeBase, find or create the
     * remnant corresponding the object.
     */
    static SafeRemnant<T>* getOrCreate(T* object);
    ~SafeRemnant();
    /**
     * Dereference, i.e., find the pointee corresponding to the remnant.
     */
    T* get() const;
    /**
     * Expire the remnant so that it can no longer be dereferenced, i.e.,
     * get will return 0. Called by the pointee's destructor.
     */
    void expire();

private:
    SafeRemnant(T* object);
    T* object_;
    /**< The pointee. */
};

template <class T>
inline T* SafeRemnant<T>::get() const {
    return object_;
}

template <class T>
inline void SafeRemnant<T>::expire() {
    object_ = 0;
}

template <class T>
SafeRemnant<T>*
SafeRemnant<T>::getOrCreate(T* object) {
    // Each pointee \p object has at most one remnant. If it already exists,
    // ust it.
    if (object and object->remnant_) {
        return object->remnant_;
    }
    // Otherwise, create a new one associated with the pointee.
    return new SafeRemnant<T>(object);
}

template <class T>
SafeRemnant<T>::SafeRemnant(T* object) : object_(object) {
    // Associate this remnant with the pointee.
    // This is so that the pointee can expire this remnant upon its destruction.
    if (object) {
        object->remnant_ = this;
    }
}

template <class T>
SafeRemnant<T>::~SafeRemnant() {
    // Disassociate with pointee.
    if (object_) {
        // Pointee's back-pointer to remnant will no longer be valid, reset
        // to zero so that pointee's destructor won't call it.
        object_->remnant_ = 0;
        // If no other C++ object claims ownership, delete the pointee.
        if (not object_->hasOwner()) {
            delete object_;
        }
    }
}

} // namespace regina

#endif
