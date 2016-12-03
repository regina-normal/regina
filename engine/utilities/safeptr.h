
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

/*! \file utilities/safeptr.h
 *  \brief Provides a smart pointer that is safe in the sense that it
 *  cannot be dangling.
 */

#ifndef __SAFEPTR_H
#ifndef __DOXYGEN
#define __SAFEPTR_H
#endif

#include <boost/intrusive_ptr.hpp>

/**
 * \weakgroup utilities
 * @{
 */

namespace regina {

template <class> class SafePointeeBase;

namespace detail {
    template <class> class SafeRemnant;
}

/**
 * A reference counted smart pointer that supports alternate models of
 * ownership.  Specifically, there are two models of ownership for the
 * pointee (of type \a T):
 *
 * 1. The pointee may be owned by the smart pointer(s), in which
 *    case it will be destroyed when the last smart pointer to it is destroyed.
 * 2. Alternatively, the pointee may be owned by some other C++ object not
 *    using this smart pointer class.  In this case, even when the last smart
 *    point to it is destroyed, the pointee itself will not be destroyed.
 *
 * The pointee can indicate at runtime which model of ownership is in effect,
 * through the return value of the function T::hasOwner().
 *
 * The requirements for the pointee type \a T are as follows:
 *
 * - \a T must subclass from \c SafePointeeBase<T>.
 * - \a T must implement a member function <tt>bool hasOwner()</tt>,
 *   which returns \c true if and only if some other C++ object (which is
 *   not a SafePtr) owns the pointee.
 *
 * Destruction works as follows:
 *
 * - If a pointee's destructor is called, all SafePtr pointers that point to it
 *   will become expired.  These SafePtr pointers are still safe to use, but
 *   get() will return \c null.
 * - If a pointee has one or more SafePtr pointers to it, then when the last
 *   of these safe pointers is destroyed, the pointee itself will be destroyed
 *   if and only if the pointee's hasOwner() returns \c false.
 *
 * Under the hood, SafePtr uses SafeRemnant to achieve this.
 *
 * @author Matthias Goerner
 */
template<class T>
class SafePtr {
public:
    /**
     * The pointee type.  This typedef is used by the boost infrastructure.
     */
    typedef T element_type;

    /**
     * Constructs a new null pointer.
     */
    SafePtr();
    /**
     * Constructs a new safe pointer that points to the given object.
     *
     * @param object the pointee.  This may be \c null.
     */
    SafePtr(T* object);
    /**
     * Copy constructor.  This constructor can also be used to cast a
     * SafePtr for a derived class \a Y to a SafePtr for a base class \a T.
     *
     * \pre the class \a T (whose constructor is called) is a base class
     * of \a Y.
     *
     * @param other the pointer to copy.
     */
    template<class Y> SafePtr(const SafePtr<Y>& other);
    /**
     * Returns a raw pointer to the pointee, or \c null if the pointee
     * has already been destroyed.
     *
     * @return the pointee.
     */
    T* get() const;

    /**
     * Returns \c true if the pointee is non-null and has not yet been
     * destroyed.
     *
     * This is equivalent to testing whether get() is not \c null.
     *
     * @return \c true if and only if the pointee is non-null and has
     * not yet been destroyed.
     */
    operator bool() const;

    /**
     * Resets this to point to the given object.  This behaves in a
     * similar manner to destroying the old safe pointer and constructing a
     * new one: in particular, the old pointee may be destroyed if it meets
     * the conditions outlined in the class notes above.
     *
     * @param object the new pointee.  This may be \c null.
     */
    void reset(T* object = 0);

    /**
     * Disable the default assignment operator.
     */
    SafePtr<T>& operator = (const SafePtr<T>&) = delete;

private:
    boost::intrusive_ptr<detail::SafeRemnant<typename T::SafePointeeType>>
        remnant_;
    /**< The remnant that points to the pointee. */
};

} // namespace regina

/*@}*/

namespace boost {
/**
 * Extracts a raw pointer from the given safe pointer.
 *
 * This is required for SafePtr to support the boost dereferencable concept.
 *
 * @param ptr a safe pointer.  This may be \c null.
 * @return the corresponding raw pointer.
 */
template<class T>
inline T* get_pointer(regina::SafePtr<T> const& ptr) {
    return ptr.get();
}
} // namespace boost

namespace regina {

template<class T>
inline SafePtr<T>::SafePtr() {
}

template<class T>
inline SafePtr<T>::SafePtr(T* object) {
    if (object) {
        remnant_.reset(detail::SafeRemnant<typename T::SafePointeeType>::
            getOrCreate(object));
    }
}

template<class T>
template<class Y>
inline SafePtr<T>::SafePtr(const SafePtr<Y> &other) : SafePtr(other.get()) {
}

// By virtue of how \c SafePtr's are constructed, get() always holds
// a pointer to T or a dervied class of T.
template<class T>
inline T* SafePtr<T>::get() const {
    if (not remnant_) {
        return 0;
    }
    return static_cast<T*>(remnant_->get());
}

template<class T>
inline SafePtr<T>::operator bool() const {
    return (remnant_ && remnant_->get());
}

template<class T>
inline void SafePtr<T>::reset(T* object) {
    if (object) {
        remnant_.reset(detail::SafeRemnant<typename T::SafePointeeType>::
            getOrCreate(object));
    } else
        remnant_.reset(0);
}

} // namespace regina

#endif
