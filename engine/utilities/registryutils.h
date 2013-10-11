
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/*! \file utilities/registryutils.h
 *  \brief Miscellaneous utility classes for working with registries of
 *  packet types, normal coordinate flavours, and so on.
 */

#ifndef __REGISTRYUTILS_H
#ifndef __DOXYGEN
#define __REGISTRYUTILS_H
#endif

#include <cstddef>

namespace regina {

/**
 * A convenience base class for a function object, which does nothing
 * beyond provide a \a ReturnType typedef.
 *
 * Specifically, a function object \a F that returns type \a T could inherit
 * from the base class \a Returns<T>.  This will ensure that \a F includes a
 * typedef \a F::ReturnType representing type \a T.
 *
 * See the three-argument forFlavour() routine for an example of where
 * such a function object might be used.
 *
 * \ifacespython Not present.
 */
template <typename ReturnType_>
struct Returns {
    /**
     * Indicates the return type for a function object.
     */
    typedef ReturnType_ ReturnType;
};

/**
 * A function object that creates a new vector subclassed from
 * \a VectorBase, where the particular subclass is chosen according to
 * the template argument to operator().  Typically \a VectorBase will be a
 * normal surface vector base class (such as NNormalSurfaceVector).
 *
 * This routine is intended for use with normal surface flavour registry
 * routines, such as the three-argument variant of forFlavour().
 *
 * \ifacespython Not present.
 */
template <class VectorBase>
struct NewVector : public Returns<VectorBase*> {
    size_t len_;
        /**< The length of the new vector to create. */

    /**
     * Creates a new function object, whose bracket operator will create a
     * new vector of the given length.
     *
     * @param len the length of the vector to create.
     */
    inline NewVector(size_t len) : len_(len) {
    }

    /**
     * Creates a new vector of the subclass Flavour::Class.
     * The length of the vector will match the value passed to the class
     * constructor for this function object.
     *
     * \pre It is known at compile time that Flavour::Class will be a
     * subclass of \a VectorBase.
     *
     * @return a new vector of the subclass Flavour::Class.
     */
    template <typename Flavour>
    inline VectorBase* operator() (Flavour) {
        return new typename Flavour::Class(len_);
    }
};

} // namespace regina

#endif

