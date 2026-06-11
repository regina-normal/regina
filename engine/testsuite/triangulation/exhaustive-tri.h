
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#ifndef __TESTEXHAUSTIVE_H
#define __TESTEXHAUSTIVE_H

#include "census/census.h"
#include "census/gluingpermsearcher2.h"
#include "census/gluingpermsearcher3.h"
#include "census/gluingpermsearcher4.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

// ------------------------------ 2-D ------------------------------

/**
 * Run the given test over all triangulations from a census of
 * closed 2-manifold triangulations that is generated on the fly.
 *
 * \param action the test to run.  This should be a callable object that takes
 * two arguments: a triangulation, and a human-readable triangulation name.
 */
template <std::invocable<regina::Triangulation<2>&&, const char*> Action>
inline void runCensusAllClosed(Action&& action) {
    regina::FacetPairing<2>::findAllPairings(
            8 /* size */, false /* bounded */, -1 /* bdry faces */,
            [&action](const regina::FacetPairing<2>& p,
                regina::FacetPairing<2>::IsoList autos) {
        regina::GluingPermSearcher<2>::findAllPerms(p, std::move(autos),
                false /* orbl only */,
                [&action](const regina::GluingPerms<2>& g) {
            auto t = g.triangulate();
            std::invoke(std::forward<Action>(action), t, t.neoSig().c_str());
        });
    });
}

/**
 * Run the given test over all triangulations from a census of
 * 2-manifold triangulations with boundary that is generated on the fly.
 *
 * \param action the test to run.  This should be a callable object that takes
 * two arguments: a triangulation, and a human-readable triangulation name.
 */
template <std::invocable<regina::Triangulation<2>&&, const char*> Action>
inline void runCensusAllBounded(Action&& action) {
    regina::FacetPairing<2>::findAllPairings(
            6 /* size */, true /* bounded */, -1 /* bdry faces */,
            [&action](const regina::FacetPairing<2>& p,
                regina::FacetPairing<2>::IsoList autos) {
        regina::GluingPermSearcher<2>::findAllPerms(p, std::move(autos),
                false /* orbl only */,
                [&action](const regina::GluingPerms<2>& g) {
            auto t = g.triangulate();
            std::invoke(std::forward<Action>(action), t, t.neoSig().c_str());
        });
    });
}

// ------------------------------ 3-D ------------------------------

/**
 * Run the given test over all triangulations from a census of
 * closed and potentially minimal 3-manifold triangulations that is
 * generated on the fly.
 *
 * \param action the test to run.  This should be a callable object that takes
 * two arguments: a triangulation, and a human-readable triangulation name.
 * \param small indicates whether a smaller census should be used.  This can
 * help with the overall running time when the test \a action is expensive.
 */
template <std::invocable<regina::Triangulation<3>&&, const char*> Action>
inline void runCensusMinClosed(Action&& action, bool small = false) {
    regina::FacetPairing<3>::findAllPairings(
            (small ? 3 : 4) /* size */,
            false /* bounded */, -1 /* bdry faces */,
            [&action](const regina::FacetPairing<3>& p,
                regina::FacetPairing<3>::IsoList autos) {
        regina::GluingPermSearcher<3>::findAllPerms(p, std::move(autos),
                false /* orbl only */, true /* finite only */,
                regina::CensusPurge::NonMinimalPrime |
                    regina::CensusPurge::P2Reducible,
                [&action](const regina::GluingPerms<3>& g) {
            auto t = g.triangulate();
            if (t.isValid() && ! t.isIdeal())
                std::invoke(std::forward<Action>(action), t,
                    t.neoSig().c_str());
        });
    });
}

/**
 * Run the given test over all triangulations from a census of
 * closed 3-manifold triangulations that is generated on the fly.
 *
 * \param action the test to run.  This should be a callable object that takes
 * two arguments: a triangulation, and a human-readable triangulation name.
 * \param small indicates whether a smaller census should be used.  This can
 * help with the overall running time when the test \a action is expensive.
 */
template <std::invocable<regina::Triangulation<3>&&, const char*> Action>
inline void runCensusAllClosed(Action&& action, bool small = false) {
    regina::FacetPairing<3>::findAllPairings(
            (small ? 2 : 3) /* size */,
            false /* bounded */, -1 /* bdry faces */,
            [&action](const regina::FacetPairing<3>& p,
                regina::FacetPairing<3>::IsoList autos) {
        regina::GluingPermSearcher<3>::findAllPerms(p, std::move(autos),
                false /* orbl only */, true /* finite only */,
                regina::CensusPurge::None,
                [&action](const regina::GluingPerms<3>& g) {
            auto t = g.triangulate();
            if (t.isValid() && ! t.isIdeal())
                std::invoke(std::forward<Action>(action), t,
                    t.neoSig().c_str());
        });
    });
}

