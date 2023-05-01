
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "triangulation/forward.h"
#include "triangulation/isosigencoding.h"
#include "triangulation/isosigtype.h"
#include "utilities/exception.h"
#include "../helpers.h"
#include "../docstrings/triangulation/isosigencoding.h"
#include "../docstrings/triangulation/isosigtype.h"

using regina::Perm;

template <int dim>
void addIsoSigClassic(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(IsoSigClassic)

    using Type = regina::IsoSigClassic<dim>;
    auto s = pybind11::class_<Type>(m, name, rdoc_scope)
        .def(pybind11::init<const regina::Component<dim>&>(), rdoc::__init)
        .def("simplex", &Type::simplex, rdoc::simplex)
        .def("perm", &Type::perm, rdoc::perm)
        .def("next", &Type::next, rdoc::next)
        ;
    regina::python::disable_eq_operators(s);

    RDOC_SCOPE_END
}

template <int dim>
void addIsoSigEdgeDegrees(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(IsoSigDegrees)

    using Type = regina::IsoSigEdgeDegrees<dim>;
    auto s = pybind11::class_<Type>(m, name, rdoc_scope)
        .def(pybind11::init<const regina::Component<dim>&>(), rdoc::__init)
        .def("simplex", &Type::simplex, rdoc::simplex)
        .def("perm", &Type::perm, rdoc::perm)
        .def("next", &Type::next, rdoc::next)
        ;
    regina::python::disable_eq_operators(s);

    RDOC_SCOPE_END
}

template <int dim>
void addIsoSigRidgeDegrees(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(IsoSigDegrees)

    using Type = regina::IsoSigRidgeDegrees<dim>;
    auto s = pybind11::class_<Type>(m, name, rdoc_scope)
        .def(pybind11::init<const regina::Component<dim>&>(), rdoc::__init)
        .def("simplex", &Type::simplex, rdoc::simplex)
        .def("perm", &Type::perm, rdoc::perm)
        .def("next", &Type::next, rdoc::next)
        ;
    regina::python::disable_eq_operators(s);

    RDOC_SCOPE_END
}

template <int dim>
void addIsoSigPrintable(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(IsoSigPrintable)

    using Encoding = regina::IsoSigPrintable<dim>;
    auto s = pybind11::class_<Encoding>(m, name, rdoc_scope)
        .def_readonly_static("charsPerPerm", &Encoding::charsPerPerm)
        .def_static("emptySig", &Encoding::emptySig, rdoc::emptySig)
        .def_static("encode", [](
                size_t size,
                const std::vector<uint8_t>& facetAction,
                const std::vector<size_t>& joinDest,
                const std::vector<typename Perm<dim+1>::Index>& joinGluing) {
            if (joinDest.size() != joinGluing.size())
                throw regina::InvalidArgument("The arguments "
                    "joinDest and joinGluing must be lists of the same size");
            return Encoding::encode(size, facetAction.size(),
                facetAction.data(), joinDest.size(), joinDest.data(),
                joinGluing.data());
        }, pybind11::arg("size"), pybind11::arg("facetAction"),
            pybind11::arg("joinDest"), pybind11::arg("joinGluing"),
            rdoc::encode)
        ;
    regina::python::no_eq_static(s);

    RDOC_SCOPE_END
}

