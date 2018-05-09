
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

#include <boost/optional.hpp>
#include <boost/python.hpp>
#include "maths/integer.h"
#include "../helpers.h"

using namespace boost::python;
using regina::Integer;

namespace {
    Integer& (Integer::*divByExact_large)(const Integer&) =
        &Integer::divByExact;
    Integer& (Integer::*divByExact_long)(long) =
        &Integer::divByExact;
    Integer (Integer::*divExact_large)(const Integer&) const =
        &Integer::divExact;
    Integer (Integer::*divExact_long)(long) const =
        &Integer::divExact;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_stringValue,
        Integer::stringValue, 0, 1);

    boost::python::tuple divisionAlg(const Integer& n,
            const Integer& divisor) {
        Integer remainder;
        Integer quotient = n.divisionAlg(divisor, remainder);
        return boost::python::make_tuple(quotient, remainder);
    }
}

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
Integer
_MyCtor(Integer & self, const boost::python::object &o)
{
  std::cout << "My constructor" << std::endl;
  
  return Integer(100);
}

class MyCallPolicy : public boost::python::default_call_policies
{
public:
  static bool precall(PyObject *p)
  {
    std::cout << "precall" << std::endl;
    
    boost::python::object o(boost::python::borrowed(p));

    boost::python::object r = o[0];

    if (not PyLong_Check(r.ptr())) {
      std::cout << "Not a long" << std::endl;
      return false;
    }

    int overflow;
    
    PyLong_AsLongAndOverflow(r.ptr(), &overflow);

    if (overflow != 0) {
      std::cout << "Overflow" << std::endl;
    }

    std::cout << "not overflow" << std::endl;
    
    return true;
  }
};

void addInteger() {
    {
      scope s = class_<Integer>("Integer")
            .def(init<long>())
            .def(init<const Integer&>())
            .def(init<const regina::LargeInteger&>())
	  //.def("__init__", &_MyCtor, MyCallPolicy())
            .def(init<double>())
            .def(init<const char*, optional<int> >())
            .def("isNative", &Integer::isNative)
            .def("isZero", &Integer::isZero)
            .def("sign", &Integer::sign)
            .def("isInfinite", &Integer::isInfinite)
            .def("makeInfinite", &Integer::makeInfinite)
            .def("longValue", &Integer::longValue)
            .def("stringValue", &Integer::stringValue, OL_stringValue())
            .def("swap", &Integer::swap)
            .def(self == long())
            .def(self == regina::LargeInteger())
            .def(self != long())
            .def(self != regina::LargeInteger())
            .def(self < self)
            .def(self < long())
            .def(self > self)
            .def(self > long())
            .def(self <= self)
            .def(self <= long())
            .def(self >= self)
            .def(self >= long())
            .def(self + self)
            .def(self + long())
            .def(self - self)
            .def(self - long())
            .def(self * self)
            .def(self * long())
            .def(self / self)
            .def(self / long())
            .def("divExact", divExact_large)
            .def("divExact", divExact_long)
            .def(self % self)
            .def(self % long())
            .def("divisionAlg", divisionAlg)
            .def(- self)
            .def(self += self)
            .def(self += long())
            .def(self -= self)
            .def(self -= long())
            .def(self *= self)
            .def(self *= long())
            .def(self /= self)
            .def(self /= long())
            .def("divByExact", divByExact_large, return_internal_reference<>())
            .def("divByExact", divByExact_long, return_internal_reference<>())
            .def(self %= self)
            .def(self %= long())
            .def("negate", &Integer::negate)
            .def("raiseToPower", &Integer::raiseToPower)
            .def("abs", &Integer::abs)
            .def("gcdWith", &Integer::gcdWith)
            .def("gcd", &Integer::gcd)
            .def("lcmWith", &Integer::lcmWith)
            .def("lcm", &Integer::lcm)
            .def("gcdWithCoeffs", &Integer::gcdWithCoeffs)
            .def("legendre", &Integer::legendre)
            .def("randomBoundedByThis", &Integer::randomBoundedByThis)
            .def("randomBinary", &Integer::randomBinary)
            .def("randomCornerBinary", &Integer::randomCornerBinary)
            .def("makeLarge", &Integer::makeLarge)
            .def("tryReduce", &Integer::tryReduce)
            .def(long() + self)
            .def(long() * self)
            .def(self_ns::str(self))
            .def(self_ns::repr(self))
            .def(regina::python::add_eq_operators())
            .staticmethod("randomBinary")
            .staticmethod("randomCornerBinary")
        ;

        // Apparently there is no way in python to make a module attribute
        // read-only.
        s.attr("zero") = Integer::zero;
        s.attr("one") = Integer::one;
    }

    boost::python::implicitly_convertible<long, Integer>();
    boost::python::implicitly_convertible<std::string, Integer>();

    regina::python::register_int_from_py_index<unsigned long>();
    
    scope().attr("NInteger") = scope().attr("Integer");
}