/**
 * Run the given test over all triangulations from a census of
 * 3-manifold triangulations with boundary facets that is generated on the fly.
 *
 * \param action the test to run.  This should be a callable object that takes
 * two arguments: a triangulation, and a human-readable triangulation name.
 * \param small indicates whether a smaller census should be used.  This can
 * help with the overall running time when the test \a action is expensive.
 */
template <std::invocable<regina::Triangulation<3>&&, const char*> Action>
inline void runCensusAllBounded(Action&& action, bool small = false) {
    regina::FacetPairing<3>::findAllPairings(
            (small ? 2 : 3) /* size */,
            true /* bounded */, -1 /* bdry faces */,
            [&action](const regina::FacetPairing<3>& p,
                regina::FacetPairing<3>::IsoList autos) {
        regina::GluingPermSearcher<3>::findAllPerms(p, std::move(autos),
                false /* orbl only */, true /* finite only */,
                regina::CensusPurge::None,
                [&action](const regina::GluingPerms<3>& g) {
            auto t = g.triangulate();
            if (t.isValid() && ! t.isIdeal())
                std::invoke(std::forward<Action>(action), t,
                    t.neoSig().c_str());
        });
    });
}

/**
 * Run the given test over all triangulations from a census of
 * ideal 3-manifold triangulations that is generated on the fly.
 *
 * \param action the test to run.  This should be a callable object that takes
 * two arguments: a triangulation, and a human-readable triangulation name.
 * \param small indicates whether a smaller census should be used.  This can
 * help with the overall running time when the test \a action is expensive.
 */
template <std::invocable<regina::Triangulation<3>&&, const char*> Action>
inline void runCensusAllIdeal(Action&& action, bool small = false) {
    regina::FacetPairing<3>::findAllPairings(
            (small ? 2 : 3) /* size */,
            false /* bounded */, -1 /* bdry faces */,
            [&action](const regina::FacetPairing<3>& p,
                regina::FacetPairing<3>::IsoList autos) {
        regina::GluingPermSearcher<3>::findAllPerms(p, std::move(autos),
                false /* orbl only */, false /* finite only */,
                regina::CensusPurge::None,
                [&action](const regina::GluingPerms<3>& g) {
            auto t = g.triangulate();
            if (t.isValid() && t.isIdeal())
                std::invoke(std::forward<Action>(action), t,
                    t.neoSig().c_str());
        });
    });
}

/**
 * Run the given test over all triangulations from a census of 3-manifold
 * triangulations with no boundary facets that is generated on the fly.
 * These triangulations could be either closed or ideal.
 *
 * \param action the test to run.  This should be a callable object that takes
 * two arguments: a triangulation, and a human-readable triangulation name.
 * \param small indicates whether a smaller census should be used.  This can
 * help with the overall running time when the test \a action is expensive.
 */
template <std::invocable<regina::Triangulation<3>&&, const char*> Action>
inline void runCensusAllNoBdry(Action&& action, bool small = false) {
    regina::FacetPairing<3>::findAllPairings(
            (small ? 2 : 3) /* size */,
            false /* bounded */, -1 /* bdry faces */,
            [&action](const regina::FacetPairing<3>& p,
                regina::FacetPairing<3>::IsoList autos) {
        regina::GluingPermSearcher<3>::findAllPerms(p, std::move(autos),
                false /* orbl only */, false /* finite only */,
                regina::CensusPurge::None,
                [&action](const regina::GluingPerms<3>& g) {
            auto t = g.triangulate();
            if (t.isValid())
                std::invoke(std::forward<Action>(action), t,
                    t.neoSig().c_str());
        });
    });
}

// ------------------------------ 4-D ------------------------------

/**
 * Run the given test over all triangulations from a census of
 * closed 4-manifold triangulations that is generated on the fly.
 *
 * \param action the test to run.  This should be a callable object that takes
 * two arguments: a triangulation, and a human-readable triangulation name.
 * \param size the number of pentachora for the census, or 0 if a sensible
 * default should be used.
 */
