
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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
#include <boost/optional.hpp>

using namespace boost::python;

namespace regina {
namespace python {

template<typename T>
struct register_int_from_py_index {

    static
    bool _in_range(const long value) {
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

    static
    boost::optional<T> _convert_py_int(PyObject * obj) {
        if (PyInt_Check(obj)) {
            // No error checking
            const long value = PyInt_AS_LONG(obj);
            if (_in_range(value)) {
                return value;
            }
        }
        
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
    
    static
    boost::optional<T> _convert_py_index(PyObject * const obj) {
        if (not obj) {
            return boost::none;
        }
        
        PyObject * const attr = PyObject_GetAttrString(obj, "__index__");
        if (not PyCallable_Check(attr)) {
            return boost::none;
        }
        
        PyObject * const args = PyTuple_New(0);
        if (not args) {
            return boost::none;
        }
        
        PyObject * const result = PyObject_CallObject(attr, args);
        Py_DECREF(args);
        
        if (not result) {
            return boost::none;
        }

        const boost::optional<T> value = _convert_py_int(result);
        
        Py_DECREF(result);
        
        return value;
    }
    
    static
    void * convertible(PyObject * const obj) {
        if (_convert_py_index(obj)) {
            return obj;
        }
        
        return nullptr;
    }

    using converter_data =
        boost::python::converter::rvalue_from_python_stage1_data;
    
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

    register_int_from_py_index() {
        boost::python::converter::registry::push_back(
            convertible,
            construct,
            boost::python::type_id<T>());
    }
};

} } // namespace regina::python

static
void _registerIntFromPyIndex()
{
    regina::python::register_int_from_py_index<unsigned long>();
}

void addRegisterIntFromPyIndex() {
    def("_registerIntFromPyIndex", &_registerIntFromPyIndex);
}
