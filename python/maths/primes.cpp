
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
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

#include "../pybind11/pybind11.h"
#include "../pybind11/stl.h"
#include "maths/primes.h"
#include "../helpers.h"

using regina::Integer;
using regina::Primes;

void addPrimes(pybind11::module_& m) {
    auto c = pybind11::class_<Primes>(m, "Primes")
        .def_static("size", &Primes::size)
        .def_static("prime", &Primes::prime,
            pybind11::arg(), pybind11::arg("autoGrow") = true)
        .def_static("primeDecomp", &Primes::primeDecomp)
        .def_static("primeDecompInt", [](const Integer& n) {
            std::vector<Integer> factors = Primes::primeDecomp(n);
            pybind11::list ans;
            for (auto& f : factors)
                ans.append(f.longValue());
            return ans;
        })
        .def_static("primePowerDecomp", &Primes::primePowerDecomp)
        .def_static("primePowerDecompInt", [](const Integer& n) {
            std::vector<std::pair<Integer, unsigned long> >
                factors = Primes::primePowerDecomp(n);
            pybind11::list ans;
            for (auto& f : factors)
                ans.append(pybind11::make_tuple(f.first.longValue(), f.second));
            return ans;
        })
    ;
    regina::python::no_eq_operators(c);

    m.attr("NPrimes") = m.attr("Primes");
}

