
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
#include "triangulation/dim4.h"

namespace regina::detail {

template std::string TriangulationBase<4>::isoSigFrom
    <IsoSigPrintable<4>>(size_t, const Perm<5>&, Isomorphism<4>*) const;
template std::string TriangulationBase<4>::isoSig<
    IsoSigClassic<4>, IsoSigPrintable<4>>() const;
template std::pair<std::string, Isomorphism<4>>
    TriangulationBase<4>::isoSigDetail<
    IsoSigClassic<4>, IsoSigPrintable<4>>() const;
template Triangulation<4> TriangulationBase<4>::fromIsoSig(const std::string&);
template size_t TriangulationBase<4>::isoSigComponentSize(const std::string&);

template AbelianGroup TriangulationBase<4>::homology<1>() const;
template AbelianGroup TriangulationBase<4>::homology<2>() const;
template AbelianGroup TriangulationBase<4>::homology<3>() const;

template MatrixInt TriangulationBase<4>::boundaryMap<1>() const;
template MatrixInt TriangulationBase<4>::boundaryMap<2>() const;
template MatrixInt TriangulationBase<4>::boundaryMap<3>() const;
template MatrixInt TriangulationBase<4>::boundaryMap<4>() const;
template MatrixInt TriangulationBase<4>::dualBoundaryMap<1>() const;
template MatrixInt TriangulationBase<4>::dualBoundaryMap<2>() const;
template MatrixInt TriangulationBase<4>::dualBoundaryMap<3>() const;
template MatrixInt TriangulationBase<4>::dualBoundaryMap<4>() const;
template MatrixInt TriangulationBase<4>::dualToPrimal<0>() const;
template MatrixInt TriangulationBase<4>::dualToPrimal<1>() const;
template MatrixInt TriangulationBase<4>::dualToPrimal<2>() const;
template MatrixInt TriangulationBase<4>::dualToPrimal<3>() const;

template const GroupPresentation& TriangulationBase<4>::group(bool) const;

template void TriangulationBase<4>::calculateSkeleton();
template void TriangulationBase<4>::cloneSkeleton(const TriangulationBase<4>&);
template void TriangulationBase<4>::clearBaseProperties();
template void TriangulationBase<4>::swapBaseData(TriangulationBase<4>&);

template bool TriangulationBase<4>::pachner(Face<4, 0>*, bool, bool);
template bool TriangulationBase<4>::pachner(Face<4, 1>*, bool, bool);
template bool TriangulationBase<4>::pachner(Face<4, 2>*, bool, bool);
template bool TriangulationBase<4>::pachner(Face<4, 3>*, bool, bool);
template bool TriangulationBase<4>::pachner(Face<4, 4>*, bool, bool);

template void TriangulationBase<4>::writeTextShort(std::ostream&) const;
template void TriangulationBase<4>::writeTextLong(std::ostream&) const;

template void TriangulationBase<4>::reorderBFS(bool);
template void TriangulationBase<4>::makeDoubleCover();
template void TriangulationBase<4>::subdivide();
template bool TriangulationBase<4>::finiteToIdeal();

template void TriangulationBase<4>::writeDot(std::ostream&, bool) const;

} // namespace regina::detail
