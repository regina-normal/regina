
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
const unsigned long NBinaryS4::mult_table[24] = { 0x0, 0x45da96, 0x6c812e, 0xa85d5c, 0x8106fa, 0x3b23b4, 0xc8765a, 0x9533f0, 0x30a5ac, 0x5110e2, 0x865c8, 0x61c046, 0xc198ca, 0x2124b8, 0xe0484c, 0x669b26, 0x15bad2, 0x2680a0, 0x758202, 0x28c168, 0x1301b2, 0x7aad24, 0x4fd80e, 0x401458 };

// NBinaryS4::names[i] is the cycle decomposition with sign  
const std::string NBinaryS4::names[48] = { 
 "1", "-1", "(34)", "(43)", "(234)", "-(234)", "(23)", "(32)", "(243)", "-(243)", "(24)", "(42)", "(12)(34)", 
"(34)(12)", "(12)", "(21)", "(123)", "-(123)", "(1234)", "(2341)", "(124)", "-(124)", "(1243)", 
"(3124)", "(132)", "-(132)", "(1342)", "(2134)", "(134)", "-(134)", "(13)", "(31)", "(24)(13)", 
"(13)(24)", "(1324)", "(4132)", "(142)", "-(142)", "(1432)", "(2143)", "(143)", "-(143)", "(14)", "(41)", 
"(14)(23)", "(23)(14)", "(1423)", "(3142)" };

const std::string NBinaryS4::spinornames[48] = { "1", "-1", "-\\frac{1}{2}\\sqrt{2}i-\\frac{1}{2}\\sqrt{2}j",
 "\\frac{1}{2}\\sqrt{2}i+\\frac{1}{2}\\sqrt{2}j", "\\frac{1}{2}-\\frac{1}{2}i+\\frac{1}{2}j+\\frac{1}{2}k", 
"\\frac{-1}{2}+\\frac{1}{2}i-\\frac{1}{2}j-\\frac{1}{2}k", "\\frac{1}{2}\\sqrt{2}j-\\frac{1}{2}\\sqrt{2}k", 
"-\\frac{1}{2}\\sqrt{2}j+\\frac{1}{2}\\sqrt{2}k", "\\frac{1}{2}+\\frac{1}{2}i-\\frac{1}{2}j-\\frac{1}{2}k", 
"\\frac{-1}{2}-\\frac{1}{2}i+\\frac{1}{2}j+\\frac{1}{2}k", "\\frac{1}{2}\\sqrt{2}i+\\frac{1}{2}\\sqrt{2}k", 
"-\\frac{1}{2}\\sqrt{2}i-\\frac{1}{2}\\sqrt{2}k", "-1k", "1k", "\\frac{1}{2}\\sqrt{2}i-\\frac{1}{2}\\sqrt{2}j", 
"-\\frac{1}{2}\\sqrt{2}i+\\frac{1}{2}\\sqrt{2}j", "\\frac{1}{2}+\\frac{1}{2}i+\\frac{1}{2}j+\\frac{1}{2}k", 
"\\frac{-1}{2}-\\frac{1}{2}i-\\frac{1}{2}j-\\frac{1}{2}k", "\\frac{1}{2}\\sqrt{2}-\\frac{1}{2}\\sqrt{2}j", 
"\\frac{1}{2}\\sqrt{2}+\\frac{1}{2}\\sqrt{2}j", "\\frac{1}{2}+\\frac{1}{2}i+\\frac{1}{2}j-\\frac{1}{2}k", 
"\\frac{-1}{2}-\\frac{1}{2}i-\\frac{1}{2}j+\\frac{1}{2}k", "\\frac{1}{2}\\sqrt{2}-\\frac{1}{2}\\sqrt{2}i", 
"\\frac{1}{2}\\sqrt{2}+\\frac{1}{2}\\sqrt{2}i", "\\frac{1}{2}-\\frac{1}{2}i-\\frac{1}{2}j-\\frac{1}{2}k", 
"\\frac{-1}{2}+\\frac{1}{2}i+\\frac{1}{2}j+\\frac{1}{2}k", "\\frac{1}{2}\\sqrt{2}+\\frac{1}{2}\\sqrt{2}i", 
"\\frac{1}{2}\\sqrt{2}-\\frac{1}{2}\\sqrt{2}i", "\\frac{1}{2}-\\frac{1}{2}i+\\frac{1}{2}j-\\frac{1}{2}k", 
"\\frac{-1}{2}+\\frac{1}{2}i-\\frac{1}{2}j+\\frac{1}{2}k", "-\\frac{1}{2}\\sqrt{2}i+\\frac{1}{2}\\sqrt{2}k",
"\\frac{1}{2}\\sqrt{2}i-\\frac{1}{2}\\sqrt{2}k", "-1j", "1j", "\\frac{1}{2}\\sqrt{2}+\\frac{1}{2}\\sqrt{2}k", 
"\\frac{1}{2}\\sqrt{2}-\\frac{1}{2}\\sqrt{2}k", "\\frac{1}{2}-\\frac{1}{2}i-\\frac{1}{2}j+\\frac{1}{2}k", 
"\\frac{-1}{2}+\\frac{1}{2}i+\\frac{1}{2}j-\\frac{1}{2}k", "\\frac{1}{2}\\sqrt{2}+\\frac{1}{2}\\sqrt{2}j", 
"\\frac{1}{2}\\sqrt{2}-\\frac{1}{2}\\sqrt{2}j", "\\frac{1}{2}+\\frac{1}{2}i-\\frac{1}{2}j+\\frac{1}{2}k", 
"\\frac{-1}{2}-\\frac{1}{2}i+\\frac{1}{2}j-\\frac{1}{2}k", "\\frac{1}{2}\\sqrt{2}j+\\frac{1}{2}\\sqrt{2}k", 
"-\\frac{1}{2}\\sqrt{2}j-\\frac{1}{2}\\sqrt{2}k", "-1i", "1i", "\\frac{1}{2}\\sqrt{2}-\\frac{1}{2}\\sqrt{2}k", 
"\\frac{1}{2}\\sqrt{2}+\\frac{1}{2}\\sqrt{2}k" };




} // namespace regina

