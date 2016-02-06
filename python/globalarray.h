
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

/*! \file python/globalarray.h
 *  \brief Allows global C++ arrays to be wrapped neatly in Python.
 */

#ifndef __GLOBALARRAY_H
#ifndef __DOXYGEN
#define __GLOBALARRAY_H
#endif

#include <boost/python.hpp>
#include "helpers.h"

namespace regina {

/**
 * Utility classes and routines to assist with Python bindings for
 * the calculation engine.
 */
namespace python {

template <typename T, class ReturnValuePolicy>
class GlobalArray2D;

template <typename T, class ReturnValuePolicy>
class GlobalArray3D;

/**
 * A template class that references a constant one-dimensional C++ array.
 *
 * An object of this class can be passed through to Python to allow
 * the user access to the underlying C++ array.
 * For the Python user, the usual list operator [] can be used to access the
 * elements of the array.  Range checking is performed on any index that is
 * passed.
 *
 * The \a ReturnValuePolicy template argument specifies the return value
 * policy for element lookup.
 *
 * For each different set of template parameters, the corresponding
 * GlobalArray class must be wrapped in Python before the first object of
 * this class is constructed.  This wrapping is performed by calling
 * wrapClass().
 *
 * Note that elements of this array can only be inspected, not modified.
 *
 * \pre The output operator &lt;&lt; is defined for type \a T.
 */
template <typename T, class ReturnValuePolicy =
    ::boost::python::return_by_value>
class GlobalArray {
    private:
        const T* data;
            /**< The underlying constant C++ array. */
        size_t nElements;
            /**< The number of elements in the underlying C++ array. */

    public:
        /**
         * Constructs a new wrapper object for the given C++ array.
         *
         * \pre This class has already been wrapped in Python by calling
         * wrapClass().
         *
         * @param newData the constant C++ array that is to be wrapped.
         * @param newSize the number of elements in this C++ array.
         */
        GlobalArray(const T newData[], size_t newSize) : data(newData),
                nElements(newSize) {
        }

        /**
         * Constructs a new wrapper object that wraps the same C++
         * array as the given wrapper object.  Note that the same
         * underlying C++ array is referenced, i.e., the C++ array
         * itself is not cloned.
         *
         * @param cloneMe the wrapper object to clone.
         */
        GlobalArray(const GlobalArray<T, ReturnValuePolicy>& cloneMe) :
                data(cloneMe.data), nElements(cloneMe.nElements) {
        }

        /**
         * Return the number of elements in this array.
         *
         * @return the number of elements.
         */
        size_t size() const {
            return nElements;
        }

        /**
         * Returns the array element at the given index.
         * If the index is out of range, a Python error will be thrown.
         *
         * @param index the given array index.
         * @return the element at the given index.
         */
        const T& getItem(size_t index) const {
            if (index >= nElements) {
                PyErr_SetString(PyExc_IndexError,
                    "global array index out of range");
                ::boost::python::throw_error_already_set();
            }
            return data[index];
        }

        /**
         * Returns the array element at the given index.
         * If the index is out of range, a Python error will be thrown.
         *
         * @param index the given array index.
         * @return the element at the given index.
         */
        const T& operator [] (size_t index) const {
            return getItem(index);
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
            for (size_t i = 0; i < nElements; i++)
                out << data[i] << ' ';
            out << "]";
            return out;
        }

        /**
         * Constructs a Boost.Python wrapper for this class.
         *
         * This routine must be called for each set of template
         * parameters before the first object of this class is
         * constructed.
         *
         * @param className the name that will be assigned to this
         * class in Python.
         */
        static void wrapClass(const char* className) {
            ::boost::python::class_<GlobalArray<T, ReturnValuePolicy> >
                    (className, ::boost::python::no_init)
                .def("__getitem__",
                    &GlobalArray<T, ReturnValuePolicy>::getItem,
                    ::boost::python::return_value_policy<ReturnValuePolicy>())
                .def("__len__", &GlobalArray<T, ReturnValuePolicy>::size)
                .def(::boost::python::self_ns::str(::boost::python::self))
                .def(regina::python::add_eq_operators())
            ;
        }

    private:
        /**
         * A constructor that performs no initialisation whatsoever.
         * This array must be initialised using init() before it is used.
         *
         * \pre This class has already been wrapped in Python by calling
         * wrapClass().
         */
        GlobalArray() : data(0), nElements(0) {
        }

        /**
         * Initialises this wrapper to reference the given C++ array.
         *
         * @param newData the constant C++ array that is to be wrapped.
         * @param newSize the number of elements in this C++ array, for
         * the purposes of range checking during element retrieval.
         */
        void init(const T* newData, size_t newSize) {
            data = newData;
            nElements = newSize;
        }

