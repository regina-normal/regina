dnl
dnl    Regina - A Normal Surface Theory Calculator
dnl    Configure Script Macros
dnl
dnl    Copyright (c) 2002, Ben Burton
dnl    For further details contact Ben Burton (benb@acm.org).
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

dnl
dnl    REGINA_DO_NOT_COMPILE
dnl
dnl    Adds the given list of items to $DO_NOT_COMPILE.  Items already
dnl    listed in $DO_NOT_COMPILE will not be added again.
dnl
dnl    Example: REGINA_DO_NOT_COMPILE([DOCSMAN DOCSHTML])
dnl
AC_DEFUN(REGINA_DO_NOT_COMPILE, [
  for item in $1; do
    ruled_out="no"
    for j in $DO_NOT_COMPILE; do
      if test "$j" = "$item"; then
        ruled_out="yes"
        break
      fi
    done
    if test "$ruled_out" = "no"; then
      DO_NOT_COMPILE="$DO_NOT_COMPILE $item"
    fi
  done
])

dnl
dnl    REGINA_ERR_MISSING(MISSING-ITEM, REQUIRED-BY)
dnl
dnl    Quits with an error stating what was missing and which
dnl    build component requires it.
dnl
dnl    Example: REGINA_ERR_MISSING(Library libgmp, [the calculation engine])
dnl
AC_DEFUN(REGINA_ERR_MISSING, [
  AC_MSG_ERROR([[$1 is unavailable; this is needed by $2.]])
])

dnl
dnl    REGINA_HEADER_JNI(TARGET-LIST, REQUIRED-BY)
dnl
dnl    TODO
dnl
AC_DEFUN(REGINA_HEADER_JNI, [
  REGINA_DO_NOT_COMPILE([$1])
  AC_MSG_WARN([Support for $2 is not yet ready.])
])

dnl
dnl    REGINA_HEADER_OPTIONAL(HEADER, TARGET-LIST, REQUIRED-BY)
dnl
dnl    Checks for the given optional header.
dnl    Issues a warning and adds <TARGET-LIST> (which may consist of
dnl    several targets) to $DO_NOT_COMPILE if the header is missing.
dnl
dnl    Example: REGINA_HEADER_OPTIONAL(popt.h, UTILS, [command-line utilities])
dnl
AC_DEFUN(REGINA_HEADER_OPTIONAL, [
  AC_CHECK_HEADERS([$1], , [
    REGINA_WARN_MISSING([Header $1], [$3])
    REGINA_DO_NOT_COMPILE([$2])
  ])
])

dnl
dnl    REGINA_HEADER_REQUIRED(HEADER, REQUIRED-BY)
dnl
dnl    Checks for the given mandatory header.
dnl    Quits with an error message if the header is missing.
dnl
dnl    Example: REGINA_HEADER_REQUIRED(pthread.h, [the calculation engine])
dnl
AC_DEFUN(REGINA_HEADER_REQUIRED, [
  AC_CHECK_HEADERS([$1], , [
    REGINA_ERR_MISSING([Header $1], [$2])
  ])
])

dnl
dnl    REGINA_LIB_OPTIONAL(LIBNAME, FUNCTION-IN-LIB, FLAGSET,
dnl        TARGET-LIST, REQUIRED-BY)
dnl
dnl    Checks for the given optional library.
dnl    Issues a warning and adds <TARGET-LIST> (which may consist of
dnl    several targets) to $DO_NOT_COMPILE if the library is missing.
dnl    Adds -l<LIBNAME> to $<FLAGSET> if the library was found.
dnl
dnl    AC_SUBST()s the following variables:
dnl
dnl        <FLAGSET>: As described above; this is only set if the library
dnl                   was found.
dnl
dnl    Example: REGINA_LIB_OPTIONAL(popt, poptGetContext, UTILS_LIBS,
dnl                 UTILS, [command-line utilities])
dnl
AC_DEFUN(REGINA_LIB_OPTIONAL, [
  AC_CHECK_LIB([$1], [$2], [
    $3="-l$1"
    AC_SUBST([$3])
  ], [
    REGINA_WARN_MISSING([Library lib$1], [$5])
    REGINA_DO_NOT_COMPILE([$4])
  ])
])

