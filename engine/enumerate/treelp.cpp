
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

using TypeIt = typename FixedArray<uint8_t>::const_iterator;

template class LPData<LPConstraintNone, Integer>;
template VectorLarge LPData<LPConstraintNone, Integer>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;
template VectorInt LPData<LPConstraintNone, Integer>::
    extractStrictSolution<VectorInt>() const;
template VectorInt LPData<LPConstraintNone, Integer>::
    extractTautSolution<VectorInt>() const;

template class LPData<LPConstraintNone, NativeLong>;
template VectorLarge LPData<LPConstraintNone, NativeLong>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;
template VectorInt LPData<LPConstraintNone, NativeLong>::
    extractStrictSolution<VectorInt>() const;
template VectorInt LPData<LPConstraintNone, NativeLong>::
    extractTautSolution<VectorInt>() const;

template class LPData<LPConstraintEulerPositive, Integer>;
template VectorLarge LPData<LPConstraintEulerPositive, Integer>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

template class LPData<LPConstraintEulerPositive, NativeLong>;
template VectorLarge LPData<LPConstraintEulerPositive, NativeLong>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

template class LPData<LPConstraintEulerZero, Integer>;
template VectorLarge LPData<LPConstraintEulerZero, Integer>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

template class LPData<LPConstraintEulerZero, NativeLong>;
template VectorLarge LPData<LPConstraintEulerZero, NativeLong>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

template class LPData<LPConstraintNonSpun, Integer>;
template VectorLarge LPData<LPConstraintNonSpun, Integer>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

template class LPData<LPConstraintNonSpun, NativeLong>;
template VectorLarge LPData<LPConstraintNonSpun, NativeLong>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

#ifdef INT128_AVAILABLE
template class LPData<LPConstraintNone, NativeInteger<16>>;
template VectorLarge LPData<LPConstraintNone, NativeInteger<16>>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;
template VectorInt LPData<LPConstraintNone, NativeInteger<16>>::
    extractStrictSolution<VectorInt>() const;
template VectorInt LPData<LPConstraintNone, NativeInteger<16>>::
    extractTautSolution<VectorInt>() const;

template class LPData<LPConstraintEulerPositive, NativeInteger<16>>;
template VectorLarge LPData<LPConstraintEulerPositive, NativeInteger<16>>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

template class LPData<LPConstraintEulerZero, NativeInteger<16>>;
template VectorLarge LPData<LPConstraintEulerZero, NativeInteger<16>>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

template class LPData<LPConstraintNonSpun, NativeInteger<16>>;
template VectorLarge LPData<LPConstraintNonSpun, NativeInteger<16>>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;
#endif

} // namespace regina
