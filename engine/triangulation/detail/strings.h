
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file triangulation/detail/strings.h
 *  \brief Implementation details that hard-code strings appropriate for
 *  each dimension.
 */

#ifndef __STRINGS_H_DETAIL
#ifndef __DOXYGEN
#define __STRINGS_H_DETAIL
#endif

namespace regina {
namespace detail {

/**
 * \weakgroup detail
 * @{
 */

/**
 * Contains a collection of compile-time constant strings that describe
 * features of the dimension \a dim.
 *
 * \tparam dim_ any dimension between 0 and 15 inclusive.
 */
template <int dim_>
struct Strings {
#ifdef __DOXYGEN
    /**
     * A string that gives the dimension \a dim as a number.
     *
     * An example for \a dim = 3 is "3".
     */
    static constexpr const char* dim;
    /**
     * A string that gives the name of a <i>dim</i>-face, all in lower case.
     *
     * An example for \a dim = 3 is "tetrahedron".
     * An example for \a dim = 7 is "7-face".
     */
    static constexpr const char* face;
    /**
     * A string that gives the name of a <i>dim</i>-face, with the first
     * word capitalised.
     *
     * An example for \a dim = 3 is "Tetrahedron".
     * An example for \a dim = 7 is "7-face".
     */
    static constexpr const char* Face;
    /**
     * A string that gives the plural name of a <i>dim</i>-face, all in lower
     * case.
     *
     * An example for \a dim = 3 is "tetrahedra".
     * An example for \a dim = 7 is "7-faces".
     */
    static constexpr const char* faces;
    /**
     * A string that gives the plural name of a <i>dim</i>-face, with the first
     * word capitalised.
     *
     * An example for \a dim = 3 is "Tetrahedra".
     * An example for \a dim = 7 is "7-faces".
     */
    static constexpr const char* Faces;
    /**
     * A string that gives the name of a <i>dim</i>-simplex, all in lower case.
     *
     * An example for \a dim = 3 is "tetrahedron".
     * An example for \a dim = 7 is "7-simplex".
     */
    static constexpr const char* simplex;
    /**
     * A string that gives the name of a <i>dim</i>-simplex, with the first
     * word capitalised.
     *
     * An example for \a dim = 3 is "Tetrahedron".
     * An example for \a dim = 7 is "7-simplex".
     */
    static constexpr const char* Simplex;
#endif // __DOXYGEN
};

#ifndef __DOXYGEN

template <>
struct Strings<0> {
    static constexpr const char* dim = "0";
    static constexpr const char* face = "vertex";
    static constexpr const char* Face = "Vertex";
    static constexpr const char* faces = "vertices";
    static constexpr const char* Faces = "Vertices";
    static constexpr const char* simplex = "vertex";
    static constexpr const char* Simplex = "Vertex";
};

template <>
struct Strings<1> {
    static constexpr const char* dim = "1";
    static constexpr const char* face = "edge";
    static constexpr const char* Face = "Edge";
    static constexpr const char* faces = "edges";
    static constexpr const char* Faces = "Edges";
    static constexpr const char* simplex = "edge";
    static constexpr const char* Simplex = "Edge";
};

template <>
struct Strings<2> {
    static constexpr const char* dim = "2";
    static constexpr const char* face = "triangle";
    static constexpr const char* Face = "Triangle";
    static constexpr const char* faces = "triangles";
    static constexpr const char* Faces = "Triangles";
    static constexpr const char* simplex = "triangle";
    static constexpr const char* Simplex = "Triangle";
};

template <>
struct Strings<3> {
    static constexpr const char* dim = "3";
    static constexpr const char* face = "tetrahedron";
    static constexpr const char* Face = "Tetrahedron";
    static constexpr const char* faces = "tetrahedra";
    static constexpr const char* Faces = "Tetrahedra";
    static constexpr const char* simplex = "tetrahedron";
    static constexpr const char* Simplex = "Tetrahedron";
};

template <>
struct Strings<4> {
    static constexpr const char* dim = "4";
    static constexpr const char* face = "pentachoron";
    static constexpr const char* Face = "Pentachoron";
    static constexpr const char* faces = "pentachora";
    static constexpr const char* Faces = "Pentachora";
    static constexpr const char* simplex = "pentachoron";
    static constexpr const char* Simplex = "Pentachoron";
};

template <>
struct Strings<5> {
    static constexpr const char* dim = "5";
    static constexpr const char* face = "5-face";
    static constexpr const char* Face = "5-face";
    static constexpr const char* faces = "5-faces";
    static constexpr const char* Faces = "5-faces";
    static constexpr const char* simplex = "5-simplex";
    static constexpr const char* Simplex = "5-simplex";
};

template <>
struct Strings<6> {
    static constexpr const char* dim = "6";
    static constexpr const char* face = "6-face";
    static constexpr const char* Face = "6-face";
    static constexpr const char* faces = "6-faces";
    static constexpr const char* Faces = "6-faces";
    static constexpr const char* simplex = "6-simplex";
    static constexpr const char* Simplex = "6-simplex";
};

template <>
struct Strings<7> {
    static constexpr const char* dim = "7";
    static constexpr const char* face = "7-face";
    static constexpr const char* Face = "7-face";
    static constexpr const char* faces = "7-faces";
    static constexpr const char* Faces = "7-faces";
    static constexpr const char* simplex = "7-simplex";
    static constexpr const char* Simplex = "7-simplex";
};

template <>
struct Strings<8> {
    static constexpr const char* dim = "8";
    static constexpr const char* face = "8-face";
    static constexpr const char* Face = "8-face";
    static constexpr const char* faces = "8-faces";
    static constexpr const char* Faces = "8-faces";
    static constexpr const char* simplex = "8-simplex";
    static constexpr const char* Simplex = "8-simplex";
};

template <>
struct Strings<9> {
    static constexpr const char* dim = "9";
    static constexpr const char* face = "9-face";
    static constexpr const char* Face = "9-face";
    static constexpr const char* faces = "9-faces";
    static constexpr const char* Faces = "9-faces";
    static constexpr const char* simplex = "9-simplex";
    static constexpr const char* Simplex = "9-simplex";
};

template <>
struct Strings<10> {
    static constexpr const char* dim = "10";
    static constexpr const char* face = "10-face";
    static constexpr const char* Face = "10-face";
    static constexpr const char* faces = "10-faces";
    static constexpr const char* Faces = "10-faces";
    static constexpr const char* simplex = "10-simplex";
    static constexpr const char* Simplex = "10-simplex";
};

template <>
struct Strings<11> {
    static constexpr const char* dim = "11";
    static constexpr const char* face = "11-face";
    static constexpr const char* Face = "11-face";
    static constexpr const char* faces = "11-faces";
    static constexpr const char* Faces = "11-faces";
    static constexpr const char* simplex = "11-simplex";
    static constexpr const char* Simplex = "11-simplex";
};

template <>
struct Strings<12> {
    static constexpr const char* dim = "12";
    static constexpr const char* face = "12-face";
    static constexpr const char* Face = "12-face";
    static constexpr const char* faces = "12-faces";
    static constexpr const char* Faces = "12-faces";
    static constexpr const char* simplex = "12-simplex";
    static constexpr const char* Simplex = "12-simplex";
};

template <>
struct Strings<13> {
    static constexpr const char* dim = "13";
    static constexpr const char* face = "13-face";
    static constexpr const char* Face = "13-face";
    static constexpr const char* faces = "13-faces";
    static constexpr const char* Faces = "13-faces";
    static constexpr const char* simplex = "13-simplex";
    static constexpr const char* Simplex = "13-simplex";
};

template <>
struct Strings<14> {
    static constexpr const char* dim = "14";
    static constexpr const char* face = "14-face";
    static constexpr const char* Face = "14-face";
    static constexpr const char* faces = "14-faces";
    static constexpr const char* Faces = "14-faces";
    static constexpr const char* simplex = "14-simplex";
    static constexpr const char* Simplex = "14-simplex";
};

template <>
struct Strings<15> {
    static constexpr const char* dim = "15";
    static constexpr const char* face = "15-face";
    static constexpr const char* Face = "15-face";
    static constexpr const char* faces = "15-faces";
    static constexpr const char* Faces = "15-faces";
    static constexpr const char* simplex = "15-simplex";
    static constexpr const char* Simplex = "15-simplex";
};

#endif // __DOXYGEN

/*@}*/

} } // namespace regina::detail

#endif

