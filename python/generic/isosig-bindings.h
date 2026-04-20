
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
#include "triangulation/isosig.h"
#include "utilities/exception.h"
#include "../helpers.h"
#include "../docstrings/triangulation/isosig.h"

namespace regina::python {

/**
 * Adds all of the available variants of isoSig() to the python bindings for a
 * C++ triangulation class.
 *
 * To use this for the C++ class `Triangulation<dim>`, simply call
 * `regina::python::isosig_options<dim>(c)`, where \a c is the
 * `pybind11::class_` object that wraps `Triangulation<dim>`.
 */
template <int dim, PythonClassWrapper PythonClass>
requires requires {
    // We use nested requirements here because I believe this means that if
    // supportedDim(dim) fails then the compiler will not even try to
    // instantiate Triangulation<dim>.
    requires (regina::supportedDim(dim));
    requires (std::same_as<typename PythonClass::type, Triangulation<dim>>);
}
void isosig_options(PythonClass& classWrapper) {
    RDOC_SCOPE_BASE(detail::TriangulationBase)

    using LockFree = regina::IsoSigPrintableLockFree<dim>;

    classWrapper
        .def("isoSig",
            &Triangulation<dim>::template isoSig<>,
            rbase::isoSig)
        .def("isoSig_LockFree",
            &Triangulation<dim>::template isoSig<
                regina::IsoSigClassic<dim>, LockFree>,
            rbase::isoSig)
        .def("isoSig_EdgeDegrees",
            &Triangulation<dim>::template isoSig<
                regina::IsoSigEdgeDegrees<dim>>,
            rbase::isoSig)
        .def("isoSig_EdgeDegrees_LockFree",
            &Triangulation<dim>::template isoSig<
                regina::IsoSigEdgeDegrees<dim>, LockFree>,
            rbase::isoSig)
        .def("isoSig_RidgeDegrees",
            &Triangulation<dim>::template isoSig<
                regina::IsoSigRidgeDegrees<dim>>,
            rbase::isoSig)
        .def("isoSig_RidgeDegrees_LockFree",
            &Triangulation<dim>::template isoSig<
                regina::IsoSigRidgeDegrees<dim>, LockFree>,
            rbase::isoSig)

        .def("isoSigDetail",
            &Triangulation<dim>::template isoSigDetail<>,
            rbase::isoSigDetail)
        .def("isoSigDetail_LockFree",
            &Triangulation<dim>::template isoSigDetail<
                regina::IsoSigClassic<dim>, LockFree>,
            rbase::isoSigDetail)
        .def("isoSigDetail_EdgeDegrees",
            &Triangulation<dim>::template isoSigDetail<
                regina::IsoSigEdgeDegrees<dim>>,
            rbase::isoSigDetail)
        .def("isoSigDetail_EdgeDegrees_LockFree",
            &Triangulation<dim>::template isoSigDetail<
                regina::IsoSigEdgeDegrees<dim>, LockFree>,
            rbase::isoSigDetail)
        .def("isoSigDetail_RidgeDegrees",
            &Triangulation<dim>::template isoSigDetail<
                regina::IsoSigRidgeDegrees<dim>>,
            rbase::isoSigDetail)
        .def("isoSigDetail_RidgeDegrees_LockFree",
            &Triangulation<dim>::template isoSigDetail<
                regina::IsoSigRidgeDegrees<dim>, LockFree>,
            rbase::isoSigDetail)
        ;
}

} // namespace regina::python

template <int dim> requires (regina::supportedDim(dim))
void addIsoSigClassic(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN_MAIN
    RDOC_SCOPE_BASE(IsoSigTypeAPI)

    using Type = regina::IsoSigClassic<dim>;
    auto s = pybind11::class_<Type>(m, name, rdoc::IsoSigClassic)
        .def(pybind11::init<const regina::Component<dim>&>(), rbase::__init)
        .def("simplex", &Type::simplex, rbase::simplex)
        .def("perm", &Type::perm, rbase::perm)
        .def("next", &Type::next, rbase::next)
        ;
    regina::python::disable_eq_operators(s);

    RDOC_SCOPE_END
}

