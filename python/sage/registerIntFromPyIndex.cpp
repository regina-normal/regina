
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 2019, Matthias Goerner                                  *
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

// Without the from-python conversion we register here, a user could
// not type a command like "T.simplex(0)" into Sage. The reason is
// that the Sage preparser turns it into "T.simplex(Integer(0))" where
// Integer is a sage.rings.integer.Integer.
//
// Luckily, such an Integer follows PEP 357 and provides an __index__
// method. This file implements a from-python conversion for integer types
// checking whether the python object has an __index__ method returning an
// integer.
//
// This conversion is not registered by default. It needs to be registered
// explicitly with engine._registerIntFromPyIndex() in __init__.py when
// we detect regina is run from sage.

#include "../pybind11/pybind11.h"
#if 0
#include <boost/python.hpp>
#include <boost/optional.hpp>

using namespace boost::python;

namespace regina {
namespace python {

// from_python conversion of python objects implementing PEP 357 to integer
// type T. Calling the c'tor will register the conversion with boost::python.
template<typename T>
struct register_int_from_py_index {

    // Checks whether the given value can be held by the integer type T.
    static
    bool _in_range(const long value) {
        // Note that value < std::numeric_limits<T>::min() is not working
        // correctly when T is unsigned and has the same number of bits
        // than value.
        // This is because when comparing an unsigned and signed number with
        // the same number of bits, C will interpret the signed number as
        // unsigned number with the same number of bits, so the negative
        // number becomes a really large unsigned number.
        // Thus, we are branching here:
        if (not std::numeric_limits<T>::is_signed) {
            if (value < 0) {
                return false;
            }
        } else {
            if (value < std::numeric_limits<T>::min()) {
                return false;
            }
        }
        if (value > std::numeric_limits<T>::max()) {
            return false;
        }
        
        return true;
    }

    // Try to convert a python object to an integer of type T.
    // Return empty boost::optional if python object is not holding
    // the right type or the number is too big.
    static
    boost::optional<T> _convert_py_int(PyObject * obj) {
        #if PY_MAJOR_VERSION < 3
        // Python 2 has two integer types, one holding a C long and
        // one holding arbitrary precision.
        if (PyInt_Check(obj)) {
            const long value = PyInt_AS_LONG(obj);
            if (_in_range(value)) {
                return value;
            }
        }
        #endif
        
        if (PyLong_Check(obj)) {
            int overflow;
            const long value = PyLong_AsLongAndOverflow(obj, &overflow);
            if (overflow == 0) {
                if (_in_range(value)) {
                    return value;
                }
            }
        }
        
        return boost::none;
    }

    // Get an integer from a python object by calling its __index__ method
    static
    boost::optional<T> _convert_py_index(PyObject * const obj) {
        if (not obj) {
            return boost::none;
        }

        // Check whether it has __index__
        if (not PyObject_HasAttrString(obj, "__index__")) {
            return boost::none;
        }

        // Get __index__
        PyObject * const attr = PyObject_GetAttrString(obj, "__index__");
        if (not PyCallable_Check(attr)) {
            return boost::none;
        }

        // Arguments for the call to __index__
        PyObject * const args = PyTuple_New(0);
        if (not args) {
            return boost::none;
        }

        // Call __index__
        PyObject * const result = PyObject_CallObject(attr, args);
        Py_DECREF(args);
        
        if (not result) {
            return boost::none;
        }

        // Extract integer from result of call
        const boost::optional<T> value = _convert_py_int(result);
        
        Py_DECREF(result);
        
        return value;
    }

    // Implements boost::python convertible concept.
    //
    // Note that we say it is not convertible if __index__ does not return
    // an integer or the integer is too large. Thus, the user will see in
    // these cases the typical boost message that the signature did not
    // match. It might be better, if we actually threw an exception and
    // gave a python error similar to what, e.g., range(10)[f] does if
    // f.__index__ returned a non-int:
    //    TypeError: __index__ returned non-(int,long) (type str)
    static
    void * convertible(PyObject * const obj) {
        if (_convert_py_index(obj)) {
            return obj;
        }
        
        return nullptr;
    }

    using converter_data =
        boost::python::converter::rvalue_from_python_stage1_data;

    // Implements boost::python constructing a C++ int from a python object
    static
    void construct(PyObject * const obj,
                   converter_data * const data) {

        using converter_storage =
            boost::python::converter::rvalue_from_python_storage<unsigned long>;
        
        void * const storage = ((converter_storage *) data)->storage.bytes;

        if (const boost::optional<T> value = _convert_py_index(obj)) {
            new (storage) T(*value);
        } else {
            new (storage) T(0);
        }
        
        data->convertible = storage;
    }

    // Registers the from_python conversion
    register_int_from_py_index() {
        boost::python::converter::registry::push_back(
            convertible,
            construct,
            boost::python::type_id<T>());
    }
};

} } // namespace regina::python

void addRegisterIntFromPyIndex() {
    regina::python::register_int_from_py_index<size_t>(m);
    regina::python::register_int_from_py_index<int>(m);
}
#endif

