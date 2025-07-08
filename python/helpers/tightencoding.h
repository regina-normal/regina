
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

/*! \file python/helpers/tightencoding.h
 *  \brief Assists with wrapping Regina's tight encoding and decoding routines.
 */

namespace regina {

template <typename> struct TightEncodable;

namespace python {

/**
 * Adds tight encoding and decoding functions to the python bindings for a
 * C++ class that provides its own tightEncoding() and tightDecoding()
 * member functions.
 *
 * This will add corresponding tightEncoding() and tightDecoding() functions
 * to the python class, with the Python docstrings \a docEnc and \a docDec
 * respectively.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>regina::python::add_tight_encoding(c)</t>, where \a c is the
 * pybind11::class_ object that wraps \a T.
 */
template <class C, typename... options>
void add_tight_encoding(pybind11::class_<C, options...>& c,
        const char* docEnc, const char* docDec) {
    static_assert(! std::is_base_of_v<TightEncodable<C>, C>,
        "The docstring version of add_tight_encoding() should "
        "only be used for classes that provide their own "
        "custom tightEncoding() and tightDecoding() functions.");

    c.def("tightEncoding", &C::tightEncoding, docEnc);
    c.def("tightDecoding", &C::tightDecoding, docDec);
}

/**
 * Adds tight encoding and decoding functions to the python bindings for a
 * C++ class that derives from regina::TightEncodable.
 *
 * This will add corresonding tightEncoding() and tightDecoding() functions
 * to the python class, as provided by the regina::TightEncodable (templated)
 * C++ base class.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>regina::python::add_tight_encoding(c)</t>, where \a c is the
 * pybind11::class_ object that wraps \a T.
 */
template <class C, typename... options>
void add_tight_encoding(pybind11::class_<C, options...>& c) {
    static_assert(std::is_base_of_v<TightEncodable<C>, C>,
        "The no-docstring version of add_tight_encoding() should "
        "only be used for classes C that derive from TightEncodable<C>.");

    c.def("tightEncoding", &C::tightEncoding,
        regina::python::doc::common::TightEncodable_encoding);
    c.def("tightDecoding", &C::tightDecoding,
        regina::python::doc::common::TightEncodable_decoding);
}

} } // namespace regina::python
