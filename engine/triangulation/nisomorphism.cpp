
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

#include "triangulation/ntriangulation.h"
#include "triangulation/ngenericisomorphism.tcc"
#include "triangulation/nisomorphism.h"

namespace regina {

// Instatiate all templates from the .tcc file.
template void NGenericIsomorphism<3>::writeTextShort(std::ostream&) const;
template void NGenericIsomorphism<3>::writeTextLong(std::ostream&) const;
template bool NGenericIsomorphism<3>::isIdentity() const;
template NGenericIsomorphism<3>::NGenericIsomorphism(
    const NGenericIsomorphism<3>&);
template NIsomorphism* NGenericIsomorphism<3>::random(unsigned);
template NTriangulation* NGenericIsomorphism<3>::apply(const NTriangulation*)
    const;
template void NGenericIsomorphism<3>::applyInPlace(NTriangulation*) const;

} // namespace regina

