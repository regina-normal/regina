
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
            auto i = pybind11::class_<SafeIterator>(m, name)
                .def("next", &SafeIterator::next, // for python 2
                    pybind11::return_value_policy::reference_internal)
                .def("__next__", &SafeIterator::next, // for python 3
                    pybind11::return_value_policy::reference_internal)
                ;
            regina::python::add_eq_operators(i);
        }
};

/**
 * A single Python object that implements the Python iterable/iterator
 * interface for a given C++ iterator pair.
 *
 * This is designed for scenarios where the iterator pair is not obtained
 * from the underlying container via the usual begin() and end() functions.
 * For example, this can be used with NormalSurfaces::vectors(), where the
 * iterator pair is obtained via beginVectors() and endVectors() instead.
 *
 * The iterator will store its own local Python reference to the underlying
 * container.  If the container is held using a shared pointer (as, for example,
 * a normal surface list is), then this will ensure that the container survives
 * for the entire iteration, even if the list was a temporary object.
 */
template <class Iterator>
class MATCH_PYBIND11_VISIBILITY BeginEndIterator {
    private:
        Iterator curr_, end_;
        pybind11::object localRef_;

    public:
        template <typename Container>
        BeginEndIterator(Iterator begin, Iterator end, const Container& c) :
                curr_(begin), end_(end), localRef_(pybind11::cast(c)) {
        }

        BeginEndIterator(const BeginEndIterator&) = default;

        decltype(*curr_) next() {
            if (curr_ == end_)
                throw pybind11::stop_iteration();
            return *curr_++;
        }

        bool operator == (const BeginEndIterator& rhs) const {
            return curr_ == rhs.curr_;
        }

        bool operator != (const BeginEndIterator& rhs) const {
            return curr_ != rhs.curr_;
        }

        static void addBindings(pybind11::module_& m, const char* name) {
            auto i = pybind11::class_<BeginEndIterator>(m, name)
                .def("__iter__", [](pybind11::object const& it) {
                    return it;
                })
                .def("next", &BeginEndIterator::next, // for python 2
                    pybind11::return_value_policy::reference_internal)
                .def("__next__", &BeginEndIterator::next, // for python 3
                    pybind11::return_value_policy::reference_internal)
                ;
            regina::python::add_eq_operators(i);
        }
};

} // namespace regina::python

