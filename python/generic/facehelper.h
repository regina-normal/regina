
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "triangulation/generic.h"
#include "../helpers.h"

namespace regina {
namespace python {

/**
 * A generic function that iterates through <tt>t.faces<subdim>()</tt>
 * and returns the corresponding faces as a Python list.
 *
 * This is used as the Python binding for T.vertices(), T.edges() and so on,
 * for several types T.
 */
template <class T, int dim, int subdim>
boost::python::list faces_list(const T& t) {
    boost::python::list ans;
    for (auto f : t.template faces<subdim>())
        ans.append(boost::python::ptr(f));
    return ans;
}

/**
 * Implementation details for Python bindings of template member functions.
 *
 * Python does not support templates, and so we bind C++ template member
 * functions (such as Triangulation::countFaces<subdim>() or
 * Simplex::face<subdim>()) by converting the C++ template argument \a subdim
 * into the the first argument of the Python function (i.e., the function in
 * Python has one more argument than in C++).
 *
 * Note that some of these C++ functions return different types depending on
 * the argument \a subdim; we resolve this by converting return values
 * to \a PyObject pointers here, instead of letting Boost.Python do it later.
 */
template <class T, int dim, int subdim>
struct FaceHelper {
    typedef regina::Face<dim, subdim> Face;

    static size_t countFacesFrom(const T& t, int subdimArg) {
        if (subdimArg == subdim)
            return t.template countFaces<subdim>();
        return FaceHelper<T, dim, subdim - 1>::countFacesFrom(t, subdimArg);
    }

    template <typename Index>
    static PyObject* faceFrom(const T& t, int subdimArg, Index f) {
        // TODO: Make this work with return_internal_reference.
        // That is, ensure a lifespan dependency between t and the result.
        if (subdimArg == subdim) {
            boost::python::to_python_indirect<regina::Face<dim, subdim>&,
                boost::python::detail::make_reference_holder> convert;
            return convert(t.template face<subdim>(f));
        }
        return FaceHelper<T, dim, subdim - 1>::template faceFrom<Index>(
            t, subdimArg, f);
    }

    static boost::python::list facesFrom(const T& t, int subdimArg) {
        if (subdimArg == subdim) {
            boost::python::list ans;
            for (auto f : t.template faces<subdim>())
                ans.append(boost::python::ptr(f));
            return ans;
        }
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
    typedef regina::Face<dim, 0> Face;

    static size_t countFacesFrom(const T& t, int) {
        return t.template countFaces<0>();
    }

    template <typename Index>
    static PyObject* faceFrom(const T& t, int, Index f) {
        // TODO: Make this work with return_internal_reference.
        // That is, ensure a lifespan dependency between t and the result.
        boost::python::to_python_indirect<regina::Face<dim, 0>&,
            boost::python::detail::make_reference_holder> convert;
        return convert(t.template face<0>(f));
    }

    static boost::python::list facesFrom(const T& t, int) {
        boost::python::list ans;
        for (auto f : t.template faces<0>())
            ans.append(boost::python::ptr(f));
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
    static PyObject* faceFrom(const T&, int, Index) {
        throw -1;
    }

    static boost::python::list facesFrom(const T&, int) {
        throw -1;
    }

    template <int permSize>
    static Perm<permSize> faceMappingFrom(const T&, int, int) {
        throw -1;
    }
};

/**
 * Throws an exception.  The error message will state that the argument
 * for the face dimension (which should be the first argument of the
 * function, corresponding to the C++ template argument) must be in the
 * range 0, ..., <i>dim</i>-1.
 */
void invalidFaceDimension(const char* functionName, int dim);

/**
 * The Python binding for the C++ template member function
 * T::countFaces<subdimArg>(), where the valid range for the C++ template
 * parameter \a subdimArg is 0, ..., <i>dim</i>-1.
 */
template <class T, int dim>
size_t countFaces(const T& t, int subdimArg) {
    if (subdimArg < 0 || subdimArg >= dim)
        invalidFaceDimension("countFaces", dim);
    return FaceHelper<T, dim, dim - 1>::countFacesFrom(t, subdimArg);
}

/**
 * The Python binding for the C++ template member function
 * T::face<subdimArg>(f), where the valid range for the C++ template
 * parameter \a subdimArg is 0, ..., <i>dim</i>-1.
 */
template <class T, int dim, typename Index>
PyObject* face(const T& t, int subdimArg, Index f) {
    if (subdimArg < 0 || subdimArg >= dim)
        invalidFaceDimension("face", dim);
    return FaceHelper<T, dim, dim - 1>::template faceFrom<Index>(
        t, subdimArg, f);
}

/**
 * The Python binding for the C++ template member function
 * T::faces<subdimArg>(), where the valid range for the C++ template
 * parameter \a subdimArg is 0, ..., <i>dim</i>-1.
 */
template <class T, int dim>
boost::python::list faces(const T& t, int subdimArg) {
    if (subdimArg < 0 || subdimArg >= dim)
        invalidFaceDimension("faces", dim);
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
        invalidFaceDimension("faceMapping", dim);
    return FaceHelper<T, dim, dim - 1>::template faceMappingFrom<permSize>(
        t, subdimArg, f);
}

} } // namespace regina::python

