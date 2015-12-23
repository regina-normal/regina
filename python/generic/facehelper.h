
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include <boost/python.hpp>
#include "generic/face.h"
#include "../helpers.h"

namespace regina {

template <int, int> class Face;

namespace python {

template <class T, int dim, int subdim>
boost::python::list faces_list(const T& t, int subdimArg) {
    boost::python::list ans;
    for (auto f : t.template faces<subdim>())
        ans.append(boost::python::ptr(f));
    return ans;
}

template <class T, int dim, int subdim>
struct FaceHelper {
    typedef regina::Face<dim, subdim> Face;

    static size_t countFacesFrom(const T& t, int subdimArg) {
        if (subdimArg == subdim)
            return t.template countFaces<subdim>();
        return FaceHelper<T, dim, subdim - 1>::countFacesFrom(t, subdimArg);
    }

    static PyObject* faceFrom(const T& t, int subdimArg, size_t f) {
        // TODO: Make this work with return_internal_reference.
        // That is, ensure a lifespan dependency between t and the result.
        if (subdimArg == subdim) {
            boost::python::to_python_indirect<regina::Face<dim, subdim>&,
                boost::python::detail::make_reference_holder> convert;
            return convert(t.template face<subdim>(f));
        }
        return FaceHelper<T, dim, subdim - 1>::faceFrom(t, subdimArg, f);
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
};

template <class T, int dim>
struct FaceHelper<T, dim, 0> {
    typedef regina::Face<dim, 0> Face;

    static size_t countFacesFrom(const T& t, int) {
        return t.template countFaces<0>();
    }

    static PyObject* faceFrom(const T& t, int, size_t f) {
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
};

void invalidFaceDimension(const char* function, int dim);

template <class T, int dim>
size_t countFaces(const T& t, int subdimArg) {
    if (subdimArg < 0 || subdimArg >= dim)
        invalidFaceDimension("countFaces", dim);
    return FaceHelper<T, dim, dim - 1>::countFacesFrom(t, subdimArg);
}

template <class T, int dim>
PyObject* face(const T& t, int subdimArg, size_t f) {
    if (subdimArg < 0 || subdimArg >= dim)
        invalidFaceDimension("face", dim);
    return FaceHelper<T, dim, dim - 1>::faceFrom(t, subdimArg, f);
}

template <class T, int dim>
boost::python::list faces(const T& t, int subdimArg) {
    if (subdimArg < 0 || subdimArg >= dim)
        invalidFaceDimension("faces", dim);
    return FaceHelper<T, dim, dim - 1>::facesFrom(t, subdimArg);
}

} } // namespace regina::python

