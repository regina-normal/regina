
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "triangulation/nedge.h"

namespace regina {

const int edgeNumber[4][4] = {
    { -1, 0, 1, 2 },
    {  0,-1, 3, 4 },
    {  1, 3,-1, 5 },
    {  2, 4, 5,-1 }};

const int edgeStart[6] =
    { 0, 0, 0, 1, 1, 2 };

const int edgeEnd[6] =
    { 1, 2, 3, 2, 3, 3 };

const int NEdge::edgeNumber[4][4] = {
    { -1, 0, 1, 2 },
    {  0,-1, 3, 4 },
    {  1, 3,-1, 5 },
    {  2, 4, 5,-1 }};

const int NEdge::edgeVertex[6][2] = {
    { 0, 1 },
    { 0, 2 },
    { 0, 3 },
    { 1, 2 },
    { 1, 3 },
    { 2, 3 }};

const NPerm NEdge::ordering[6] = {
    NPerm(0, 1, 2, 3),
    NPerm(0, 2, 3, 1),
    NPerm(0, 3, 1, 2),
    NPerm(1, 2, 0, 3),
    NPerm(1, 3, 2, 0),
    NPerm(2, 3, 0, 1)
};

} // namespace regina

