
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

/*! \file utilities/safeptr.h
 *  \brief Provides a smart pointer safe in that it cannot be dangling.
 */

#ifndef __SAFEPTR_H
#ifndef __DOXYGEN
#define __SAFEPTR_H
#endif

#include <boost/intrusive_ptr.hpp>

namespace regina {

template <class> class SafePointeeBase;
template <class> class SafeRemnant;

// Base class for \c SafePtr. Users should always use \c SafePtr.
template <class T>
class SafePtrBase {
public:
    ~SafePtrBase();

private:
    SafePtrBase();

protected:
    // Create a \c SafePtr with given pointee \p object.
    SafePtrBase(T* object);
    SafePtrBase(const SafePtrBase &);
    // Get the pointee.
    T* getBase_() const;
private:
    boost::intrusive_ptr<SafeRemnant<typename T::SafePointeeType>> remnant_;
    /**< The remnant that points to the pointee. */
};

/**
 * A reference counted smart pointer that allows its pointee to indicate that it
 * cannot be destroyed because some other C++ class not using this smart pointer
 * owns it:
 *
 * - A pointee must subclass from \c SafePointeeBase and implement hasOwner
 *   to indicate whether it is not safe to destroy it because some other
 *   object is having a non-SafePtr pointing to the pointee.
 * - If a pointee's destructor is called, all \c SafePtr pointing to it
 *   will be expired and become non-dereferencable (get() returning 0).
 * - \c SafePtr will destroy a pointee if the last \c SafePtr pointing
 *   to the pointee goes out of scope and if the pointee's hasOwner
 *   returns false.
 *
 * Under the hood, \c SafePtr is using \c SafeRemnant to achieve this.
 */

template<class T>
class SafePtr : protected SafePtrBase<T>
{
public:
    // For the boost infrastructure.
    typedef T element_type;

    /**
     * Constructs a \c SafePtr pointing at the given pointee \p object.
     */
    SafePtr(T* object);
    /**
     * \c SafePtr's can be cast to \c SafePtr's for a base class.
     */
    template<class Y> SafePtr(const SafePtr<Y> &);
    /**
     * Return a raw pointer to the pointee, zero if pointee was destroyed.
     */
    T* get() const;
};

} // namespace regina

namespace boost {
// Dereferencable concept for \c SafePtr's.
template<class T> T* get_pointer(
        regina::SafePtr<T> const& ptr) {
    return ptr.get();
}
} // namespace boost

namespace regina {

template <class T>
SafePtrBase<T>::SafePtrBase(const SafePtrBase& other)
    : remnant_(other.remnant_) { }

template <class T>
SafePtrBase<T>::SafePtrBase(T* object) {
    if (object) {
        remnant_.reset(SafeRemnant<typename T::SafePointeeType>::getOrCreate(object));
    }
}
    
template <class T>
SafePtrBase<T>::~SafePtrBase() { }

template <class T>
T* SafePtrBase<T>::getBase_() const {
    if (not remnant_) {
        return 0;
    }
    return static_cast<T*>(remnant_->get());
}

template<class T> SafePtr<T>::SafePtr(T* object)
    : SafePtrBase<T>(object) { }

// This template can only be instantiated if T (whose constructor is called)
// is a base class of Y (returned by other.get()).
template<class T> template<class Y>
SafePtr<T>::SafePtr(const SafePtr<Y> &other)
    : SafePtr(other.get()) { }

// By virtue of how \c SafePtr's are constructed, getBase_() always holds
// a pointer to T or a dervied class of T.
template<class T> T* SafePtr<T>::get() const {
    return static_cast<T*>(SafePtrBase<T>::getBase_());
}

} // namespace regina

#endif
