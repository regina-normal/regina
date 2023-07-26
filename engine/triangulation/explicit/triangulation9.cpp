
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

template std::string TriangulationBase<9>::isoSigFrom
    <IsoSigPrintable<9>>(size_t, const Perm<10>&, Isomorphism<9>*) const;
template std::string TriangulationBase<9>::isoSig<
    IsoSigClassic<9>, IsoSigPrintable<9>>() const;
template std::pair<std::string, Isomorphism<9>>
    TriangulationBase<9>::isoSigDetail<
    IsoSigClassic<9>, IsoSigPrintable<9>>() const;
template Triangulation<9> TriangulationBase<9>::fromIsoSig(const std::string&);
template size_t TriangulationBase<9>::isoSigComponentSize(const std::string&);

template AbelianGroup TriangulationBase<9>::homology<1>() const;
template AbelianGroup TriangulationBase<9>::homology<2>() const;
template AbelianGroup TriangulationBase<9>::homology<3>() const;
template AbelianGroup TriangulationBase<9>::homology<4>() const;
template AbelianGroup TriangulationBase<9>::homology<5>() const;
template AbelianGroup TriangulationBase<9>::homology<6>() const;
template AbelianGroup TriangulationBase<9>::homology<7>() const;

template MatrixInt TriangulationBase<9>::boundaryMap<1>() const;
template MatrixInt TriangulationBase<9>::boundaryMap<2>() const;
template MatrixInt TriangulationBase<9>::boundaryMap<3>() const;
template MatrixInt TriangulationBase<9>::boundaryMap<4>() const;
template MatrixInt TriangulationBase<9>::boundaryMap<5>() const;
template MatrixInt TriangulationBase<9>::boundaryMap<6>() const;
template MatrixInt TriangulationBase<9>::boundaryMap<7>() const;
template MatrixInt TriangulationBase<9>::boundaryMap<8>() const;
template MatrixInt TriangulationBase<9>::boundaryMap<9>() const;
template MatrixInt TriangulationBase<9>::dualBoundaryMap<1>() const;
template MatrixInt TriangulationBase<9>::dualBoundaryMap<2>() const;
template MatrixInt TriangulationBase<9>::dualBoundaryMap<3>() const;
template MatrixInt TriangulationBase<9>::dualBoundaryMap<4>() const;
template MatrixInt TriangulationBase<9>::dualBoundaryMap<5>() const;
template MatrixInt TriangulationBase<9>::dualBoundaryMap<6>() const;
template MatrixInt TriangulationBase<9>::dualBoundaryMap<7>() const;
template MatrixInt TriangulationBase<9>::dualBoundaryMap<8>() const;
template MatrixInt TriangulationBase<9>::dualToPrimal<0>() const;
template MatrixInt TriangulationBase<9>::dualToPrimal<1>() const;
template MatrixInt TriangulationBase<9>::dualToPrimal<2>() const;
template MatrixInt TriangulationBase<9>::dualToPrimal<3>() const;
template MatrixInt TriangulationBase<9>::dualToPrimal<4>() const;
template MatrixInt TriangulationBase<9>::dualToPrimal<5>() const;
template MatrixInt TriangulationBase<9>::dualToPrimal<6>() const;
template MatrixInt TriangulationBase<9>::dualToPrimal<7>() const;
template MatrixInt TriangulationBase<9>::dualToPrimal<8>() const;

template const GroupPresentation& TriangulationBase<9>::group() const;

template void TriangulationBase<9>::calculateSkeleton();
template void TriangulationBase<9>::cloneSkeleton(const TriangulationBase<9>&);
template void TriangulationBase<9>::clearBaseProperties();
template void TriangulationBase<9>::swapBaseData(TriangulationBase<9>&);

template bool TriangulationBase<9>::pachner(Face<9, 0>*, bool, bool);
template bool TriangulationBase<9>::pachner(Face<9, 1>*, bool, bool);
template bool TriangulationBase<9>::pachner(Face<9, 2>*, bool, bool);
template bool TriangulationBase<9>::pachner(Face<9, 3>*, bool, bool);
template bool TriangulationBase<9>::pachner(Face<9, 4>*, bool, bool);
template bool TriangulationBase<9>::pachner(Face<9, 5>*, bool, bool);
template bool TriangulationBase<9>::pachner(Face<9, 6>*, bool, bool);
template bool TriangulationBase<9>::pachner(Face<9, 7>*, bool, bool);
template bool TriangulationBase<9>::pachner(Face<9, 8>*, bool, bool);
template bool TriangulationBase<9>::pachner(Face<9, 9>*, bool, bool);

template void TriangulationBase<9>::writeTextShort(std::ostream&) const;
template void TriangulationBase<9>::writeTextLong(std::ostream&) const;

template void TriangulationBase<9>::reorderBFS(bool);
template void TriangulationBase<9>::makeDoubleCover();
template bool TriangulationBase<9>::finiteToIdeal();

template std::string TriangulationBase<9>::source(Language) const;
template void TriangulationBase<9>::writeDot(std::ostream&, bool) const;

} // namespace regina::detail
