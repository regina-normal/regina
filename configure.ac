dnl
dnl    Regina - A Normal Surface Theory Calculator
dnl    Configure Script Generator
dnl
dnl    Copyright (c) 2002, Ben Burton
dnl    For further details contact Ben Burton (benb@acm.org).
dnl
dnl    Process this file with autoconf to produce a configure script.
dnl    Autoconf version >= 2.50 is required.
dnl
dnl    This file is free software; you can redistribute it and/or
dnl    modify it under the terms of the GNU General Public License as
dnl    published by the Free Software Foundation; either version 2 of the
dnl    License, or (at your option) any later version.
dnl
dnl    This file is distributed in the hope that it will be useful, but
dnl    WITHOUT ANY WARRANTY; without even the implied warranty of
dnl    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl    General Public License for more details.
dnl
dnl    You should have received a copy of the GNU General Public
dnl    License along with this program; if not, write to the Free
dnl    Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
dnl    MA 02111-1307, USA.
dnl

###########################################################
#
# Initialisation.
#
###########################################################

AC_INIT(Regina, [3.0.1], [regina-user@lists.sourceforge.net], regina)
AC_CONFIG_SRCDIR(engine/engine/triangulation/ntriangulation.cpp)
AM_INIT_AUTOMAKE(regina, [3.0.1])
AM_CONFIG_HEADER(config.h)

###########################################################
#
# Check for C/C++ tools.
#
###########################################################

AC_PROG_CXX
AC_PROG_CC
AC_PROG_CXXCPP
AC_PROG_CPP
AM_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Sets the default language to C++ for subsequent tests.
AC_LANG(C++)

###########################################################
#
# Check for Java tools.
#
###########################################################

REGINA_PROG_JAVA([JAVAUI DOCSJAVAUI], [the Java user interface])

REGINA_PROG_OPTIONAL([fastjar jar], JAR,
  [Jar command for creating Java JAR archives], [JAVAUI DOCSJAVAUI],
  [the Java user interface])

###########################################################
#
# Check for documentation tools.
#
###########################################################

REGINA_PROG_OPTIONAL(doxygen, DOXYGEN,
  [Doxygen command for C++ source documentation], DOCSENGINE,
  [calculation engine documentation])

AC_ARG_VAR(DOT, [Dot command for drawing directed graphs])
AC_CHECK_PROGS(DOT, dot, none)
if test "$DOT" = "none"; then
  AC_SUBST(HAVE_DOT, NO)
  AC_SUBST(DOT, "\$(top_srcdir)/missing dot")
  REGINA_WARN_MISSING(dot, [graphs in the engine documentation])
else
  AC_SUBST(HAVE_DOT, YES)
fi

REGINA_PROG_OPTIONAL(docbook2man, DOCBOOK2MAN,
  [Command for converting SGML to man pages], DOCSMAN,
  [the man pages])

REGINA_PROG_OPTIONAL(jade, JADE,
  [Jade command for converting SGML to HTML], [DOCSHTML DOCSPDF DOCSRTF],
  [the reference manual])

REGINA_PROG_OPTIONAL(pdfjadetex, PDFJADETEX,
  [JadeTeX command for converting TeX to PDF], DOCSPDF,
  [the PDF reference manual])

REGINA_PROG_OPTIONAL(javadoc, JAVADOC,
  [JavaDoc command for Java source documentation], DOCSJAVAUI,
  [Java UI class documentation])

###########################################################
#
# Check for mandatory headers and libraries.
#
###########################################################

REGINA_HEADER_REQUIRED(zlib.h, [the calculation engine])
REGINA_LIB_REQUIRED(z, zlibVersion, [the calculation engine])

REGINA_HEADER_REQUIRED(gmp.h, [the calculation engine])
REGINA_LIB_REQUIRED(gmp, __gmpz_init, [the calculation engine])

# Library libpthread seems to need be linked in only on systems where it
# exists separately from libc.
REGINA_HEADER_REQUIRED(pthread.h, [the calculation engine])
AC_CHECK_LIB(pthread, pthread_create)

