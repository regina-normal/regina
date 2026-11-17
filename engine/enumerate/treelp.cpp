
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

template class LPData<Integer, LPConstraintNone>;
template VectorLarge LPData<Integer, LPConstraintNone>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;
template VectorInt LPData<Integer, LPConstraintNone>::
    extractStrictSolution<VectorInt>() const;
template VectorInt LPData<Integer, LPConstraintNone>::
    extractTautSolution<VectorInt>() const;

template class LPData<NativeLong, LPConstraintNone>;
template VectorLarge LPData<NativeLong, LPConstraintNone>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;
template VectorInt LPData<NativeLong, LPConstraintNone>::
    extractStrictSolution<VectorInt>() const;
template VectorInt LPData<NativeLong, LPConstraintNone>::
    extractTautSolution<VectorInt>() const;

template class LPData<Integer, LPConstraintEulerPositive>;
template VectorLarge LPData<Integer, LPConstraintEulerPositive>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

template class LPData<NativeLong, LPConstraintEulerPositive>;
template VectorLarge LPData<NativeLong, LPConstraintEulerPositive>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

template class LPData<Integer, LPConstraintEulerZero>;
template VectorLarge LPData<Integer, LPConstraintEulerZero>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

template class LPData<NativeLong, LPConstraintEulerZero>;
template VectorLarge LPData<NativeLong, LPConstraintEulerZero>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

template class LPData<Integer, LPConstraintNonSpun>;
template VectorLarge LPData<Integer, LPConstraintNonSpun>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

template class LPData<NativeLong, LPConstraintNonSpun>;
template VectorLarge LPData<NativeLong, LPConstraintNonSpun>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

#ifdef INT128_AVAILABLE
template class LPData<NativeInteger<16>, LPConstraintNone>;
template VectorLarge LPData<NativeInteger<16>, LPConstraintNone>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;
template VectorInt LPData<NativeInteger<16>, LPConstraintNone>::
    extractStrictSolution<VectorInt>() const;
template VectorInt LPData<NativeInteger<16>, LPConstraintNone>::
    extractTautSolution<VectorInt>() const;

template class LPData<NativeInteger<16>, LPConstraintEulerPositive>;
template VectorLarge LPData<NativeInteger<16>, LPConstraintEulerPositive>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

template class LPData<NativeInteger<16>, LPConstraintEulerZero>;
template VectorLarge LPData<NativeInteger<16>, LPConstraintEulerZero>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;

template class LPData<NativeInteger<16>, LPConstraintNonSpun>;
template VectorLarge LPData<NativeInteger<16>, LPConstraintNonSpun>::
    extractSurfaceSolution<VectorLarge, TypeIt>(TypeIt) const;
#endif

} // namespace regina
