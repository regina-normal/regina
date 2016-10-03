
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file utilities/saferemnant.h
 *  \brief Implementation details for safe pointers.
 */

#ifndef __SAFEREMNANT_H
#ifndef __DOXYGEN
#define __SAFEREMNANT_H
#endif

// Boost versions prior to 1.55 do not provide intrusive_ref_counter.
#include <boost/version.hpp>
static_assert(BOOST_VERSION >= 105500,
    "Regina now requires boost version 1.55 or later.");

#include <boost/smart_ptr/intrusive_ref_counter.hpp>

namespace regina {
namespace detail {

/**
 * \weakgroup utilities
 * @{
 */

/**
 * A helper class for SafePtr.
 *
 * The dereferencing of a SafePtr is indirected through this class,
 * i.e., SafePtr is pointing to this class, which is pointing to the pointee.
 * The pointee in turn derives from SafePointeeBase.
 *
 * Each remnant is associated with at most one pointee, and each pointee
 * is associated with at most one remnant.  However, both relationships
 * are optional:
 *
 * - a remnant will have no corresponding pointee if its original pointee
 *   has since been destroyed;
 *
 * - a pointee will have no corresponding remnant if there are no SafePtr
 *   pointers that point to it.
 *
 * The lifetime of a remnant is tied to the existence of SafePtr objects
 * that point to it (through reference counting), not to that of the
 * corresponding pointee.
 *
 * \tparam T the pointee type.  This must derive from SafePointeeBase<T>.
 *
 * @author Matthias Goerner
 */
template <class T>
class SafeRemnant : public boost::intrusive_ref_counter<SafeRemnant<T>> {

public:
    /**
     * Given an object deriving from SafePointeeBase, find or create the
     * remnant corresponding to the object.
     *
     * Each pointee \p object has at most one remnant. If it already exists,
     * this routine returns it.  If not, this routine creates a new one.
     *
     * @param object the object deriving from SafePointeeBase.
     * @return the corresponding remnant.
     */
    static SafeRemnant<T>* getOrCreate(T* object);
    /**
     * Destroys this remnant.
     * If the remnant is associated with a pointee, then:
     *
     * - if no other C++ object claims ownership of the pointee
     *   (as determined by T::hasOwner()), then the pointee will be deleted;
     *
     * - if some other C++ object does claim ownership of the pointee
     *   then the pointee will be left intact, but its reference to this
     *   remnant will be cleared.
     */
    ~SafeRemnant();
    /**
     * Dereference this remnant.
     *
     * @return the pointee corresponding to this remnant, or 0 if the
     * original pointee has since been destroyed.
     */
    T* get() const;
    /**
     * Expire the remnant so that it can no longer be dereferenced.
     * Any subsequent call to get() will return 0.
     *
     * This routine is called by the pointee's destructor.
     */
    void expire();

private:
    /**
     * Associates this remnant with the given pointee.
     * This is so that the pointee can expire this remnant upon its
     * destruction.
     *
     * @param object the pointee associated with this remnant.
     */
    SafeRemnant(T* object);

    T* object_;
        /**< The pointee. */
};

/*@}*/

// Implementation details for SafePointeeBase

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
    if (object and object->remnant_) {
        return object->remnant_;
    }
    return new SafeRemnant<T>(object);
}

template <class T>
SafeRemnant<T>::SafeRemnant(T* object) : object_(object) {
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

} } // namespace regina::detail

#endif
