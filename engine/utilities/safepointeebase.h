
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file utilities/safepointeebase.h
 *  \brief Provides a base class for objects pointable by SafePtr.
 */

#ifndef __SAFEPOINTEEBASE_H
#ifndef __DOXYGEN
#define __SAFEPOINTEEBASE_H
#endif

#include <atomic>

namespace regina {

template <class T> class SafePtr;

/**
 * \weakgroup utilities
 * @{
 */

/**
 * A base class for objects of type \a T to be referenceable by a SafePtr.
 * Such objects are referred to as \e pointees of SafePtr.
 *
 * The class \a T must derive from SafePointeeBase<T>, and must implement
 * hasOwner() to indicate whether any non-SafePtr claims ownership of it.
 * Details of ownership semantics (and in particular, the requirements for
 * hasOwner()) are explained in the documentation for SafePtr.
 *
 * The overhead introduced by subclassing from SafePointeeBase without using
 * the features of the accompanying smart pointer SafePtr are minimal:
 * one extra atomic integer that needs to be zeroed upon construction.
 *
 * Regina's classes that derive from SafePointeeBase do so to help with
 * python wrapping.
 *
 * \tparam T the type of object being pointed to.  This must derive from
 * SafePointeeBase<T>.
 *
 * @author Matthias Goerner
 */
template <class T>
class SafePointeeBase {
public:
    /**
     * Prevent derived classes from accidentally calling the copy constructor.
     * A derived class' copy constructor by default calls the protected default
     * constructor, which it should because it sets the refCount_ to zero on
     * the copied object.
     */
    SafePointeeBase(const SafePointeeBase &) = delete;

    /**
     * Prevent derived classes from accidentally calling the assignment
     * operator.
     */
    SafePointeeBase & operator=(const SafePointeeBase &) = delete;

    /**
     * The type of object being pointed to.
     */
    typedef T SafePointeeType;

    /**
     * Is there one or more SafePtr currently pointing to this object?
     */
    bool hasSafePtr() const;

protected:
    /**
     * Default constructor.
     */
    SafePointeeBase();

private:
    template<class U> friend class regina::SafePtr;

    std::atomic<int> refCount_;
        /**< Counts how many SafePtr are pointing to this object. */
};

/*@}*/

// Inline functions for SafePointeeBase

template <class T>
inline SafePointeeBase<T>::SafePointeeBase() : refCount_(0) {
}

template <class T>
inline bool SafePointeeBase<T>::hasSafePtr() const {
    return refCount_.load() > 0;
}

} // namespace regina

#endif
