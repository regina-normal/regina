
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

/*! \file python/helpers/docstrings.h
 *  \brief Assists with Python docstrings that are generated from the C++ docs.
 */

// Docstrings that are generated once but used across many source files:
namespace regina::python::doc::common {
    extern const char* Packet_append;
    extern const char* PacketData_anonID;
    extern const char* PacketData_packet;
    extern const char* PacketOf;
    extern const char* PacketOf_copy;
    extern const char* make_packet;
    extern const char* make_packet_2;

    extern const char* TightEncodable_encoding;
    extern const char* TightEncodable_decoding;

    extern const char* eq_reference;
    extern const char* neq_reference;
    extern const char* eq_None;
    extern const char* neq_None;
    extern const char* eq_disabled;
    extern const char* eq_packet_disabled;
    extern const char* eq_packet_invalid;
    extern const char* eq_none_static;
    extern const char* eq_none_abstract;
}

/**
 * To access docstrings, python binding code should be structured as:
 *
 * RDOC_SCOPE_BEGIN...
 * ...
 * RDOC_SCOPE_SWITCH...
 * ...
 * RDOC_SCOPE_SWITCH... (etc.)
 * ...
 * RDOC_SCOPE_END
 *
 * Each begin/switch will set the namespace alias rdoc, either to
 * regina::python::doc if you are using ..._MAIN, or to
 * regina::python::doc::s_ if you are using ..._SCOPE(s).
 *
 * You can then access each individual docstring as rdoc::name.
 *
 * If you are within a ..._SCOPE(s) block and s is the name of a
 * class/struct/etc., then rdoc refers to the namespace containing the
 * \e members of s (which uses a trailing underscore, as noted above),
 * and rdoc_scope is an alias for the docstring for s itself (which has
 * no trailing underscore).
 *
 * If you have an inner class/enum/etc. that is declared within some outer
 * class/struct, then you can use RDOC_SCOPE_INNER_BEGIN(s) or
 * RDOC_SCOPE_INNER_SWITCH(s), with a matching RDOC_SCOPE_INNER_END.
 * This must be placed inside the outer BEGIN ... END block: it will preserve
 * the outer \a rdoc namespace and \a rdoc_scope string declarations, and will
 * declare a new namespace alias \a rdoc_inner and string \a rdoc_inner_scope
 * for working with \a s.  Such inner scopes can (at present) only be nested
 * one level deep.
 *
 * If you are within a ..._SCOPE(c) block for some class c and you also wish to
 * access docstrings for one or more of its base classes b1,b2,..., then you
 * can use RDOC_SCOPE_BASE(b1) or RDOC_SCOPE_BASE_2(b1, b2).  This will
 * preserve all existing declarations (including \a rdoc), and will also
 * declare new aliases \a rbase and (if relevant) \a rbase2 for the
 * namespaces containing the members of \a b1 and \a b2 respectively.
 * There is no matching END macro; these base class declarations will last
 * until the scope for c is closed.
 *
 * Each BEGIN ... SWITCH or BEGIN ... END block will be contained within
 * curly braces (in particular, local variables will go out of scope at the
 * end of the block).
 */
#define RDOC_SCOPE_BEGIN(scope)  { \
    const char* rdoc_scope = regina::python::doc::scope; \
    namespace rdoc = regina::python::doc::scope ## _; \
    namespace rdoc_global = regina::python::doc;
#define RDOC_SCOPE_BEGIN_MAIN    { \
    namespace rdoc = regina::python::doc;
#define RDOC_SCOPE_SWITCH(scope) } RDOC_SCOPE_BEGIN(scope)
#define RDOC_SCOPE_SWITCH_MAIN   } RDOC_SCOPE_BEGIN_MAIN
#define RDOC_SCOPE_END           }

#define RDOC_SCOPE_INNER_BEGIN(scope)  { \
    const char* rdoc_inner_scope = rdoc::scope; \
    namespace rdoc_inner = rdoc::scope ## _;
#define RDOC_SCOPE_INNER_SWITCH(scope) } RDOC_SCOPE_INNER_BEGIN(scope)
#define RDOC_SCOPE_INNER_END           }

#define RDOC_SCOPE_BASE(base) \
    namespace rbase = regina::python::doc::base ##_;
#define RDOC_SCOPE_BASE_2(base, base2) \
    namespace rbase = regina::python::doc::base ##_; \
    namespace rbase2 = regina::python::doc::base2 ##_;

