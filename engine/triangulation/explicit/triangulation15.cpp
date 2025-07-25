
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

template std::string TriangulationBase<15>::isoSigFrom
    <IsoSigPrintable<15>>(size_t, const Perm<16>&, Isomorphism<15>*) const;
template std::string TriangulationBase<15>::isoSig<
    IsoSigClassic<15>, IsoSigPrintable<15>>() const;
template std::pair<std::string, Isomorphism<15>>
    TriangulationBase<15>::isoSigDetail<
    IsoSigClassic<15>, IsoSigPrintable<15>>() const;
template Triangulation<15> TriangulationBase<15>::fromIsoSig(
    const std::string&);
template size_t TriangulationBase<15>::isoSigComponentSize(const std::string&);

template AbelianGroup TriangulationBase<15>::homology<1>() const;
template AbelianGroup TriangulationBase<15>::homology<2>() const;
template AbelianGroup TriangulationBase<15>::homology<3>() const;
template AbelianGroup TriangulationBase<15>::homology<4>() const;
template AbelianGroup TriangulationBase<15>::homology<5>() const;
template AbelianGroup TriangulationBase<15>::homology<6>() const;
template AbelianGroup TriangulationBase<15>::homology<7>() const;
template AbelianGroup TriangulationBase<15>::homology<8>() const;
template AbelianGroup TriangulationBase<15>::homology<9>() const;
template AbelianGroup TriangulationBase<15>::homology<10>() const;
template AbelianGroup TriangulationBase<15>::homology<11>() const;
template AbelianGroup TriangulationBase<15>::homology<12>() const;
template AbelianGroup TriangulationBase<15>::homology<13>() const;

template MatrixInt TriangulationBase<15>::boundaryMap<1>() const;
template MatrixInt TriangulationBase<15>::boundaryMap<2>() const;
template MatrixInt TriangulationBase<15>::boundaryMap<3>() const;
template MatrixInt TriangulationBase<15>::boundaryMap<4>() const;
template MatrixInt TriangulationBase<15>::boundaryMap<5>() const;
template MatrixInt TriangulationBase<15>::boundaryMap<6>() const;
template MatrixInt TriangulationBase<15>::boundaryMap<7>() const;
template MatrixInt TriangulationBase<15>::boundaryMap<8>() const;
template MatrixInt TriangulationBase<15>::boundaryMap<9>() const;
template MatrixInt TriangulationBase<15>::boundaryMap<10>() const;
template MatrixInt TriangulationBase<15>::boundaryMap<11>() const;
template MatrixInt TriangulationBase<15>::boundaryMap<12>() const;
template MatrixInt TriangulationBase<15>::boundaryMap<13>() const;
template MatrixInt TriangulationBase<15>::boundaryMap<14>() const;
template MatrixInt TriangulationBase<15>::boundaryMap<15>() const;
template MatrixInt TriangulationBase<15>::dualBoundaryMap<1>() const;
template MatrixInt TriangulationBase<15>::dualBoundaryMap<2>() const;
template MatrixInt TriangulationBase<15>::dualBoundaryMap<3>() const;
template MatrixInt TriangulationBase<15>::dualBoundaryMap<4>() const;
template MatrixInt TriangulationBase<15>::dualBoundaryMap<5>() const;
template MatrixInt TriangulationBase<15>::dualBoundaryMap<6>() const;
template MatrixInt TriangulationBase<15>::dualBoundaryMap<7>() const;
template MatrixInt TriangulationBase<15>::dualBoundaryMap<8>() const;
template MatrixInt TriangulationBase<15>::dualBoundaryMap<9>() const;
template MatrixInt TriangulationBase<15>::dualBoundaryMap<10>() const;
template MatrixInt TriangulationBase<15>::dualBoundaryMap<11>() const;
template MatrixInt TriangulationBase<15>::dualBoundaryMap<12>() const;
template MatrixInt TriangulationBase<15>::dualBoundaryMap<13>() const;
template MatrixInt TriangulationBase<15>::dualBoundaryMap<14>() const;
template MatrixInt TriangulationBase<15>::dualToPrimal<0>() const;
template MatrixInt TriangulationBase<15>::dualToPrimal<1>() const;
template MatrixInt TriangulationBase<15>::dualToPrimal<2>() const;
template MatrixInt TriangulationBase<15>::dualToPrimal<3>() const;
template MatrixInt TriangulationBase<15>::dualToPrimal<4>() const;
template MatrixInt TriangulationBase<15>::dualToPrimal<5>() const;
template MatrixInt TriangulationBase<15>::dualToPrimal<6>() const;
template MatrixInt TriangulationBase<15>::dualToPrimal<7>() const;
template MatrixInt TriangulationBase<15>::dualToPrimal<8>() const;
template MatrixInt TriangulationBase<15>::dualToPrimal<9>() const;
template MatrixInt TriangulationBase<15>::dualToPrimal<10>() const;
template MatrixInt TriangulationBase<15>::dualToPrimal<11>() const;
template MatrixInt TriangulationBase<15>::dualToPrimal<12>() const;
template MatrixInt TriangulationBase<15>::dualToPrimal<13>() const;
template MatrixInt TriangulationBase<15>::dualToPrimal<14>() const;

