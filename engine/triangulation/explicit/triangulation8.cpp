
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

template std::string TriangulationBase<8>::isoSigFrom
    <IsoSigPrintable<8>>(size_t, const Perm<9>&, Isomorphism<8>*) const;
template std::string TriangulationBase<8>::isoSig<
    IsoSigClassic<8>, IsoSigPrintable<8>>() const;
template std::pair<std::string, Isomorphism<8>>
    TriangulationBase<8>::isoSigDetail<
    IsoSigClassic<8>, IsoSigPrintable<8>>() const;
template Triangulation<8> TriangulationBase<8>::fromIsoSig(const std::string&);
template size_t TriangulationBase<8>::isoSigComponentSize(const std::string&);

template AbelianGroup TriangulationBase<8>::homology<1>() const;
template AbelianGroup TriangulationBase<8>::homology<2>() const;
template AbelianGroup TriangulationBase<8>::homology<3>() const;
template AbelianGroup TriangulationBase<8>::homology<4>() const;
template AbelianGroup TriangulationBase<8>::homology<5>() const;
template AbelianGroup TriangulationBase<8>::homology<6>() const;

template MatrixInt TriangulationBase<8>::boundaryMap<1>() const;
template MatrixInt TriangulationBase<8>::boundaryMap<2>() const;
template MatrixInt TriangulationBase<8>::boundaryMap<3>() const;
template MatrixInt TriangulationBase<8>::boundaryMap<4>() const;
template MatrixInt TriangulationBase<8>::boundaryMap<5>() const;
template MatrixInt TriangulationBase<8>::boundaryMap<6>() const;
template MatrixInt TriangulationBase<8>::boundaryMap<7>() const;
template MatrixInt TriangulationBase<8>::boundaryMap<8>() const;
template MatrixInt TriangulationBase<8>::dualBoundaryMap<1>() const;
template MatrixInt TriangulationBase<8>::dualBoundaryMap<2>() const;
template MatrixInt TriangulationBase<8>::dualBoundaryMap<3>() const;
template MatrixInt TriangulationBase<8>::dualBoundaryMap<4>() const;
template MatrixInt TriangulationBase<8>::dualBoundaryMap<5>() const;
template MatrixInt TriangulationBase<8>::dualBoundaryMap<6>() const;
template MatrixInt TriangulationBase<8>::dualBoundaryMap<7>() const;
template MatrixInt TriangulationBase<8>::dualToPrimal<0>() const;
template MatrixInt TriangulationBase<8>::dualToPrimal<1>() const;
template MatrixInt TriangulationBase<8>::dualToPrimal<2>() const;
template MatrixInt TriangulationBase<8>::dualToPrimal<3>() const;
template MatrixInt TriangulationBase<8>::dualToPrimal<4>() const;
template MatrixInt TriangulationBase<8>::dualToPrimal<5>() const;
template MatrixInt TriangulationBase<8>::dualToPrimal<6>() const;
template MatrixInt TriangulationBase<8>::dualToPrimal<7>() const;

template const GroupPresentation& TriangulationBase<8>::group() const;

template void TriangulationBase<8>::calculateSkeleton();
template void TriangulationBase<8>::cloneSkeleton(const TriangulationBase<8>&);
template void TriangulationBase<8>::clearBaseProperties();
template void TriangulationBase<8>::swapBaseData(TriangulationBase<8>&);

template bool TriangulationBase<8>::internalPachner(Face<8, 0>*, bool, bool);
template bool TriangulationBase<8>::internalPachner(Face<8, 1>*, bool, bool);
template bool TriangulationBase<8>::internalPachner(Face<8, 2>*, bool, bool);
template bool TriangulationBase<8>::internalPachner(Face<8, 3>*, bool, bool);
template bool TriangulationBase<8>::internalPachner(Face<8, 4>*, bool, bool);
template bool TriangulationBase<8>::internalPachner(Face<8, 5>*, bool, bool);
template bool TriangulationBase<8>::internalPachner(Face<8, 6>*, bool, bool);
template bool TriangulationBase<8>::internalPachner(Face<8, 7>*, bool, bool);
template bool TriangulationBase<8>::internalPachner(Face<8, 8>*, bool, bool);

template bool TriangulationBase<8>::internal20(Face<8, 0>*, bool, bool);
template bool TriangulationBase<8>::internal20(Face<8, 1>*, bool, bool);
template bool TriangulationBase<8>::internal20(Face<8, 2>*, bool, bool);

template void TriangulationBase<8>::writeTextShort(std::ostream&) const;
template void TriangulationBase<8>::writeTextLong(std::ostream&) const;

template void TriangulationBase<8>::reorderBFS(bool);
template Triangulation<8> TriangulationBase<8>::doubleCover() const;
template Triangulation<8> TriangulationBase<8>::doubleOverBoundary() const;
template bool TriangulationBase<8>::finiteToIdeal();

template std::string TriangulationBase<8>::source(Language) const;
template void TriangulationBase<8>::writeDot(std::ostream&, bool) const;

} // namespace regina::detail