    friend class GlobalArray2D<T, ReturnValuePolicy>;
};

/**
 * Writes a string representation of the given array, including all of
 * its elements, to the given output stream.
 *
 * @param out the output stream to which to write.
 * @param arr the array whose contents are to be written.
 * @return the given output stream.
 */
template <typename T, class ReturnValuePolicy>
std::ostream& operator << (std::ostream& out,
        const GlobalArray<T, ReturnValuePolicy>& arr) {
    return arr.writeText(out);
}

/**
 * A template class that references a constant two-dimensional C++ array.
 *
 * An object of this class can be passed through to Python to allow
 * the user access to the underlying C++ array.
 * For the Python user, the usual list operator [] can be used to access the
 * elements of the array.  Range checking is performed on any indices that
 * are passed.
 *
 * The \a ReturnValuePolicy template argument specifies the return value
 * policy for element lookup.
 *
 * For each different set of template parameters, the corresponding
 * GlobalArray2D class must be wrapped in Python before the first object of
 * this class is constructed.  This wrapping is performed by calling
 * wrapClass().
 *
 * Note that elements of this array can only be inspected, not modified.
 *
 * \pre The output operator &lt;&lt; is defined for type \a T.
 */
template <typename T, class ReturnValuePolicy =
    ::boost::python::return_by_value>
class GlobalArray2D {
    public:
        typedef GlobalArray<T, ReturnValuePolicy> Row;
            /**< A wrapper class for each row of this array. */

    private:
        Row* data;
            /**< The individual rows of the underlying C++ array. */
        size_t nRows;
            /**< The number of rows in the underlying C++ array. */

    public:
        /**
         * Constructs a new wrapper object for the given 2-dimensional
         * C++ array.
         *
         * \pre This class has already been wrapped in Python by calling
         * wrapClass().
         * \pre The number of rows is strictly positive.
         *
         * @param newData the constant 2-dimensional C++ array that is to
         * be wrapped.
         * @param newRows the number of rows in this C++ array, i.e.,
         * the first array dimension.
         * @tparam newCols the number of columns in this C++ array, i.e.,
         * the second array dimension.  This must be a compile-time constant.
         */
        template<size_t newCols>
        GlobalArray2D(const T newData[][newCols], size_t newRows) :
                data(new Row[newRows]), nRows(newRows) {
            static_assert(newCols > 0,
                "GlobalArray2D: newCols must be positive.");
            for (size_t i = 0; i < nRows; i++)
                data[i].init(newData[i], newCols);
        }

        /**
         * Constructs a new 2-dimensional wrapper object for the given
         * 1-dimensional C++ array.
         *
         * If the Python array is to have \a r rows and \c columns, the
         * Python element <tt>array[i][j]</tt> will correspond to the
         * C++ element <tt>array[(i * c) + j]</tt>.
         *
         * \pre This class has already been wrapped in Python by calling
         * wrapClass().
         * \pre The numbers of rows and columns are both strictly positive.
         *
         * @param newData the constant 1-dimensional C++ array that is to
         * be wrapped.
         * @param newRows the number of rows in the corresponding
         * 2-dimensional array, i.e., the first array dimension.
         * @param newCols the number of columns in the corresponding
         * 2-dimensional array, i.e., the second array dimension.
         */
        GlobalArray2D(const T newData[], size_t newRows, size_t newCols) :
                data(new Row[newRows]), nRows(newRows) {
            for (size_t i = 0; i < nRows; i++)
                data[i].init(newData + (newCols * i), newCols);
        }

        /**
         * Constructs a new wrapper object that wraps the same C++
         * array as the given wrapper object.  Note that the same
         * underlying C++ array is referenced, i.e., the C++ array
         * itself is not cloned.
         *
         * @param cloneMe the wrapper object to clone.
         */
        GlobalArray2D(const GlobalArray2D<T, ReturnValuePolicy>& cloneMe) :
                data(new Row[cloneMe.nRows]), nRows(cloneMe.nRows) {
            for (size_t i = 0; i < nRows; i++)
                data[i].init(cloneMe.data[i].data, cloneMe.cols());
        }

        /**
         * Class destructor.
         *
         * All of the internal row wrappers are destroyed.
         */
        ~GlobalArray2D() {
            delete[] data;
        }

        /**
         * Return the number of rows in this array.
         *
         * @return the number of rows.
         */
        size_t rows() const {
            return nRows;
        }

