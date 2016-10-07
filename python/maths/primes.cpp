
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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

#include <boost/python.hpp>
#include "maths/primes.h"
#include "../helpers.h"

using namespace boost::python;
using regina::Integer;
using regina::Primes;

namespace {
    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_prime, Primes::prime, 1, 2);

    boost::python::list primeDecomp_list(const Integer& n) {
        std::vector<Integer> factors = Primes::primeDecomp(n);

        boost::python::list ans;
        for (std::vector<Integer>::const_iterator it = factors.begin();
                it != factors.end(); ++it)
            ans.append(*it);
        return ans;
    }

    boost::python::list primeDecomp_list_int(const Integer& n) {
        std::vector<Integer> factors = Primes::primeDecomp(n);

        boost::python::list ans;
        for (std::vector<Integer>::const_iterator it = factors.begin();
                it != factors.end(); ++it)
            ans.append(it->longValue());
        return ans;
    }

    boost::python::list primePowerDecomp_list(const Integer& n) {
        std::vector<std::pair<Integer, unsigned long> >
            factors = Primes::primePowerDecomp(n);

        boost::python::list ans;
        for (std::vector<std::pair<Integer, unsigned long> >::
                const_iterator it = factors.begin(); it != factors.end(); ++it)
            ans.append(boost::python::make_tuple(it->first, it->second));
        return ans;
    }

    boost::python::list primePowerDecomp_list_int(const Integer& n) {
        std::vector<std::pair<Integer, unsigned long> >
            factors = Primes::primePowerDecomp(n);

        boost::python::list ans;
        for (std::vector<std::pair<Integer, unsigned long> >::
                const_iterator it = factors.begin(); it != factors.end(); ++it)
            ans.append(boost::python::make_tuple(
                it->first.longValue(), it->second));
        return ans;
    }
}

void addPrimes() {
    class_<Primes>("Primes", no_init)
        .def("size", &Primes::size)
        .def("prime", &Primes::prime, OL_prime())
        .def("primeDecomp", primeDecomp_list)
        .def("primeDecompInt", primeDecomp_list_int)
        .def("primePowerDecomp", primePowerDecomp_list)
        .def("primePowerDecompInt", primePowerDecomp_list_int)
        .def(regina::python::no_eq_operators())
        .staticmethod("size")
        .staticmethod("prime")
        .staticmethod("primeDecomp")
        .staticmethod("primeDecompInt")
        .staticmethod("primePowerDecomp")
        .staticmethod("primePowerDecompInt")
    ;

    scope().attr("NPrimes") = scope().attr("Primes");
}