dnl
dnl    REGINA_LIB_REQUIRED(LIBNAME, FUNCTION-IN-LIB, REQUIRED-BY)
dnl
dnl    Checks for the given mandatory library.
dnl    Quits with an error message if the library is missing.
dnl
dnl    Example: REGINA_LIB_REQUIRED(z, zlibVersion, [the calculation engine])
dnl
AC_DEFUN(REGINA_LIB_REQUIRED, [
  AC_CHECK_LIB([$1], [$2], , [
    REGINA_ERR_MISSING([Library lib$1], [$3])
  ])
])

dnl
dnl    REGINA_PROG_JAVA(TARGET-LIST, REQUIRED-BY)
dnl
dnl    TODO
dnl
AC_DEFUN(REGINA_PROG_JAVA, [
  REGINA_DO_NOT_COMPILE([$1])
  AC_MSG_WARN([Support for $2 is not yet ready.])
])

dnl
dnl    REGINA_PROG_OPTIONAL(PROG-LIST, VARIABLE, DESCRIPTION, TARGET-LIST,
dnl        REQUIRED-BY)
dnl
dnl    Checks for the given optional program.  The first program from
dnl    <PROG-LIST> that is found will be used.  Alternatively the user
dnl    can override this list by setting <VARIABLE> in the environment.
dnl
dnl    Issues a warning and adds <TARGET-LIST> (which may consist of
dnl    several targets) to $DO_NOT_COMPILE if the program is missing.
dnl
dnl    Variable <VARIABLE> is defined as precious with the help
dnl    text <DESCRIPTION>.
dnl
dnl    AC_SUBST()s the following variables:
dnl
dnl        <VARIABLE>: The program from the given list that was found,
dnl                    or an appropriate invocation of "missing" if
dnl                    no program was found.
dnl
dnl    Example: REGINA_LIB_OPTIONAL(popt, poptGetContext, UTILS_LIBS,
dnl                 UTILS, [command-line utilities])
dnl
AC_DEFUN(REGINA_PROG_OPTIONAL, [
  AC_ARG_VAR([$2], [$3])
  AC_CHECK_PROGS([$2], [$1], none)
  if test "$$2" = "none"; then
    REGINA_DO_NOT_COMPILE([$4])
    for first_prog in $1; do break; done
    AC_SUBST([$2], ["\$(top_srcdir)/missing $first_prog"])
    REGINA_WARN_MISSING([Program $first_prog], [$5])
  fi
])

dnl
dnl    REGINA_SHOULD_BUILD(ITEM, VALUE, DESCRIPTION)
dnl
dnl    Checks whether ITEM is listed in $DO_NOT_COMPILE.
dnl    The given description will be used in the corresponding
dnl    configuration progress message.
dnl
dnl    AC_SUBST()s the following variables:
dnl
dnl        REGINA_BUILD_<ITEM>: <VALUE> if ITEM should be built,
dnl                             empty otherwise.
dnl
dnl    Example: REGINA_SHOULD_BUILD(DOCSHTML, html, [the reference manual])
dnl
AC_DEFUN(REGINA_SHOULD_BUILD, [
  AC_MSG_CHECKING([whether to build $3])
  should_build="yes"
  for j in $DO_NOT_COMPILE; do
    if test "$j" = "$1"; then
      should_build="no"
    fi
  done
  AC_MSG_RESULT([$should_build])
  if test "$should_build" = "yes"; then
    REGINA_BUILD_$1="$2"
  else
    REGINA_BUILD_$1=
  fi
  AC_SUBST(REGINA_BUILD_$1)
])