        /**
         * Return the number of columns in this array.
         *
         * @return the number of columns.
         */
        size_t cols() const {
            return data[0].size();
        }

        /**
         * Returns the requested row of the array.
         * If the row index is out of range, a Python error will be thrown.
         *
         * @param index the index of the requested row.
         * @return the row at the given index.
         */
        const Row& getItem(size_t index) const {
            if (index >= nRows) {
                PyErr_SetString(PyExc_IndexError,
                    "global array index out of range");
                ::boost::python::throw_error_already_set();
            }
            return data[index];
        }

        /**
         * Returns the requested row of the array.
         * If the row index is out of range, a Python error will be thrown.
         *
         * @param index the index of the requested row.
         * @return the row at the given index.
         */
        const Row& operator [] (size_t index) const {
            return getItem(index);
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
            for (size_t i = 0; i < nRows; i++)
                out << data[i] << ' ';
            out << "]";
            return out;
        }

        /**
         * Constructs a Boost.Python wrapper for this class.
         *
         * This routine must be called for each set of template
         * parameters before the first object of this class is
         * constructed.
         *
         * @param className the name that will be assigned to this
         * class in Python.
         */
        static void wrapClass(const char* className) {
            ::boost::python::class_<GlobalArray2D<T, ReturnValuePolicy> >
                    (className, ::boost::python::no_init)
                .def("__getitem__",
                    &GlobalArray2D<T, ReturnValuePolicy>::getItem,
                    ::boost::python::return_internal_reference<>())
                .def("__len__", &GlobalArray2D<T, ReturnValuePolicy>::rows)
                .def(::boost::python::self_ns::str(::boost::python::self))
                .def(regina::python::add_eq_operators())
            ;
        }

    private:
        /**
         * A constructor that performs no initialisation whatsoever.
         * This array must be initialised using init() before it is used.
         *
         * \pre This class has already been wrapped in Python by calling
         * wrapClass().
         */
        GlobalArray2D() : data(0), nRows(0) {
        }

        /**
         * Initialises this wrapper to reference the given 2-dimensional
         * C++ array.
         *
         * \pre The number of rows is strictly positive.
         *
         * @param newData the constant 2-dimensional C++ array that is to
         * be wrapped.
         * @param newRows the number of rows in this C++ array, i.e.,
         * the first array dimension.
         * @param newCols the number of columns in this C++ array, i.e.,
         * the second array dimension.  This must be a compile-time constant.
         */
        template<size_t newCols>
        void init(const T newData[][newCols], size_t newRows) {
            static_assert(newCols > 0,
                "GlobalArray2D: newCols must be positive.");
            data = new Row[newRows];
            nRows = newRows;
            for (size_t i = 0; i < nRows; i++)
                data[i].init(newData[i], newCols);
        }

        /**
         * Initialises this wrapper to reference the same 2-dimensional
         * C++ array as the given wrapper.
         *
         * @param cloneMe the wrapper object to clone.
         */
        void init(const GlobalArray2D<T, ReturnValuePolicy>& cloneMe) {
            data = new Row[cloneMe.nRows];
            nRows = cloneMe.nRows;
            for (size_t i = 0; i < nRows; i++)
                data[i].init(cloneMe.data[i].data, cloneMe.cols());
        }

    friend class GlobalArray3D<T, ReturnValuePolicy>;
};

/**
 * Writes a string representation of the given array, including all of
 * its elements, to the given output stream.
 *
 * @param out the output stream to which to write.
 * @param arr the array whose contents are to be written.
 * @return the given output stream.
 */
template <typename T, class ReturnValuePolicy>
std::ostream& operator << (std::ostream& out,
        const GlobalArray2D<T, ReturnValuePolicy>& arr) {
    return arr.writeText(out);
}

/**
 * A template class that references a constant three-dimensional C++ array.
 *
 * An object of this class can be passed through to Python to allow
 * the user access to the underlying C++ array.
 * For the Python user, the usual list operator [] can be used to access the
 * elements of the array.  Range checking is performed on any indices that
 * are passed.
 *
 * The \a ReturnValuePolicy template argument specifies the return value
 * policy for element lookup.
 *
 * For each different set of template parameters, the corresponding
 * GlobalArray3D class must be wrapped in Python before the first object of
 * this class is constructed.  This wrapping is performed by calling
 * wrapClass().
 *
 * Note that elements of this array can only be inspected, not modified.
 *
 * \pre The output operator &lt;&lt; is defined for type \a T.
 */
template <typename T, class ReturnValuePolicy =
    ::boost::python::return_by_value>
class GlobalArray3D {
    public:
        typedef GlobalArray2D<T, ReturnValuePolicy> Subarray;
            /**< A wrapper class for each two-dimensional subarray. */

