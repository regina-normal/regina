
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2011-2025, Ben Burton                                   *
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

#include "angle/anglestructure.h"
#include "enumerate/treeconstraint.h"
#include "enumerate/treelp-impl.h"
#include "surface/normalsurface.h"

namespace regina {

// Instantiate templates for standard combinations of template arguments.
template class LPMatrix<Integer>;
template class LPMatrix<NativeLong>;

template class LPInitialTableaux<LPConstraintNone>;
template class LPInitialTableaux<LPConstraintEulerPositive>;
template class LPInitialTableaux<LPConstraintEulerZero>;
template class LPInitialTableaux<LPConstraintNonSpun>;

template class LPData<LPConstraintNone, Integer>;
template VectorInt LPData<LPConstraintNone, Integer>::
    extractSolution<VectorInt>(const char*) const;
template VectorLarge LPData<LPConstraintNone, Integer>::
    extractSolution<VectorLarge>(const char*) const;

template class LPData<LPConstraintNone, NativeLong>;
template VectorInt LPData<LPConstraintNone, NativeLong>::
    extractSolution<VectorInt>(const char*) const;
template VectorLarge LPData<LPConstraintNone, NativeLong>::
    extractSolution<VectorLarge>(const char*) const;

template class LPData<LPConstraintEulerPositive, Integer>;
template VectorInt LPData<LPConstraintEulerPositive, Integer>::
    extractSolution<VectorInt>(const char*) const;
template VectorLarge LPData<LPConstraintEulerPositive, Integer>::
    extractSolution<VectorLarge>(const char*) const;

template class LPData<LPConstraintEulerPositive, NativeLong>;
template VectorInt LPData<LPConstraintEulerPositive, NativeLong>::
    extractSolution<VectorInt>(const char*) const;
template VectorLarge LPData<LPConstraintEulerPositive, NativeLong>::
    extractSolution<VectorLarge>(const char*) const;

template class LPData<LPConstraintEulerZero, Integer>;
template VectorInt LPData<LPConstraintEulerZero, Integer>::
    extractSolution<VectorInt>(const char*) const;
template VectorLarge LPData<LPConstraintEulerZero, Integer>::
    extractSolution<VectorLarge>(const char*) const;

template class LPData<LPConstraintEulerZero, NativeLong>;
template VectorInt LPData<LPConstraintEulerZero, NativeLong>::
    extractSolution<VectorInt>(const char*) const;
template VectorLarge LPData<LPConstraintEulerZero, NativeLong>::
    extractSolution<VectorLarge>(const char*) const;

template class LPData<LPConstraintNonSpun, Integer>;
template VectorInt LPData<LPConstraintNonSpun, Integer>::
    extractSolution<VectorInt>(const char*) const;
template VectorLarge LPData<LPConstraintNonSpun, Integer>::
    extractSolution<VectorLarge>(const char*) const;

template class LPData<LPConstraintNonSpun, NativeLong>;
template VectorInt LPData<LPConstraintNonSpun, NativeLong>::
    extractSolution<VectorInt>(const char*) const;
template VectorLarge LPData<LPConstraintNonSpun, NativeLong>::
    extractSolution<VectorLarge>(const char*) const;

#ifdef INT128_AVAILABLE
template class LPData<LPConstraintNone, NativeInteger<16>>;
template VectorInt LPData<LPConstraintNone, NativeInteger<16>>::
    extractSolution<VectorInt>(const char*) const;
template VectorLarge LPData<LPConstraintNone, NativeInteger<16>>::
    extractSolution<VectorLarge>(const char*) const;

template class LPData<LPConstraintEulerPositive, NativeInteger<16>>;
template VectorInt LPData<LPConstraintEulerPositive, NativeInteger<16>>::
    extractSolution<VectorInt>(const char*) const;
template VectorLarge LPData<LPConstraintEulerPositive, NativeInteger<16>>::
    extractSolution<VectorLarge>(const char*) const;

template class LPData<LPConstraintEulerZero, NativeInteger<16>>;
template VectorInt LPData<LPConstraintEulerZero, NativeInteger<16>>::
    extractSolution<VectorInt>(const char*) const;
template VectorLarge LPData<LPConstraintEulerZero, NativeInteger<16>>::
    extractSolution<VectorLarge>(const char*) const;

template class LPData<LPConstraintNonSpun, NativeInteger<16>>;
template VectorInt LPData<LPConstraintNonSpun, NativeInteger<16>>::
    extractSolution<VectorInt>(const char*) const;
template VectorLarge LPData<LPConstraintNonSpun, NativeInteger<16>>::
    extractSolution<VectorLarge>(const char*) const;
#endif

} // namespace regina
