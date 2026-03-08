
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "maths/primes.h"
#include "../helpers.h"
#include "../docstrings/maths/primes.h"

using regina::Integer;
using regina::Primes;

namespace {
    pybind11::list nativePrimeDecomp(long n) {
        // Since n fits into a long, all of the prime factors must fit into a
        // long also.  Therefore unsafeValue() is fine to use here.
        auto factors = Primes::primeDecomp(n);
        pybind11::list ans;
        for (auto& f : factors)
            ans.append(f.unsafeValue<long>());
        return ans;
    }

    pybind11::list nativePrimePowerDecomp(long n) {
        // Since n fits into a long, all of the prime factors must fit into a
        // long also.  Therefore unsafeValue() is fine to use here.
        auto factors = Primes::primePowerDecomp(n);
        pybind11::list ans;
        for (auto& f : factors)
            ans.append(pybind11::make_tuple(
                f.first.unsafeValue<long>(), f.second));
        return ans;
    }
}

void addPrimes(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Primes)

    auto c = pybind11::class_<Primes>(m, "Primes", rdoc_scope)
        .def_static("size", &Primes::size, rdoc::size)
        .def_static("prime", &Primes::prime,
            pybind11::arg(), pybind11::arg("autoGrow") = true,
            rdoc::prime)
        .def_static("primeDecomp", &Primes::primeDecomp,
            rdoc::primeDecomp)
        .def_static("primeDecomp", &nativePrimeDecomp,
            rdoc::primeDecomp)
        .def_static("primeDecompInt", &nativePrimeDecomp, // deprecated
            rdoc::primeDecomp)
        .def_static("primePowerDecomp", &Primes::primePowerDecomp,
            rdoc::primePowerDecomp)
        .def_static("primePowerDecomp", &nativePrimePowerDecomp,
            rdoc::primePowerDecomp)
        .def_static("primePowerDecompInt", // deprecated
            &nativePrimePowerDecomp, rdoc::primePowerDecomp)
    ;
    regina::python::no_eq_static(c);

    RDOC_SCOPE_END
}