template const GroupPresentation& TriangulationBase<15>::group() const;

template void TriangulationBase<15>::calculateSkeleton();
template void TriangulationBase<15>::cloneSkeleton(
    const TriangulationBase<15>&);
template void TriangulationBase<15>::clearBaseProperties();
template void TriangulationBase<15>::swapBaseData(TriangulationBase<15>&);

template bool TriangulationBase<15>::internalPachner(Face<15, 0>*, bool, bool);
template bool TriangulationBase<15>::internalPachner(Face<15, 1>*, bool, bool);
template bool TriangulationBase<15>::internalPachner(Face<15, 2>*, bool, bool);
template bool TriangulationBase<15>::internalPachner(Face<15, 3>*, bool, bool);
template bool TriangulationBase<15>::internalPachner(Face<15, 4>*, bool, bool);
template bool TriangulationBase<15>::internalPachner(Face<15, 5>*, bool, bool);
template bool TriangulationBase<15>::internalPachner(Face<15, 6>*, bool, bool);
template bool TriangulationBase<15>::internalPachner(Face<15, 7>*, bool, bool);
template bool TriangulationBase<15>::internalPachner(Face<15, 8>*, bool, bool);
template bool TriangulationBase<15>::internalPachner(Face<15, 9>*, bool, bool);
template bool TriangulationBase<15>::internalPachner(Face<15, 10>*, bool, bool);
template bool TriangulationBase<15>::internalPachner(Face<15, 11>*, bool, bool);
template bool TriangulationBase<15>::internalPachner(Face<15, 12>*, bool, bool);
template bool TriangulationBase<15>::internalPachner(Face<15, 13>*, bool, bool);
template bool TriangulationBase<15>::internalPachner(Face<15, 14>*, bool, bool);
template bool TriangulationBase<15>::internalPachner(Face<15, 15>*, bool, bool);

template bool TriangulationBase<15>::internal20(Face<15, 0>*, bool, bool);
template bool TriangulationBase<15>::internal20(Face<15, 1>*, bool, bool);
template bool TriangulationBase<15>::internal20(Face<15, 2>*, bool, bool);

template void TriangulationBase<15>::writeTextShort(std::ostream&) const;
template void TriangulationBase<15>::writeTextLong(std::ostream&) const;

template void TriangulationBase<15>::reorderBFS(bool);
template Triangulation<15> TriangulationBase<15>::doubleCover() const;
template Triangulation<15> TriangulationBase<15>::doubleOverBoundary() const;
template bool TriangulationBase<15>::finiteToIdeal();

template std::string TriangulationBase<15>::source(Language) const;
template void TriangulationBase<15>::writeDot(std::ostream&, bool) const;

} // namespace regina::detail
