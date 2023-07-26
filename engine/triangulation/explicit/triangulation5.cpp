
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

template std::string TriangulationBase<5>::isoSigFrom
    <IsoSigPrintable<5>>(size_t, const Perm<6>&, Isomorphism<5>*) const;
template std::string TriangulationBase<5>::isoSig<
    IsoSigClassic<5>, IsoSigPrintable<5>>() const;
template std::pair<std::string, Isomorphism<5>>
    TriangulationBase<5>::isoSigDetail<
    IsoSigClassic<5>, IsoSigPrintable<5>>() const;
template Triangulation<5> TriangulationBase<5>::fromIsoSig(const std::string&);
template size_t TriangulationBase<5>::isoSigComponentSize(const std::string&);

template AbelianGroup TriangulationBase<5>::homology<1>() const;
template AbelianGroup TriangulationBase<5>::homology<2>() const;
template AbelianGroup TriangulationBase<5>::homology<3>() const;

template MatrixInt TriangulationBase<5>::boundaryMap<1>() const;
template MatrixInt TriangulationBase<5>::boundaryMap<2>() const;
template MatrixInt TriangulationBase<5>::boundaryMap<3>() const;
template MatrixInt TriangulationBase<5>::boundaryMap<4>() const;
template MatrixInt TriangulationBase<5>::boundaryMap<5>() const;
template MatrixInt TriangulationBase<5>::dualBoundaryMap<1>() const;
template MatrixInt TriangulationBase<5>::dualBoundaryMap<2>() const;
template MatrixInt TriangulationBase<5>::dualBoundaryMap<3>() const;
template MatrixInt TriangulationBase<5>::dualBoundaryMap<4>() const;
template MatrixInt TriangulationBase<5>::dualToPrimal<0>() const;
template MatrixInt TriangulationBase<5>::dualToPrimal<1>() const;
template MatrixInt TriangulationBase<5>::dualToPrimal<2>() const;
template MatrixInt TriangulationBase<5>::dualToPrimal<3>() const;
template MatrixInt TriangulationBase<5>::dualToPrimal<4>() const;

template const GroupPresentation& TriangulationBase<5>::group() const;

template void TriangulationBase<5>::calculateSkeleton();
template void TriangulationBase<5>::cloneSkeleton(const TriangulationBase<5>&);
template void TriangulationBase<5>::clearBaseProperties();
template void TriangulationBase<5>::swapBaseData(TriangulationBase<5>&);

template bool TriangulationBase<5>::pachner(Face<5, 0>*, bool, bool);
template bool TriangulationBase<5>::pachner(Face<5, 1>*, bool, bool);
template bool TriangulationBase<5>::pachner(Face<5, 2>*, bool, bool);
template bool TriangulationBase<5>::pachner(Face<5, 3>*, bool, bool);
template bool TriangulationBase<5>::pachner(Face<5, 4>*, bool, bool);
template bool TriangulationBase<5>::pachner(Face<5, 5>*, bool, bool);

template void TriangulationBase<5>::writeTextShort(std::ostream&) const;
template void TriangulationBase<5>::writeTextLong(std::ostream&) const;

template void TriangulationBase<5>::reorderBFS(bool);
template void TriangulationBase<5>::makeDoubleCover();
template bool TriangulationBase<5>::finiteToIdeal();

template std::string TriangulationBase<5>::source(Language) const;
template void TriangulationBase<5>::writeDot(std::ostream&, bool) const;

} // namespace regina::detail