# The libxml2 check seems to need to be done under C, not C++.
AC_LANG_PUSH(C)
AM_PATH_XML2([2.0.0], , [
  REGINA_ERR_MISSING([Library libxml2], [the calculation engine])
])
AC_LANG_POP(C)

###########################################################
#
# Check for optional headers and libraries.
#
###########################################################

REGINA_HEADER_OPTIONAL(popt.h, UTILS, [command-line utilities])
REGINA_LIB_OPTIONAL(popt, poptGetContext, UTILS_LIBS, UTILS,
  [command-line utilities])

AM_PATH_CPPUNIT([1.8.0], , [
  REGINA_WARN_MISSING([Library cppunit], [the test suite])
  REGINA_DO_NOT_COMPILE(TESTSUITE)
])

REGINA_HEADER_JNI(ENGINEJNI, [the JNI calculation engine module])

###########################################################
#
# Miscellaneous other headers.
#
###########################################################

AC_HEADER_STDC
AC_CHECK_HEADERS([unistd.h])

###########################################################
#
# Finalise include and library paths.
#
###########################################################

# Includes required by the calculation engine.
ENGINE_INCLUDES="-I\$(top_srcdir)/engine/engine $XML_CFLAGS"
AC_SUBST(ENGINE_INCLUDES)

# Libraries required by the calculation engine.
ENGINE_LIBS="$XML_LIBS"
AC_SUBST(ENGINE_LIBS)

# Additional libraries required by the test suite.
TESTSUITE_LIBS="$CPPUNIT_LIBS"
AC_SUBST(TESTSUITE_LIBS)

# The actual calculation engine library with which to link.
LIB_ENGINE="\$(top_srcdir)/engine/engine/libregina-engine.la"
AC_SUBST(LIB_ENGINE)

###########################################################
#
# Check for compiler characteristics.
#
###########################################################

# Check for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Check for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([strtol strtoul])

# Other custom checks.
REGINA_TYPE_HASH_SET
REGINA_VAR_IOS_NOCREATE

###########################################################
#
# Decide what should and should not be built.
#
###########################################################

REGINA_SHOULD_BUILD(ENGINE, engine, [the calculation engine library])
REGINA_SHOULD_BUILD(ENGINEJNI, jni, [the JNI calculation engine module])
REGINA_SHOULD_BUILD(JAVAUI, javaui, [the Java user interface])
REGINA_SHOULD_BUILD(UTILS, utils, [command-line utilities])
REGINA_SHOULD_BUILD(TESTSUITE, testsuite, [the test suite])
REGINA_SHOULD_BUILD(DOCSENGINE, engine, [calculation engine docs])
REGINA_SHOULD_BUILD(DOCSJAVAUI, javaui, [Java user interface docs])
REGINA_SHOULD_BUILD(DOCSHTML, html, [the reference manual])
REGINA_SHOULD_BUILD(DOCSMAN, man, [man pages])

###########################################################
#
# Prepare output files.
#
###########################################################

# Makefiles:
AC_CONFIG_FILES([Makefile
                 docs/Makefile
                 docs/engine/Makefile
                 docs/javaui/Makefile
                 docs/images/Makefile
                 docs/images/admon/Makefile
                 docs/images/packet/Makefile
                 docs/images/shots/Makefile
                 engine/Makefile
                 engine/doc-files/Makefile
                 engine/engine/Makefile
                 engine/engine/algebra/Makefile
                 engine/engine/angle/Makefile
                 engine/engine/census/Makefile
                 engine/engine/file/Makefile
                 engine/engine/foreign/Makefile
                 engine/engine/maths/Makefile
                 engine/engine/packet/Makefile
                 engine/engine/progress/Makefile
                 engine/engine/property/Makefile
                 engine/engine/split/Makefile
                 engine/engine/subcomplex/Makefile
                 engine/engine/surfaces/Makefile
                 engine/engine/triangulation/Makefile
                 engine/engine/utilities/Makefile
                 examples/Makefile
                 icons/Makefile
                 pylib/Makefile
                 scripts/Makefile
                 testsuite/Makefile
                 testsuite/maths/Makefile
                 utils/Makefile])

# Other files:
AC_CONFIG_FILES([engine/doc-files/docs.conf])

AC_OUTPUT

