
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2026, Ben Burton                                   *
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

#include "enumerate/treetraversal-impl.h"

namespace regina {

// Instantiate templates for standard combinations of template arguments.
template class TreeTraversal<Integer>;
template class TreeTraversal<NativeLong>;
template class TreeEnumeration<Integer>;
template class TreeEnumeration<NativeLong>;
template class TautEnumeration<Integer>;
template class TautEnumeration<NativeLong>;

template class TreeTraversal<Integer, LPConstraintNone, BanBoundary>;
template class TreeTraversal<NativeLong, LPConstraintNone, BanBoundary>;
template class TreeEnumeration<Integer, LPConstraintNone, BanBoundary>;
template class TreeEnumeration<NativeLong, LPConstraintNone, BanBoundary>;

template class TreeTraversal<Integer, LPConstraintEulerPositive>;
template class TreeTraversal<NativeLong, LPConstraintEulerPositive>;
template class TreeSingleSoln<Integer, LPConstraintEulerPositive>;
template class TreeSingleSoln<NativeLong, LPConstraintEulerPositive>;

template class TreeTraversal<Integer, LPConstraintEulerZero>;
template class TreeTraversal<NativeLong, LPConstraintEulerZero>;
template class TreeEnumeration<Integer, LPConstraintEulerZero>;
template class TreeEnumeration<NativeLong, LPConstraintEulerZero>;

template class TreeTraversal<Integer, LPConstraintNonSpun>;
template class TreeTraversal<NativeLong, LPConstraintNonSpun>;
template class TreeEnumeration<Integer, LPConstraintNonSpun>;
template class TreeEnumeration<NativeLong, LPConstraintNonSpun>;

#ifdef INT128_AVAILABLE
template class TreeTraversal<NativeInteger<16>>;
template class TreeEnumeration<NativeInteger<16>>;
template class TautEnumeration<NativeInteger<16>>;

template class TreeTraversal<NativeInteger<16>, LPConstraintNone, BanBoundary>;
template class TreeEnumeration<NativeInteger<16>, LPConstraintNone, BanBoundary>;

template class TreeTraversal<NativeInteger<16>, LPConstraintEulerPositive>;
template class TreeSingleSoln<NativeInteger<16>, LPConstraintEulerPositive>;

template class TreeTraversal<NativeInteger<16>, LPConstraintEulerZero>;
template class TreeEnumeration<NativeInteger<16>, LPConstraintEulerZero>;

template class TreeTraversal<NativeInteger<16>, LPConstraintNonSpun>;
template class TreeEnumeration<NativeInteger<16>, LPConstraintNonSpun>;
#endif

} // namespace regina
