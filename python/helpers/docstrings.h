
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

/*! \file python/helpers/docstrings.h
 *  \brief Assists with Python docstrings that are generated from the C++ docs.
 */

#ifndef __HELPERS_DOCSTRINGS_H
#ifndef __DOXYGEN
#define __HELPERS_DOCSTRINGS_H
#endif

/**
 * To access docstrings, Python binding code should be structured as:
 *
 * RDOC_SCOPE_BEGIN...
 * ...
 * RDOC_SCOPE_SWITCH...
 * ...
 * RDOC_SCOPE_SWITCH... (etc.)
 * ...
 * RDOC_SCOPE_END
 *
 * Each begin/switch will set the alias rdoc to either the namespace
 * regina::python::doc if you are using ..._MAIN, or to the struct
 * regina::python::doc::s if you are using ..._SCOPE(s).
 *
 * You can then access each individual docstring as rdoc::name.
 *
 * If you are within a ..._SCOPE(s) block and \a s is the name of a
 * class/struct/etc., then `rdoc::member` refers to the docstring for
 * `s::member`, and `rdoc::__class` refers to the docstring for \a s itself.
 *
 * If you are within a ..._SCOPE(c) block for some class c and you also wish to
 * access docstrings for one or more of its base classes b1,b2,..., then you
 * can use RDOC_SCOPE_BASE(b1) (or RDOC_SCOPE_BASE_2(b1, b2), or so on).
 * This will preserve all existing declarations (including \a rdoc), and will
 * also declare new aliases \a rbase and (if relevant) \a rbase2 for the
 * namespaces containing the members of \a b1 and \a b2 respectively.
 * There is no matching END macro; these base class declarations will last
 * until the scope for c is closed.
 *
 * Each BEGIN ... SWITCH or BEGIN ... END block will be contained within
 * curly braces (in particular, local variables will go out of scope at the
 * end of the block).
 *
 * Finally: if you are writing new code and have not yet generated the
 * docstrings, please do not just leave them out - it will be very easy
 * to forget to insert them later.  Instead use the docstring RDOC_TODO,
 * which will expand to a "still to be written" string, and importantly
 * can be searched (and ideally _fixed_) before each formal Regina release.
 */
#define RDOC_SCOPE_BEGIN(scope)  { \
    using rdoc = regina::python::doc::scope;
#define RDOC_SCOPE_BEGIN_MAIN    { \
    namespace rdoc = regina::python::doc;
#define RDOC_SCOPE_SWITCH(scope) } RDOC_SCOPE_BEGIN(scope)
#define RDOC_SCOPE_SWITCH_MAIN   } RDOC_SCOPE_BEGIN_MAIN
#define RDOC_SCOPE_END           }

#define RDOC_SCOPE_BASE(base) \
    using rbase = regina::python::doc::base;
#define RDOC_SCOPE_BASE_2(base, base2) \
    using rbase = regina::python::doc::base; \
    using rbase2 = regina::python::doc::base2;

#define RDOC_TODO regina::python::doc::common::todo

namespace regina::python {

/**
 * A common base class to use for all documentation-only classes.
 */
struct DocOnlyBase {
};

/**
 * An empty C++ class that we can wrap in order to create a documentation-only
 * class in Python.
 *
 * These C++ types need to be unique for each Python documentation-only class.
 * Therefore we supply the docstring helper class (containing the actual
 * documentation that we wish to offer through Python) as a template argument.
 */
template <ClassDocType Docs>
struct DocOnlyClass : DocOnlyBase {
};

/**
 * Adds a empty documentation-only class.
 *
 * This provides a way to import C++ class notes into Python that would
 * otherwise not be attached to any Python class.
 *
 * The Python class will be empty, except for the docstring.
 */
template <ClassDocType Docs>
void add_doc_only_class(pybind11::module_& m, const char* name) {
    pybind11::class_<DocOnlyClass<Docs>, DocOnlyBase>(m, name, Docs::__class);
}

namespace doc::common {
    // Note: docstrings should be wrapped at 70 characters per line;
    // the hard maximum is 72.

    inline constexpr const char todo[] =
R"doc(The Python documentation for this class or function has not yet been
extracted from the C++ source code. Please inform the Regina developers
about this omission.)doc";

} } // namespace regina::python::doc::common

#endif
