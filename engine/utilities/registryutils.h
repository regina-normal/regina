
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

} // namespace regina

#endif

