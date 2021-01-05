
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

#include "triangulation/detail/pachner-impl.h"

namespace regina {
namespace detail {

// Precalculated values of movePerm() in standard dimensions:

const Perm<3> movePerm_2_1[2][2] = {
  { Perm<3>(0,2,1), Perm<3>(1,0,2) },
  { Perm<3>(1,0,2), Perm<3>(1,0,2) } };

const Perm<4> movePerm_3_1[3][2] = {
  { Perm<4>(0,3,1,2), Perm<4>(2,0,1,3) },
  { Perm<4>(1,0,3,2), Perm<4>(2,0,1,3) },
  { Perm<4>(2,0,1,3), Perm<4>(2,0,1,3) } };

const Perm<4> movePerm_3_2[2][3] = {
  { Perm<4>(0,2,3,1), Perm<4>(1,0,3,2), Perm<4>(1,2,0,3) },
  { Perm<4>(1,2,0,3), Perm<4>(1,2,0,3), Perm<4>(1,2,0,3) } };

const Perm<5> movePerm_4_1[4][2] = {
  { Perm<5>(0,4,1,2,3), Perm<5>(3,0,1,2,4) },
  { Perm<5>(1,0,4,2,3), Perm<5>(3,0,1,2,4) },
  { Perm<5>(2,0,1,4,3), Perm<5>(3,0,1,2,4) },
  { Perm<5>(3,0,1,2,4), Perm<5>(3,0,1,2,4) } };

const Perm<5> movePerm_4_2[3][3] = {
  { Perm<5>(0,3,4,1,2), Perm<5>(2,0,4,1,3), Perm<5>(2,3,0,1,4) },
  { Perm<5>(1,3,0,4,2), Perm<5>(2,1,0,4,3), Perm<5>(2,3,0,1,4) },
  { Perm<5>(2,3,0,1,4), Perm<5>(2,3,0,1,4), Perm<5>(2,3,0,1,4) } };

const Perm<5> movePerm_4_3[2][4] = {
  { Perm<5>(0,2,3,4,1), Perm<5>(1,0,3,4,2),
    Perm<5>(1,2,0,4,3), Perm<5>(1,2,3,0,4) },
  { Perm<5>(1,2,3,0,4), Perm<5>(1,2,3,0,4),
    Perm<5>(1,2,3,0,4), Perm<5>(1,2,3,0,4) } };

} } // namespace regina::detail
