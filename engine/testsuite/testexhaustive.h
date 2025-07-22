
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#ifndef __TESTEXHAUSTIVE_H
#define __TESTEXHAUSTIVE_H

#include "triangulation/forward.h"

/**
 * The functions in this header allow you to run a test over all
 * triangulations or link diagrams from a census.
 *
 * The \a small parameter indicates that a smaller census should be
 * used; this is appropriate when the corresponding test is extremely slow.
 *
 * The \a size parameter can be used to manually set the maximum number of
 * top-dimensional simplices; a value of 0 means the default should be used.
 *
 * The \a pairingFilter parameter allows you to run the census over an
 * arbitrary subset of facet pairings (those for which the filter returns
 * \c true).  If a pairing filter is used then \a size must be explicitly
 * specified (it cannot be 0).
 *
 * Each test function takes as arguments a triangulation and its
 * human-readable name.
 */

using Triangulation2TestFunction = void (*)(const regina::Triangulation<2>&,
    const char*);
using Triangulation3TestFunction = void (*)(const regina::Triangulation<3>&,
    const char*);
using Triangulation4TestFunction = void (*)(const regina::Triangulation<4>&,
    const char*);
using LinkTestFunction = void (*)(const regina::Link&, const char*);

void runCensusAllClosed(Triangulation2TestFunction f);
void runCensusAllBounded(Triangulation2TestFunction f);

void runCensusMinClosed(Triangulation3TestFunction f, bool small_ = false);
void runCensusAllClosed(Triangulation3TestFunction f, bool small_ = false);
void runCensusAllBounded(Triangulation3TestFunction f, bool small_ = false);
void runCensusAllIdeal(Triangulation3TestFunction f, bool small_ = false);
void runCensusAllNoBdry(Triangulation3TestFunction f, bool small_ = false);

void runCensusAllClosed(Triangulation4TestFunction f, int size = 0);
void runCensusAllBounded(Triangulation4TestFunction f, int size = 0);
void runCensusAllNoBdry(Triangulation4TestFunction f, int size = 0);
void runCensus(bool (*pairingFilter)(const regina::FacetPairing<4>&),
    Triangulation4TestFunction f, int size, bool orblOnly = false);

void runCensusAllVirtual(LinkTestFunction f, bool small_ = false);

#endif
