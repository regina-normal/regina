
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

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "triangulation/isosig.h"
#include "utilities/typeutils.h"
#include "../helpers.h"
#include "../helpers/bytesequence.h"
#include "../docstrings/triangulation/isosig.h"

using pybind11::overload_cast;
using regina::IsoSigBinary;
using regina::IsoSigData;
using regina::IsoSigPrintable;
using regina::IsoSigPrintableLockFree;

namespace regina::python {
    // This function is implemented in isosig-bindings.h.
    // We do _not_ want to include its implementation here, because we do not
    // want to inadvertently force the instantiation of the full triangulation
    // classes for _all_ dimensions simultaneously in the same source file.
    template <int generation, int dim, DocstringClass Docs, typename Encoding>
    requires ((generation == 1 || generation == 2) && regina::supportedDim(dim))
    void add_isosig_encoding_functions(pybind11::class_<Encoding>&);
}

void addIsoSigEncodings(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(IsoSigPrintable)

    auto p = pybind11::class_<IsoSigPrintable>(m, "IsoSigPrintable",
            rdoc::__class)
        .def_static("charsPerPerm", [](int dim) {
            try {
                return regina::select_constexpr<2, regina::maxDim() + 1, int>(
                        dim, [](auto d) {
                    return IsoSigPrintable::charsPerPerm<d>;
                });
            } catch (const std::runtime_error&) {
                throw regina::InvalidArgument("Not a supported dimension");
            }
        }, rdoc::charsPerPerm)
        .def_static("encodeEmpty", &IsoSigPrintable::encodeEmpty,
            rdoc::encodeEmpty)
        .def_static("generation", &IsoSigPrintable::generation,
            rdoc::generation)
        ;
    regina::for_constexpr<2, regina::maxDim() + 1>([&p](auto dim) {
        regina::python::add_isosig_encoding_functions<1, dim, rdoc>(p);
        regina::python::add_isosig_encoding_functions<2, dim, rdoc>(p);
    });
    regina::python::no_eq_static(p);

    RDOC_SCOPE_SWITCH(IsoSigPrintableLockFree)

    auto f = pybind11::class_<IsoSigPrintableLockFree>(m,
            "IsoSigPrintableLockFree", rdoc::__class)
        .def_static("charsPerPerm", [](int dim) {
            try {
                return regina::select_constexpr<2, regina::maxDim() + 1, int>(
                        dim, [](auto d) {
                    return IsoSigPrintableLockFree::charsPerPerm<d>;
                });
            } catch (const std::runtime_error&) {
                throw regina::InvalidArgument("Not a supported dimension");
            }
        }, rdoc::charsPerPerm)
        .def_static("encodeEmpty", &IsoSigPrintableLockFree::encodeEmpty,
            rdoc::encodeEmpty)
        ;
    regina::for_constexpr<2, regina::maxDim() + 1>([&f](auto dim) {
        regina::python::add_isosig_encoding_functions<1, dim, rdoc>(f);
        regina::python::add_isosig_encoding_functions<2, dim, rdoc>(f);
    });
    regina::python::no_eq_static(f);

    RDOC_SCOPE_SWITCH(IsoSigBinary)

    auto b = pybind11::class_<IsoSigBinary>(m, "IsoSigBinary", rdoc::__class)
        .def_static("encodeEmpty", &IsoSigBinary::encodeEmpty,
            rdoc::encodeEmpty)
        .def_static("asString", [](const regina::ByteSequence& sig, int dim) {
            try {
                return regina::select_constexpr<2, regina::maxDim() + 1,
                        std::string>(dim, [&sig](auto d) {
                    return IsoSigBinary::asString<d>(sig);
                });
            } catch (const regina::InvalidArgument& exc) {
                throw;
            } catch (const std::runtime_error&) {
                throw regina::InvalidArgument("Not a supported dimension");
            }
        }, rdoc::asString)
        ;
    regina::for_constexpr<2, regina::maxDim() + 1>([&b](auto dim) {
        // This encoding is for second-generation signatures only.
        regina::python::add_isosig_encoding_functions<2, dim, rdoc>(b);
    });
    regina::python::no_eq_static(b);

    RDOC_SCOPE_END
}

