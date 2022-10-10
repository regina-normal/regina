
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
    extern const char* Output_detail;
    extern const char* Output_str;
    extern const char* Output_utf8;

    extern const char* PacketData_anonID;
    extern const char* PacketData_packet;
    extern const char* PacketOf;
    extern const char* PacketOf_copy;
    extern const char* make_packet;
    extern const char* make_packet_2;

    extern const char* ListView;
    extern const char* ListView_ListView;
    extern const char* ListView_iter;
    extern const char* ListView_array;
    extern const char* ListView_empty;
    extern const char* ListView_size;
    extern const char* ListView_front;
    extern const char* ListView_back;
    extern const char* ListView_eq;
    extern const char* ListView_neq;

    extern const char* eq_reference;
    extern const char* neq_reference;
    extern const char* eq_None;
    extern const char* neq_None;
    extern const char* eq_disabled;
    extern const char* eq_packet_disabled;
    extern const char* eq_packet_invalid;
    extern const char* eq_never_instantiated;

    extern const char* iter;
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
 * Each BEGIN ... SWITCH or BEGIN ... END block will be contained within
 * curly braces (in particular, local variables will go out of scope at the
 * end of the block).
 */
#define RDOC_SCOPE_BEGIN(scope)  { const char* rdoc_scope = regina::python::doc::scope; namespace rdoc = regina::python::doc::scope ## _;
#define RDOC_SCOPE_BEGIN_MAIN    { namespace rdoc = regina::python::doc;
#define RDOC_SCOPE_SWITCH(scope) } RDOC_SCOPE_BEGIN(scope)
#define RDOC_SCOPE_SWITCH_MAIN   } RDOC_SCOPE_BEGIN_MAIN
#define RDOC_SCOPE_END           }