template <int dim> requires (regina::supportedDim(dim))
void addIsoSigEdgeDegrees(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN_MAIN
    RDOC_SCOPE_BASE(IsoSigTypeAPI)

    using Type = regina::IsoSigEdgeDegrees<dim>;
    auto s = pybind11::class_<Type>(m, name, rdoc::IsoSigDegrees)
        .def(pybind11::init<const regina::Component<dim>&>(), rbase::__init)
        .def("simplex", &Type::simplex, rbase::simplex)
        .def("perm", &Type::perm, rbase::perm)
        .def("next", &Type::next, rbase::next)
        ;
    regina::python::disable_eq_operators(s);

    RDOC_SCOPE_END
}

template <int dim> requires (regina::supportedDim(dim))
void addIsoSigRidgeDegrees(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN_MAIN
    RDOC_SCOPE_BASE(IsoSigTypeAPI)

    using Type = regina::IsoSigRidgeDegrees<dim>;
    auto s = pybind11::class_<Type>(m, name, rdoc::IsoSigDegrees)
        .def(pybind11::init<const regina::Component<dim>&>(), rbase::__init)
        .def("simplex", &Type::simplex, rbase::simplex)
        .def("perm", &Type::perm, rbase::perm)
        .def("next", &Type::next, rbase::next)
        ;
    regina::python::disable_eq_operators(s);

    RDOC_SCOPE_END
}

template <int dim>
requires (regina::supportedDim(dim))
void addIsoSigData(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(IsoSigData)

    using Data = regina::IsoSigData<dim>;
    auto s = pybind11::class_<Data>(m, name, rdoc_scope)
        .def(pybind11::init<regina::Component<dim>*>(), rdoc::__init)
        .def(pybind11::init<const Data&>(), rdoc::__copy)
        .def("size", &Data::size, rdoc::size)
        .def("facetTypes", [](const Data& data) {
            pybind11::list ans;
            for (auto f : data.facetTypes())
                ans.append(f);
            return ans;
        }, rdoc::facetTypes)
        .def("adjacentSimplices", [](const Data& data) {
            pybind11::list ans;
            for (auto f : data.adjacentSimplices())
                ans.append(f);
            return ans;
        }, rdoc::adjacentSimplices)
        .def("adjacentGluings", [](const Data& data) {
            pybind11::list ans;
            for (auto f : data.adjacentGluings())
                ans.append(f);
            return ans;
        }, rdoc::adjacentGluings)
        .def("locks", [](const Data& data) {
            pybind11::list ans;
            for (auto f : data.locks())
                ans.append(f);
            return ans;
        }, rdoc::locks)
        .def("hasLocks", &Data::hasLocks, rdoc::hasLocks)
        .def("fillFrom", &Data::fillFrom,
            pybind11::arg("simplex"), pybind11::arg("vertices"),
            pybind11::arg("relabelling") = nullptr,
            rdoc::fillFrom)
        .def("swap", &Data::swap, rdoc::swap)
        ;
    regina::python::add_eq_operators(s, rdoc::__eq);

    regina::python::add_global_swap<Data>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

template <int dim, bool supportLocks>
requires (regina::supportedDim(dim))
void addIsoSigPrintable(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(IsoSigPrintable)

    using Encoding = regina::IsoSigPrintable<dim, supportLocks>;
    auto s = pybind11::class_<Encoding>(m, name, rdoc_scope)
        .def_readonly_static("charsPerPerm", &Encoding::charsPerPerm)
        .def_static("encodeEmpty", &Encoding::encodeEmpty, rdoc::encodeEmpty)
        .def_static("encode", &Encoding::encode, rdoc::encode)
        ;
    regina::python::no_eq_static(s);

    RDOC_SCOPE_END
}

