
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include "dim4/dim4edge.h"

namespace regina {

const int Dim4Edge::edgeNumber[5][5] = {
    { -1, 0, 1, 2, 3 },
    { 0, -1, 4, 5, 6 },
    { 1, 4, -1, 7, 8 },
    { 2, 5, 7, -1, 9 },
    { 3, 6, 8, 9, -1 }};

const int Dim4Edge::edgeVertex[10][2] = {
    { 0, 1 },
    { 0, 2 },
    { 0, 3 },
    { 0, 4 },
    { 1, 2 },
    { 1, 3 },
    { 1, 4 },
    { 2, 3 },
    { 2, 4 },
    { 3, 4 }};

const NPerm5 Dim4Edge::ordering[10] = {
    NPerm5(0, 1, 2, 3, 4),
    NPerm5(0, 2, 1, 3, 4),
    NPerm5(0, 3, 1, 2, 4),
    NPerm5(0, 4, 1, 2, 3),
    NPerm5(1, 2, 0, 3, 4),
    NPerm5(1, 3, 0, 2, 4),
    NPerm5(1, 4, 0, 2, 3),
    NPerm5(2, 3, 0, 1, 4),
    NPerm5(2, 4, 0, 1, 3),
    NPerm5(3, 4, 0, 1, 2)
};

} // namespace regina

