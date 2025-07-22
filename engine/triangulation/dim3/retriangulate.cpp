
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

#include "triangulation/dim3.h"
#include "triangulation/isosigtype.h"
#include "triangulation/detail/isosig-impl.h"
#include "triangulation/detail/retriangulate-impl.h"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace regina {

namespace detail {
    /**
     * Provides domain-specific details for the 3-D retriangulation process.
     *
     * For propagation of 3-D triangulations, we do not make use of the
     * options type `Retriangulator::PropagationOptions`.
     */
    template <>
    struct RetriangulateParams<Triangulation<3>> {
        static std::string sig(const Triangulation<3>& tri) {
            return tri.isoSig<IsoSigEdgeDegrees<3>>();
        }

        static std::string rigidSig(const Triangulation<3>& tri) {
            // Currently rigidity is not supported for triangulations.
            return tri.isoSig<IsoSigEdgeDegrees<3>>();
        }

        static constexpr const char* progressStage = "Exploring triangulations";

        template <class Retriangulator>
        static void propagateFrom(const std::string& sig, size_t maxSize,
                Retriangulator* retriang) {
            Triangulation<3> t = Triangulation<3>::fromIsoSig(sig);
            size_t i;
            for (i = 0; i < t.countEdges(); ++i)
                if (auto alt = t.withPachner(t.edge(i)))
                    if (retriang->candidate(std::move(*alt), sig))
                        return;

            if (t.size() < maxSize)
                for (i = 0; i < t.countTriangles(); ++i)
                    if (auto alt = t.withPachner(t.triangle(i)))
                        if (retriang->candidate(std::move(*alt), sig))
                            return;
        }
    };
} // namespace detail

// Instantiate all necessary retriangulation template functions
// so the full implementation can stay out of the headers.

template bool regina::detail::retriangulateInternal<Triangulation<3>, true>(
    const Triangulation<3>&, bool, int, int, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Triangulation<3>, true>&&);

template bool regina::detail::retriangulateInternal<Triangulation<3>, false>(
    const Triangulation<3>&, bool, int, int, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Triangulation<3>, false>&&);

// These could be inlined, but we pull them out of the header because otherwise
// they interfere with Link's rewrite() implementation.
//
// Basically the problem is that link/rewrite.cpp includes retriangulate-impl.h,
// which then complains about the inlined Triangulation<3> code using an
// undefined specialisation RetriangulateParams<Triangulation<3>>.
//
// This would not have been a problem if link.h did not have to include
// triangulation/dim3.h (which is included just to inline Link::complement()).

bool Triangulation<3>::simplifyExhaustive(int height, int threads,
        ProgressTrackerOpen* tracker) {
    if (countComponents() > 1) {
        if (tracker)
            tracker->setFinished();
        throw FailedPrecondition(
            "simplifyExhaustive() requires a connected triangulation");
    }

    return regina::detail::simplifyExhaustiveInternal<Triangulation<3>>(
        *this, height, threads, tracker);
}

bool Triangulation<3>::improveTreewidth(ssize_t maxAttempts, int height,
        int threads, ProgressTrackerOpen* tracker) {
    if (countComponents() > 1) {
        if (tracker)
            tracker->setFinished();
        throw FailedPrecondition(
            "improveTreewidth() requires a connected triangulation");
    }

    return regina::detail::improveTreewidthInternal<Triangulation<3>>(
        *this, maxAttempts, height, threads, tracker);
}

} // namespace regina

