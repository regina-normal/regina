
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

#include "triangulation/detail/algebra-impl.h"
#include "triangulation/detail/isosig-impl.h"
#include "triangulation/detail/pachner-impl.h"
#include "triangulation/detail/skeleton-impl.h"
#include "triangulation/generic.h"

namespace regina::detail {

template std::string TriangulationBase<6>::isoSigFrom
    <IsoSigPrintable<6>>(size_t, const Perm<7>&, Isomorphism<6>*) const;
template std::string TriangulationBase<6>::isoSig<
    IsoSigClassic<6>, IsoSigPrintable<6>>() const;
template std::pair<std::string, Isomorphism<6>>
    TriangulationBase<6>::isoSigDetail<
    IsoSigClassic<6>, IsoSigPrintable<6>>() const;
template Triangulation<6> TriangulationBase<6>::fromIsoSig(const std::string&);
template size_t TriangulationBase<6>::isoSigComponentSize(const std::string&);

template AbelianGroup TriangulationBase<6>::homology<1>() const;
template AbelianGroup TriangulationBase<6>::homology<2>() const;
template AbelianGroup TriangulationBase<6>::homology<3>() const;
template AbelianGroup TriangulationBase<6>::homology<4>() const;

template MatrixInt TriangulationBase<6>::boundaryMap<1>() const;
template MatrixInt TriangulationBase<6>::boundaryMap<2>() const;
template MatrixInt TriangulationBase<6>::boundaryMap<3>() const;
template MatrixInt TriangulationBase<6>::boundaryMap<4>() const;
template MatrixInt TriangulationBase<6>::boundaryMap<5>() const;
template MatrixInt TriangulationBase<6>::boundaryMap<6>() const;
template MatrixInt TriangulationBase<6>::dualBoundaryMap<1>() const;
template MatrixInt TriangulationBase<6>::dualBoundaryMap<2>() const;
template MatrixInt TriangulationBase<6>::dualBoundaryMap<3>() const;
template MatrixInt TriangulationBase<6>::dualBoundaryMap<4>() const;
template MatrixInt TriangulationBase<6>::dualBoundaryMap<5>() const;

template const GroupPresentation& TriangulationBase<6>::fundamentalGroup() const;

template void TriangulationBase<6>::calculateSkeleton();
template void TriangulationBase<6>::clearBaseProperties();
template void TriangulationBase<6>::swapBaseData(TriangulationBase<6>&);

template bool TriangulationBase<6>::pachner(Face<6, 0>*, bool, bool);
template bool TriangulationBase<6>::pachner(Face<6, 1>*, bool, bool);
template bool TriangulationBase<6>::pachner(Face<6, 2>*, bool, bool);
template bool TriangulationBase<6>::pachner(Face<6, 3>*, bool, bool);
template bool TriangulationBase<6>::pachner(Face<6, 4>*, bool, bool);
template bool TriangulationBase<6>::pachner(Face<6, 5>*, bool, bool);
template bool TriangulationBase<6>::pachner(Face<6, 6>*, bool, bool);

} // namespace regina::detail
