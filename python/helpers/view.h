
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file python/helpers/view.h
 *  \brief Assists with wrapping lightweight C++ views.
 */

#include <ranges>

namespace regina::python {

/**
 * Adds Python bindings for a lightweight C++ standard view type.
 *
 * The view type \a T must be provided as a template argument to addStdView().
 * Typically you would not write out the explicit type name (since this is an
 * implementation detail subject to change); instead you would use a
 * `decltype()` expression.
 *
 * The Python class corresponding to \a T will be named `View_suffix`.
 * The \a suffix argument should not be overly long, and does not need to
 * accurately reflect the specific internal data types used by Regina's
 * calculation engine; however, it is important that these suffixes be unique
 * across all view types.
 *
 * Note that the Python module that is passed to addStdView() should be
 * the submodule `regina.internal`, not the main module `regina`.
 *
 * It is assumed that the view type \a T has not yet been wrapped in Python.
 * If it has, then this routine will throw an exception.  Note that this
 * behaviour is different from addTableView, which detects and gracefully
 * avoids attempts to bind the same type multiple times.
 *
 * Return value policies work as follows:
 *
 * - When accessing an element of the view (e.g., via front() or back(), or
 *   via iteration or indexing), the relationship between the element and the
 *   view will be defined by the template argument \a Policy.  By default,
 *   elements that are pointers (e.g., when calling `Triangulation::vertices()`)
 *   will use a `reference_internal` policy, and thus keep the view alive;
 *   elements that are values (e.g., when calling `Vertex::embeddings()`) will
 *   use a `copy` policy, and thus will have independent lifespans.
 *
 * - If you need the elements of the view to keep the underlying _container_
 *   alive, you will need to indicate this when wrapping whatever C++ functions
 *   return this view type.  Since views are passed by value, you cannot use
 *   `reference_internal`; instead you should pass an appropriate `keep_alive`
 *   argument.  For example, `Triangulation::vertices()` would use
 *   `pybind11::keep_alive<0,1>()`.  Note that this assumes that the elements
 *   are pointers (or if not, that the \a Policy argument has been set
 *   appropriately): thus the elements keep the view alive, and in turn the
 *   view keeps the container alive.
 */
template <std::ranges::view T, pybind11::return_value_policy Policy =
    (std::is_pointer_v<std::ranges::range_value_t<T>> ?
        pybind11::return_value_policy::reference_internal :
        pybind11::return_value_policy::copy)>
requires (std::ranges::random_access_range<T>)
void addStdView(pybind11::module_& internal, const char* suffix) {
    auto c = pybind11::class_<T>(internal,
            (std::string("View_") + suffix).c_str(),
R"doc(A lightweight view object that can be used for iteration and random
access to all elements of a container.

This access is read-only, in the sense that both the container and its
elements are read-only. (Of course, if the elements are C++ pointers,
such as faces of triangulations or crossings in links, then this means
that although the _pointers_ cannot be reassigned to point to different
objects, the objects they _point_ to can still be modified.)

These view objects are small enough to pass by value, with no need for
any specialised move operations or swap functions.

These view classes are deliberately difficult to access, and they live
within an internal Python namespace.  You would typically only interact
with a view when it is returned from a function (e.g.,
``Link.crossings()``), and in most cases you would simply iterate over
this resulting view without ever knowing its exact type.)doc")
        .def("__iter__", [](const T& view) {
            return pybind11::make_iterator<Policy>(view.begin(), view.end());
        }, pybind11::keep_alive<0, 1>(), // iterator keeps view alive
R"doc(Returns a Python iterator over the elements of this view.

Returns:
    an iterator over the elements of this view.)doc")
        .def("__getitem__", [](const T& view, size_t index) {
            if (index >= std::ranges::size(view))
                throw pybind11::index_error("Index out of range");
            return *(view.begin() + index);
        }, Policy,
R"doc(Returns the requested element of this view.

Parameter ``index``:
    indicates which element to return; this must be between 0 and
    ``size()-1`` inclusive.

Returns:
    the (*index*)th element in this view.)doc")
        .def("front", [](const T& view) {
            if (std::ranges::empty(view))
                throw pybind11::index_error("View is empty");
            return *view.begin();
        }, Policy,
R"doc(Returns the first element of this view.

Precondition:
    This view is not empty.

Returns:
    the first element in this view.)doc")
        .def("back", [](const T& view) {
            if (std::ranges::empty(view))
                throw pybind11::index_error("View is empty");
            return *std::ranges::rbegin(view);
        }, Policy,
R"doc(Returns the last element of this view.

Precondition:
    This view is not empty.

Returns:
    the last element in this view.)doc")
        .def("empty", [](const T& view) {
            return std::ranges::empty(view);
        },
R"doc(Determines if this view is empty.

Returns:
    ``True`` if and only if this view is empty.)doc")
        ;

    auto size = [](const T& view) { return std::ranges::size(view); };
    static const char* sizeDoc =
R"doc(Returns the number of elements in this view.

Returns:
    the number of elements.)doc";
    c.def("size", size, sizeDoc);
    c.def("__len__", size, sizeDoc);

    regina::python::add_output_custom(c, [](const T& view, std::ostream& out) {
        out << "[ ";
        // For very small ranges, output all elements.
        // For larger ranges, do not output everything.
        if (std::ranges::empty(view)) {
            out << "[ ]";
        } else if (std::ranges::size(view) <= 5) {
            bool started = false;
            for (const auto& i : view) {
                if (started)
                    out << ", ";
                else
                    started = true;
                regina::python::writeRepr(out, i);
            }
            out << ' ';
        } else {
            auto it = view.begin();
            for (int i = 0; i < 3; ++i) {
                regina::python::writeRepr(out, *it++);
                out << ", ";
            }
            out << "..., ";
            regina::python::writeRepr(out, *std::ranges::rbegin(view));
            out << ' ';
        }
        out << ']';
    }, "<internal>.View");

    disable_eq_operators(c);
}

} // namespace regina::python