dnl
dnl    REGINA_TYPE_HASH_SET
dnl
dnl    Determines characteristics of class hash_set<...> from the
dnl    Standard Template Library.
dnl
dnl    AC_SUBST()s the following variables:
dnl
dnl        HASH_SET_HEADER: The header file containing class hash_set<...>.
dnl
dnl    AC_DEFINE()s the following variables:
dnl
dnl        __HASH_NAMESPACE: The namespace containing class hash_set<...>.
dnl        __NO_NAMESPACE_ALIASES: 1 if namespace std cannot be alised,
dnl                                0 otherwise.
dnl
AC_DEFUN(REGINA_TYPE_HASH_SET, [
  # Search for the hash_set header.
  HASH_SET_HEADER=
  AC_CHECK_HEADERS([ext/hash_set hash_set], [
    HASH_SET_HEADER="$ac_header"
    break
  ])
  if test -z "$HASH_SET_HEADER"; then
    AC_MSG_ERROR([[The hash_set headers could not be found.]])
  fi
  AC_SUBST(HASH_SET_HEADER)

  # Determine which namespace the hash_set template is in.
  AC_MSG_CHECKING([[hash_set namespace]])
  HASH_SET_NAMESPACE=none
  for i in std __gnu_cxx; do
    echo "#include <$HASH_SET_HEADER>" > conftest.$ac_ext
    echo "$i::hash_set<int> set;" >> conftest.$ac_ext
    ac_compile='${CXX-g++} -c $CXXFLAGS $CPPFLAGS conftest.$ac_ext'
    if AC_TRY_EVAL(ac_compile); then
      HASH_SET_NAMESPACE="$i"
      break
    fi
  done
  AC_MSG_RESULT([$HASH_SET_NAMESPACE])
  if test "$HASH_SET_NAMESPACE" = "none"; then
    AC_MSG_ERROR([[The namespace for class hash_set<...> could not be determined.]])
  fi
  AC_DEFINE_UNQUOTED(__HASH_NAMESPACE, [$HASH_SET_NAMESPACE],
    [Namespace for class hash_set<...>.])

  # Determine whether we can alias a namespace to std.
  AC_MSG_CHECKING([[whether namespace std can be aliased]])
  echo "#include <$HASH_SET_HEADER>" > conftest.$ac_ext
  echo "namespace stdalias = $HASH_SET_NAMESPACE;" >> conftest.$ac_ext
  echo "stdalias::hash_set<int> s;" >> conftest.$ac_ext
  ac_compile='${CXX-g++} -c $CXXFLAGS $CPPFLAGS conftest.$ac_ext'
  if AC_TRY_EVAL(ac_compile); then
    AC_MSG_RESULT([yes])
    NO_NAMESPACE_ALIASES=0
  else
    AC_MSG_RESULT([no])
    NO_NAMESPACE_ALIASES=1
  fi
  AC_DEFINE_UNQUOTED(__NO_NAMESPACE_ALIASES, [$NO_NAMESPACE_ALIASES],
    [Define to 1 if namespace std cannot be aliased.])
])

dnl
dnl    REGINA_VAR_IOS_NOCREATE
dnl
dnl    Determines whether std::ios::nocreate is a valid file opening flag.
dnl
dnl    AC_DEFINE()s the following variables:
dnl
dnl        __USE_IOS_NOCREATE: 1 if this flag is valid, 0 otherwise.
dnl
AC_DEFUN(REGINA_VAR_IOS_NOCREATE, [
  AC_MSG_CHECKING([[for std::ios::nocreate]])
  AC_TRY_COMPILE([
    #include <fstream>
  ], [
    std::ifstream in("foo", std::ios::nocreate);
  ], [
    AC_MSG_RESULT([yes])
    USE_IOS_NOCREATE=1
  ], [
    AC_MSG_RESULT([no])
    USE_IOS_NOCREATE=0
  ])
  AC_DEFINE_UNQUOTED(__USE_IOS_NOCREATE, [$USE_IOS_NOCREATE],
    [Define to 1 if flag std::ios::nocreate is valid.])
])

dnl
dnl    REGINA_WARN_MISSING(MISSING-ITEM, REQUIRED-BY)
dnl
dnl    Issues a warning stating what was missing and which
dnl    build component requires it.
dnl
dnl    Example: REGINA_WARN_MISSING(Library libpopt, [command-line utilities])
dnl
AC_DEFUN(REGINA_WARN_MISSING, [
  AC_MSG_WARN([[$1 is unavailable; $2 will not be built.]])
])

