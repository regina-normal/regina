
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
#include "../helpers/bytesequence.h"
#include "../docstrings/triangulation/isosig.h"

using regina::IsoSigBinary;
using regina::IsoSigPrintable;
using regina::IsoSigPrintableLockFree;

using regina::IsoSigClassic;
using regina::IsoSigEdgeDegrees;
using regina::IsoSigRidgeDegrees;

namespace regina::python {

/**
 * This function:
 *
 * - converts the Python type \a encoding into a C++ type \a Encoding, which
 *   must represent an encoding for the given generation of isomorphism
 *   signature, or throws an InvalidArgument exception if this is not possible;
 *
 * - calls the generic lambda \a action with \a Encoding as its template
 *   parameter, and returns the result.
 *
 * The generic lambda \a action should be of the form
 * `[=]<regina::IsoSigEncoding Encoding>() { ... }`, and its return type
 * should not depend on the template parameter \a Encoding.
 */
template <int generation>
requires (generation == 1 || generation == 2)
auto forIsoSigEncoding(pybind11::type encoding, auto action) {
    using pytype = pybind11::type;
    if (encoding.is(pytype::of<IsoSigPrintable>()))
        return action.template operator()<IsoSigPrintable>();
    if (encoding.is(pytype::of<IsoSigPrintableLockFree>()))
        return action.template operator()<IsoSigPrintableLockFree>();
    if constexpr (generation > 1)
        if (encoding.is(pytype::of<IsoSigBinary>()))
            return action.template operator()<IsoSigBinary>();
    throw regina::InvalidArgument(
        "Not a supported isomorphism signature encoding");
}

/**
 * This function:
 *
 * - converts the Python type \a sigType into a C++ type \a Type, which must
 *   represent a <i>dim</i>-dimensional isomorphism signature type, or throws
 *   an InvalidArgument exception if this is not possible;
 *
 * - calls the generic lambda \a action with \a Type as its template parameter,
 *   and returns the result.
 *
 * The generic lambda \a action should be of the form
 * `[=]<regina::IsoSigType<dim> Type>() { ... }`, and its return type should
 * not depend on the template parameter \a Type.
 */
template <int dim> requires (regina::supportedDim(dim))
auto forIsoSigType(pybind11::type sigType, auto action) {
    using pytype = pybind11::type;
    if (sigType.is(pytype::of<IsoSigClassic<dim>>()))
        return action.template operator()<IsoSigClassic<dim>>();
    if (sigType.is(pytype::of<IsoSigRidgeDegrees<dim>>()))
        return action.template operator()<IsoSigRidgeDegrees<dim>>();
    if (sigType.is(pytype::of<IsoSigEdgeDegrees<dim>>()))
        return action.template operator()<IsoSigEdgeDegrees<dim>>();
    throw regina::InvalidArgument("Not a supported isomorphism signature type");
}

/**
 * Adds the generation/dimension-specific functions to the python bindings for
 * the given encoding class.
 *
 * The type Encoding must conform to `IsoSigEncoding<generation, dim>`; however,
 * we do not enforce it as a constraint because we do not want to instantiate
 * the full triangulation classes for all dimensions at once when binding an
 * encoding class.
 */
template <int generation, int dim, typename Encoding>
requires ((generation == 1 || generation == 2) && regina::supportedDim(dim))
void add_isosig_encoding_functions(pybind11::class_<Encoding>& c) {
    static_assert(regina::IsoSigEncoding<Encoding, generation, dim>);

    c.def_static("encode",
        static_cast<typename Encoding::Signature (*)(
            const IsoSigData<generation, dim>&)>(&Encoding::encode));
    c.def_static("length",
        static_cast<size_t (*)(const IsoSigData<generation, dim>&)>(
            &Encoding::length));
}

/**
 * Adds all of the available variants of sig() / isoSig() / neoSig() to the
 * python bindings for a C++ triangulation class.
 *
 * To use this for the C++ class `Triangulation<dim>`, simply call
 * `regina::python::add_isosig_variants<dim>(c)`, where \a c is the
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
void add_isosig_variants(PythonClass& classWrapper) {
    RDOC_SCOPE_BASE(detail::TriangulationBase)

    // When encodings are passed as runtime options, we use pybind11::cast()
    // on the return values since different encodings may give different return
    // types.
    //
    // I'm not sure if there's a good (and fast) way to specify defaults for
    // Encoding/Type arguments, which does not involve gratuitous conversions
    // at load time and does not enforce constraints on the order in which
    // classes are wrapped.  Happy for suggestions here.  In the meantime,
    // instead of using default arguments, we will just wrap multiple versions
    // of signature functions (with vs without the defaults).

    classWrapper
        .def("isoSig", &Triangulation<dim>::template isoSig<>, rbase::isoSig)
        .def("isoSig", [](const Triangulation<dim>& t,
                pybind11::type encoding) {
            return regina::python::forIsoSigEncoding<1>(encoding,
                    [&t]<regina::IsoSigEncoding<1, dim> Encoding>() {
                return pybind11::cast(t.template isoSig<Encoding>());
            });
        }, pybind11::arg("Encoding"), rbase::isoSig)
        .def("isoSig", [](const Triangulation<dim>& t,
                pybind11::type encoding, pybind11::type sigType) {
            return regina::python::forIsoSigEncoding<1>(encoding,
                    [&t, sigType]<regina::IsoSigEncoding<1, dim> Encoding>() {
                return regina::python::forIsoSigType<dim>(sigType,
                        [&t]<regina::IsoSigType<dim> Type>() {
                    return pybind11::cast(t.template isoSig<Encoding, Type>());
                });
            });
        }, pybind11::arg("Encoding"), pybind11::arg("Type"), rbase::isoSig)

        .def("neoSig", &Triangulation<dim>::template neoSig<>,
            pybind11::arg("oriented") = false, rbase::neoSig)
        .def("neoSig", [](const Triangulation<dim>& t, bool oriented,
                pybind11::type encoding) {
            return regina::python::forIsoSigEncoding<2>(encoding,
                    [&t, oriented]<regina::IsoSigEncoding<2, dim> Encoding>() {
                return pybind11::cast(t.template neoSig<Encoding>(oriented));
            });
        }, pybind11::arg("oriented"), // no default, since Encoding is required
            pybind11::arg("Encoding"),
            rbase::neoSig)
        .def("neoSig", [](const Triangulation<dim>& t, bool oriented,
                pybind11::type encoding, pybind11::type sigType) {
            return regina::python::forIsoSigEncoding<2>(encoding,
                    [=, &t]<regina::IsoSigEncoding<2, dim> Encoding>() {
                return regina::python::forIsoSigType<dim>(sigType,
                        [=, &t]<regina::IsoSigType<dim> Type>() {
                    return pybind11::cast(
                        t.template neoSig<Encoding, Type>(oriented));
                });
            });
        }, pybind11::arg("oriented"), // no default, since Encoding is required
            pybind11::arg("Encoding"),
            pybind11::arg("Type"),
            rbase::neoSig)

        .def("isoSigDetail", &Triangulation<dim>::template isoSigDetail<>,
            rbase::isoSigDetail)
        .def("isoSigDetail", [](const Triangulation<dim>& t,
                pybind11::type encoding) {
            return regina::python::forIsoSigEncoding<1>(encoding,
                    [&t]<regina::IsoSigEncoding<1, dim> Encoding>() {
                return pybind11::cast(t.template isoSigDetail<Encoding>());
            });
        }, pybind11::arg("Encoding"), rbase::isoSigDetail)
        .def("isoSigDetail", [](const Triangulation<dim>& t,
                pybind11::type encoding, pybind11::type sigType) {
            return regina::python::forIsoSigEncoding<1>(encoding,
                    [&t, sigType]<regina::IsoSigEncoding<1, dim> Encoding>() {
                return regina::python::forIsoSigType<dim>(sigType,
                        [&t]<regina::IsoSigType<dim> Type>() {
                    return pybind11::cast(
                        t.template isoSigDetail<Encoding, Type>());
                });
            });
        }, pybind11::arg("Encoding"), pybind11::arg("Type"),
            rbase::isoSigDetail)

        .def("neoSigDetail", &Triangulation<dim>::template neoSigDetail<>,
            pybind11::arg("oriented") = false, rbase::neoSigDetail)
        .def("neoSigDetail", [](const Triangulation<dim>& t, bool oriented,
                pybind11::type encoding) {
            return regina::python::forIsoSigEncoding<2>(encoding,
                    [&t, oriented]<regina::IsoSigEncoding<2, dim> Encoding>() {
                return pybind11::cast(t.template neoSigDetail<Encoding>(
                    oriented));
            });
        }, pybind11::arg("oriented"), // no default, since Encoding is required
            pybind11::arg("Encoding"),
            rbase::neoSigDetail)
        .def("neoSigDetail", [](const Triangulation<dim>& t, bool oriented,
                pybind11::type encoding, pybind11::type sigType) {
            return regina::python::forIsoSigEncoding<2>(encoding,
                    [=, &t]<regina::IsoSigEncoding<2, dim> Encoding>() {
                return regina::python::forIsoSigType<dim>(sigType,
                        [=, &t]<regina::IsoSigType<dim> Type>() {
                    return pybind11::cast(
                        t.template neoSigDetail<Encoding, Type>(oriented));
                });
            });
        }, pybind11::arg("oriented"), // no default, since Encoding is required
            pybind11::arg("Encoding"),
            pybind11::arg("Type"),
            rbase::neoSigDetail)

        .def("sig", [](const Triangulation<dim>& t, int gen, bool oriented) {
            if (gen == 1)
                return t.template sig<1>(oriented);
            else if (gen == 2)
                return t.template sig<2>(oriented);
            else
                throw regina::InvalidArgument(
                    "Not a supported generation of isomorphism signature");
        }, pybind11::arg("generation"),
            pybind11::arg("oriented") = false,
            rbase::sig)
        .def("sig", [](const Triangulation<dim>& t, int gen, bool oriented,
                pybind11::type encoding) {
            if (gen == 1) {
                return regina::python::forIsoSigEncoding<1>(encoding,
                        [=, &t]<regina::IsoSigEncoding<1, dim> Encoding>() {
                    return pybind11::cast(t.template sig<1, Encoding>(
                        oriented));
                });
            } else if (gen == 2) {
                return regina::python::forIsoSigEncoding<2>(encoding,
                        [=, &t]<regina::IsoSigEncoding<2, dim> Encoding>() {
                    return pybind11::cast(t.template sig<2, Encoding>(
                        oriented));
                });
            } else {
                throw regina::InvalidArgument(
                    "Not a supported generation of isomorphism signature");
            }
        }, pybind11::arg("generation"),
            pybind11::arg("oriented"), // no default, since Encoding is required
            pybind11::arg("Encoding"),
            rbase::sig)
        .def("sig", [](const Triangulation<dim>& t, int gen, bool oriented,
                pybind11::type encoding, pybind11::type sigType) {
            if (gen == 1) {
                return regina::python::forIsoSigEncoding<1>(encoding,
                        [=, &t]<regina::IsoSigEncoding<1, dim> Encoding>() {
                    return regina::python::forIsoSigType<dim>(sigType,
                            [=, &t]<regina::IsoSigType<dim> Type>() {
                        return pybind11::cast(t.template sig<1, Encoding, Type>(
                            oriented));
                    });
                });
            } else if (gen == 2) {
                return regina::python::forIsoSigEncoding<2>(encoding,
                        [=, &t]<regina::IsoSigEncoding<2, dim> Encoding>() {
                    return regina::python::forIsoSigType<dim>(sigType,
                            [=, &t]<regina::IsoSigType<dim> Type>() {
                        return pybind11::cast(t.template sig<2, Encoding, Type>(
                            oriented));
                    });
                });
            } else {
                throw regina::InvalidArgument(
                    "Not a supported generation of isomorphism signature");
            }
        }, pybind11::arg("generation"),
            pybind11::arg("oriented"), // no default, since Encoding is required
            pybind11::arg("Encoding"),
            pybind11::arg("Type"),
            rbase::sig)
        ;
}

} // namespace regina::python

template <int dim> requires (regina::supportedDim(dim))
void addIsoSigClassic(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(IsoSigClassic)

    using Type = IsoSigClassic<dim>;
    auto s = pybind11::class_<Type>(m, name, rdoc_scope)
        .def(pybind11::init<const regina::Component<dim>&, bool>(),
            rdoc::__init)
        .def("simplex", &Type::simplex, rdoc::simplex)
        .def("perm", &Type::perm, rdoc::perm)
        .def("next", &Type::next, rdoc::next)
        ;
    regina::python::disable_eq_operators(s);

    RDOC_SCOPE_END
}

template <int dim> requires (regina::supportedDim(dim))
void addIsoSigEdgeDegrees(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(IsoSigDegrees)

    using Type = IsoSigEdgeDegrees<dim>;
    auto s = pybind11::class_<Type>(m, name, rdoc_scope)
        .def(pybind11::init<const regina::Component<dim>&, bool>(),
            rdoc::__init)
        .def("simplex", &Type::simplex, rdoc::simplex)
        .def("perm", &Type::perm, rdoc::perm)
        .def("next", &Type::next, rdoc::next)
        ;
    regina::python::disable_eq_operators(s);

    RDOC_SCOPE_END
}

template <int dim> requires (regina::supportedDim(dim))
void addIsoSigRidgeDegrees(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(IsoSigDegrees)

    using Type = IsoSigRidgeDegrees<dim>;
    auto s = pybind11::class_<Type>(m, name, rdoc_scope)
        .def(pybind11::init<const regina::Component<dim>&, bool>(),
            rdoc::__init)
        .def("simplex", &Type::simplex, rdoc::simplex)
        .def("perm", &Type::perm, rdoc::perm)
        .def("next", &Type::next, rdoc::next)
        ;
    regina::python::disable_eq_operators(s);

    RDOC_SCOPE_END
}

template <int generation, int dim>
requires (regina::supportedDim(dim))
void addIsoSigData(pybind11::module_& m, const char* name) {
    using Data = regina::IsoSigData<generation, dim>;

    RDOC_SCOPE_BASE(IsoSigData) // for global_swap

    if constexpr (generation == 1) {
        RDOC_SCOPE_BEGIN(IsoSigData1)

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
        regina::python::add_global_swap<Data>(m, rbase::global_swap);

        RDOC_SCOPE_END
    } else {
        RDOC_SCOPE_BEGIN(IsoSigData2)

        auto s = pybind11::class_<Data>(m, name, rdoc_scope)
            .def_static("minimal", [](pybind11::type sigType,
                    regina::Component<dim>* c, bool oriented,
                    regina::Isomorphism<dim>* iso) {
                return regina::python::forIsoSigType<dim>(sigType,
                        [=]<regina::IsoSigType<dim> Type>() {
                    return Data::template minimal<Type>(c, oriented, iso);
                });
            },
                pybind11::arg("Type"),
                pybind11::arg("component"),
                pybind11::arg("oriented"),
                pybind11::arg("relabelling") = nullptr,
                rdoc::minimal)
            .def(pybind11::init<const Data&>(), rdoc::__copy)
            .def("size", &Data::size, rdoc::size)
            .def("facetTypes", &Data::facetTypes,
                pybind11::return_value_policy::copy, rdoc::facetTypes)
            .def("countFacetBits", &Data::countFacetBits, rdoc::countFacetBits)
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
            .def("isOriented", &Data::isOriented, rdoc::isOriented)
            .def("swap", &Data::swap, rdoc::swap)
            ;
        regina::python::add_eq_operators(s, rdoc::__eq);
        regina::python::add_cmp_operators(s, rdoc::__cmp);
        regina::python::add_global_swap<Data>(m, rbase::global_swap);

        RDOC_SCOPE_END
    }
}

