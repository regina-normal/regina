
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

#include "triangulation/detail/algebra-impl.h"
#include "triangulation/detail/isosig-impl.h"
#include "triangulation/detail/moves-impl.h"
#include "triangulation/detail/skeleton-impl.h"
#include "triangulation/detail/triangulation-impl.h"
#include "triangulation/generic.h"

namespace regina::detail {

template std::string TriangulationBase<13>::isoSigFrom
    <IsoSigPrintable<13>>(size_t, const Perm<14>&, Isomorphism<13>*) const;
template std::string TriangulationBase<13>::isoSig<
    IsoSigClassic<13>, IsoSigPrintable<13>>() const;
template std::pair<std::string, Isomorphism<13>>
    TriangulationBase<13>::isoSigDetail<
    IsoSigClassic<13>, IsoSigPrintable<13>>() const;
template Triangulation<13> TriangulationBase<13>::fromIsoSig(
    const std::string&);
template size_t TriangulationBase<13>::isoSigComponentSize(const std::string&);

template AbelianGroup TriangulationBase<13>::homology<1>() const;
template AbelianGroup TriangulationBase<13>::homology<2>() const;
template AbelianGroup TriangulationBase<13>::homology<3>() const;
template AbelianGroup TriangulationBase<13>::homology<4>() const;
template AbelianGroup TriangulationBase<13>::homology<5>() const;
template AbelianGroup TriangulationBase<13>::homology<6>() const;
template AbelianGroup TriangulationBase<13>::homology<7>() const;
template AbelianGroup TriangulationBase<13>::homology<8>() const;
template AbelianGroup TriangulationBase<13>::homology<9>() const;
template AbelianGroup TriangulationBase<13>::homology<10>() const;
template AbelianGroup TriangulationBase<13>::homology<11>() const;

template MatrixInt TriangulationBase<13>::boundaryMap<1>() const;
template MatrixInt TriangulationBase<13>::boundaryMap<2>() const;
template MatrixInt TriangulationBase<13>::boundaryMap<3>() const;
template MatrixInt TriangulationBase<13>::boundaryMap<4>() const;
template MatrixInt TriangulationBase<13>::boundaryMap<5>() const;
template MatrixInt TriangulationBase<13>::boundaryMap<6>() const;
template MatrixInt TriangulationBase<13>::boundaryMap<7>() const;
template MatrixInt TriangulationBase<13>::boundaryMap<8>() const;
template MatrixInt TriangulationBase<13>::boundaryMap<9>() const;
template MatrixInt TriangulationBase<13>::boundaryMap<10>() const;
template MatrixInt TriangulationBase<13>::boundaryMap<11>() const;
template MatrixInt TriangulationBase<13>::boundaryMap<12>() const;
template MatrixInt TriangulationBase<13>::boundaryMap<13>() const;
template MatrixInt TriangulationBase<13>::dualBoundaryMap<1>() const;
template MatrixInt TriangulationBase<13>::dualBoundaryMap<2>() const;
template MatrixInt TriangulationBase<13>::dualBoundaryMap<3>() const;
template MatrixInt TriangulationBase<13>::dualBoundaryMap<4>() const;
template MatrixInt TriangulationBase<13>::dualBoundaryMap<5>() const;
template MatrixInt TriangulationBase<13>::dualBoundaryMap<6>() const;
template MatrixInt TriangulationBase<13>::dualBoundaryMap<7>() const;
template MatrixInt TriangulationBase<13>::dualBoundaryMap<8>() const;
template MatrixInt TriangulationBase<13>::dualBoundaryMap<9>() const;
template MatrixInt TriangulationBase<13>::dualBoundaryMap<10>() const;
template MatrixInt TriangulationBase<13>::dualBoundaryMap<11>() const;
template MatrixInt TriangulationBase<13>::dualBoundaryMap<12>() const;
template MatrixInt TriangulationBase<13>::dualToPrimal<0>() const;
template MatrixInt TriangulationBase<13>::dualToPrimal<1>() const;
template MatrixInt TriangulationBase<13>::dualToPrimal<2>() const;
template MatrixInt TriangulationBase<13>::dualToPrimal<3>() const;
template MatrixInt TriangulationBase<13>::dualToPrimal<4>() const;
template MatrixInt TriangulationBase<13>::dualToPrimal<5>() const;
template MatrixInt TriangulationBase<13>::dualToPrimal<6>() const;
template MatrixInt TriangulationBase<13>::dualToPrimal<7>() const;
template MatrixInt TriangulationBase<13>::dualToPrimal<8>() const;
template MatrixInt TriangulationBase<13>::dualToPrimal<9>() const;
template MatrixInt TriangulationBase<13>::dualToPrimal<10>() const;
template MatrixInt TriangulationBase<13>::dualToPrimal<11>() const;
template MatrixInt TriangulationBase<13>::dualToPrimal<12>() const;

template const GroupPresentation& TriangulationBase<13>::group() const;

template void TriangulationBase<13>::calculateSkeleton();
template void TriangulationBase<13>::cloneSkeleton(
    const TriangulationBase<13>&);
template void TriangulationBase<13>::clearBaseProperties();
template void TriangulationBase<13>::swapBaseData(TriangulationBase<13>&);

template bool TriangulationBase<13>::internalPachner(Face<13, 0>*, bool, bool);
template bool TriangulationBase<13>::internalPachner(Face<13, 1>*, bool, bool);
template bool TriangulationBase<13>::internalPachner(Face<13, 2>*, bool, bool);
template bool TriangulationBase<13>::internalPachner(Face<13, 3>*, bool, bool);
template bool TriangulationBase<13>::internalPachner(Face<13, 4>*, bool, bool);
template bool TriangulationBase<13>::internalPachner(Face<13, 5>*, bool, bool);
template bool TriangulationBase<13>::internalPachner(Face<13, 6>*, bool, bool);
template bool TriangulationBase<13>::internalPachner(Face<13, 7>*, bool, bool);
template bool TriangulationBase<13>::internalPachner(Face<13, 8>*, bool, bool);
template bool TriangulationBase<13>::internalPachner(Face<13, 9>*, bool, bool);
template bool TriangulationBase<13>::internalPachner(Face<13, 10>*, bool, bool);
template bool TriangulationBase<13>::internalPachner(Face<13, 11>*, bool, bool);
template bool TriangulationBase<13>::internalPachner(Face<13, 12>*, bool, bool);
template bool TriangulationBase<13>::internalPachner(Face<13, 13>*, bool, bool);

template bool TriangulationBase<13>::internal20(Face<13, 0>*, bool, bool);
template bool TriangulationBase<13>::internal20(Face<13, 1>*, bool, bool);
template bool TriangulationBase<13>::internal20(Face<13, 2>*, bool, bool);

template void TriangulationBase<13>::writeTextShort(std::ostream&) const;
template void TriangulationBase<13>::writeTextLong(std::ostream&) const;

template void TriangulationBase<13>::reorderBFS(bool);
template Triangulation<13> TriangulationBase<13>::doubleCover() const;
template Triangulation<13> TriangulationBase<13>::doubleOverBoundary() const;
template bool TriangulationBase<13>::makeIdeal();

template std::string TriangulationBase<13>::source(Language) const;
template void TriangulationBase<13>::writeDot(std::ostream&, bool) const;

} // namespace regina::detail
