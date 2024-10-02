
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

#include "utilities/boolset.h"

using regina::BoolSet;

static_assert(! BoolSet().hasTrue());
static_assert(! BoolSet().hasFalse());

static_assert(BoolSet(true).hasTrue());
static_assert(! BoolSet(true).hasFalse());
static_assert(BoolSet(true).contains(true));
static_assert(! BoolSet(true).contains(false));

static_assert(! BoolSet(false).hasTrue());
static_assert(BoolSet(false).hasFalse());
static_assert(! BoolSet(false).contains(true));
static_assert(BoolSet(false).contains(false));

static_assert(BoolSet(true, true).hasTrue());
static_assert(BoolSet(true, true).hasFalse());

static_assert(BoolSet() == BoolSet(false, false));
static_assert(BoolSet() != BoolSet(true, true));
static_assert(BoolSet() < BoolSet(true, true));
static_assert(BoolSet() <= BoolSet(true, true));
static_assert(! (BoolSet() > BoolSet(true, true)));
static_assert(! (BoolSet() >= BoolSet(true, true)));

static_assert((BoolSet() | true) == BoolSet(true));
static_assert((BoolSet(true) & false) == BoolSet());
static_assert((BoolSet(true) ^ true) == BoolSet());
static_assert((BoolSet(true) ^ false) == BoolSet(true, true));
static_assert(~ BoolSet(true) == false);

static_assert(BoolSet(true).byteCode() == 1);
static_assert(BoolSet::fromByteCode(2) == false);

static_assert(! (BoolSet(true) == BoolSet(true, true)));
static_assert(BoolSet(true) != BoolSet(true, true));
static_assert(BoolSet(true) < BoolSet(true, true));
static_assert(BoolSet(true) <= BoolSet(true, true));
static_assert(! (BoolSet(true) > BoolSet(true, true)));
static_assert(! (BoolSet(true) >= BoolSet(true, true)));

static_assert(! (BoolSet(true) == BoolSet(false)));
static_assert(BoolSet(true) != BoolSet(false));
static_assert(! (BoolSet(true) < BoolSet(false)));
static_assert(! (BoolSet(true) <= BoolSet(false)));
static_assert(! (BoolSet(true) > BoolSet(false)));
static_assert(! (BoolSet(true) >= BoolSet(false)));

static_assert(BoolSet(true) == BoolSet(true));
static_assert(! (BoolSet(true) != BoolSet(true)));
static_assert(! (BoolSet(true) < BoolSet(true)));
static_assert(BoolSet(true) <= BoolSet(true));
static_assert(! (BoolSet(true) > BoolSet(true)));
static_assert(BoolSet(true) >= BoolSet(true));
