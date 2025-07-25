
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

#include "enumerate/hilbertprimal.h"
#include "libnormaliz/cone.h"

namespace regina {

std::vector<std::vector<mpz_class>> HilbertPrimal::normaliz(
        const std::vector<std::vector<mpz_class>>& input) {
    libnormaliz::Cone<mpz_class> cone(
        libnormaliz::Type::integral_closure, input);
    libnormaliz::ConeProperties wanted(
        libnormaliz::ConeProperty::HilbertBasis);
    cone.deactivateChangeOfPrecision();
    cone.compute(wanted);

    if (! cone.isComputed(libnormaliz::ConeProperty::HilbertBasis))
        throw UnsolvedCase("Normaliz was not able to compute the "
            "Hilbert basis on a maximal face");

    return cone.getHilbertBasis();
}

} // namespace regina

