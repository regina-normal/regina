
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

#include <sstream>
#include <string>
#include "algebra/nbinaryS4.h"

namespace regina {

// technically this is just the cocycle on S4 which describes the non-trivial Z_2-central extension of A5
// elements of S4 are listed in order of NPerm4::S4.
const unsigned long NBinaryS4::mult_table[24] = { 0, 4577942, 7110958, 11033948, 8455930, 3875764, 13137498, 9778160, 3188140, 5312738, 550344, 6406214, 12687562, 2172088, 14698572, 6724390, 1424082, 2523296, 7700994, 2670952, 1245618, 8039716, 5232654, 4199512 };


// NBinaryA5::names[i] is the cycle decomposition of NPerm5::S5[i] provided i is even.  If i is odd, 
//  it is the notation for the corresponding element of the binary A5 group.
const std::string NBinaryS4::names[48] = { "1", "(34)", "(234)", "(23)", "(243)", "(24)", "(12)(34)", "(12)", "(123)", "(1234)", "(124)", "(1243)", "(132)", "(1342)", "(134)", "(13)", "(24)(13)", "(1324)", "(142)", "(1432)", "(143)", "(14)", "(14)(23)", "(1423)",  "-1", "(43)", "-(234)", "(32)", "-(243)", "(42)", "(34)(12)", "(21)", "-(123)", "-(1234)", "-(124)", "-(1243)", "-(132)", "-(1342)", "-(134)", "(31)", "(13)(24)", "-(1324)", "-(142)", "-(1432)", "-(143)", "(41)", "(23)(14)", "-(1423)" }; 

const unsigned long NBinaryS4::floormask = ( ~((unsigned long) 32) );

const std::string NBinaryS4::spinornames[48] = {"1", "(34)", "(234)", "(23)", "(243)", "(24)", "(12)(34)", "(12)", "(123)", "(1234)", "(124)", "(1243)", "(132)", "(1342)", "(134)", "(13)", "(24)(13)", "(1324)", "(142)", "(1432)", "(143)", "(14)", "(14)(23)", "(1423)",  "-1", "(43)", "-(234)", "(32)", "-(243)", "(42)", "(34)(12)", "(21)", "-(123)", "-(1234)", "-(124)", "-(1243)", "-(132)", "-(1342)", "-(134)", "(31)", "(13)(24)", "-(1324)", "-(142)", "-(1432)", "-(143)", "(41)", "(23)(14)", "-(1423)" };

} // namespace regina

