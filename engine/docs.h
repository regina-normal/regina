
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

/*! \file docs.h
 *  \brief Contains miscellaneous documentation.
 */

/*! \mainpage
 *
 *  <center><b>
 *  Regina<br>
 *  Software for low-dimensional topology<br>
 *  Copyright &copy; 1999-2016, The Regina development team
 *  </b></center>
 *
 *  This documentation describes the functions, classes and related
 *  entities in the C++ calculation engine, as well as how these can
 *  be accessed through Python.
 *
 *  To start: visit the <a href="modules.html">Modules</a> page and take
 *  a look around, or browse through the classes regina::Triangulation<3> and
 *  regina::NormalSurfaces.
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
 *  http://regina-normal.github.io/ , 1999-2016.
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
 *  maintained, is primarily developed by Marc Culler and Nathan Dunfield,
 *  with contributions from many people.  SnapPy and the corresponding
 *  SnapPea kernel are distributed under the terms of the GNU General Public
 *  License, version 2 or any later version, as published by the Free Software
 *  Foundation.
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
 *  As of version 4.5, Regina (finally) pays attention to character encodings.
 *
 *  The calculation engine uses UTF-8 for all strings (except possibly
 *  for filenames; see below).  This means that programmers who pass
 *  strings \e into routines must ensure that they use UTF-8, and
 *  programmers who receive strings \e from routines may assume that
 *  they are returned in UTF-8.  Note that plain ASCII is a subset of
 *  UTF-8, so plain ASCII text is always fine to use.
 *
 *  Regina's XML data files are also stored using UTF-8.  Older
 *  versions of Regina used LATIN1 (the default for the Qt libraries)
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
 *  It should be noted that ancient data files that use the old binary
 *  format (Regina 2.x, before mid-2002) only support plain ASCII text.
 *  Support for the old binary format is likely to be removed entirely in the
 *  very near future.
 *
 *  \ifacespython Users and programmers who use the Python interface must
 *  take special care, since Python does not pass strings around in UTF-8
 *  by default.
 *
 *  Proper support for character encodings is quite new, and the main
 *  author rarely uses this (being a native English speaker).  If you
 *  see Regina treating international characters in unexpected ways,
 *  please mail the author(s) or file a bug report so the problem can be
 *  fixed!
 */

/*! \page stddim Standard dimensions
 *
 *  Whilst Regina was originally designed for working with 3-manifolds,
 *  it now offers varying levels of support for manifolds and triangulations
 *  in other dimensions.
 *
 *  Regina's <i>standard dimensions</i> are those for which it offers
 *  rich support (as opposed to basic support).
 *  For the current release, the <b>standard dimensions are 2, 3 and 4</b>.
 *
 *  This list is expected to grow in future releases.
 */

// Put ourselves in the regina namespace so that automatic links work.
namespace regina {

/*! \page pythonapi Python users
 *
 *  Regina's calculation engine is provided as a shared C++ library, and
 *  this API documentation describes the native C++ interface for
 *  working with this library.
 *
 *  Regina also provides a Python module, which wraps many of the
 *  classes, functions, methods and so on from this library so that
 *  Python users can access them.
 *
 *  Python users should read this documentation as follows:
 *
 *  - Standard C++ types become native Python types.  For example, the C++ type
 *    \c std::string becomes the native Python string type.
 *
 *  - Everything within the C++ namespace \a regina becomes part of the
 *    Python module \a regina.  For example, the C++ class
 *    regina::Triangulation<3> becomes the Python class regina.Triangulation3.
 *
 *  - Regina's most important C++ classes and functions are wrapped
 *    in Python.  However, not all classes and functions are wrapped.
 *    If a class or function is not available in Python then you will
 *    see a bold <b>Python:</b> note indicating this.  See for instance
 *    the class Bitmask, or the function duplicate().
 *
 *  - Most of Regina's classes and functions have the same interface
 *    in both C++ and Python, but occasionally there are differences.
 *    Again, you will see a bold <b>Python:</b> note indicating this.
 *    See for instance the method Triangulation<3>::tetrahedra(),
 *    or the global function writeResUsage().
 *
 *  <h3>Testing equality</h3>
 *
 *  It is important to understand how Python's equality tests
 *  <tt>x == y</tt> and <tt>x is y</tt> operate under Python.
 *
 *  If \a x is a Python variable representing one of Regina's objects, then
 *  internally \a x stores a reference to one of Regina's native C++ objects.
 *  Importantly, there may be \e many different Python variables that
 *  all stores references to the \e same underlying C++ object.
 *
 *  This means that the Python test <tt>x is y</tt> is unreliable.
 *  If <tt>x is y</tt> returns \c True then certainly \a x and \a y refer to
 *  the same C++ object; however, if <tt>x is y</tt> returns \c False then
 *  it is still possible that they refer to the same C++ object.
 *
 *  The solution is to always use the test <tt>x == y</tt>.  Regina
 *  offers three types of classes, and these behave differently under Python:
 *
 *  - Some classes use <i>comparison by value</i>.  Here <tt>x == y</tt>
 *    tests whether the contents of \a x and \a y are mathematically
 *    equivalent.  Examples of such classes are \ref IntegerBase "Integer",
 *    Rational, and AbelianGroup.
 *
 *    These classes all provide C++ comparison operators == and !=.  You
 *    can read the documentation for these operators to understand
 *    exactly what mathematical condition(s) are being tested.
 *
 *  - Some classes use <i>comparison by reference</i>.  Here <tt>x == y</tt>
 *    tests whether \a x and \a y refer to the same underlying C++ object.
 *    This is similar to how the test <tt>x is y</tt> would behave in a
 *    native Python application.  Examples of such classes are
 *    Triangulation<3> and Tetrahedron<3>.
 *
 *    These classes do not provide C++ comparison operators == or !=.
 *
 *  - Some classes are never instantiated, and so can never be compared
 *    at all.  These classes typically contain only static methods.
 *    Examples of such classes are Example<dim> and
 *    \ref regina::i18n::Locale "Locale".
 *
 *  If you wish to find out how a particular class \a C behaves, you can
 *  examine the attribute <tt>C.equalityType</tt>.  This will return one of
 *  the values \c BY_VALUE, \c BY_REFERENCE or \c NEVER_INSTANTIATED
 *  respectively:
 *
 *  \code{.unparsed}
 *  >>> print Triangulation3.equalityType
 *  BY_REFERENCE
 *  \endcode
 */

} // namespace regina
