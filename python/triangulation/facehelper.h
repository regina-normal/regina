
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

#ifndef __REGINA_PYTHON_FACEHELPER_H
#define __REGINA_PYTHON_FACEHELPER_H

#include <pybind11/pybind11.h>
#include "triangulation/forward.h"
#include "../helpers.h"

namespace regina::python {

/**
 * A type associated with triangulations that holds faces of different
 * dimensions, and that offers Python-friendly `countFaces()`, `faces()` and
 * `face()` functions that take the face dimension as a runtime argument.
 *
 * Examples of such types include `Triangulation<dim>`, `Component<dim>`, and
 * `BoundaryComponent<dim>`.
 */
template <typename T>
concept FaceHolder = requires(const T x, int subdim, size_t index) {
    { x.countFaces(subdim) } -> std::same_as<size_t>;
    x.faces(subdim);
    x.face(subdim, index);
};

/**
 * The type of the function pointer `T::countFaces(subdim)`.
 *
 * This can help distinguish non-templated functions of the form
 * `T::countFaces(subdim)` (which are wrapped in Python) from templated
 * functions `T::countFaces<subdim>()` (which are not).
 */
template <FaceHolder T>
using countFacesFunc = size_t (T::*)(int) const;

/**
 * The type of the function pointer `T::face(subdim, index)`.
 *
 * This can help distinguish the non-templated `face(subdim, index)` (which is
 * wrapped in Python) from the templated `face<subdim>(index)` (which is not).
 */
template <FaceHolder T>
using faceFunc = decltype(std::declval<T>().face(0, 0)) (T::*)(int, size_t)
    const;

/**
 * The type of the function pointer `T::faces(subdim)`.
 *
 * This can help distinguish the non-templated `faces(subdim)` (which is
 * wrapped in Python) from the templated `faces<subdim>()` (which is not).
 */
template <FaceHolder T>
using facesFunc = decltype(std::declval<T>().faces(0)) (T::*)(int) const;

/**
 * The type of the function pointer `Vertex<dim>::linkingSurface()`.
 *
 * This can help disambiguate the different variants of linkingSurface():
 * although at most one is available for any `(dim, subdim)` combination,
 * gcc is not able to distinguish between them without an explicit cast
 * (but clang is fine).
 */
template <int dim> requires (dim == 3 || dim == 4)
using vertexLinkingSurface =
    regina::SafeHypersurface<dim> (regina::Vertex<dim>::*)() const;

/**
 * The type of the function pointer `Face<dim, subdim>::linkingSurface()`
 * where `subdim > 0`.
 *
 * This can help disambiguate the different variants of linkingSurface():
 * although at most one is available for any `(dim, subdim)` combination,
 * gcc is not able to distinguish between them without an explicit cast
 * (but clang is fine).
 */
template <int dim, int subdim> requires ((dim == 3 || dim == 4) && subdim > 0)
using generalLinkingSurface =
    std::pair<regina::SafeHypersurface<dim>, bool>
        (regina::Face<dim, subdim>::*)() const;

/**
 * The Python binding for the C++ template member function
 * `T::face<lowerdim>(f)`, where the C++ template parameter \a lowerdim is
 * passed to Python at runtime.
 *
 * The return value policy will be treated as
 * `pybind11::return_value_policy::reference`.
 */
template <FaceOrSimplexClass T>
requires (T::subdimension > 0)
pybind11::object face(const T& t, int lowerdim, int f) {
    if (lowerdim < 0 || lowerdim >= T::subdimension)
        throw InvalidArgument("face(): unsupported subface dimension");

    // Since the return type of face() depends on lowerdim, we convert return
    // values to python objects now instead of letting pybind11 do it later.
    //
    // The cost: this circumvents pybind11's normal casting mechanism, and so
    // we do not get the lifespan relationships that we would normally get from
    // return_value_policy::reference_internal.  Instead we just get the
    // default return value policy when passing a pointer to pybind11::cast(),
    // which is pybind11::return_value_policy::reference.
    return select_constexpr<0, T::subdimension, pybind11::object>(lowerdim,
            [&t, f](auto k) {
        return pybind11::cast(t.template face<k>(f));
    });
}

/**
 * The Python binding for the C++ template member function
 * `T::faceMapping<lowerdim>(f)`, where the C++ template parameter \a lowerdim
 * is passed to Python at runtime.
 */
template <FaceOrSimplexClass T>
requires (T::subdimension > 0)
Perm<T::dimension + 1> faceMapping(const T& t, int lowerdim, int f) {
    if (lowerdim < 0 || lowerdim >= T::subdimension)
        throw InvalidArgument("faceMapping(): unsupported subface dimension");
    return select_constexpr<0, T::subdimension, Perm<T::dimension + 1>>(
            lowerdim, [&t, f](auto k) {
        return t.template faceMapping<k>(f);
    });
}

} // namespace regina::python

#endif
