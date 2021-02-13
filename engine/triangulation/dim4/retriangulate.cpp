
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

#include "triangulation/dim4.h"
#include "triangulation/detail/retriangulate-impl.h"

namespace regina {

namespace detail {
    template <>
    struct RetriangulateParams<Triangulation<4>> {
        static std::string sig(const Triangulation<4>& tri) {
            return tri.isoSig();
        }

        static constexpr const char* progressStage = "Exploring triangulations";

        static bool satisfiesPreconditions(const Triangulation<4>&) {
            return true;
        }

        template <class Retriangulator>
        static void propagateFrom(const std::string& sig, size_t maxSize,
                Retriangulator* retriang) {
            Triangulation<4>* t = Triangulation<4>::fromIsoSig(sig);
            size_t i;

            for (i = 0; i < t->countVertices(); ++i)
                if (t->pachner(t->vertex(i), true, false)) {
                    Triangulation<4> alt(*t, false);
                    alt.pachner(alt.vertex(i), false, true);
                    if (retriang->candidate(alt, sig)) {
                        delete t;
                        return;
                    }
                    // We cannot use alt from here, since candidate() might
                    // have changed it.
                }

            for (i = 0; i < t->countEdges(); ++i)
                if (t->pachner(t->edge(i), true, false)) {
                    Triangulation<4> alt(*t, false);
                    alt.pachner(alt.edge(i), false, true);
                    if (retriang->candidate(alt, sig)) {
                        delete t;
                        return;
                    }
                    // We cannot use alt from here, since candidate() might
                    // have changed it.
                }

            for (i = 0; i < t->countTriangles(); ++i)
                if (t->pachner(t->triangle(i), true, false)) {
                    Triangulation<4> alt(*t, false);
                    alt.pachner(alt.triangle(i), false, true);
                    if (retriang->candidate(alt, sig)) {
                        delete t;
                        return;
                    }
                    // We cannot use alt from here, since candidate() might
                    // have changed it.
                }

            if (t->size() + 2 <= maxSize)
                for (i = 0; i < t->countTetrahedra(); ++i)
                    if (t->pachner(t->tetrahedron(i), true, false)) {
                        Triangulation<4> alt(*t, false);
                        alt.pachner(alt.tetrahedron(i), false, true);
                        if (retriang->candidate(alt, sig)) {
                            delete t;
                            return;
                        }
                        // We cannot use alt from here, since candidate() might
                        // have changed it.
                    }

            if (t->size() + 4 <= maxSize)
                for (i = 0; i < t->size(); ++i) {
                    // 1-5 moves are always legal.
                    Triangulation<4> alt(*t, false);
                    alt.pachner(alt.pentachoron(i), true, true);
                    if (retriang->candidate(alt, sig)) {
                        delete t;
                        return;
                    }
                    // We cannot use alt from here, since candidate() might
                    // have changed it.
                }

            delete t;
        }
    };
} // namespace detail

template <bool withSig>
bool Triangulation<4>::retriangulateInternal(int height, unsigned nThreads,
        ProgressTrackerOpen* tracker,
        regina::detail::RetriangulateActionFunc<
            Triangulation<4>, withSig>&& action) const {
    if (tracker) {
        try {
            std::thread(&regina::detail::enumerate<Triangulation<4>, withSig>,
                *this, height, nThreads, tracker, std::move(action)).detach();
            return true;
        } catch (const std::system_error& e) {
            return false;
        }
    } else
        return regina::detail::enumerate<Triangulation<4>, withSig>(
            *this, height, nThreads, tracker, std::move(action));
}

// Instantiate all retriangulateInternal() template functions
// so the full implementation can stay out of the headers.

template REGINA_API bool Triangulation<4>::retriangulateInternal<true>(
    int, unsigned, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Triangulation<4>, true>&&) const;

template REGINA_API bool Triangulation<4>::retriangulateInternal<false>(
    int, unsigned, ProgressTrackerOpen*,
    regina::detail::RetriangulateActionFunc<Triangulation<4>, false>&&) const;

bool Triangulation<4>::simplifyExhaustive(int height, unsigned nThreads,
        ProgressTrackerOpen* tracker) {
    return retriangulate(height, nThreads, tracker,
        [](Triangulation<4>& alt, Triangulation<4>& original, size_t minSimp) {
            if (alt.size() < minSimp) {
                // Since we are allowed to change alt, we use swap(),
                // which avoids yet another round of remaking the tetrahedron
                // gluings.
                Packet::ChangeEventSpan span(&original);
                original.swap(alt);
                original.intelligentSimplify();
                return true;
            } else
                return false;
        },
        std::ref(*this), size());
}

} // namespace regina

