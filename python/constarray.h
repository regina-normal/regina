
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file python/constarray.h
 *  \brief Allows lightweight array-like objects to be wrapped neatly in Python.
 */

#ifndef __CONSTARRAY_H
#ifndef __DOXYGEN
#define __CONSTARRAY_H
#endif

#include "pybind11/pybind11.h"
#include "helpers.h"

/**
 * Utility classes and routines to assist with Python bindings for
 * the calculation engine.
 */
namespace regina::python {

/**
 * A template class that references a constant one-dimensional "array-like"
 * object.  This can be used with plain C-style arrays, or can also be used
 * with constant "array-like" objects such as Perm<4>::S4, which behave like
 * arrays but in fact compute their values on the fly.
 *
 * The requirements for the type Array are:
 *
 * - It must provide a square bracket operator, which returns its results
 *   by value (not by reference or pointer).  This means that we do not
 *   need to specify any return value policy.
 *
 * - An object of type Array is both cheap and safe to copy by value.
 *   For example, this is true for Perm<4>::S4 which is a struct with no
 *   data members.  It is also true for C-style arrays, since what is
 *   actually copied is a pointer, not the array contents.
 *
 * In Python, the usual list operator [] can be used to access the elements
 * of the array.  Range checking is performed on any index that is passed.
 *
 * For each different template parameter, the corresponding
 * ConstArray class must be wrapped in Python before the first object of
 * this class is wrapped.  This wrapping is performed by calling wrapClass().
 *
 * \pre The output operator &lt;&lt; is defined for the objects contained in
 * this array (i.e., for the return type of the square bracket operator).
 */
template <typename Array, typename Index = size_t>
class ConstArray {
    private:
        const Array data_;
            /**< The underlying array. */
        Index size_;
            /**< The number of elements in the underlying array. */

    public:
        /**
         * Constructs a new wrapper object for the given array-like object.
         *
         * \pre This class has already been wrapped in Python by calling
         * wrapClass().
         *
         * @param data the array that is to be wrapped.
         * @param size the number of elements in this array.
         */
        ConstArray(const Array& data, Index size) : data_(data), size_(size) {
        }

        /**
         * Constructs a new wrapper for the same underlying array as the
         * given wrapper.  Note that the underlying array itself is not copied.
         */
        ConstArray(const ConstArray&) = default;

        /**
         * Sets this to wrap the same underlying array as the given wrapper.
         * Note that the underlying array itself is not copied.
         */
        ConstArray& operator = (const ConstArray&) = default;

        /**
         * Return the number of elements in this array.
         *
         * @return the number of elements.
         */
        Index size() const {
            return size_;
        }

        /**
         * Returns the array element at the given index.
         * If the index is out of range, a Python error will be thrown.
         *
         * @param index the given array index.
         * @return the element at the given index.
         */
        auto getItem(Index index) const {
            if (index >= size_)
                throw pybind11::index_error("Array index out of range");
            return data_[index];
        }

        /**
         * Writes a string representation of this array, including all
         * of its elements, to the given output stream.
         *
         * @param out the output stream to which to write.
         * @return a reference to \a out.
         */
        std::ostream& writeText(std::ostream& out) const {
            out << "[ ";
            for (Index i = 0; i < size_; ++i)
                out << data_[i] << ' ';
            out << "]";
            return out;
        }

        /**
         * Constructs a Python wrapper for this class.
         *
         * This routine must be called for each template parameter before
         * the first object of this class is wrapped.
         *
         * @param m the Python module in which these wrappers should be
         * installed.
         * @param className the name that will be assigned to this
         * class in Python.
         */
        static void wrapClass(pybind11::module_& m, const char* className) {
            auto c = pybind11::class_<ConstArray>(m, className)
                .def("__getitem__", &ConstArray::getItem)
                .def("__len__", &ConstArray::size)
            ;
            regina::python::add_output_ostream(c, PYTHON_REPR_NONE);
            regina::python::add_eq_operators(c);
        }
};

/**
 * Writes a string representation of the given array, including all of
 * its elements, to the given output stream.
 *
 * @param out the output stream to which to write.
 * @param arr the array whose contents are to be written.
 * @return the given output stream.
 */
template <typename Array, typename Index>
std::ostream& operator << (std::ostream& out,
        const ConstArray<Array, Index>& arr) {
    return arr.writeText(out);
}

} // namespace regina::python

#endif
