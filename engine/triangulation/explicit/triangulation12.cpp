
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

template std::string TriangulationBase<12>::isoSigFrom
    <IsoSigPrintable<12>>(size_t, const Perm<13>&, Isomorphism<12>*) const;
template std::string TriangulationBase<12>::isoSig<
    IsoSigClassic<12>, IsoSigPrintable<12>>() const;
template std::pair<std::string, Isomorphism<12>>
    TriangulationBase<12>::isoSigDetail<
    IsoSigClassic<12>, IsoSigPrintable<12>>() const;
template Triangulation<12> TriangulationBase<12>::fromIsoSig(
    const std::string&);
template size_t TriangulationBase<12>::isoSigComponentSize(const std::string&);

template AbelianGroup TriangulationBase<12>::homology<1>() const;
template AbelianGroup TriangulationBase<12>::homology<2>() const;
template AbelianGroup TriangulationBase<12>::homology<3>() const;
template AbelianGroup TriangulationBase<12>::homology<4>() const;
template AbelianGroup TriangulationBase<12>::homology<5>() const;
template AbelianGroup TriangulationBase<12>::homology<6>() const;
template AbelianGroup TriangulationBase<12>::homology<7>() const;
template AbelianGroup TriangulationBase<12>::homology<8>() const;
template AbelianGroup TriangulationBase<12>::homology<9>() const;
template AbelianGroup TriangulationBase<12>::homology<10>() const;

template MatrixInt TriangulationBase<12>::boundaryMap<1>() const;
template MatrixInt TriangulationBase<12>::boundaryMap<2>() const;
template MatrixInt TriangulationBase<12>::boundaryMap<3>() const;
template MatrixInt TriangulationBase<12>::boundaryMap<4>() const;
template MatrixInt TriangulationBase<12>::boundaryMap<5>() const;
template MatrixInt TriangulationBase<12>::boundaryMap<6>() const;
template MatrixInt TriangulationBase<12>::boundaryMap<7>() const;
template MatrixInt TriangulationBase<12>::boundaryMap<8>() const;
template MatrixInt TriangulationBase<12>::boundaryMap<9>() const;
template MatrixInt TriangulationBase<12>::boundaryMap<10>() const;
template MatrixInt TriangulationBase<12>::boundaryMap<11>() const;
template MatrixInt TriangulationBase<12>::boundaryMap<12>() const;
template MatrixInt TriangulationBase<12>::dualBoundaryMap<1>() const;
template MatrixInt TriangulationBase<12>::dualBoundaryMap<2>() const;
template MatrixInt TriangulationBase<12>::dualBoundaryMap<3>() const;
template MatrixInt TriangulationBase<12>::dualBoundaryMap<4>() const;
template MatrixInt TriangulationBase<12>::dualBoundaryMap<5>() const;
template MatrixInt TriangulationBase<12>::dualBoundaryMap<6>() const;
template MatrixInt TriangulationBase<12>::dualBoundaryMap<7>() const;
template MatrixInt TriangulationBase<12>::dualBoundaryMap<8>() const;
template MatrixInt TriangulationBase<12>::dualBoundaryMap<9>() const;
template MatrixInt TriangulationBase<12>::dualBoundaryMap<10>() const;
template MatrixInt TriangulationBase<12>::dualBoundaryMap<11>() const;
template MatrixInt TriangulationBase<12>::dualToPrimal<0>() const;
template MatrixInt TriangulationBase<12>::dualToPrimal<1>() const;
template MatrixInt TriangulationBase<12>::dualToPrimal<2>() const;
template MatrixInt TriangulationBase<12>::dualToPrimal<3>() const;
template MatrixInt TriangulationBase<12>::dualToPrimal<4>() const;
template MatrixInt TriangulationBase<12>::dualToPrimal<5>() const;
template MatrixInt TriangulationBase<12>::dualToPrimal<6>() const;
template MatrixInt TriangulationBase<12>::dualToPrimal<7>() const;
template MatrixInt TriangulationBase<12>::dualToPrimal<8>() const;
template MatrixInt TriangulationBase<12>::dualToPrimal<9>() const;
template MatrixInt TriangulationBase<12>::dualToPrimal<10>() const;
template MatrixInt TriangulationBase<12>::dualToPrimal<11>() const;

template const GroupPresentation& TriangulationBase<12>::fundamentalGroup() const;

template void TriangulationBase<12>::calculateSkeleton();
template void TriangulationBase<12>::clearBaseProperties();
template void TriangulationBase<12>::swapBaseData(TriangulationBase<12>&);

template bool TriangulationBase<12>::pachner(Face<12, 0>*, bool, bool);
template bool TriangulationBase<12>::pachner(Face<12, 1>*, bool, bool);
template bool TriangulationBase<12>::pachner(Face<12, 2>*, bool, bool);
template bool TriangulationBase<12>::pachner(Face<12, 3>*, bool, bool);
template bool TriangulationBase<12>::pachner(Face<12, 4>*, bool, bool);
template bool TriangulationBase<12>::pachner(Face<12, 5>*, bool, bool);
template bool TriangulationBase<12>::pachner(Face<12, 6>*, bool, bool);
template bool TriangulationBase<12>::pachner(Face<12, 7>*, bool, bool);
template bool TriangulationBase<12>::pachner(Face<12, 8>*, bool, bool);
template bool TriangulationBase<12>::pachner(Face<12, 9>*, bool, bool);
template bool TriangulationBase<12>::pachner(Face<12, 10>*, bool, bool);
template bool TriangulationBase<12>::pachner(Face<12, 11>*, bool, bool);
template bool TriangulationBase<12>::pachner(Face<12, 12>*, bool, bool);

} // namespace regina::detail
