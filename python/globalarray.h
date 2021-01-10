
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

/*! \file python/globalarray.h
 *  \brief Allows global C++ arrays to be wrapped neatly in Python.
 */

#ifndef __GLOBALARRAY_H
#ifndef __DOXYGEN
#define __GLOBALARRAY_H
#endif

#include "pybind11/pybind11.h"
#include "helpers.h"

namespace regina {

/**
 * Utility classes and routines to assist with Python bindings for
 * the calculation engine.
 */
namespace python {

template <typename T, pybind11::return_value_policy rvp>
class GlobalArray2D;

template <typename T, pybind11::return_value_policy rvp>
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
 * The \a rvp template argument specifies the return value policy for element
 * lookup.
 *
 * For each different set of template parameters, the corresponding
 * GlobalArray class must be wrapped in Python before the first object of
 * this class is wrapped.  This wrapping is performed by calling wrapClass().
 *
 * Note that elements of this array can only be inspected, not modified.
 *
 * \pre The output operator &lt;&lt; is defined for type \a T.
 */
template <typename T, pybind11::return_value_policy rvp =
    pybind11::return_value_policy::copy>
class GlobalArray {
    private:
        const T* data_;
            /**< The underlying constant C++ array. */
        size_t size_;
            /**< The number of elements in the underlying C++ array. */

    public:
        /**
         * Constructs a new wrapper object for the given C++ array.
         *
         * \pre This class has already been wrapped in Python by calling
         * wrapClass().
         *
         * @param data the constant C++ array that is to be wrapped.
         * @param size the number of elements in this C++ array.
         */
        GlobalArray(const T data[], size_t size) : data_(data), size_(size) {
        }

        /**
         * Constructs a new wrapper object that wraps the same C++
         * array as the given wrapper object.  Note that the same
         * underlying C++ array is referenced, i.e., the C++ array
         * itself is not cloned.
         *
         * @param cloneMe the wrapper object to clone.
         */
        GlobalArray(const GlobalArray<T, rvp>& cloneMe) :
                data_(cloneMe.data_), size_(cloneMe.size_) {
        }

        /**
         * Return the number of elements in this array.
         *
         * @return the number of elements.
         */
        size_t size() const {
            return size_;
        }

        /**
         * Returns the array element at the given index.
         * If the index is out of range, a Python error will be thrown.
         *
         * @param index the given array index.
         * @return the element at the given index.
         */
        const T& getItem(size_t index) const {
            if (index >= size_)
                throw pybind11::index_error("Global array index out of range");
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
            for (size_t i = 0; i < size_; i++)
                out << data_[i] << ' ';
            out << "]";
            return out;
        }

        /**
         * Constructs a Python wrapper for this class.
         *
         * This routine must be called for each set of template
         * parameters before the first object of this class is wrapped.
         *
         * @param m the Python module in which these wrappers should be
         * installed.
         * @param className the name that will be assigned to this
         * class in Python.
         */
        static void wrapClass(pybind11::module_& m, const char* className) {
            auto c = pybind11::class_<GlobalArray<T, rvp>>(m, className)
                .def("__getitem__", &GlobalArray<T, rvp>::getItem,
                    rvp)
                .def("__len__", &GlobalArray<T, rvp>::size)
            ;
            regina::python::add_output_ostream(c);
            regina::python::add_eq_operators(c);
        }

    private:
        /**
         * A constructor that performs no initialisation whatsoever.
         * This array must be initialised using init() before it is used.
         *
         * \pre This class has already been wrapped in Python by calling
         * wrapClass().
         */
        GlobalArray() : data_(0), size_(0) {
        }

        /**
         * Initialises this wrapper to reference the given C++ array.
         *
         * @param data the constant C++ array that is to be wrapped.
         * @param size the number of elements in this C++ array, for
         * the purposes of range checking during element retrieval.
         */
        void init(const T* data, size_t size) {
            data_ = data;
            size_ = size;
        }

