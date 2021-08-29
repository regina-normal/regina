
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2021, Ben Burton                                   *
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

#include "enumerate/treetraversal-impl.h"

namespace regina {

// Instantiate templates for standard combinations of template arguments.
template class TreeTraversal<LPConstraintNone, BanNone, Integer>;
template class TreeTraversal<LPConstraintNone, BanNone, NativeLong>;
template class TreeEnumeration<LPConstraintNone, BanNone, Integer>;
template class TreeEnumeration<LPConstraintNone, BanNone, NativeLong>;
template class TautEnumeration<LPConstraintNone, BanNone, Integer>;
template class TautEnumeration<LPConstraintNone, BanNone, NativeLong>;

template class TreeTraversal<LPConstraintNone, BanBoundary, Integer>;
template class TreeTraversal<LPConstraintNone, BanBoundary, NativeLong>;
template class TreeEnumeration<LPConstraintNone, BanBoundary, Integer>;
template class TreeEnumeration<LPConstraintNone, BanBoundary, NativeLong>;

template class TreeTraversal<LPConstraintEulerPositive, BanNone, Integer>;
template class TreeTraversal<LPConstraintEulerPositive, BanNone, NativeLong>;
template class TreeSingleSoln<LPConstraintEulerPositive, BanNone, Integer>;
template class TreeSingleSoln<LPConstraintEulerPositive, BanNone, NativeLong>;

template class TreeTraversal<LPConstraintEulerZero, BanNone, Integer>;
template class TreeTraversal<LPConstraintEulerZero, BanNone, NativeLong>;
template class TreeEnumeration<LPConstraintEulerZero, BanNone, Integer>;
template class TreeEnumeration<LPConstraintEulerZero, BanNone, NativeLong>;

template class TreeTraversal<LPConstraintNonSpun, BanNone, Integer>;
template class TreeTraversal<LPConstraintNonSpun, BanNone, NativeLong>;
template class TreeEnumeration<LPConstraintNonSpun, BanNone, Integer>;
template class TreeEnumeration<LPConstraintNonSpun, BanNone, NativeLong>;

#ifdef INT128_AVAILABLE
template class TreeTraversal<LPConstraintNone, BanNone, NativeInteger<16>>;
template class TreeEnumeration<LPConstraintNone, BanNone, NativeInteger<16>>;
template class TautEnumeration<LPConstraintNone, BanNone, NativeInteger<16>>;

template class TreeTraversal<LPConstraintNone, BanBoundary, NativeInteger<16>>;
template class TreeEnumeration<LPConstraintNone, BanBoundary, NativeInteger<16>>;

template class TreeTraversal<LPConstraintEulerPositive, BanNone,
    NativeInteger<16>>;
template class TreeSingleSoln<LPConstraintEulerPositive, BanNone,
    NativeInteger<16>>;

template class TreeTraversal<LPConstraintEulerZero, BanNone, NativeInteger<16>>;
template class TreeEnumeration<LPConstraintEulerZero, BanNone, NativeInteger<16>>;

template class TreeTraversal<LPConstraintNonSpun, BanNone, NativeInteger<16>>;
template class TreeEnumeration<LPConstraintNonSpun, BanNone, NativeInteger<16>>;
#endif

} // namespace regina
