
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
 *  A Normal Surface Theory Calculator<br>
 *  Copyright &copy; 1999-2008, Ben Burton
 *  </b></center>
 *
 *  This documentation describes the functions, classes and related
 *  entities in the C++ calculation engine, as well as the mirroring of
 *  these in Python.
 *
 *  <h3>Citation</h3>
 *
 *  If you find Regina useful in your research, please consider citing it as
 *  you would any other paper that you use.  The introductory chapter of the
 *  Regina handbook includes a suggested form of reference.
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
 *  <h3>SnapPea Kernel</h3>
 *
 *  Since version 4.2 (July 2005), Regina includes portions of the
 *  SnapPea kernel which it uses for certain geometric calculations.
 *  SnapPea is copyright &copy; 1991-2005 by Jeff Weeks, who has graciously
 *  given permission for it to be distributed under the terms of the
 *  GNU General Public License as described above.
 *
 *  <h3>Orb Kernel</h3>
 *
 *  Since version 4.3.1 (May 2006), Regina includes snippets of code from
 *  Orb for use in importing and exporting files in Orb / Casson format.
 *  Orb is based on SnapPea (see above) with additional code written by
 *  Damian Heard, who has also given permission for his code to be
 *  distributed under the terms of the GNU General Public License.
 *
 *  <h3>Acknowledgements</h3>
 *
 *  Many people have been of assistance with this project, be it through
 *  time, knowledge, testing or code.  Please see the full list of
 *  acknowledgements in the final chapter of the Regina handbook.
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
