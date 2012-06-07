
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

#include "census/dim2edgepairing.h"
#include "census/ngenericfacetpairing.tcc"
#include "dim2/dim2triangle.h"
#include "dim2/dim2triangulation.h"

namespace regina {

// Instatiate all templates from the .tcc file.
template NGenericFacetPairing<2>::NGenericFacetPairing(
    const NGenericFacetPairing<2>&);
template NGenericFacetPairing<2>::NGenericFacetPairing(
    const Dim2Triangulation&);
template bool NGenericFacetPairing<2>::isClosed() const;
template std::string NGenericFacetPairing<2>::toString() const;
template std::string NGenericFacetPairing<2>::dotHeader(const char*);
template void NGenericFacetPairing<2>::writeDotHeader(std::ostream&,
    const char*);
template std::string NGenericFacetPairing<2>::dot(const char*, bool, bool)
    const;
template void NGenericFacetPairing<2>::writeDot(std::ostream&, const char*,
    bool, bool) const;
template std::string NGenericFacetPairing<2>::toTextRep() const;
template Dim2EdgePairing* NGenericFacetPairing<2>::fromTextRep(
    const std::string&);
template bool NGenericFacetPairing<2>::isCanonical() const;
template bool NGenericFacetPairing<2>::isCanonicalInternal(
    NGenericFacetPairing<2>::IsoList&) const;
template void NGenericFacetPairing<2>::findAutomorphisms(
    NGenericFacetPairing<2>::IsoList&) const;
template bool NGenericFacetPairing<2>::findAllPairings(unsigned, NBoolSet,
    int, NGenericFacetPairing<2>::Use, void*, bool);
template void* NGenericFacetPairing<2>::run(void*);

} // namespace regina

