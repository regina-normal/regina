
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "triangulation/detail/isosig-impl.h"
#include "triangulation/detail/pachner-impl.h"
#include "triangulation/detail/skeleton-impl.h"
#include "triangulation/generic.h"

namespace regina::detail {

template std::string TriangulationBase<5>::isoSigFrom
    <IsoSigPrintable<5>>(size_t, const Perm<6>&, Isomorphism<5>*) const;
template std::string TriangulationBase<5>::isoSig
    <IsoSigPrintable<5>>(Isomorphism<5>**) const;
template Triangulation<5>* TriangulationBase<5>::fromIsoSig(
    const std::string&);
template size_t TriangulationBase<5>::isoSigComponentSize(const std::string&);

template void TriangulationBase<5>::calculateSkeleton();
template void TriangulationBase<5>::clearBaseProperties();
template void TriangulationBase<5>::swapBaseData(TriangulationBase<5>&);

template bool TriangulationBase<5>::pachner(Face<5, 0>*, bool, bool);
template bool TriangulationBase<5>::pachner(Face<5, 1>*, bool, bool);
template bool TriangulationBase<5>::pachner(Face<5, 2>*, bool, bool);
template bool TriangulationBase<5>::pachner(Face<5, 3>*, bool, bool);
template bool TriangulationBase<5>::pachner(Face<5, 4>*, bool, bool);
template bool TriangulationBase<5>::pachner(Face<5, 5>*, bool, bool);

} // namespace regina::detail
