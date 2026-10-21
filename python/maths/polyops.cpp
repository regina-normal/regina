
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

#include "regina-config.h" // for REGINA_PYBIND11_VERSION
#include <pybind11/pybind11.h>
#if REGINA_PYBIND11_VERSION == 3
#include <pybind11/native_enum.h>
#endif
#include <pybind11/stl.h>
#include "maths/integer.h"
#include "maths/polyops.h"
#include "../helpers.h"
#include "../docstrings/maths/polyops.h"

using namespace pybind11::literals;

using regina::Integer;

namespace {
    /**
     * Reimplement polynomialProduct() to use `std::vector`, which pybind11
     * understands.
     */
    template <regina::PolynomialProductAlgorithm algorithm>
    pybind11::list pyPolynomialProduct(pybind11::list lhs, pybind11::list rhs) {
        regina::FixedArray<Integer> x(lhs.size()), y(rhs.size());
        for (size_t i = 0; i < lhs.size(); ++i)
            x[i] = lhs[i].cast<Integer>();
        for (size_t i = 0; i < rhs.size(); ++i)
            y[i] = rhs[i].cast<Integer>();

        auto product = regina::polynomialProduct<algorithm>(x, y);
        pybind11::list ans;
        for (auto&& c : product)
            ans.append(std::move(c));
        return ans;
    }
}

void addKaratsuba(pybind11::module_& m) {
    using PPA = regina::PolynomialProductAlgorithm;

    RDOC_SCOPE_BEGIN(PolynomialProductAlgorithm)

#if REGINA_PYBIND11_VERSION == 3
    pybind11::native_enum<PPA>(m, "PolynomialProductAlgorithm", "enum.Enum",
            rdoc::__class)
#elif REGINA_PYBIND11_VERSION == 2
    pybind11::enum_<PPA>(m, "PolynomialProductAlgorithm", rdoc::__class)
#endif
        .value("Default", PPA::Default, rdoc::Default)
        .value("Classic", PPA::Classic, rdoc::Classic)
        .value("Karatsuba", PPA::Karatsuba, rdoc::Karatsuba)
#if REGINA_PYBIND11_VERSION == 3
        .finalize()
#endif
        ;

    RDOC_SCOPE_SWITCH_MAIN

    m.def("polynomialProduct", [](pybind11::list lhs, pybind11::list rhs,
            PPA alg) {
        try {
            switch (alg) {
                case PPA::Default:
                    return pyPolynomialProduct<PPA::Default>(lhs, rhs);
                case PPA::Classic:
                    return pyPolynomialProduct<PPA::Classic>(lhs, rhs);
                case PPA::Karatsuba:
                    return pyPolynomialProduct<PPA::Karatsuba>(lhs, rhs);
            }
        } catch (pybind11::cast_error const&) {
            throw regina::InvalidArgument(
                "List element not convertible to Integer");
        }
    }, "lhs"_a, "rhs"_a, "algorithm"_a, rdoc::polynomialProduct);

    RDOC_SCOPE_END
}

