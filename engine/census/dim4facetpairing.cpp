
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

#include "census/dim4facetpairing.h"
#include "census/ngeneralfacetpairing.tcc"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4triangulation.h"

namespace regina {

// Instatiate all templates from the .tcc file.
template NGeneralFacetPairing<4>::NGeneralFacetPairing(
    const NGeneralFacetPairing<4>&);
template NGeneralFacetPairing<4>::NGeneralFacetPairing(
    const Dim4Triangulation&);
template bool NGeneralFacetPairing<4>::isClosed() const;
template std::string NGeneralFacetPairing<4>::toString() const;
template std::string NGeneralFacetPairing<4>::dotHeader(const char*);
template void NGeneralFacetPairing<4>::writeDotHeader(std::ostream&,
    const char*);
template std::string NGeneralFacetPairing<4>::dot(const char*, bool, bool)
    const;
template void NGeneralFacetPairing<4>::writeDot(std::ostream&, const char*,
    bool, bool) const;
template std::string NGeneralFacetPairing<4>::toTextRep() const;
template Dim4FacetPairing* NGeneralFacetPairing<4>::fromTextRep(
    const std::string&);
template bool NGeneralFacetPairing<4>::isCanonical() const;
template bool NGeneralFacetPairing<4>::isCanonicalInternal(
    NGeneralFacetPairing<4>::IsoList&) const;
template void NGeneralFacetPairing<4>::findAutomorphisms(
    NGeneralFacetPairing<4>::IsoList&) const;
template bool NGeneralFacetPairing<4>::findAllPairings(unsigned, NBoolSet,
    int, NGeneralFacetPairing<4>::Use, void*, bool);
template void* NGeneralFacetPairing<4>::run(void*);

} // namespace regina

