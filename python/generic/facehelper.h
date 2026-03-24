
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

#ifndef __REGINA_PYTHON_FACEHELPER_H
#define __REGINA_PYTHON_FACEHELPER_H

#include <pybind11/pybind11.h>
#include "triangulation/generic.h"
#include "../helpers.h"

namespace regina::python {

/**
 * The type of the function pointer `T::countFaces(subdim)`.
 *
 * This can help distinguish non-templated functions of the form
 * `T::countFaces(subdim)` (which are wrapped in Python) from templated
 * functions `T::countFaces<subdim>()` (which are not).
 */
template <typename T>
using countFacesFunc = size_t (T::*)(int) const;

/**
 * The type of the function pointer `T::face(subdim, index)`.
 *
 * This can help distinguish the non-templated `face(subdim, index)` (which is
 * wrapped in Python) from the templated `face<subdim>(index)` (which is not).
 */
template <typename T>
using faceFunc = decltype(std::declval<T>().face(0, 0)) (T::*)(int, size_t)
    const;

/**
 * The type of the function pointer `T::faces(subdim)`.
 *
 * This can help distinguish the non-templated `faces(subdim)` (which is
 * wrapped in Python) from the templated `faces<subdim>()` (which is not).
 */
template <typename T>
using facesFunc = decltype(std::declval<T>().faces(0)) (T::*)(int) const;

/**
 * Implementation details for Python bindings of template member functions.
 *
 * Python does not support templates, and so we bind C++ template member
 * functions (such as `Component::faces<subdim>()` or `Simplex::face<subdim>())`
 * by converting the C++ template argument \a subdim into the the first
 * argument of the Python function (i.e., the function in Python has one more
 * argument than in C++).
 *
 * This helper class is designed to work with "auxiliary" types \a T such as
 * Component and BoundaryComponent, not the "primary" type
 * `Triangulation<dim>`.  This is due to the limitations surrounding lifespan
 * management (see below for details).
 *
 * Note that some of these C++ functions return different types depending on
 * the argument \a subdim; we resolve this by converting return values
 * to python objects here, instead of letting pybind11 do it later.
 * The cost of returning a `pybind11::object` is that we circumvent pybind11's
 * normal casting mechanism, and so we do not get the lifespan relationships
 * that we would normally get from `return_value_policy::reference_internal`
 * (as we do get, for instance, through fixed-subdimension routines such
 * as `vertex()` or `vertices()`).  Instead all objects that are returned
 * will be treated with a policy of `pybind11::return_value_policy::reference`.
 *
 * Note: when given a pointer, `pybind11::cast()` and `pybind11::list::append()`
 * both default to a return value policy of `reference`, not `take_ownership`.
 *
 * \pre All of the functions in FaceHelper have the precondition that
 * `0 <= subdimArg <= subdim`.
 */
template <typename T, int subdim>
requires (subdim >= 0 && subdim <= T::dimension)
struct FaceHelper {
    static pybind11::list facesFrom(const T& t, int subdimArg) {
        if constexpr (subdim == 0) {
            // Must have subdimArg == 0.
            pybind11::list ans;
            for (auto f : t.template faces<0>())
                ans.append(pybind11::cast(f));
            return ans;
        } else {
            if (subdimArg == subdim) {
                pybind11::list ans;
                for (auto f : t.template faces<subdim>())
                    ans.append(pybind11::cast(f));
                return ans;
            } else
                return FaceHelper<T, subdim-1>::facesFrom(t, subdimArg);
        }
    }

    static pybind11::object faceFrom(const T& t, int subdimArg, size_t f) {
        if constexpr (subdim == 0) {
            // Must have subdimArg == 0.
            return pybind11::cast(t.template face<0>(f));
        } else {
            if (subdimArg == subdim)
                return pybind11::cast(t.template face<subdim>(f));
            else
                return FaceHelper<T, subdim-1>::faceFrom(t, subdimArg, f);
        }
    }
};

/**
 * Throws an exception.  The error message will state that the argument
 * for the face dimension (which should be the first argument of the original
 * function, corresponding to the C++ template argument) must be in the
 * range \a minDim, ..., \a maxDim.
 */
void invalidFaceDimension(const char* functionName, int minDim, int maxDim);

/**
 * The Python binding for the C++ template member function
 * `T::faces<subdim>()`, where the valid range for the C++ template
 * parameter \a subdim is `0, ..., T::dimension-1`.
 *
 * This is used by Component<dim> and BoundaryComponent<dim>, all for
 * standard dim only.
 *
 * The return value policy will be treated as
 * pybind11::return_value_policy::reference.
 */
template <typename T>
pybind11::object faces(const T& t, int subdim) {
    if (subdim < 0 || subdim >= T::dimension)
        throw InvalidArgument("faces(): unsupported face dimension");
    return FaceHelper<T, T::dimension - 1>::facesFrom(t, subdim);
}

/**
 * The Python binding for the C++ template member function
 * `T::face<subdim>(f)`, where the valid range for the C++ template
 * parameter \a subdim is `0, ..., T::dimension-1`.
 *
 * This is used by Component<dim> and BoundaryComponent<dim>, all for
 * standard dim only.
 *
 * The return value policy will be treated as
 * pybind11::return_value_policy::reference.
 */
template <typename T>
pybind11::object face(const T& t, int subdim, size_t f) {
    if (subdim < 0 || subdim >= T::dimension)
        throw InvalidArgument("face(): unsupported face dimension");
    return FaceHelper<T, T::dimension - 1>::faceFrom(t, subdim, f);
}

/**
 * The Python binding for the C++ template member function
 * `Face<dim, subdim>::face<lowerdim>(f)`, where the valid range for
 * the C++ template parameter \a lowerdim is `0, ..., subdim-1`.
 *
 * The return value policy will be treated as
 * `pybind11::return_value_policy::reference`.
 */
template <int dim, int subdim>
requires (subdim > 0)
pybind11::object face(const Face<dim, subdim>& t, int lowerdim, int f) {
    if (lowerdim < 0 || lowerdim >= subdim)
        throw InvalidArgument("face(): unsupported subface dimension");

    // Since the return type of face() depends on lowerdim, we convert return
    // values to python objects now instead of letting pybind11 do it later.
    //
    // The cost: this circumvents pybind11's normal casting mechanism, and so
    // we do not get the lifespan relationships that we would normally get from
    // return_value_policy::reference_internal.  Instead we just get the
    // default return value policy when passing a pointer to pybind11::cast(),
    // which is pybind11::return_value_policy::reference.
    return select_constexpr<0, subdim, pybind11::object>(lowerdim,
            [&t, f](auto k) {
        return pybind11::cast(t.template face<k>(f));
    });
}

/**
 * The Python binding for the C++ template member function
 * `Face<dim, subdim>::faceMapping<lowerdim>(f)`, where the valid range for
 * the C++ template parameter \a lowerdim is `0, ..., subdim-1`.
 */
template <int dim, int subdim>
requires (subdim > 0)
Perm<dim + 1> faceMapping(const Face<dim, subdim>& t, int lowerdim, int f) {
    if (lowerdim < 0 || lowerdim >= subdim)
        throw InvalidArgument("faceMapping(): unsupported subface dimension");
    return select_constexpr<0, subdim, Perm<dim + 1>>(lowerdim,
            [&t, f](auto k) {
        return t.template faceMapping<k>(f);
    });
}

} // namespace regina::python

#endif
