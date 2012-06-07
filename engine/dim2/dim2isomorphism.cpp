
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

#include <algorithm>
#include <cstdlib>
#include "dim2/dim2isomorphism.h"
#include "dim2/dim2triangulation.h"
#include "triangulation/ngeneralisomorphism.tcc"

namespace regina {

// Instatiate all templates from the .tcc file.
template void NGeneralIsomorphism<2>::writeTextShort(std::ostream&) const;
template void NGeneralIsomorphism<2>::writeTextLong(std::ostream&) const;
template bool NGeneralIsomorphism<2>::isIdentity() const;
template NGeneralIsomorphism<2>::NGeneralIsomorphism(
    const NGeneralIsomorphism<2>&);
template Dim2Isomorphism* NGeneralIsomorphism<2>::random(unsigned);
template Dim2Triangulation* NGeneralIsomorphism<2>::apply(
        const Dim2Triangulation*) const;
template void NGeneralIsomorphism<2>::applyInPlace(Dim2Triangulation*) const;

} // namespace regina

