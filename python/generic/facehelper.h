
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include "triangulation/generic.h"
#include "../helpers.h"

namespace pybind11 { namespace detail {

/**
 * Tell pybind11 how to convert a C++ list of triangulation faces into a
 * Python list.  This allows pybind11 to automagically convert the return
 * values for functions such as vertices(), edges(), faces(subdim), etc.,
 * within the C++ classes Triangulation<dim> and Component<dim>.
 */
template <int dim, int subdim>
struct type_caster<regina::FaceList<dim, subdim>> {
    private:
        typedef regina::FaceList<dim, subdim> FaceListType;

    public:
        PYBIND11_TYPE_CASTER(FaceListType, _("FaceList"));

        bool load(handle, bool) {
            // Never allow conversion from Python to a C++ FaceList.
            return false;
        }

        static handle cast(const FaceListType& src, return_value_policy policy,
                handle parent) {
            // Conversion from C++ to Python:
            pybind11::list ans;
            for (auto f : src)
                ans.append(pybind11::cast(f, policy, parent));
            return ans.release();
        }
};

} } // namespace pybind11::detail

namespace regina {
namespace python {

/**
 * A helper class used to enforce return value policies for member functions
 * that return C++ pointers, when the return value has already been converted
 * to a pybind11 object.
 *
 * The object passed to the constructor is the object whose member
 * function we are calling (i.e., "this" in C++).
 *
 * Once the function has been called and the return value converted to a
 * pybind11 object, the function addNurse() should be called to enforce
 * the corresponding policy.  If the return value is a python container
 * (e.g., a pybind11::list), then addNurse() should be called on each
 * element of the container.
 *
 * Why have this class at all?  Some functions (such as Triangulation::face()
 * and Triangulation::faces()) do not have a well-defined C++ return type, and
 * therefore must cast the return value to a pybind11 object as part of the
 * C++ function call (in order to ensure a consistent return type).  If this
 * cast is done by calling pybind11::cast() on a C++ pointer, it always uses
 * the policy pybind11::return_value_policy::reference.  We therefore use
 * this class to adjust the return value policy if necessary afterwards.
 */
template <pybind11::return_value_policy policy>
class PatientManager;

template <>
class PatientManager<pybind11::return_value_policy::reference> {
    // Nothing to do, since pybind11::cast() already uses this policy
    // when given a C++ pointer.
    public:
        template <typename T>
        PatientManager(const T& patient) {}

        void addNurse(pybind11::handle nurse) {}
};

template <>
class PatientManager<pybind11::return_value_policy::reference_internal> {
    // This policy requires us to add an extra keep_alive for each nurse.
    private:
        pybind11::handle patient_;

    public:
        template <typename T>
        PatientManager(const T& patient) {
            patient_ = pybind11::cast(patient);
        }
        void addNurse(pybind11::handle nurse) {
            pybind11::detail::keep_alive_impl(nurse, patient_);
        }
};

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
 * to python objects here, instead of letting pybind11 do it later.
 * The cost of returning a pybind11::object is that we circumvent pybind11's
 * normal casting mechanism, and so we do not get the lifespan relationships
 * that we would normally get from return_value_policy::reference_internal
 * (as we do get, for instance, through fixed-subdimension routines such
 * as vertex() or vertices()).
 *
 * Note: when given a pointer, pybind11::cast() and pybind11::list::append()
 * both default to a return value policy of reference, not take_ownership.
 */
template <class T, int dim, int subdim>
struct FaceHelper {
    typedef regina::Face<dim, subdim> Face;

    static size_t countFacesFrom(const T& t, int subdimArg) {
        if (subdimArg == subdim)
            return t.template countFaces<subdim>();
        return FaceHelper<T, dim, subdim - 1>::countFacesFrom(t, subdimArg);
    }

    template <typename Index, pybind11::return_value_policy policy>
    static pybind11::object faceFrom(const T& t, int subdimArg, Index f) {
        if (subdimArg == subdim) {
            PatientManager<policy> patient(t);
            auto ans = pybind11::cast(t.template face<subdim>(f));
            patient.addNurse(ans);
            return ans;
        }
        return FaceHelper<T, dim, subdim - 1>::template faceFrom<Index, policy>(
            t, subdimArg, f);
    }

    template <pybind11::return_value_policy policy>
    static pybind11::object facesFrom(const T& t, int subdimArg) {
        if (subdimArg == subdim) {
            PatientManager<policy> patient(t);
            pybind11::list ans;
            for (auto f : t.template faces<subdim>()) {
                auto elt = pybind11::cast(f);
                patient.addNurse(elt);
                ans.append(elt);
            }
            return ans;
        }
        return FaceHelper<T, dim, subdim - 1>::template facesFrom<policy>(
            t, subdimArg);
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

    template <typename Index, pybind11::return_value_policy policy>
    static pybind11::object faceFrom(const T& t, int, Index f) {
        PatientManager<policy> patient(t);
        auto ans = pybind11::cast(t.template face<0>(f));
        patient.addNurse(ans);
        return ans;
    }

    template <pybind11::return_value_policy policy>
    static pybind11::object facesFrom(const T& t, int) {
        PatientManager<policy> patient(t);
        pybind11::list ans;
        for (auto f : t.template faces<0>()) {
            auto elt = pybind11::cast(f);
            patient.addNurse(elt);
            ans.append(elt);
        }
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

    template <typename Index, pybind11::return_value_policy policy>
    static pybind11::object faceFrom(const T&, int, Index) {
        throw -1;
    }

    template <pybind11::return_value_policy policy>
    static pybind11::object facesFrom(const T&, int) {
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
template <class T, int dim, typename Index,
        pybind11::return_value_policy policy>
pybind11::object face(const T& t, int subdimArg, Index f) {
    if (subdimArg < 0 || subdimArg >= dim)
        invalidFaceDimension("face", dim);
    return FaceHelper<T, dim, dim - 1>::template faceFrom<Index, policy>(
        t, subdimArg, f);
}

/**
 * The Python binding for the C++ template member function
 * T::faces<subdimArg>(), where the valid range for the C++ template
 * parameter \a subdimArg is 0, ..., <i>dim</i>-1.
 */
template <class T, int dim, pybind11::return_value_policy policy>
pybind11::object faces(const T& t, int subdimArg) {
    if (subdimArg < 0 || subdimArg >= dim)
        invalidFaceDimension("faces", dim);
    return FaceHelper<T, dim, dim - 1>::template facesFrom<policy>(
        t, subdimArg);
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

