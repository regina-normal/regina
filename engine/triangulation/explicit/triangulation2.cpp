
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
#include "triangulation/dim2.h"

namespace regina::detail {

template std::string TriangulationBase<2>::isoSigFrom
    <IsoSigPrintable<2>>(size_t, const Perm<3>&, Isomorphism<2>*) const;
template std::string TriangulationBase<2>::isoSig<
    IsoSigClassic<2>, IsoSigPrintable<2>>() const;
template std::pair<std::string, Isomorphism<2>>
    TriangulationBase<2>::isoSigDetail<
    IsoSigClassic<2>, IsoSigPrintable<2>>() const;
template Triangulation<2> TriangulationBase<2>::fromIsoSig(const std::string&);
template size_t TriangulationBase<2>::isoSigComponentSize(const std::string&);

template AbelianGroup TriangulationBase<2>::homology<1>() const;

template MatrixInt TriangulationBase<2>::boundaryMap<1>() const;
template MatrixInt TriangulationBase<2>::boundaryMap<2>() const;
template MatrixInt TriangulationBase<2>::dualBoundaryMap<1>() const;
template MatrixInt TriangulationBase<2>::dualBoundaryMap<2>() const;
template MatrixInt TriangulationBase<2>::dualToPrimal<0>() const;
template MatrixInt TriangulationBase<2>::dualToPrimal<1>() const;

template const GroupPresentation& TriangulationBase<2>::group() const;

template void TriangulationBase<2>::calculateSkeleton();
template void TriangulationBase<2>::cloneSkeleton(const TriangulationBase<2>&);
template void TriangulationBase<2>::clearBaseProperties();
template void TriangulationBase<2>::swapBaseData(TriangulationBase<2>&);

template bool TriangulationBase<2>::internalPachner(Face<2, 0>*, bool, bool);
template bool TriangulationBase<2>::internalPachner(Face<2, 1>*, bool, bool);
template bool TriangulationBase<2>::internalPachner(Face<2, 2>*, bool, bool);

template bool TriangulationBase<2>::internal20(Face<2, 0>*, bool, bool);

template bool TriangulationBase<2>::internalShellBoundary(Simplex<2>*,
    bool, bool);

template void TriangulationBase<2>::writeTextShort(std::ostream&) const;
template void TriangulationBase<2>::writeTextLong(std::ostream&) const;

template void TriangulationBase<2>::reorderBFS(bool);
template Triangulation<2> TriangulationBase<2>::doubleCover() const;
template Triangulation<2> TriangulationBase<2>::doubleOverBoundary() const;
template void TriangulationBase<2>::subdivide();
template bool TriangulationBase<2>::finiteToIdeal();

template std::string TriangulationBase<2>::source(Language) const;
template void TriangulationBase<2>::writeDot(std::ostream&, bool) const;

} // namespace regina::detail