template <std::invocable<regina::Triangulation<4>&&, const char*> Action>
inline void runCensusAllClosed(Action&& action, int size = 0) {
    regina::FacetPairing<4>::findAllPairings(
            size == 0 ? 2 : size, false /* bounded */, -1 /* bdry facets */,
            [&action](const regina::FacetPairing<4>& p,
                regina::FacetPairing<4>::IsoList autos) {
        regina::GluingPermSearcher<4>::findAllPerms(p, std::move(autos),
                false /* orbl only */, true /* finite only */,
                [&action](const regina::GluingPerms<4>& g) {
            auto t = g.triangulate();
            if (t.isValid() && ! t.isIdeal())
                std::invoke(std::forward<Action>(action), t,
                    t.neoSig().c_str());
        });
    });
}

/**
 * Run the given test over all triangulations from a census of
 * 4-manifold triangulations with boundary facets that is generated on the fly.
 *
 * \param action the test to run.  This should be a callable object that takes
 * two arguments: a triangulation, and a human-readable triangulation name.
 * \param size the number of pentachora for the census, or 0 if a sensible
 * default should be used.
 */
template <std::invocable<regina::Triangulation<4>&&, const char*> Action>
inline void runCensusAllBounded(Action&& action, int size = 0) {
    regina::FacetPairing<4>::findAllPairings(
            size == 0 ? 2 : size, true /* bounded */, -1 /* bdry facets */,
            [&action](const regina::FacetPairing<4>& p,
                regina::FacetPairing<4>::IsoList autos) {
        regina::GluingPermSearcher<4>::findAllPerms(p, std::move(autos),
                false /* orbl only */, true /* finite only */,
                [&action](const regina::GluingPerms<4>& g) {
            auto t = g.triangulate();
            if (t.isValid() && ! t.isIdeal())
                std::invoke(std::forward<Action>(action), t,
                    t.neoSig().c_str());
        });
    });
}

/**
 * Run the given test over all triangulations from a census of 4-manifold
 * triangulations with no boundary facets that is generated on the fly.
 * These triangulations could be either closed or ideal.
 *
 * \param action the test to run.  This should be a callable object that takes
 * two arguments: a triangulation, and a human-readable triangulation name.
 * \param size the number of pentachora for the census, or 0 if a sensible
 * default should be used.
 */
template <std::invocable<regina::Triangulation<4>&&, const char*> Action>
inline void runCensusAllNoBdry(Action&& action, int size = 0) {
    regina::FacetPairing<4>::findAllPairings(
            size == 0 ? 2 : size, false /* bounded */, -1 /* bdry facets */,
            [&action](const regina::FacetPairing<4>& p,
                regina::FacetPairing<4>::IsoList autos) {
        regina::GluingPermSearcher<4>::findAllPerms(p, std::move(autos),
                false /* orbl only */, false /* finite only */,
                [&action](const regina::GluingPerms<4>& g) {
            auto t = g.triangulate();
            if (t.isValid())
                std::invoke(std::forward<Action>(action), t,
                    t.neoSig().c_str());
        });
    });
}

/**
 * Run the given test over all triangulations from a partial census of
 * 4-manifold triangulations that is generated on the fly.
 *
 * This census will only generate triangulations that are modelled by facet
 * pairings that pass the given filter.  There will be no further restrictions
 * on whether the resulting triangulations are closed, ideal, or have boundary
 * facets.
 *
 * \param action the test to run.  This should be a callable object that takes
 * two arguments: a triangulation, and a human-readable triangulation name.
 * \param filter the filter indicating which facet pairings will be considered.
 * This should be a callable object that takes a facet pairing and returns
 * `true` if and only if that pairing should be used.
 * \param size the number of pentachora for the census.  Unlike the other
 * exhaustive census test routines, here the size parameter is mandatory and
 * must not be zero (i.e., there is no sensible default).
 */
template <std::invocable<regina::Triangulation<4>&&, const char*> Action,
    std::predicate<const regina::FacetPairing<4>&> Filter>
inline void runPartialCensus(Action&& action, Filter&& filter, int size) {
    if (size == 0)
        throw regina::InvalidArgument(
            "runPartialCensus() requires a non-zero size");

    regina::FacetPairing<4>::findAllPairings(
            size, { true, true } /* bounded */, -1 /* bdry facets */,
            [&action, &filter](const regina::FacetPairing<4>& p,
                regina::FacetPairing<4>::IsoList autos) {
        if (std::invoke(std::forward<Filter>(filter), p))
            regina::GluingPermSearcher<4>::findAllPerms(p, std::move(autos),
                    false /* orbl only */, false /* finite only */,
                    [&action](const regina::GluingPerms<4>& g) {
                auto t = g.triangulate();
                if (t.isValid())
                    std::invoke(std::forward<Action>(action), t,
                        t.neoSig().c_str());
            });
    });
}

#endif
