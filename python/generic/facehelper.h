
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

#ifndef __REGINA_PYTHON_FACEHELPER_H
#define __REGINA_PYTHON_FACEHELPER_H

#include "../pybind11/pybind11.h"
#include "triangulation/generic.h"
#include "../helpers.h"

// On some systems we get warnings about regina's helper classes having
// greater visibility than the pybind11 code that it uses.  We can fix
// this by setting the same visibility attributes that pybind11 uses.
#ifdef __GNUG__
    #define MATCH_PYBIND11_VISIBILITY __attribute__((visibility("hidden")))
#else
    #define MATCH_PYBIND11_VISIBILITY
#endif

namespace regina::python {

/**
 * The type of the function pointer T::countFaces(subdim).
 */
template <typename T>
using countFacesFunc = size_t (T::*)(int) const;

/**
 * The type of the function pointer T::face(subdim, index).
 */
template <typename T>
using faceFunc = decltype(T().face(0, 0)) (T::*)(int, size_t) const;

/**
 * The type of the function pointer T::faces(subdim).
 */
template <typename T>
using facesFunc = decltype(T().faces(0)) (T::*)(int) const;

/**
 * Implementation details for Python bindings of template member functions.
 *
 * Python does not support templates, and so we bind C++ template member
 * functions (such as Component::countFaces<subdim>() or
 * Simplex::face<subdim>()) by converting the C++ template argument \a subdim
 * into the the first argument of the Python function (i.e., the function in
 * Python has one more argument than in C++).
 *
 * This helper class is designed to work with "auxiliary" types \a T such as
 * Component and BoundaryComponent, not the "primary" type Triangulation<dim>.
 * This is due to the limitations surrounding lifespan management (see below
 * for details).
 *
 * Note that some of these C++ functions return different types depending on
 * the argument \a subdim; we resolve this by converting return values
 * to python objects here, instead of letting pybind11 do it later.
 * The cost of returning a pybind11::object is that we circumvent pybind11's
 * normal casting mechanism, and so we do not get the lifespan relationships
 * that we would normally get from return_value_policy::reference_internal
 * (as we do get, for instance, through fixed-subdimension routines such
 * as vertex() or vertices()).  Instead all objects that are returned
 * will be treated with a policy of pybind11::return_value_policy::reference.
 *
 * Note: when given a pointer, pybind11::cast() and pybind11::list::append()
 * both default to a return value policy of reference, not take_ownership.
 */
template <class T, int dim, int subdim>
struct FaceHelper {
    using Face = regina::Face<dim, subdim>;

    static size_t countFacesFrom(const T& t, int subdimArg) {
        if (subdimArg == subdim)
            return t.template countFaces<subdim>();
        return FaceHelper<T, dim, subdim - 1>::countFacesFrom(t, subdimArg);
    }

    template <typename Index>
    static pybind11::object faceFrom(const T& t, int subdimArg, Index f) {
        if (subdimArg == subdim)
            return pybind11::cast(t.template face<subdim>(f));
        else
            return FaceHelper<T, dim, subdim - 1>::
                template faceFrom<Index>(t, subdimArg, f);
    }

    static pybind11::list facesFrom(const T& t, int subdimArg) {
        if (subdimArg == subdim) {
            pybind11::list ans;
            for (auto f : t.template faces<subdim>())
                ans.append(pybind11::cast(f));
            return ans;
        } else
            return FaceHelper<T, dim, subdim - 1>::facesFrom(t, subdimArg);
    }

    template <int permSize>
    static Perm<permSize> faceMappingFrom(const T& t, int subdimArg, int f) {
        if (subdimArg == subdim)
            return t.template faceMapping<subdim>(f);
        return FaceHelper<T, dim, subdim - 1>::
            template faceMappingFrom<permSize>(t, subdimArg, f);
    }
};

/**
 * Implementation details for Python bindings of template member functions.
 *
 * See the notes above.
 */
template <class T, int dim>
struct FaceHelper<T, dim, 0> {
    using Face = regina::Face<dim, 0>;

    static size_t countFacesFrom(const T& t, int) {
        return t.template countFaces<0>();
    }

    template <typename Index>
    static pybind11::object faceFrom(const T& t, int, Index f) {
        return pybind11::cast(t.template face<0>(f));
    }

    static pybind11::list facesFrom(const T& t, int) {
        pybind11::list ans;
        for (auto f : t.template faces<0>())
            ans.append(pybind11::cast(f));
        return ans;
    }

    template <int permSize>
    static Perm<permSize> faceMappingFrom(const T& t, int, int f) {
        return t.template faceMapping<0>(f);
    }
};

/**
 * Implementation details for Python bindings of template member functions.
 *
 * See the notes above.
 *
 * The compiler needs to instantiate this class, but none of its methods
 * should ever be called.
 */
template <class T, int dim>
struct FaceHelper<T, dim, -1> {
    static size_t countFacesFrom(const T&, int) {
        throw -1;
    }

    template <typename Index>
    static pybind11::object faceFrom(const T&, int, Index) {
        throw -1;
    }

    static pybind11::list facesFrom(const T&, int) {
        throw -1;
    }

    template <int permSize>
    static Perm<permSize> faceMappingFrom(const T&, int, int) {
        throw -1;
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
 * T::countFaces<subdimArg>(), where the valid range for the C++ template
 * parameter \a subdimArg is 0, ..., \a maxSubdim.
 */
template <class T, int dim, int maxSubdim>
size_t countFaces(const T& t, int subdimArg) {
    if (subdimArg < 0 || subdimArg > maxSubdim)
        invalidFaceDimension("countFaces", 0, maxSubdim);
    return FaceHelper<T, dim, maxSubdim>::countFacesFrom(t, subdimArg);
}

/**
 * The Python binding for the C++ template member function
 * T::face<subdimArg>(f), where the valid range for the C++ template
 * parameter \a subdimArg is 0, ..., <i>dim</i>-1.
 *
 * The return value policy will be treated as
 * pybind11::return_value_policy::reference.
 */
template <class T, int dim, typename Index>
pybind11::object face(const T& t, int subdimArg, Index f) {
    if (subdimArg < 0 || subdimArg >= dim)
        invalidFaceDimension("face", 0, dim - 1);
    return FaceHelper<T, dim, dim - 1>::template faceFrom<Index>(
        t, subdimArg, f);
}

/**
 * The Python binding for the C++ template member function
 * T::faces<subdimArg>(), where the valid range for the C++ template
 * parameter \a subdimArg is 0, ..., <i>dim</i>-1.
 *
 * The return value policy will be treated as
 * pybind11::return_value_policy::reference.
 */
template <class T, int dim>
pybind11::object faces(const T& t, int subdimArg) {
    if (subdimArg < 0 || subdimArg >= dim)
        invalidFaceDimension("faces", 0, dim - 1);
    return FaceHelper<T, dim, dim - 1>::facesFrom(t, subdimArg);
}

/**
 * The Python binding for the C++ template member function
 * T::faceMapping<subdimArg>(f), where the valid range for the C++ template
 * parameter \a subdimArg is 0, ..., <i>dim</i>-1, and where the function
 * returns a permutation on permSize elements.
 */
template <class T, int dim, int permSize = dim + 1>
Perm<permSize> faceMapping(const T& t, int subdimArg, int f) {
    if (subdimArg < 0 || subdimArg >= dim)
        invalidFaceDimension("faceMapping", 0, dim - 1);
    return FaceHelper<T, dim, dim - 1>::template faceMappingFrom<permSize>(
        t, subdimArg, f);
}

} // namespace regina::python

#endif
