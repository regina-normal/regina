
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "triangulation/detail/triangulation-impl.h"
#include "triangulation/generic.h"

namespace regina::detail {

template std::string TriangulationBase<14>::isoSigFrom
    <IsoSigPrintable<14>>(size_t, const Perm<15>&, Isomorphism<14>*) const;
template std::string TriangulationBase<14>::isoSig<
    IsoSigClassic<14>, IsoSigPrintable<14>>() const;
template std::pair<std::string, Isomorphism<14>>
    TriangulationBase<14>::isoSigDetail<
    IsoSigClassic<14>, IsoSigPrintable<14>>() const;
template Triangulation<14> TriangulationBase<14>::fromIsoSig(
    const std::string&);
template size_t TriangulationBase<14>::isoSigComponentSize(const std::string&);

template AbelianGroup TriangulationBase<14>::homology<1>() const;
template AbelianGroup TriangulationBase<14>::homology<2>() const;
template AbelianGroup TriangulationBase<14>::homology<3>() const;
template AbelianGroup TriangulationBase<14>::homology<4>() const;
template AbelianGroup TriangulationBase<14>::homology<5>() const;
template AbelianGroup TriangulationBase<14>::homology<6>() const;
template AbelianGroup TriangulationBase<14>::homology<7>() const;
template AbelianGroup TriangulationBase<14>::homology<8>() const;
template AbelianGroup TriangulationBase<14>::homology<9>() const;
template AbelianGroup TriangulationBase<14>::homology<10>() const;
template AbelianGroup TriangulationBase<14>::homology<11>() const;
template AbelianGroup TriangulationBase<14>::homology<12>() const;

template MatrixInt TriangulationBase<14>::boundaryMap<1>() const;
template MatrixInt TriangulationBase<14>::boundaryMap<2>() const;
template MatrixInt TriangulationBase<14>::boundaryMap<3>() const;
template MatrixInt TriangulationBase<14>::boundaryMap<4>() const;
template MatrixInt TriangulationBase<14>::boundaryMap<5>() const;
template MatrixInt TriangulationBase<14>::boundaryMap<6>() const;
template MatrixInt TriangulationBase<14>::boundaryMap<7>() const;
template MatrixInt TriangulationBase<14>::boundaryMap<8>() const;
template MatrixInt TriangulationBase<14>::boundaryMap<9>() const;
template MatrixInt TriangulationBase<14>::boundaryMap<10>() const;
template MatrixInt TriangulationBase<14>::boundaryMap<11>() const;
template MatrixInt TriangulationBase<14>::boundaryMap<12>() const;
template MatrixInt TriangulationBase<14>::boundaryMap<13>() const;
template MatrixInt TriangulationBase<14>::boundaryMap<14>() const;
template MatrixInt TriangulationBase<14>::dualBoundaryMap<1>() const;
template MatrixInt TriangulationBase<14>::dualBoundaryMap<2>() const;
template MatrixInt TriangulationBase<14>::dualBoundaryMap<3>() const;
template MatrixInt TriangulationBase<14>::dualBoundaryMap<4>() const;
template MatrixInt TriangulationBase<14>::dualBoundaryMap<5>() const;
template MatrixInt TriangulationBase<14>::dualBoundaryMap<6>() const;
template MatrixInt TriangulationBase<14>::dualBoundaryMap<7>() const;
template MatrixInt TriangulationBase<14>::dualBoundaryMap<8>() const;
template MatrixInt TriangulationBase<14>::dualBoundaryMap<9>() const;
template MatrixInt TriangulationBase<14>::dualBoundaryMap<10>() const;
template MatrixInt TriangulationBase<14>::dualBoundaryMap<11>() const;
template MatrixInt TriangulationBase<14>::dualBoundaryMap<12>() const;
template MatrixInt TriangulationBase<14>::dualBoundaryMap<13>() const;
template MatrixInt TriangulationBase<14>::dualToPrimal<0>() const;
template MatrixInt TriangulationBase<14>::dualToPrimal<1>() const;
template MatrixInt TriangulationBase<14>::dualToPrimal<2>() const;
template MatrixInt TriangulationBase<14>::dualToPrimal<3>() const;
template MatrixInt TriangulationBase<14>::dualToPrimal<4>() const;
template MatrixInt TriangulationBase<14>::dualToPrimal<5>() const;
template MatrixInt TriangulationBase<14>::dualToPrimal<6>() const;
template MatrixInt TriangulationBase<14>::dualToPrimal<7>() const;
template MatrixInt TriangulationBase<14>::dualToPrimal<8>() const;
template MatrixInt TriangulationBase<14>::dualToPrimal<9>() const;
template MatrixInt TriangulationBase<14>::dualToPrimal<10>() const;
template MatrixInt TriangulationBase<14>::dualToPrimal<11>() const;
template MatrixInt TriangulationBase<14>::dualToPrimal<12>() const;
template MatrixInt TriangulationBase<14>::dualToPrimal<13>() const;

template const GroupPresentation& TriangulationBase<14>::group() const;

template void TriangulationBase<14>::calculateSkeleton();
template void TriangulationBase<14>::cloneSkeleton(
    const TriangulationBase<14>&);
template void TriangulationBase<14>::clearBaseProperties();
template void TriangulationBase<14>::swapBaseData(TriangulationBase<14>&);

template bool TriangulationBase<14>::pachner(Face<14, 0>*, bool, bool);
template bool TriangulationBase<14>::pachner(Face<14, 1>*, bool, bool);
template bool TriangulationBase<14>::pachner(Face<14, 2>*, bool, bool);
template bool TriangulationBase<14>::pachner(Face<14, 3>*, bool, bool);
template bool TriangulationBase<14>::pachner(Face<14, 4>*, bool, bool);
template bool TriangulationBase<14>::pachner(Face<14, 5>*, bool, bool);
template bool TriangulationBase<14>::pachner(Face<14, 6>*, bool, bool);
template bool TriangulationBase<14>::pachner(Face<14, 7>*, bool, bool);
template bool TriangulationBase<14>::pachner(Face<14, 8>*, bool, bool);
template bool TriangulationBase<14>::pachner(Face<14, 9>*, bool, bool);
template bool TriangulationBase<14>::pachner(Face<14, 10>*, bool, bool);
template bool TriangulationBase<14>::pachner(Face<14, 11>*, bool, bool);
template bool TriangulationBase<14>::pachner(Face<14, 12>*, bool, bool);
template bool TriangulationBase<14>::pachner(Face<14, 13>*, bool, bool);
template bool TriangulationBase<14>::pachner(Face<14, 14>*, bool, bool);

template void TriangulationBase<14>::writeTextShort(std::ostream&) const;
template void TriangulationBase<14>::writeTextLong(std::ostream&) const;

template void TriangulationBase<14>::reorderBFS(bool);
template void TriangulationBase<14>::makeDoubleCover();
template bool TriangulationBase<14>::finiteToIdeal();

template std::string TriangulationBase<14>::source(Language) const;
template void TriangulationBase<14>::writeDot(std::ostream&, bool) const;

} // namespace regina::detail
