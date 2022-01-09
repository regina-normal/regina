
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include "triangulation/dim3.h"

namespace regina::detail {

template std::string TriangulationBase<3>::isoSigFrom
    <IsoSigPrintable<3>>(size_t, const Perm<4>&, Isomorphism<3>*) const;
template std::string TriangulationBase<3>::isoSig<
    IsoSigClassic<3>, IsoSigPrintable<3>>() const;
template std::pair<std::string, Isomorphism<3>>
    TriangulationBase<3>::isoSigDetail<
    IsoSigClassic<3>, IsoSigPrintable<3>>() const;
template Triangulation<3> TriangulationBase<3>::fromIsoSig(const std::string&);
template size_t TriangulationBase<3>::isoSigComponentSize(const std::string&);

template AbelianGroup TriangulationBase<3>::homology<1>() const;
template AbelianGroup TriangulationBase<3>::homology<2>() const;

template MatrixInt TriangulationBase<3>::boundaryMap<1>() const;
template MatrixInt TriangulationBase<3>::boundaryMap<2>() const;
template MatrixInt TriangulationBase<3>::boundaryMap<3>() const;
template MatrixInt TriangulationBase<3>::dualBoundaryMap<1>() const;
template MatrixInt TriangulationBase<3>::dualBoundaryMap<2>() const;
template MatrixInt TriangulationBase<3>::dualBoundaryMap<3>() const;

template const GroupPresentation& TriangulationBase<3>::fundamentalGroup() const;

template void TriangulationBase<3>::calculateSkeleton();
template void TriangulationBase<3>::clearBaseProperties();
template void TriangulationBase<3>::swapBaseData(TriangulationBase<3>&);

template bool TriangulationBase<3>::pachner(Face<3, 0>*, bool, bool);
template bool TriangulationBase<3>::pachner(Face<3, 1>*, bool, bool);
template bool TriangulationBase<3>::pachner(Face<3, 2>*, bool, bool);
template bool TriangulationBase<3>::pachner(Face<3, 3>*, bool, bool);

} // namespace regina::detail
