
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "triangulation/detail/algebra-impl.h"
#include "triangulation/detail/isosig-impl.h"
#include "triangulation/detail/moves-impl.h"
#include "triangulation/detail/skeleton-impl.h"
#include "triangulation/detail/triangulation-impl.h"
#include "triangulation/generic.h"

namespace regina::detail {

template std::string TriangulationBase<11>::isoSigFrom
    <IsoSigPrintable<11>>(size_t, const Perm<12>&, Isomorphism<11>*) const;
template std::string TriangulationBase<11>::isoSig<
    IsoSigClassic<11>, IsoSigPrintable<11>>() const;
template std::pair<std::string, Isomorphism<11>>
    TriangulationBase<11>::isoSigDetail<
    IsoSigClassic<11>, IsoSigPrintable<11>>() const;
template Triangulation<11> TriangulationBase<11>::fromIsoSig(
    const std::string&);
template size_t TriangulationBase<11>::isoSigComponentSize(const std::string&);

template AbelianGroup TriangulationBase<11>::homology<1>() const;
template AbelianGroup TriangulationBase<11>::homology<2>() const;
template AbelianGroup TriangulationBase<11>::homology<3>() const;
template AbelianGroup TriangulationBase<11>::homology<4>() const;
template AbelianGroup TriangulationBase<11>::homology<5>() const;
template AbelianGroup TriangulationBase<11>::homology<6>() const;
template AbelianGroup TriangulationBase<11>::homology<7>() const;
template AbelianGroup TriangulationBase<11>::homology<8>() const;
template AbelianGroup TriangulationBase<11>::homology<9>() const;

template MatrixInt TriangulationBase<11>::boundaryMap<1>() const;
template MatrixInt TriangulationBase<11>::boundaryMap<2>() const;
template MatrixInt TriangulationBase<11>::boundaryMap<3>() const;
template MatrixInt TriangulationBase<11>::boundaryMap<4>() const;
template MatrixInt TriangulationBase<11>::boundaryMap<5>() const;
template MatrixInt TriangulationBase<11>::boundaryMap<6>() const;
template MatrixInt TriangulationBase<11>::boundaryMap<7>() const;
template MatrixInt TriangulationBase<11>::boundaryMap<8>() const;
template MatrixInt TriangulationBase<11>::boundaryMap<9>() const;
template MatrixInt TriangulationBase<11>::boundaryMap<10>() const;
template MatrixInt TriangulationBase<11>::boundaryMap<11>() const;
template MatrixInt TriangulationBase<11>::dualBoundaryMap<1>() const;
template MatrixInt TriangulationBase<11>::dualBoundaryMap<2>() const;
template MatrixInt TriangulationBase<11>::dualBoundaryMap<3>() const;
template MatrixInt TriangulationBase<11>::dualBoundaryMap<4>() const;
template MatrixInt TriangulationBase<11>::dualBoundaryMap<5>() const;
template MatrixInt TriangulationBase<11>::dualBoundaryMap<6>() const;
template MatrixInt TriangulationBase<11>::dualBoundaryMap<7>() const;
template MatrixInt TriangulationBase<11>::dualBoundaryMap<8>() const;
template MatrixInt TriangulationBase<11>::dualBoundaryMap<9>() const;
template MatrixInt TriangulationBase<11>::dualBoundaryMap<10>() const;
template MatrixInt TriangulationBase<11>::dualToPrimal<0>() const;
template MatrixInt TriangulationBase<11>::dualToPrimal<1>() const;
template MatrixInt TriangulationBase<11>::dualToPrimal<2>() const;
template MatrixInt TriangulationBase<11>::dualToPrimal<3>() const;
template MatrixInt TriangulationBase<11>::dualToPrimal<4>() const;
template MatrixInt TriangulationBase<11>::dualToPrimal<5>() const;
template MatrixInt TriangulationBase<11>::dualToPrimal<6>() const;
template MatrixInt TriangulationBase<11>::dualToPrimal<7>() const;
template MatrixInt TriangulationBase<11>::dualToPrimal<8>() const;
template MatrixInt TriangulationBase<11>::dualToPrimal<9>() const;
template MatrixInt TriangulationBase<11>::dualToPrimal<10>() const;

template const GroupPresentation& TriangulationBase<11>::group() const;

template void TriangulationBase<11>::calculateSkeleton();
template void TriangulationBase<11>::cloneSkeleton(
    const TriangulationBase<11>&);
template void TriangulationBase<11>::clearBaseProperties();
template void TriangulationBase<11>::swapBaseData(TriangulationBase<11>&);

template bool TriangulationBase<11>::internalPachner(Face<11, 0>*, bool, bool);
template bool TriangulationBase<11>::internalPachner(Face<11, 1>*, bool, bool);
template bool TriangulationBase<11>::internalPachner(Face<11, 2>*, bool, bool);
template bool TriangulationBase<11>::internalPachner(Face<11, 3>*, bool, bool);
template bool TriangulationBase<11>::internalPachner(Face<11, 4>*, bool, bool);
template bool TriangulationBase<11>::internalPachner(Face<11, 5>*, bool, bool);
template bool TriangulationBase<11>::internalPachner(Face<11, 6>*, bool, bool);
template bool TriangulationBase<11>::internalPachner(Face<11, 7>*, bool, bool);
template bool TriangulationBase<11>::internalPachner(Face<11, 8>*, bool, bool);
template bool TriangulationBase<11>::internalPachner(Face<11, 9>*, bool, bool);
template bool TriangulationBase<11>::internalPachner(Face<11, 10>*, bool, bool);
template bool TriangulationBase<11>::internalPachner(Face<11, 11>*, bool, bool);

template bool TriangulationBase<11>::internal20(Face<11, 0>*, bool, bool);
template bool TriangulationBase<11>::internal20(Face<11, 1>*, bool, bool);
template bool TriangulationBase<11>::internal20(Face<11, 2>*, bool, bool);

template void TriangulationBase<11>::writeTextShort(std::ostream&) const;
template void TriangulationBase<11>::writeTextLong(std::ostream&) const;

template void TriangulationBase<11>::reorderBFS(bool);
template Triangulation<11> TriangulationBase<11>::doubleCover() const;
template bool TriangulationBase<11>::finiteToIdeal();

template std::string TriangulationBase<11>::source(Language) const;
template void TriangulationBase<11>::writeDot(std::ostream&, bool) const;

} // namespace regina::detail
