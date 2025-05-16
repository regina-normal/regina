
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

#include "triangulation/dim4.h"
#include "triangulation/isosigtype.h"
#include "triangulation/detail/isosig-impl.h"
#include "triangulation/detail/retriangulate-impl.h"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace regina {

namespace detail {
    /**
     * Provides domain-specific details for the 4-D retriangulation process.
     *
     * For propagation of 4-D triangulations, we do not make use of the
     * options type `Retriangulator::Options`.
     */
    template <>
    struct RetriangulateParams<Triangulation<4>> {
        static std::string sig(const Triangulation<4>& tri) {
            return tri.isoSig<IsoSigDegrees<4, 2>>();
        }

        static constexpr const char* progressStage = "Exploring triangulations";

        template <class Retriangulator>
        static void propagateFrom(const std::string& sig, size_t maxSize,
                Retriangulator* retriang) {
            Triangulation<4> t = Triangulation<4>::fromIsoSig(sig);
            size_t i;

            for (i = 0; i < t.countVertices(); ++i)
                if (auto alt = t.withPachner(t.vertex(i)))
                    if (retriang->candidate(std::move(*alt), sig))
                        return;

            for (i = 0; i < t.countEdges(); ++i)
                if (auto alt = t.withPachner(t.edge(i)))
                    if (retriang->candidate(std::move(*alt), sig))
                        return;

            for (i = 0; i < t.countTriangles(); ++i)
                if (auto alt = t.withPachner(t.triangle(i)))
                    if (retriang->candidate(std::move(*alt), sig))
                        return;

            if (t.size() + 2 <= maxSize)
                for (i = 0; i < t.countTetrahedra(); ++i)
                    if (auto alt = t.withPachner(t.tetrahedron(i)))
                        if (retriang->candidate(std::move(*alt), sig))
                            return;

            if (t.size() + 4 <= maxSize)
                for (i = 0; i < t.size(); ++i) {
                    // 1-5 moves are always legal.
                    Triangulation<4> alt(t, false, true);
                    alt.pachner(alt.pentachoron(i));
                    if (retriang->candidate(std::move(alt), sig))
                        return;
                }
        }
    };
} // namespace detail

// Instantiate all necessary retriangulation template functions
// so the full implementation can stay out of the headers.

template bool regina::detail::retriangulateInternal<Triangulation<4>, true>(
    const Triangulation<4>&, int, unsigned, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Triangulation<4>, true>&&);

template bool regina::detail::retriangulateInternal<Triangulation<4>, false>(
    const Triangulation<4>&, int, unsigned, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Triangulation<4>, false>&&);

} // namespace regina

