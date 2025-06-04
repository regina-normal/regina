
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

template std::string TriangulationBase<7>::isoSigFrom
    <IsoSigPrintable<7>>(size_t, const Perm<8>&, Isomorphism<7>*) const;
template std::string TriangulationBase<7>::isoSig<
    IsoSigClassic<7>, IsoSigPrintable<7>>() const;
template std::pair<std::string, Isomorphism<7>>
    TriangulationBase<7>::isoSigDetail<
    IsoSigClassic<7>, IsoSigPrintable<7>>() const;
template Triangulation<7> TriangulationBase<7>::fromIsoSig(const std::string&);
template size_t TriangulationBase<7>::isoSigComponentSize(const std::string&);

template AbelianGroup TriangulationBase<7>::homology<1>() const;
template AbelianGroup TriangulationBase<7>::homology<2>() const;
template AbelianGroup TriangulationBase<7>::homology<3>() const;
template AbelianGroup TriangulationBase<7>::homology<4>() const;
template AbelianGroup TriangulationBase<7>::homology<5>() const;

template MatrixInt TriangulationBase<7>::boundaryMap<1>() const;
template MatrixInt TriangulationBase<7>::boundaryMap<2>() const;
template MatrixInt TriangulationBase<7>::boundaryMap<3>() const;
template MatrixInt TriangulationBase<7>::boundaryMap<4>() const;
template MatrixInt TriangulationBase<7>::boundaryMap<5>() const;
template MatrixInt TriangulationBase<7>::boundaryMap<6>() const;
template MatrixInt TriangulationBase<7>::boundaryMap<7>() const;
template MatrixInt TriangulationBase<7>::dualBoundaryMap<1>() const;
template MatrixInt TriangulationBase<7>::dualBoundaryMap<2>() const;
template MatrixInt TriangulationBase<7>::dualBoundaryMap<3>() const;
template MatrixInt TriangulationBase<7>::dualBoundaryMap<4>() const;
template MatrixInt TriangulationBase<7>::dualBoundaryMap<5>() const;
template MatrixInt TriangulationBase<7>::dualBoundaryMap<6>() const;
template MatrixInt TriangulationBase<7>::dualToPrimal<0>() const;
template MatrixInt TriangulationBase<7>::dualToPrimal<1>() const;
template MatrixInt TriangulationBase<7>::dualToPrimal<2>() const;
template MatrixInt TriangulationBase<7>::dualToPrimal<3>() const;
template MatrixInt TriangulationBase<7>::dualToPrimal<4>() const;
template MatrixInt TriangulationBase<7>::dualToPrimal<5>() const;
template MatrixInt TriangulationBase<7>::dualToPrimal<6>() const;

template const GroupPresentation& TriangulationBase<7>::group() const;

template void TriangulationBase<7>::calculateSkeleton();
template void TriangulationBase<7>::cloneSkeleton(const TriangulationBase<7>&);
template void TriangulationBase<7>::clearBaseProperties();
template void TriangulationBase<7>::swapBaseData(TriangulationBase<7>&);

template bool TriangulationBase<7>::internalPachner(Face<7, 0>*, bool, bool);
template bool TriangulationBase<7>::internalPachner(Face<7, 1>*, bool, bool);
template bool TriangulationBase<7>::internalPachner(Face<7, 2>*, bool, bool);
template bool TriangulationBase<7>::internalPachner(Face<7, 3>*, bool, bool);
template bool TriangulationBase<7>::internalPachner(Face<7, 4>*, bool, bool);
template bool TriangulationBase<7>::internalPachner(Face<7, 5>*, bool, bool);
template bool TriangulationBase<7>::internalPachner(Face<7, 6>*, bool, bool);
template bool TriangulationBase<7>::internalPachner(Face<7, 7>*, bool, bool);

template bool TriangulationBase<7>::internal20(Face<7, 0>*, bool, bool);
template bool TriangulationBase<7>::internal20(Face<7, 1>*, bool, bool);
template bool TriangulationBase<7>::internal20(Face<7, 2>*, bool, bool);

template void TriangulationBase<7>::writeTextShort(std::ostream&) const;
template void TriangulationBase<7>::writeTextLong(std::ostream&) const;

template void TriangulationBase<7>::reorderBFS(bool);
template Triangulation<7> TriangulationBase<7>::doubleCover() const;
template Triangulation<7> TriangulationBase<7>::doubleOverBoundary() const;
template bool TriangulationBase<7>::finiteToIdeal();

template std::string TriangulationBase<7>::source(Language) const;
template void TriangulationBase<7>::writeDot(std::ostream&, bool) const;

} // namespace regina::detail