    friend class GlobalArray2D<T, rvp>;
};

/**
 * Writes a string representation of the given array, including all of
 * its elements, to the given output stream.
 *
 * @param out the output stream to which to write.
 * @param arr the array whose contents are to be written.
 * @return the given output stream.
 */
template <typename T, pybind11::return_value_policy rvp>
std::ostream& operator << (std::ostream& out,
        const GlobalArray<T, rvp>& arr) {
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
 * The \a rvp template argument specifies the return value policy for element
 * lookup.
 *
 * For each different set of template parameters, the corresponding
 * GlobalArray2D class must be wrapped in Python before the first object of
 * this class is wrapped.  This wrapping is performed by calling wrapClass().
 *
 * Note that elements of this array can only be inspected, not modified.
 *
 * \pre The output operator &lt;&lt; is defined for type \a T.
 */
template <typename T, pybind11::return_value_policy rvp =
    pybind11::return_value_policy::copy>
class GlobalArray2D {
    public:
        typedef GlobalArray<T, rvp> Row;
            /**< A wrapper class for each row of this array. */

    private:
        Row* data_;
            /**< The individual rows of the underlying C++ array. */
        size_t nRows_;
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
         * @param data the constant 2-dimensional C++ array that is to
         * be wrapped.
         * @param nRows the number of rows in this C++ array, i.e.,
         * the first array dimension.
         * @tparam nCols the number of columns in this C++ array, i.e.,
         * the second array dimension.  This must be a compile-time constant.
         */
        template <size_t nCols>
        GlobalArray2D(const T data[][nCols], size_t nRows) :
                data_(new Row[nRows]), nRows_(nRows) {
            static_assert(nCols > 0,
                "GlobalArray2D: nCols must be positive.");
            for (size_t i = 0; i < nRows_; i++)
                data_[i].init(data[i], nCols);
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
         * @param data the constant 1-dimensional C++ array that is to
         * be wrapped.
         * @param nRows the number of rows in the corresponding
         * 2-dimensional array, i.e., the first array dimension.
         * @param nCols the number of columns in the corresponding
         * 2-dimensional array, i.e., the second array dimension.
         */
        GlobalArray2D(const T data[], size_t nRows, size_t nCols) :
                data_(new Row[nRows]), nRows_(nRows) {
            for (size_t i = 0; i < nRows_; i++)
                data_[i].init(data + (nCols * i), nCols);
        }

        /**
         * Constructs a new wrapper object that wraps the same C++
         * array as the given wrapper object.  Note that the same
         * underlying C++ array is referenced, i.e., the C++ array
         * itself is not cloned.
         *
         * @param cloneMe the wrapper object to clone.
         */
        GlobalArray2D(const GlobalArray2D<T, rvp>& cloneMe) :
                data_(new Row[cloneMe.nRows_]), nRows_(cloneMe.nRows_) {
            for (size_t i = 0; i < nRows_; i++)
                data_[i].init(cloneMe.data_[i].data_, cloneMe.cols());
        }

        /**
         * Class destructor.
         *
         * All of the internal row wrappers are destroyed.
         */
        ~GlobalArray2D() {
            delete[] data_;
        }

        /**
         * Return the number of rows in this array.
         *
         * @return the number of rows.
         */
        size_t rows() const {
            return nRows_;
        }

        /**
         * Return the number of columns in this array.
         *
         * @return the number of columns.
         */
        size_t cols() const {
            return data_[0].size();
        }

        /**
         * Returns the requested row of the array.
         * If the row index is out of range, a Python error will be thrown.
         *
         * @param index the index of the requested row.
         * @return the row at the given index.
         */
        const Row& getItem(size_t index) const {
            if (index >= nRows_)
                throw pybind11::index_error("Global array index out of range");
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
            for (size_t i = 0; i < nRows_; i++)
                out << data_[i] << ' ';
            out << "]";
            return out;
        }

        /**
         * Constructs a Python wrapper for this class.
         *
         * This routine must be called for each set of template
         * parameters before the first object of this class is wrapped.
         *
         * @param m the Python module in which these wrappers should be
         * installed.
         * @param className the name that will be assigned to this
         * class in Python.
         */
        static void wrapClass(pybind11::module_& m, const char* className) {
            auto c = pybind11::class_<GlobalArray2D<T, rvp>>(m, className)
                .def("__getitem__", &GlobalArray2D<T, rvp>::getItem,
                    pybind11::return_value_policy::reference_internal)
                .def("__len__", &GlobalArray2D<T, rvp>::rows)
            ;
            regina::python::add_output_ostream(c);
            regina::python::add_eq_operators(c);
        }

    private:
        /**
         * A constructor that performs no initialisation whatsoever.
         * This array must be initialised using init() before it is used.
         *
         * \pre This class has already been wrapped in Python by calling
         * wrapClass().
         */
        GlobalArray2D() : data_(0), nRows_(0) {
        }

        /**
         * Initialises this wrapper to reference the given 2-dimensional
         * C++ array.
         *
         * \pre The number of rows is strictly positive.
         *
         * @param data the constant 2-dimensional C++ array that is to
         * be wrapped.
         * @param nRows the number of rows in this C++ array, i.e.,
         * the first array dimension.
         * @param nCols the number of columns in this C++ array, i.e.,
         * the second array dimension.  This must be a compile-time constant.
         */
        template <size_t nCols>
        void init(const T data[][nCols], size_t nRows) {
            static_assert(nCols > 0,
                "GlobalArray2D: nCols must be positive.");
            data_ = new Row[nRows];
            nRows_ = nRows;
            for (size_t i = 0; i < nRows_; i++)
                data_[i].init(data[i], nCols);
        }

        /**
         * Initialises this wrapper to reference the same 2-dimensional
         * C++ array as the given wrapper.
         *
         * @param cloneMe the wrapper object to clone.
         */
        void init(const GlobalArray2D<T, rvp>& cloneMe) {
            data_ = new Row[cloneMe.nRows_];
            nRows_ = cloneMe.nRows_;
            for (size_t i = 0; i < nRows_; i++)
                data_[i].init(cloneMe.data_[i].data_, cloneMe.cols());
        }

    friend class GlobalArray3D<T, rvp>;
};

/**
 * Writes a string representation of the given array, including all of
 * its elements, to the given output stream.
 *
 * @param out the output stream to which to write.
 * @param arr the array whose contents are to be written.
 * @return the given output stream.
 */
template <typename T, pybind11::return_value_policy rvp>
std::ostream& operator << (std::ostream& out,
        const GlobalArray2D<T, rvp>& arr) {
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
 * The \a rvp template argument specifies the return value policy for element
 * lookup.
 *
 * For each different set of template parameters, the corresponding
 * GlobalArray3D class must be wrapped in Python before the first object of
 * this class is wrapped.  This wrapping is performed by calling wrapClass().
 *
 * Note that elements of this array can only be inspected, not modified.
 *
 * \pre The output operator &lt;&lt; is defined for type \a T.
 */
template <typename T, pybind11::return_value_policy rvp =
    pybind11::return_value_policy::copy>
class GlobalArray3D {
    public:
        typedef GlobalArray2D<T, rvp> Subarray;
            /**< A wrapper class for each two-dimensional subarray. */

    private:
        Subarray* data_;
            /**< The individual two-dimensional subarrays. */
        size_t nSubarrays_;
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
         * @param data the constant 3-dimensional C++ array that is to
         * be wrapped.
         * @param dim1 the first array dimension of this C++ array.
         * @tparam dim2 the second array dimension of this C++ array.
         * This must be a compile-time constant.
         * @tparam dim3 the third array dimension of this C++ array.
         * This must be a compile-time constant.
         */
        template<size_t dim2, size_t dim3>
        GlobalArray3D(const T data[][dim2][dim3], size_t dim1) :
                data_(new Subarray[dim1]), nSubarrays_(dim1) {
            static_assert(dim2 > 0,
                "GlobalArray3D: dim2 must be positive.");
            static_assert(dim3 > 0,
                "GlobalArray3D: dim3 must be positive.");
            for (size_t i = 0; i < dim1; i++)
                data_[i].init(data[i], dim2);
        }

        /**
         * Constructs a new wrapper object that wraps the same C++
         * array as the given wrapper object.  Note that the same
         * underlying C++ array is referenced, i.e., the C++ array
         * itself is not cloned.
         *
         * @param cloneMe the wrapper object to clone.
         */
        GlobalArray3D(const GlobalArray3D<T, rvp>& cloneMe) :
                data_(new Subarray[cloneMe.nSubarrays_]),
                nSubarrays_(cloneMe.nSubarrays_) {
            for (size_t i = 0; i < nSubarrays_; i++)
                data_[i].init(cloneMe.data_[i]);
        }

        /**
         * Class destructor.
         *
         * All of the internal subarray wrappers are destroyed.
         */
        ~GlobalArray3D() {
            delete[] data_;
        }

        /**
         * Return the first dimension of this array.
         *
         * @return the first dimension.
         */
        size_t dim1() const {
            return nSubarrays_;
        }

        /**
         * Return the second dimension of this array.
         *
         * @return the second dimension.
         */
        size_t dim2() const {
            return data_[0].rows();
        }

        /**
         * Return the third dimension of this array.
         *
         * @return the third dimension.
         */
        size_t dim3() const {
            return data_[0].cols();
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
            if (index >= nSubarrays_)
                throw pybind11::index_error("Global array index out of range");
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
            for (size_t i = 0; i < nSubarrays_; i++)
                out << data_[i] << ' ';
            out << "]";
            return out;
        }

        /**
         * Constructs a Python wrapper for this class.
         *
         * This routine must be called for each set of template
         * parameters before the first object of this class is wrapped.
         *
         * @param m the Python module in which these wrappers should be
         * installed.
         * @param className the name that will be assigned to this
         * class in Python.
         */
        static void wrapClass(pybind11::module_& m, const char* className) {
            auto c = pybind11::class_<GlobalArray3D<T, rvp>>(m, className)
                .def("__getitem__", &GlobalArray3D<T, rvp>::getItem,
                    pybind11::return_value_policy::reference_internal)
                .def("__len__", &GlobalArray3D<T, rvp>::dim1)
            ;
            regina::python::add_output_ostream(c);
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
template <typename T, pybind11::return_value_policy rvp>
std::ostream& operator << (std::ostream& out,
        const GlobalArray3D<T, rvp>& arr) {
    return arr.writeText(out);
}

} } // namespace regina::python

#endif
