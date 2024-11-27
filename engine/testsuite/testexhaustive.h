
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
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

#ifndef __TESTEXHAUSTIVE_H
#define __TESTEXHAUSTIVE_H

#include "triangulation/forward.h"

/**
 * The functions in this header allow you to run a test over all
 * triangulations from a census.
 *
 * The \a small parameter indicates that a smaller census should be
 * used; this is appropriate when the corresponding test is extremely slow.
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

void runCensusAllBounded(Triangulation2TestFunction f);

void runCensusMinClosed(Triangulation3TestFunction f, bool small_ = false);
void runCensusAllClosed(Triangulation3TestFunction f, bool small_ = false);
void runCensusAllBounded(Triangulation3TestFunction f, bool small_ = false);
void runCensusAllIdeal(Triangulation3TestFunction f, bool small_ = false);
void runCensusAllNoBdry(Triangulation3TestFunction f, bool small_ = false);

void runCensusAllClosed(Triangulation4TestFunction f);
void runCensusAllBounded(Triangulation4TestFunction f);
void runCensusAllNoBdry(Triangulation4TestFunction f);

#endif
