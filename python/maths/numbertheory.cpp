
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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

#include "maths/numbertheory.h"
#include <boost/python.hpp>

using namespace boost::python;

namespace {
    boost::python::list factorise_list(unsigned long n) {
        std::list<unsigned long> factors;
        std::list<unsigned long>::const_iterator it;

        regina::factorise(n, factors);

        boost::python::list ans;
        for (it = factors.begin(); it != factors.end(); it++)
            ans.append(*it);
        return ans;
    }

    boost::python::list primesUpTo_list(const regina::NLargeInteger& roof) {
        std::list<regina::NLargeInteger> primes;
        std::list<regina::NLargeInteger>::const_iterator it;

        regina::primesUpTo(roof, primes);

        boost::python::list ans;
        for (it = primes.begin(); it != primes.end(); it++)
            ans.append(*it);
        return ans;
    }
}

void addNumberTheory() {
    def("reducedMod", regina::reducedMod);
    def("gcd", regina::gcd);
    def("gcdWithCoeffs", regina::gcdWithCoeffs);
    def("lcm", regina::lcm);
    def("modularInverse", regina::modularInverse);
    def("factorise", factorise_list);
    def("primesUpTo", primesUpTo_list);
}

