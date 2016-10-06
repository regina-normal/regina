
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

#include <sstream>
#include "maths/perm.h"

namespace regina {

const Perm<2>::Index Perm<2>::nPerms;
const Perm<2>::Index Perm<2>::nPerms_1;

const Perm<2> Perm<2>::S2[2] = { Perm<2>(), Perm<2>(1) };
const unsigned Perm<2>::invS2[2] = { 0, 1 };

const Perm<2>* Perm<2>::Sn = Perm<2>::S2;
const unsigned* Perm<2>::invSn = Perm<2>::invS2;

const Perm<2>* Perm<2>::orderedS2 = Perm<2>::S2;
const Perm<2>* Perm<2>::orderedSn = Perm<2>::S2;
const Perm<2>* Perm<2>::S1 = Perm<2>::S2;
const Perm<2>* Perm<2>::Sn_1 = Perm<2>::S2;

} // namespace regina

