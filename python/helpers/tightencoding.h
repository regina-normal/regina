
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

/*! \file python/helpers/tightencoding.h
 *  \brief Assists with wrapping Regina's tight encoding and decoding routines.
 */

namespace regina::python {

/**
 * Adds tight encoding and decoding functions to the python bindings for a
 * C++ class, with docstrings.
 *
 * This will add tightEncoding() and tightDecoding() to the python class,
 * as provided by the regina::TightEncodable (templated) C++ base class.
 * The arguments \a docEnc and \a docDec will be used as the respective
 * Python docstrings.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>regina::python::add_tight_encoding(c)</t>, where \a c is the
 * pybind11::class_ object that wraps \a T.
 *
 * The wrapped class \a T should either derive from regina::TightEncodable, or
 * should provide tightEncoding() and tightDecoding() functions in a way that
 * is consistent with the regina::TightEncodable interface.
 */
template <class C, typename... options>
void add_tight_encoding(pybind11::class_<C, options...>& c,
        const char* docEnc, const char* docDec) {
    c.def("tightEncoding", &C::tightEncoding, docEnc);
    c.def("tightDecoding", &C::tightDecoding, docDec);
}

/**
 * Adds tight encoding and decoding functions to the python bindings for a
 * C++ class, without docstrings.
 *
 * This will add tightEncoding() and tightDecoding() to the python class,
 * as provided by the regina::TightEncodable (templated) C++ base class.
 * These two functions will not be given any docstrings.
 *
 * To use this for some C++ class \a T in Regina, simply call
 * <t>regina::python::add_tight_encoding(c)</t>, where \a c is the
 * pybind11::class_ object that wraps \a T.
 *
 * The wrapped class \a T should either derive from regina::TightEncodable, or
 * should provide tightEncoding() and tightDecoding() functions in a way that
 * is consistent with the regina::TightEncodable interface.
 */
template <class C, typename... options>
void add_tight_encoding(pybind11::class_<C, options...>& c) {
    c.def("tightEncoding", &C::tightEncoding);
    c.def("tightDecoding", &C::tightDecoding);
}

} // namespace regina::python
