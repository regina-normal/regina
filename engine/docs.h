
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

/*! \file docs.h
 *  \brief Contains miscellaneous documentation.
 */

/*! \mainpage
 *
 *  <center><b>
 *  Regina<br>
 *  Software for 3-manifold topology and normal surface theory<br>
 *  Copyright &copy; 1999-2012, The Regina development team
 *  </b></center>
 *
 *  This documentation describes the functions, classes and related
 *  entities in the C++ calculation engine, as well as how these can
 *  be accessed through Python.
 *
 *  This API documentation is written in terms of C++.
 *  However, Python programmers can use exactly the same functions,
 *  classes, methods and so on.  If the Python version of a function
 *  differs from the C++ version, you will see a bold <b>Python:</b> note
 *  telling you how it differs.
 *
 *  To start: visit the <a href="modules.html">Modules</a> page and take
 *  a look around, or browse through the classes regina::NTriangulation and
 *  regina::NNormalSurfaceList.
 *
 *  <h3>Citation</h3>
 *
 *  If you find Regina useful in your research, please consider citing it as
 *  you would any other paper that you use.  A suggested form of reference is:
 *
 *  Benjamin A. Burton, Ryan Budney, William Pettersson, et al.,
 *  "Regina: Software for 3-manifold topology and normal surface theory",
 *  http://regina.sourceforge.net/, 1999-2012.
 *
 *  <h3>Authors</h3>
 *
 *  The primary developers of Regina are:
 *  <ul>
 *    <li>Benjamin Burton &lt;bab@debian.org&gt;</li>
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
 *  <h3>SnapPea and SnapPy</h3>
 *
 *  Regina includes portions of the SnapPea kernel, which it uses for some
 *  geometric calculations.  SnapPea was written by Jeff Weeks, and is now
 *  maintained by both Jeff Weeks and Nathan Dunfield.  The SnapPeaKernel is
 *  distributed under the terms of the GNU General Public License, version 2
 *  or any later version, as published by the Free Software Foundation.
 *
 *  Regina also includes code from SnapPy, the successor to SnapPea.
 *  SnapPy was written by M. Culler, N. M. Dunfield and J. R. Weeks, and is
 *  also released under the terms of the GNU General Public License.
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
