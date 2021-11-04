
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

/*! \file python/helpers/iterators.h
 *  \brief Assists with conversions involving lists and iterators.
 */

#include "utilities/exception.h"

namespace regina::python {

/**
 * Wraps an iterator over a C++ container.
 *
 * The iterator will store its own local Python reference to the container.
 * If the container is held using a shared pointer (as, for example, a normal
 * surface list is), then this will ensure that the container survives for the
 * entire iteration, even if the list was a temporary object.
 */
template <class Container>
class MATCH_PYBIND11_VISIBILITY SafeIterator {
    private:
        const Container& list_;
        pybind11::object localRef_;
        decltype(list_.begin()) it_;

    public:
        SafeIterator(const Container& list) :
                list_(list), localRef_(pybind11::cast(list)),
                it_(list.begin()) {
        }

        SafeIterator(const SafeIterator&) = default;

        decltype(*it_) next() {
            if (it_ == list_.end())
                throw pybind11::stop_iteration();
            return *it_++;
        }

        bool operator == (const SafeIterator& rhs) const {
            return it_ == rhs.it_;
        }

        bool operator != (const SafeIterator& rhs) const {
            return it_ != rhs.it_;
        }

        static void addBindings(pybind11::module_& m, const char* name) {
            auto i = pybind11::class_<SafeIterator>( m, name)
                .def("next", &SafeIterator::next) // for python 2
                .def("__next__", &SafeIterator::next) // for python 3
                ;
            regina::python::add_eq_operators(i);
        }
};

/**
 * Converts a python list into a C-style array of objects of type \a T.
 * The type \a T must be one of Regina's classes that is wrapped in Python;
 * in particular, it cannot be a native C++ type such as \c int or \c char.
 *
 * The list may contain any combination of types that we know how to convert
 * to \a T.  If some element of the given list cannot be converted, this
 * routine will throw an exception.
 *
 * In particular:
 *
 * - If \a T is regina::Integer, then the list may contain
 *   any combination of Regina's large integer classes, python integers
 *   (both \c int and \c long), and python strings.
 *
 * - If \a T is regina::Rational, then the list may contain
 *   any combination of Regina's rational and large integer classes, and
 *   python integers (both \c int and \c long).
 *
 * If this routine returns at all, then it guarantees to return a non-null
 * pointer.  The returned array will have been allocated via array \c new,
 * and must be destroyed by the caller using array \c delete.
 */
template <typename T>
T* seqFromList(pybind11::list l) {
    size_t len = l.size();
    T* coeffs = new T[len]; // throws on failure; otherwise non-null
    for (size_t i = 0; i < len; ++i) {
        try {
            coeffs[i] = l[i].cast<T>();
            continue;
        } catch (pybind11::cast_error const &) {
            delete[] coeffs;
            throw regina::InvalidArgument("List element not convertible to " +
                std::string(pybind11::str(pybind11::type::of<T>())));
        }
    }
    return coeffs;
}

} // namespace regina::python

