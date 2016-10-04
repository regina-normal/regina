
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2016, Ben Burton                                   *
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
// The header has matching "extern template class REGINA_API" declarations.
template class TreeTraversal<LPConstraintNone, BanNone, NInteger>;
template class TreeTraversal<LPConstraintNone, BanNone, NNativeLong>;
template class TreeEnumeration<LPConstraintNone, BanNone, NInteger>;
template class TreeEnumeration<LPConstraintNone, BanNone, NNativeLong>;
template class TautEnumeration<LPConstraintNone, BanNone, NInteger>;
template class TautEnumeration<LPConstraintNone, BanNone, NNativeLong>;

template class TreeTraversal<LPConstraintNone, BanBoundary, NInteger>;
template class TreeTraversal<LPConstraintNone, BanBoundary, NNativeLong>;
template class TreeEnumeration<LPConstraintNone, BanBoundary, NInteger>;
template class TreeEnumeration<LPConstraintNone, BanBoundary, NNativeLong>;

template class TreeTraversal<LPConstraintEuler, BanNone, NInteger>;
template class TreeTraversal<LPConstraintEuler, BanNone, NNativeLong>;
template class TreeSingleSoln<LPConstraintEuler, BanNone, NInteger>;
template class TreeSingleSoln<LPConstraintEuler, BanNone, NNativeLong>;

#ifdef INT128_AVAILABLE
template class TreeTraversal<LPConstraintNone, BanNone, NNativeInteger<16> >;
template class TreeEnumeration<LPConstraintNone, BanNone, NNativeInteger<16> >;
template class TautEnumeration<LPConstraintNone, BanNone, NNativeInteger<16> >;

template class TreeTraversal<LPConstraintNone, BanBoundary, NNativeInteger<16> >;
template class TreeEnumeration<LPConstraintNone, BanBoundary, NNativeInteger<16> >;

template class TreeTraversal<LPConstraintEuler, BanNone, NNativeInteger<16> >;
template class TreeSingleSoln<LPConstraintEuler, BanNone, NNativeInteger<16> >;
#endif

} // namespace regina
