
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

template std::string TriangulationBase<13>::isoSigFrom
    <IsoSigPrintable<13>>(size_t, const Perm<14>&, Isomorphism<13>*) const;
template std::string TriangulationBase<13>::isoSig
    <IsoSigPrintable<13>>(Isomorphism<13>**) const;
template Triangulation<13>* TriangulationBase<13>::fromIsoSig(
    const std::string&);
template size_t TriangulationBase<13>::isoSigComponentSize(const std::string&);

template void TriangulationBase<13>::calculateSkeleton();
template void TriangulationBase<13>::clearBaseProperties();
template void TriangulationBase<13>::swapBaseData(TriangulationBase<13>&);

template bool TriangulationBase<13>::pachner(Face<13, 0>*, bool, bool);
template bool TriangulationBase<13>::pachner(Face<13, 1>*, bool, bool);
template bool TriangulationBase<13>::pachner(Face<13, 2>*, bool, bool);
template bool TriangulationBase<13>::pachner(Face<13, 3>*, bool, bool);
template bool TriangulationBase<13>::pachner(Face<13, 4>*, bool, bool);
template bool TriangulationBase<13>::pachner(Face<13, 5>*, bool, bool);
template bool TriangulationBase<13>::pachner(Face<13, 6>*, bool, bool);
template bool TriangulationBase<13>::pachner(Face<13, 7>*, bool, bool);
template bool TriangulationBase<13>::pachner(Face<13, 8>*, bool, bool);
template bool TriangulationBase<13>::pachner(Face<13, 9>*, bool, bool);
template bool TriangulationBase<13>::pachner(Face<13, 10>*, bool, bool);
template bool TriangulationBase<13>::pachner(Face<13, 11>*, bool, bool);
template bool TriangulationBase<13>::pachner(Face<13, 12>*, bool, bool);
template bool TriangulationBase<13>::pachner(Face<13, 13>*, bool, bool);

} // namespace regina::detail
