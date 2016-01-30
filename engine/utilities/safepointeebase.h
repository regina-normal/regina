
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

/*! \file utilities/safepointeebase.h
 *  \brief Provides a base class for objects pointable by SafePtr.
 */

#ifndef __SAFEPOINTEEBASE_H
#ifndef __DOXYGEN
#define __SAFEPOINTEEBASE_H
#endif

#include "utilities/saferemnant.h"

/**
 * \weakgroup utilities
 * @{
 */

namespace regina {

/**
 * A base class for objects to be referenceable by a \c SafePtr (referred
 * to as pointee's of \c SafePtr). Every derived class needs to implement
 * hasOwner to indicate whether any non-SafePtr claims ownership of
 * it. Details of ownership semantics are explained in \c SafePtr.
 *
 * The overhead introduced by subclassing from \c SafePointeeBase without using
 * the features of the accompanying smart pointer \c SafePtr are minimal:
 * one extra pointer that needs to be zero'd upon construction.
 *
 * Most classes subclass from \c SafePointeeBase for python wrapping.
 */
template <class T>
class SafePointeeBase {
public:
    ~SafePointeeBase();

    typedef T SafePointeeType;

protected:
    /**
     * Default constructor.
     */
    SafePointeeBase() : remnant_(0) { }

private:
    // Prevent derived classes from accidentally calling copy constructor.
    // A derived classes copy constructor by default calls the above default
    // constructor, which it should because it sets the remnant_ to zero on
    // the copied object.
    SafePointeeBase(const SafePointeeBase &);

    // Similarly, for operator=
    SafePointeeBase & operator=(const SafePointeeBase &);
    
    friend class SafeRemnant<T>;
    SafeRemnant<T> *remnant_;
    /**< Points to the corresponding persistent object. */
};

template <class T>
SafePointeeBase<T>::~SafePointeeBase() {
    // If existing, expire the remnant. Thus, all SafePtr's pointing to
    // this object know that they cannot be dereferenced anylonger.
    if (remnant_) {
        remnant_->expire();
    }
}

} // namespace regina

#endif
