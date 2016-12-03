
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Test Suite                                                            *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#ifndef __EXHAUSTIVE_H
#define __EXHAUSTIVE_H

#include "triangulation/forward.h"

/**
 * The functions in this header allow you to run a test over all
 * triangulations from a census.  Each triangulation will have its
 * packet label set to its isomorphic signature (which may be useful for
 * any test failure messages).
 *
 * The \a small parameter indicates that a smaller census should be
 * used; this is appropriate when the corresponding test is extremely slow.
 */

typedef void (*Triangulation2TestFunction)(regina::Triangulation<2>*);
typedef void (*Triangulation3TestFunction)(regina::Triangulation<3>*);
typedef void (*Triangulation4TestFunction)(regina::Triangulation<4>*);

void runCensusMinClosed(Triangulation3TestFunction f, bool small_ = false);
void runCensusAllClosed(Triangulation3TestFunction f, bool small_ = false);
void runCensusAllBounded(Triangulation3TestFunction f, bool small_ = false);
void runCensusAllIdeal(Triangulation3TestFunction f, bool small_ = false);
void runCensusAllNoBdry(Triangulation3TestFunction f, bool small_ = false);

void runCensusAllClosed(Triangulation4TestFunction f);
void runCensusAllBounded(Triangulation4TestFunction f);
void runCensusAllNoBdry(Triangulation4TestFunction f);

#endif
