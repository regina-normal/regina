
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
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

/*! \file docs.h
 *  \brief Contains miscellaneous documentation.
 */

/*! \mainpage
 *
 *  <center><b>
 *  Regina<br>
 *  Software for low-dimensional topology<br>
 *  Copyright &copy; 1999-2021, The Regina development team
 *  </b></center>
 *
 *  This documentation describes the functions, classes and related
 *  entities in the C++ calculation engine, as well as how these can
 *  be accessed through Python.
 *
 *  To start: visit the <a href="modules.html">Modules</a> page and take
 *  a look around, or browse through the classes regina::Triangulation<3>,
 *  regina::Link, and regina::NormalSurfaces.
 *
 *  <h3>Python</h3>
 *
 *  Although this documentation is written for C++ programmers, Python
 *  programmers have access to many of the same functions, classes,
 *  methods and so on.  See the special page for \ref pythonapi "Python users"
 *  for a summary of the differences between C++ and Python, and some of the
 *  extra features that Python offers.
 *
 *  <h3>Citation</h3>
 *
 *  If you find Regina useful in your research, please consider citing it as
 *  you would any other paper that you use.  A suggested form of reference is:
 *
 *  Benjamin A. Burton, Ryan Budney, William Pettersson, et al.,
 *  "Regina: Software for low-dimensional topology",
 *  http://regina-normal.github.io/ , 1999-2021.
 *
 *  <h3>Authors</h3>
 *
 *  The primary developers of Regina are:
 *  <ul>
 *    <li>Benjamin Burton &lt;bab@maths.uq.edu.au&gt;</li>
 *    <li>Ryan Budney &lt;rybu@uvic.ca&gt;</li>
 *    <li>William Pettersson &lt;william.pettersson@gmail.com&gt;</li>
 *  </ul>
 *  <p>
 *  Many others have been of assistance with this project, be it through
 *  time, knowledge, testing or code.  Please see the full list of
 *  acknowledgements in the users' handbook.
 *
 *  <h3>Copying and Modification</h3>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Some of this code comes with additional permissions; see the
 *  section below regarding online distribution.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 *  MA 02110-1301, USA.
 *
 *  <h3>Online Distribution</h3>
 *
 *  Regina's own source code comes with the following permissions in
 *  addition to the GNU General Public License:
 *
 *  As an exception, when this program is distributed through (i) the
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or
 *  (iii) Google Play by Google Inc., then that store may impose any
 *  digital rights management, device limits and/or redistribution
 *  restrictions that are required by its terms of service.
 *
 *  Some third-party libraries included in Regina are not granted this
 *  exception, and must be removed from any build that is distributed on
 *  stores that cannot comply with the GNU General Public License (such as
 *  Apple's App Store).  See the third-party licenses below for details.
 *
 *  <h3>SnapPea and SnapPy</h3>
 *
 *  Regina includes portions of the SnapPea kernel and its successor SnapPy,
 *  which it uses for some geometric calculations.  The SnapPea kernel was
 *  originally written by Jeff Weeks.  SnapPy, where this kernel is now
 *  maintained, is primarily developed by Marc Culler, Nathan Dunfield and
 *  Matthias Goerner, with contributions from many people.  SnapPy and the
 *  corresponding SnapPea kernel are distributed under the terms of the
 *  GNU General Public License, version 2 or any later version, as published
 *  by the Free Software Foundation.
 *
 *  <h3>Normaliz Library</h3>
 *
 *  Regina includes a copy of libnormaliz, which it uses to help with the
 *  enumeration of fundamental normal surfaces.  Normaliz was written by
 *  Winfried Bruns, Bogdan Ichim and Christof Soeger.  It is distributed
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  <h3>pybind11</h3>
 *
 *  Regina includes a copy of pybind11, which it uses to generate python
 *  bindings for Regina's native C++ code.  The pybind11 library was written by
 *  Wenzel Jakob, and is distributed under a BSD-style license that is shipped
 *  with Regina's source code in the file python/pybind11/LICENSE.
 *
 *  <h3>Python rlcompleter module</h3>
 *
 *  Regina includes a modified copy of the Python rlcompleter module,
 *  which it uses for tab completion in the graphical user interface.  The
 *  original rlcompleter module was distributed under the Python Software
 *  Foundation License Version 2; the full license and copyright notice are
 *  shipped with Regina's source code in the file python/regina/LICENSE.
 *
 *  <h3>Orb Kernel</h3>
 *
 *  Regina includes snippets of code from Orb, for use in importing and
 *  exporting files in Orb / Casson format.  Orb is based on SnapPea
 *  (see above) with additional code written by Damian Heard, who has also
 *  given permission for his code to be distributed under the terms of the
 *  GNU General Public License.
 */

