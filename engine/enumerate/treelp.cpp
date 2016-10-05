
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

#include "enumerate/treeconstraint.h"
#include "enumerate/treelp-impl.h"

namespace regina {

// Instantiate templates for standard combinations of template arguments.
// The header has matching "extern template class REGINA_API" declarations.
template class LPMatrix<NInteger>;
template class LPMatrix<NNativeLong>;

template class LPCol<LPConstraintNone>;
template class LPCol<LPConstraintEuler>;
template class LPCol<LPConstraintNonSpun>;

template class LPInitialTableaux<LPConstraintNone>;
template class LPInitialTableaux<LPConstraintEuler>;
template class LPInitialTableaux<LPConstraintNonSpun>;

template class LPData<LPConstraintNone, NInteger>;
template class LPData<LPConstraintNone, NNativeLong>;
template class LPData<LPConstraintEuler, NInteger>;
template class LPData<LPConstraintEuler, NNativeLong>;
template class LPData<LPConstraintNonSpun, NInteger>;
template class LPData<LPConstraintNonSpun, NNativeLong>;

#ifdef INT128_AVAILABLE
template class LPData<LPConstraintNone, NativeInteger<16> >;
template class LPData<LPConstraintEuler, NativeInteger<16> >;
template class LPData<LPConstraintNonSpun, NativeInteger<16> >;
#endif

} // namespace regina
