
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

#include "census/dim4facetpairing.h"
#include "census/ngenericfacetpairing-impl.h"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4triangulation.h"

namespace regina {

// Instatiate all templates from the -impl.h file.
template NGenericFacetPairing<4>::NGenericFacetPairing(
    const NGenericFacetPairing<4>&);
template NGenericFacetPairing<4>::NGenericFacetPairing(
    const Dim4Triangulation&);
template bool NGenericFacetPairing<4>::isClosed() const;
template std::string NGenericFacetPairing<4>::str() const;
template std::string NGenericFacetPairing<4>::dotHeader(const char*);
template void NGenericFacetPairing<4>::writeDotHeader(std::ostream&,
    const char*);
template std::string NGenericFacetPairing<4>::dot(const char*, bool, bool)
    const;
template void NGenericFacetPairing<4>::writeDot(std::ostream&, const char*,
    bool, bool) const;
template std::string NGenericFacetPairing<4>::toTextRep() const;
template Dim4FacetPairing* NGenericFacetPairing<4>::fromTextRep(
    const std::string&);
template bool NGenericFacetPairing<4>::isCanonical() const;
template bool NGenericFacetPairing<4>::isCanonicalInternal(
    NGenericFacetPairing<4>::IsoList&) const;
template void NGenericFacetPairing<4>::findAutomorphisms(
    NGenericFacetPairing<4>::IsoList&) const;
template void NGenericFacetPairing<4>::findAllPairings(unsigned, NBoolSet,
    int, NGenericFacetPairing<4>::Use, void*);
template void NGenericFacetPairing<4>::enumerateInternal(NBoolSet, int,
    NGenericFacetPairing<4>::Use, void*);

} // namespace regina