/*! \page i18n Encodings for international strings
 *
 *  Regina's calculation engine uses UTF-8 for all strings (except possibly
 *  for filenames; see below).  This means that programmers who pass
 *  strings \e into routines must ensure that they use UTF-8, and
 *  programmers who receive strings \e from routines may assume that
 *  they are returned in UTF-8.  Note that plain ASCII is a subset of
 *  UTF-8, so plain ASCII text is always fine to use.
 *
 *  Regina's XML data files are also stored using UTF-8.  Very old versions
 *  of Regina used LATIN1 (the default at the time for the Qt libraries)
 *  and did not specify an encoding in the XML header; however, Regina's
 *  file I/O routines are aware of this, and will convert older data into
 *  UTF-8 as it is loaded into memory (the files themselves are
 *  of course not modified).  The routine versionUsesUTF8() may be
 *  useful for programmers who need to work with older data files at a
 *  low level.
 *
 *  File \e names are a special case, since here Regina must interact with
 *  the underlying operating system.  All filenames that are passed into
 *  routines must be presented in whatever encoding the operating system
 *  expects; Regina will simply pass them through to the standard C/C++ file
 *  I/O routines (such as fopen() or std::ifstream::open()) without modifying
 *  them in any way.
 *
 *  \ifacespython The translation of international strings between Python and
 *  C++ should be seamless: all unicode strings passed from Python to C++ will
 *  be encoded using UTF-8, and all strings passed from C++ to Python will be
 *  assumed to be encoded in UTF-8.
 */

/*! \page stddim Standard dimensions
 *
 *  Whilst Regina was originally designed for working with 3-manifolds,
 *  it now offers varying levels of support for manifolds and triangulations
 *  in other dimensions.
 *
 *  Regina's <i>standard dimensions</i> are those for which it offers
 *  rich support (as opposed to basic support).
 *  <b>The standard dimensions are 2, 3 and 4.</b>
 *
 *  The interface in higher dimensions is much more basic.  Partly this
 *  is due to inefficiencies that grow with dimension (e.g., the exponential
 *  growth in the size of the face lattice).  Partly this is due to the
 *  intrinsic difficulty of solving even simple problems in higher dimensions
 *  (for instance, in dimension&nbsp;5 it is not known how to reliably test
 *  whether a given triangulation represents a closed 5-manifold, and
 *  in dimensions &ge;&nbsp;6 this is provably undecidable).
 */