    private:
        Subarray* data;
            /**< The individual two-dimensional subarrays. */
        size_t nSubarrays;
            /**< The number of two-dimensional subarrays. */

    public:
        /**
         * Constructs a new wrapper object for the given 3-dimensional
         * C++ array.
         *
         * \pre This class has already been wrapped in Python by calling
         * wrapClass().
         * \pre The first dimension is strictly positive.
         *
         * @param newData the constant 3-dimensional C++ array that is to
         * be wrapped.
         * @param newDim1 the first array dimension of this C++ array.
         * @tparam newDim2 the second array dimension of this C++ array.
         * This must be a compile-time constant.
         * @tparam newDim3 the third array dimension of this C++ array.
         * This must be a compile-time constant.
         */
        template<size_t newDim2, size_t newDim3>
        GlobalArray3D(const T newData[][newDim2][newDim3], size_t newDim1) :
                data(new Subarray[newDim1]), nSubarrays(newDim1) {
            static_assert(newDim2 > 0,
                "GlobalArray3D: newDim2 must be positive.");
            static_assert(newDim3 > 0,
                "GlobalArray3D: newDim3 must be positive.");
            for (size_t i = 0; i < newDim1; i++)
                data[i].init(newData[i], newDim2);
        }

        /**
         * Constructs a new wrapper object that wraps the same C++
         * array as the given wrapper object.  Note that the same
         * underlying C++ array is referenced, i.e., the C++ array
         * itself is not cloned.
         *
         * @param cloneMe the wrapper object to clone.
         */
        GlobalArray3D(const GlobalArray3D<T, ReturnValuePolicy>& cloneMe) :
                data(new Subarray[cloneMe.nSubarrays]),
                nSubarrays(cloneMe.nSubarrays) {
            for (size_t i = 0; i < nSubarrays; i++)
                data[i].init(cloneMe.data[i]);
        }

        /**
         * Class destructor.
         *
         * All of the internal subarray wrappers are destroyed.
         */
        ~GlobalArray3D() {
            delete[] data;
        }

        /**
         * Return the first dimension of this array.
         *
         * @return the first dimension.
         */
        size_t dim1() const {
            return nSubarrays;
        }

        /**
         * Return the second dimension of this array.
         *
         * @return the second dimension.
         */
        size_t dim2() const {
            return data[0].rows();
        }

        /**
         * Return the third dimension of this array.
         *
         * @return the third dimension.
         */
        size_t dim3() const {
            return data[0].cols();
        }

        /**
         * Returns the requested top-dimensional subarray of the array.
         * If the subarray index is out of range, a Python error will
         * be thrown.
         *
         * @param index the index of the requested subarray.
         * @return the top-dimensional subarray at the given index.
         */
        const Subarray& getItem(size_t index) const {
            if (index >= nSubarrays) {
                PyErr_SetString(PyExc_IndexError,
                    "global array index out of range");
                ::boost::python::throw_error_already_set();
            }
            return data[index];
        }

        /**
         * Returns the requested top-dimensional subarray of the array.
         * If the subarray index is out of range, a Python error will
         * be thrown.
         *
         * @param index the index of the requested subarray.
         * @return the top-dimensional subarray at the given index.
         */
        const Subarray& operator [] (size_t index) const {
            return getItem(index);
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
            for (size_t i = 0; i < nSubarrays; i++)
                out << data[i] << ' ';
            out << "]";
            return out;
        }

        /**
         * Constructs a Boost.Python wrapper for this class.
         *
         * This routine must be called for each set of template
         * parameters before the first object of this class is
         * constructed.
         *
         * @param className the name that will be assigned to this
         * class in Python.
         */
        static void wrapClass(const char* className) {
            ::boost::python::class_<GlobalArray3D<T, ReturnValuePolicy> >
                    (className, ::boost::python::no_init)
                .def("__getitem__",
                    &GlobalArray3D<T, ReturnValuePolicy>::getItem,
                    ::boost::python::return_internal_reference<>())
                .def("__len__",
                    &GlobalArray3D<T, ReturnValuePolicy>::dim1)
                .def(::boost::python::self_ns::str(::boost::python::self))
                .def(regina::python::add_eq_operators())
            ;
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
template <typename T, class ReturnValuePolicy>
std::ostream& operator << (std::ostream& out,
        const GlobalArray3D<T, ReturnValuePolicy>& arr) {
    return arr.writeText(out);
}

} } // namespace regina::python

#endif
