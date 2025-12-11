
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#ifndef __REGINA_MATHS_FORWARD_H
#ifndef __DOXYGEN
#define __REGINA_MATHS_FORWARD_H
#endif

/*! \file maths/forward.h
 *  \brief Forward declarations for some mathematical classes, including
 *  integer, ring, vector and matrix types.
 */

#include "concepts/arithmetic.h"
#include "concepts/io.h"

namespace regina {

template <bool> class IntegerBase;
using Integer = IntegerBase<false>;
using LargeInteger = IntegerBase<true>;

template <int> class NativeInteger;

template <RingLike> struct RingTraits;

template <RingLike T>
requires Writable<T> && IntegerCompatible<T>
class Vector;

template <typename T>
requires std::default_initializable<T> && std::copyable<T> && Writable<T>
class Matrix;

} // namespace regina

#endif