// Put ourselves in the regina namespace so that automatic links work.
namespace regina {

/*! \page bgl Boost Graph Library interface
 *
 * The namespace regina::graph provides an interface for various types of
 * objects from Regina to work directly with the Boost Graph Library (BGL).
 *
 * <h3>Triangulations</h3>
 *
 * After including the header triangulation/graph.h, any object of type
 * <tt>regina::Triangulation<dim></tt> can be used directly as a graph with
 * the BGL.  Here the graph will be the <i>dual graph</i> of the triangulation:
 * this is an undirected multigraph in which each node represents a
 * <i>dim</i>-simplex of the triangulation, and each arc represents a gluing
 * between two adjacent <i>dim</i>-simplices.
 *
 * Triangulations implement several BGL graph concepts, including:
 * - vertex link graph;
 * - edge list graph;
 * - adjacency graph;
 * - incidence graph;
 * - bidirectional graph;
 * - the read-only portions of property graph.
 *
 * Triangulations are \e not mutable graphs - for the purposes of the BGL,
 * they are considered read-only.
 *
 * Dual vertices of the graph represent <i>dim</i>-dimensional simplices
 * of the underlying triangulation, and so are represented directly by a
 * pointer of type <tt>regina::Simplex<dim>*</tt>.  Dual edges (when
 * accessed through the BGL) must come with an orientation, and so are
 * represented by lightweight objects of type regina::graph::DualEdge<dim>.
 *
 * <h3>Knots and links</h3>
 *
 * After including the header link/graph.h, any object of type
 * regina::Link can be used directly as a graph with the BGL.
 * Here the graph will be the underlying 4-valent directed multigraph of the
 * link: every crossing of the link becomes a vertex of the graph, and every
 * directed arc between crossings becomes a directed edge.  Zero-crossing
 * unknot components of a link do not appear in this graph at all.
 *
 * Knots and links implement several BGL graph concepts, including:
 * - vertex link graph;
 * - edge list graph;
 * - adjacency graph;
 * - incidence graph;
 * - bidirectional graph;
 * - the read-only portions of property graph.
 *
 * Knots and links are \e not mutable graphs - for the purposes of the BGL,
 * they are considered read-only.
 *
 * Vertices of the graph are represented directly by a pointer of type
 * <tt>regina::Crossing*</tt>.  Directed edges are represented by lightweight
 * <tt>regina::StrandRef</tt> objects; see the regina::StrandRef documentation
 * for how a StrandRef object is used to identify a directed arc of the link.
 */

/*! \page pythonapi Python users
 *
 *  Regina's calculation engine is provided as a shared C++ library, and this
 *  API documentation is written to describe Regina's native C++ interface.
 *
 *  However, Regina also makes this library accessible through a Python module.
 *  If you are using Python, the notes below explain (briefly) how you
 *  should read this API documentation.
 *
 *  <h3>Python vs C++ types</h3>
 *
 *  - Standard C++ types become native Python types.  For example, the C++ type
 *    \c std::string becomes the native Python string type, the C++ type
 *    \c std::vector<T> becomes a native Python list holding objects of type
 *    \a T, and the C++ type \c std::optional<T> will either become a
 *    Python object of type \a T or the Python value \c None.
 *
 *  - Everything within the C++ namespace \a regina becomes part of the
 *    Python module \c regina.  For example, the C++ class
 *    regina::Link becomes the Python class \c regina.Link.
 *
 *  - Regina has many templated classes, and Python does not support templates.
 *    Typically Regina will replace the template arguments with an appropriate
 *    suffix in the Python class name.  For example, the C++ class
 *    Triangulation<3> becomes the Python class \c Triangulation3, and the
 *    C++ class Matrix<regina::Integer> becomes the Python class \c MatrixInt.
 *    See the individual class notes for how the Python class names are
 *    constructed.
 *
 *  <h3>Python vs C++ APIs</h3>
 *
 *  - Since Regina 7.0, almost \e all of Regina's C++ classes and functions
 *    are wrapped in Python - even the low-level and/or highly specialised
 *    classes - wherever it is meaningful to do so.  However, sometimes there
 *    is reason why a class or function cannot or should not be wrapped.
 *    If a class or function is not available in Python then you will
 *    see a bold <b>Python:</b> note indicating this.  See for instance
 *    the class MarkedVector, or the function valueOf().
 *
 *  - Most of Regina's classes and functions use the same interface
 *    in both C++ and Python, but occasionally there are differences
 *    in how the classes or functions are used.  Again, you will see a bold
 *    <b>Python:</b> note indicating this.  See for instance the method
 *    Link::rewrite(), or the global function writeResUsage().
 *
 *  <h3>Testing equality</h3>
 *
 *  It is important to understand how the comparisons
 *  <tt>x == y</tt> and <tt>x is y</tt> operate under Python.
 *  As of Regina 7.0, this behaviour has changed significantly: many
 *  more classes now compare by value, and a few now have their comparisons
 *  disabled.
 *
 *  In general, you do \e not want to use the Python test <tt>x is y</tt>.
 *  This is because each of Regina's Python objects is typically a "wrapper"
 *  that points to one of Regina's native C++ objects.  Importantly, the
 *  same native C++ object could have many Python wrappers, and so
 *  <tt>x is y</tt> could return \c False even if both \a x and \a y wrap
 *  the same native C++ object.
 *
 *  Instead, you should always use the tests <tt>x == y</tt> and/or
 *  <tt>x != y</tt>.  Regina implements this in different ways, depending
 *  on the type of class:
 *
 *  - Most of Regina's classes use <i>comparison by value</i>.  Here
 *    <tt>x == y</tt> tests whether the \e contents of \a x and \a y are
 *    the same.  What "the same " means will depend on the particular class;
 *    for example, GroupPresentation tests for identical presentations (not
 *    group isomorphism), Triangulation<3> tests for the same tetrahedron and
 *    vertex labellings (not just combinatorial isomorphism), and Attachment
 *    compares the attached file contents (ignoring the associated filenames).
 *    For the core numeric classes such as Integer, Rational, Polynomial,
 *    VectorInt and MatrixInt, this tests the usual mathematical equality.
 *
 *    For each such class, you can find out what is being compared by reading
 *    the documentation for the C++ class operators == and !=.
 *    See for example Link::operator==().
 *
 *  - Some selected classes use <i>comparison by reference</i>.  Here
 *    <tt>x == y</tt> tests whether \a x and \a y refer to the same underlying
 *    C++ object (analogous to how <tt>x is y</tt> would normally behave in a
 *    native Python application).  This is typically used for the few classes
 *    that are passed around by pointer in C++, and whose location in memory
 *    is what defines them; the most common examples you will see are
 *    crossings within links (Crossing), and skeletal objects within
 *    triangulations (e.g., Tetrahedron<3>, Vertex<4>, BoundaryComponent<2>).
 *    This is also used with objects that manage or track long computations
 *    (e.g., ProgressTracker, GluingPermSearcher4, or TreeEnumeration).
 *
 *    These classes do not provide C++ class operators == or !=.
 *
 *  - A few classes have explicitly disabled their comparison operators,
 *    since they are designed to be moved and/or copied (which makes
 *    it inappropriate to compare by reference), but they do not yet have
 *    their own customised comparisons implemented (often because such a
 *    test would be expensive and/or would be difficult to define cleanly).
 *
 *    These classes do not provide C++ class operators == or !=.
 *    If you attempt to compare two Python objects of such a class, you
 *    will receive a Python error with a message explain essentially
 *    what is written here.
 *
 *  - For some classes, comparisons are irrelevant because they only
 *    contain static methods, and you cannot instantiate them.  Examples of
 *    such classes are Example<3> and \ref regina::i18n::Locale "Locale".
 *
 *  If you wish to find out how a particular class \a C behaves, you can
 *  examine the attribute <tt>C.equalityType</tt>.  This will return one of
 *  the values \c BY_VALUE, \c BY_REFERENCE, \c DISABLED or
 *  \c NEVER_INSTANTIATED respctively:
 *
 *  \code{.unparsed}
 *  >>> print Triangulation3.equalityType
 *  BY_VALUE
 *  \endcode
 *
 *  Packet subclasses are a special case: it is often meaningful to compare
 *  packets by value (e.g., testing whether two triangulations are
 *  combinatorially identical), but it is also useful to know whether
 *  two Python wrappers identify the same underlying C++ packet (e.g., when
 *  traversing the packet tree).  To resolve this, you can compare two
 *  packets of the same type by value using the operators <tt>x == y</tt> and
 *  <tt>x != y</tt>, and you can test whether two packets of any types
 *  reference the same underlying object by calling Packet::samePacket().
 *
 *  <h3>International and special characters</h3>
 *
 *  If you use special characters (such as accented letters, other
 *  international characters, exotic punctuation, mathematical symbols and
 *  so on), all text encodings are converted seamlessly and transparently
 *  when moving between Regina and Python.  (In Python, all strings are
 *  unicode; within Regina, all strings are encoded in UTF-8.)
 *
 *  See the page on \ref i18n "encodings for international strings" for
 *  further discussion on text encodings within Regina, including an
 *  important note regarding filenames.
 */

} // namespace regina
