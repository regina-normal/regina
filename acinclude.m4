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

dnl    -----------------------------------------------------------------
dnl
dnl                        Macros written for Regina
dnl
dnl    -----------------------------------------------------------------

dnl
dnl    REGINA_CLASSPATH_RESTORE
dnl
dnl    Restores $CLASSPATH to its value when REGINA_CLASSPATH_SAVE was
dnl    last called.  This restored $CLASSPATH is exported to the environment.
dnl
AC_DEFUN(REGINA_CLASSPATH_RESTORE, [
  export CLASSPATH="$regina_old_classpath"
])

dnl
dnl    REGINA_CLASSPATH_SAVE
dnl
dnl    Saves the current value of $CLASSPATH so it can be later restored
dnl    using REGINA_CLASSPATH_RESTORE.
dnl
AC_DEFUN(REGINA_CLASSPATH_SAVE, [
  regina_old_classpath="$CLASSPATH"
])

dnl
dnl    REGINA_CXX_PARTIAL_SPECIALISATION
dnl
dnl    Ensures that the C++ compiler supports partial specialisation.
dnl
dnl    Modified from AC_CXX_PARTIAL_SPECIALIZATION which is
dnl    copyright (c) Luc Maisonobe and licensed under the GPL.
dnl
AC_DEFUN(REGINA_CXX_PARTIAL_SPECIALISATION, [
  AC_MSG_CHECKING([whether the compiler supports partial specialisation])
  AC_LANG_PUSH(C++)
  AC_TRY_COMPILE([
    template<class T, int N> class A            { public : enum e { z = 0 }; };
    template<int N>          class A<double, N> { public : enum e { z = 1 }; };
    template<class T>        class A<T, 2>      { public : enum e { z = 2 }; };
  ], [
    return (A<int,3>::z == 0) && (A<double,3>::z == 1) && (A<float,2>::z == 2);
  ], [
    AC_MSG_RESULT([yes])
  ], [
    AC_MSG_RESULT([no])
    AC_MSG_ERROR([The C++ compiler does not support partial specialisation of templates.])
  ])
  AC_LANG_POP(C++)
])

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
  AC_SUBST(JNI_INCLUDES)
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
dnl    REGINA_LIB_JAVA(LIBNAME, CLASS-IN-LIB, TARGET-LIST, REQUIRED-BY)
dnl
dnl    Checks for the given optional Java library.
dnl    Issues a warning and adds <TARGET-LIST> (which may consist of
dnl    several targets) to $DO_NOT_COMPILE if the library is missing.
dnl    Prepends the library to $CLASSPATH if the library is found.
dnl
dnl    AC_SUBST()s the following variables:
dnl
dnl        CLASSPATH: The Java compiler classpath, which is modified as
dnl                   described above.
dnl
dnl    Example: REGINA_LIB_JAVA(jython, [org.python.util.jython],
dnl                 JAVAUI, [the Java user interface])
dnl
AC_DEFUN(REGINA_LIB_JAVA, [
  AC_MSG_CHECKING([for native $1 support])
  REGINA_TRY_COMPILE_JAVA([$JAVAC], [$2], [ $2 obj = null; ], [
    AC_MSG_RESULT([yes])
  ], [
    AC_MSG_RESULT([no])
    AC_MSG_CHECKING([for $1 jar])
    found_jar=no
    for _libdir in ./lib /usr/share/java /usr/share/java/jar /usr/local/java /usr/local/java/jar /usr/local/share/java /usr/local/share/java/jar; do
      for jar in "$_libdir/$1.jar" "$_libdir/lib$1.jar" "$_libdir/lib$1-java.jar"; do
        if test -e "$jar"; then
          REGINA_CLASSPATH_SAVE
          export CLASSPATH="$jar:$CLASSPATH"
          REGINA_TRY_COMPILE_JAVA([$JAVAC], [$2], [ $2 obj = null; ], [
            AC_MSG_RESULT([$jar])
            found_jar=yes
            break
          ], [
            REGINA_CLASSPATH_RESTORE
          ])
        fi
      done
      if test "$found_jar" = "yes"; then
        break
      fi
    done
    if test "$found_jar" = "no"; then
      AC_MSG_RESULT([no])
      REGINA_DO_NOT_COMPILE([$3])
      REGINA_WARN_MISSING([Java $1 library], [$4])
    fi
  ])
  AC_SUBST(CLASSPATH)
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
dnl    REGINA_PROG_JAVAC(TARGET-LIST, REQUIRED-BY)
dnl
dnl    Finds a Java compiler that satisfies the requirements of the
dnl    Regina build process.  The user can override the choice of Java
dnl    compiler by setting $JAVAC in the environment.
dnl
dnl    Issues a warning and adds <TARGET-LIST> (which may consist of
dnl    several targets) to $DO_NOT_COMPILE if no suitable Java compiler
dnl    could be found.
dnl
dnl    Variables JAVAC, JAVACFLAGS and CLASSPATH are defined as precious.
dnl
dnl    AC_SUBST()s the following variables:
dnl
dnl        JAVAC: The selected Java compiler, or an appropriate
dnl               invocation of "missing" if no compiler was found.
dnl        JAVACFLAGS: A set of Java compiler flags (the value of
dnl                    this variable is left untouched).
dnl        CLASSPATH: The Java classpath (this variable is left untouched).
dnl        HAVE_JAVAC: 1 if a suitable compiler was found, 0 otherwise.
dnl
dnl    Example: REGINA_PROG_JAVAC(JAVAUI, [the Java user interface])
dnl
AC_DEFUN(REGINA_PROG_JAVAC, [
  AC_ARG_VAR(JAVAC, [Java compiler])
  AC_ARG_VAR(JAVACFLAGS, [Java compiler flags])
  AC_ARG_VAR(CLASSPATH, [Java compiler classpath])
  if test -z "$JAVAC"; then
    JAVAC=none
    for javac in "gcj -C" jikes jikes-gij javac; do
      REGINA_PROG_JAVAC_USABLE([$javac], [
        JAVAC="$javac"
        break
      ])
    done
    if test "$JAVAC" = "none"; then
      AC_MSG_WARN([[No suitable Java compiler could be found; $2 will not be built.]])
    fi
  else
    REGINA_PROG_JAVAC_USABLE([$JAVAC], , [
      AC_MSG_WARN([[Java compiler $1 is not suitable; $2 will not be built.]])
      JAVAC=none
    ])
  fi

  if test "$JAVAC" = "none"; then
    REGINA_DO_NOT_COMPILE([$1])
    AC_SUBST(JAVAC, "\$(top_srcdir)/missing javac")
    HAVE_JAVAC=0
  else
    HAVE_JAVAC=1
  fi
  AC_SUBST(HAVE_JAVAC)
  AC_SUBST(JAVAC)
  AC_SUBST(JAVACFLAGS)
])

dnl
dnl    REGINA_PROG_JAVAC_USABLE(JAVA-COMPILER, ACTION-IF-OK, ACTION-IF-NOT-OK)
dnl
dnl    Determines whether the given Java compiler satisfies (some of)
dnl    the requirements of the Regina build process.
dnl
AC_DEFUN(REGINA_PROG_JAVAC_USABLE, [
  javac_ok=yes
  AC_MSG_CHECKING([whether $1 compiles a simple Java class])
  REGINA_TRY_COMPILE_JAVA([$1], , [ String str = "hello"; ], , [
    javac_ok=no
  ])
  AC_MSG_RESULT([$javac_ok])

  if test "$javac_ok" = "yes"; then
    AC_MSG_CHECKING([whether $1 has javax.swing])
    REGINA_TRY_COMPILE_JAVA([$1], [ javax.swing.* ], [ JPanel p = null; ], , [
      javac_ok=no
    ])
    AC_MSG_RESULT([$javac_ok])
  fi

  if test "$javac_ok" = "yes"; then
    ifelse([$2], , :, [$2])
  else
    ifelse([$3], , :, [$3])
  fi
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
dnl    Example: REGINA_PROG_OPTIONAL(doxygen, DOXYGEN,
dnl                 [Doxygen command for C++ source documentation], DOCSENGINE,
dnl                 [calculation engine documentation])
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
  REGINA_SHOULD_BUILD_INTERNAL([$1])
  AC_MSG_RESULT([$should_build])
  if test "$should_build" = "yes"; then
    REGINA_BUILD_$1="$2"
  else
    REGINA_BUILD_$1=
  fi
  AC_SUBST(REGINA_BUILD_$1)
])

dnl
dnl    REGINA_SHOULD_BUILD(ITEM)
dnl
dnl    Checks whether ITEM is listed in $DO_NOT_COMPILE.
dnl    Variable $should_build will be set to "yes" or "no" as appropriate.
dnl
AC_DEFUN(REGINA_SHOULD_BUILD_INTERNAL, [
  should_build="yes"
  for j in $DO_NOT_COMPILE; do
    if test "$j" = "$1"; then
      should_build="no"
    fi
  done
])

dnl
dnl    REGINA_TRY_COMPILE_JAVA(COMPILER, [IMPORTS], [FUNCTION-BODY],
dnl        [ACTION-IF-OK], [ACTION-IF-NOT-OK])
dnl
dnl    Modified from AC_TRY_COMPILE_JAVA which is
dnl    copyright (c) Devin Weaver and licensed under the GPL.
dnl
dnl    Try to use the given compiler with flags $JAVACFLAGS to compile the
dnl    given Java sources.
dnl
dnl    <IMPORTS> is an optional *single* class or package to import.
dnl    <FUNCTION-BODY> is an optional function body.
dnl
dnl    Example: REGINA_TRY_COMPILE_JAVA(javac, [ javax.swing ],
dnl                 [ JPanel p = null; ], ... )
dnl
AC_DEFUN(REGINA_TRY_COMPILE_JAVA, [
  cat << \EOF > Test.java
/* [#]line __oline__ "configure" */
ifelse([$2], , , [import $2;])
public class Test {
[$3]
}
EOF
  if AC_TRY_COMMAND($1 $JAVACFLAGS Test.java) && test -s Test.class; then
    rm -f Test.java Test.class
    ifelse([$4], , :, [$4])
  else
    rm -f Test.java Test.class
    ifelse([$5], , :, [$5])
  fi
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

dnl    -----------------------------------------------------------------
dnl
dnl                  Macros borrowed from external sources
dnl
dnl    -----------------------------------------------------------------
dnl
dnl    All macros below are licensed under the GNU General Public License
dnl    unless otherwise specified.
dnl
dnl    They have been renamed (with a prepended REGEXT_) to avoid conflicts.
dnl
dnl    Macros included:
dnl
dnl        REGEXT_ACX_PTHREAD([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
dnl        REGEXT_AM_PATH_CPPUNIT(MINIMUM-VERSION, [ACTION-IF-FOUND [,
dnl            ACTION-IF-NOT-FOUND]]])
dnl

dnl ----------------------
dnl
dnl   REGEXT_ACX_PTHREAD
dnl
dnl ----------------------

dnl @synopsis REGEXT_ACX_PTHREAD([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
dnl
dnl This macro figures out how to build C programs using POSIX
dnl threads.  It sets the PTHREAD_LIBS output variable to the threads
dnl library and linker flags, and the PTHREAD_CFLAGS output variable
dnl to any special C compiler flags that are needed.  (The user can also
dnl force certain compiler flags/libs to be tested by setting these
dnl environment variables.)
dnl
dnl Also sets PTHREAD_CC to any special C compiler that is needed for
dnl multi-threaded programs (defaults to the value of CC otherwise).
dnl (This is necessary on AIX to use the special cc_r compiler alias.)
dnl
dnl If you are only building threads programs, you may wish to
dnl use these variables in your default LIBS, CFLAGS, and CC:
dnl
dnl        LIBS="$PTHREAD_LIBS $LIBS"
dnl        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
dnl        CC="$PTHREAD_CC"
dnl
dnl In addition, if the PTHREAD_CREATE_JOINABLE thread-attribute
dnl constant has a nonstandard name, defines PTHREAD_CREATE_JOINABLE
dnl to that name (e.g. PTHREAD_CREATE_UNDETACHED on AIX).
dnl
dnl ACTION-IF-FOUND is a list of shell commands to run if a threads
dnl library is found, and ACTION-IF-NOT-FOUND is a list of commands
dnl to run it if it is not found.  If ACTION-IF-FOUND is not specified,
dnl the default action will define HAVE_PTHREAD.
dnl
dnl Please let the authors know if this macro fails on any platform,
dnl or if you have any other suggestions or comments.  This macro was
dnl based on work by SGJ on autoconf scripts for FFTW (www.fftw.org)
dnl (with help from M. Frigo), as well as ac_pthread and hb_pthread
dnl macros posted by AFC to the autoconf macro repository.  We are also
dnl grateful for the helpful feedback of numerous users.
dnl
dnl @version $Id$
dnl @author Steven G. Johnson <stevenj@alum.mit.edu> and Alejandro Forero Cuervo <bachue@bachue.com>

AC_DEFUN([REGEXT_ACX_PTHREAD], [
AC_REQUIRE([AC_CANONICAL_HOST])
acx_pthread_ok=no

# We used to check for pthread.h first, but this fails if pthread.h
# requires special compiler flags (e.g. on True64 or Sequent).
# It gets checked for in the link test anyway.

# First of all, check if the user has set any of the PTHREAD_LIBS,
# etcetera environment variables, and if threads linking works using
# them:
if test x"$PTHREAD_LIBS$PTHREAD_CFLAGS" != x; then
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        AC_MSG_CHECKING([for pthread_join in LIBS=$PTHREAD_LIBS with CFLAGS=$PTHREAD_CFLAGS])
        AC_TRY_LINK_FUNC(pthread_join, acx_pthread_ok=yes)
        AC_MSG_RESULT($acx_pthread_ok)
        if test x"$acx_pthread_ok" = xno; then
                PTHREAD_LIBS=""
                PTHREAD_CFLAGS=""
        fi
        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"
fi

# We must check for the threads library under a number of different
# names; the ordering is very important because some systems
# (e.g. DEC) have both -lpthread and -lpthreads, where one of the
# libraries is broken (non-POSIX).

# Create a list of thread flags to try.  Items starting with a "-" are
# C compiler flags, and other items are library names, except for "none"
# which indicates that we try without any flags at all.

acx_pthread_flags="pthreads none -Kthread -kthread lthread -pthread -pthreads -mthreads pthread --thread-safe -mt"

# The ordering *is* (sometimes) important.  Some notes on the
# individual items follow:

# pthreads: AIX (must check this before -lpthread)
# none: in case threads are in libc; should be tried before -Kthread and
#       other compiler flags to prevent continual compiler warnings
# -Kthread: Sequent (threads in libc, but -Kthread needed for pthread.h)
# -kthread: FreeBSD kernel threads (preferred to -pthread since SMP-able)
# lthread: LinuxThreads port on FreeBSD (also preferred to -pthread)
# -pthread: Linux/gcc (kernel threads), BSD/gcc (userland threads)
# -pthreads: Solaris/gcc
# -mthreads: Mingw32/gcc, Lynx/gcc
# -mt: Sun Workshop C (may only link SunOS threads [-lthread], but it
#      doesn't hurt to check since this sometimes defines pthreads too;
#      also defines -D_REENTRANT)
# pthread: Linux, etcetera
# --thread-safe: KAI C++

case "${host_cpu}-${host_os}" in
        *solaris*)

        # On Solaris (at least, for some versions), libc contains stubbed
        # (non-functional) versions of the pthreads routines, so link-based
        # tests will erroneously succeed.  (We need to link with -pthread or
        # -lpthread.)  (The stubs are missing pthread_cleanup_push, or rather
        # a function called by this macro, so we could check for that, but
        # who knows whether they'll stub that too in a future libc.)  So,
        # we'll just look for -pthreads and -lpthread first:

        acx_pthread_flags="-pthread -pthreads pthread -mt $acx_pthread_flags"
        ;;
esac

if test x"$acx_pthread_ok" = xno; then
for flag in $acx_pthread_flags; do

        case $flag in
                none)
                AC_MSG_CHECKING([whether pthreads work without any flags])
                ;;

                -*)
                AC_MSG_CHECKING([whether pthreads work with $flag])
                PTHREAD_CFLAGS="$flag"
                ;;

                *)
                AC_MSG_CHECKING([for the pthreads library -l$flag])
                PTHREAD_LIBS="-l$flag"
                ;;
        esac

        save_LIBS="$LIBS"
        save_CFLAGS="$CFLAGS"
        LIBS="$PTHREAD_LIBS $LIBS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Check for various functions.  We must include pthread.h,
        # since some functions may be macros.  (On the Sequent, we
        # need a special flag -Kthread to make this header compile.)
        # We check for pthread_join because it is in -lpthread on IRIX
        # while pthread_create is in libc.  We check for pthread_attr_init
        # due to DEC craziness with -lpthreads.  We check for
        # pthread_cleanup_push because it is one of the few pthread
        # functions on Solaris that doesn't have a non-functional libc stub.
        # We try pthread_create on general principles.
        AC_TRY_LINK([#include <pthread.h>],
                    [pthread_t th; pthread_join(th, 0);
                     pthread_attr_init(0); pthread_cleanup_push(0, 0);
                     pthread_create(0,0,0,0); pthread_cleanup_pop(0); ],
                    [acx_pthread_ok=yes])

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"

        AC_MSG_RESULT($acx_pthread_ok)
        if test "x$acx_pthread_ok" = xyes; then
                break;
        fi

        PTHREAD_LIBS=""
        PTHREAD_CFLAGS=""
done
fi

# Various other checks:
if test "x$acx_pthread_ok" = xyes; then
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Detect AIX lossage: threads are created detached by default
        # and the JOINABLE attribute has a nonstandard name (UNDETACHED).
        AC_MSG_CHECKING([for joinable pthread attribute])
        AC_TRY_LINK([#include <pthread.h>],
                    [int attr=PTHREAD_CREATE_JOINABLE;],
                    ok=PTHREAD_CREATE_JOINABLE, ok=unknown)
        if test x"$ok" = xunknown; then
                AC_TRY_LINK([#include <pthread.h>],
                            [int attr=PTHREAD_CREATE_UNDETACHED;],
                            ok=PTHREAD_CREATE_UNDETACHED, ok=unknown)
        fi
        if test x"$ok" != xPTHREAD_CREATE_JOINABLE; then
                AC_DEFINE(PTHREAD_CREATE_JOINABLE, $ok,
                          [Define to the necessary symbol if this constant
                           uses a non-standard name on your system.])
        fi
        AC_MSG_RESULT(${ok})
        if test x"$ok" = xunknown; then
                AC_MSG_WARN([we do not know how to create joinable pthreads])
        fi

        AC_MSG_CHECKING([if more special flags are required for pthreads])
        flag=no
        case "${host_cpu}-${host_os}" in
                *-aix* | *-freebsd*)     flag="-D_THREAD_SAFE";;
                *solaris* | alpha*-osf*) flag="-D_REENTRANT";;
        esac
        AC_MSG_RESULT(${flag})
        if test "x$flag" != xno; then
                PTHREAD_CFLAGS="$flag $PTHREAD_CFLAGS"
        fi

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"

        # More AIX lossage: must compile with cc_r
        AC_CHECK_PROG(PTHREAD_CC, cc_r, cc_r, ${CC})
else
        PTHREAD_CC="$CC"
fi

AC_SUBST(PTHREAD_LIBS)
AC_SUBST(PTHREAD_CFLAGS)
AC_SUBST(PTHREAD_CC)

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test x"$acx_pthread_ok" = xyes; then
        ifelse([$1],,AC_DEFINE(HAVE_PTHREAD,1,[Define if you have POSIX threads libraries and header files.]),[$1])
        :
else
        acx_pthread_ok=no
        $2
fi

])dnl REGEXT_ACX_PTHREAD

dnl --------------------------
dnl
dnl   REGEXT_AM_PATH_CPPUNIT
dnl
dnl --------------------------

dnl
dnl REGEXT_AM_PATH_CPPUNIT([MINIMUM-VERSION, [ACTION-IF-FOUND [,
dnl     ACTION-IF-NOT-FOUND]]])
dnl
dnl Taken from the CppUnit project at http://cppunit.sourceforge.net/.
dnl
dnl Licensed under the GNU Lesser General Public License.
dnl
AC_DEFUN(REGEXT_AM_PATH_CPPUNIT,
[

AC_ARG_WITH(cppunit-prefix,[  --with-cppunit-prefix=PFX   Prefix where CppUnit is installed (optional)],
            cppunit_config_prefix="$withval", cppunit_config_prefix="")
AC_ARG_WITH(cppunit-exec-prefix,[  --with-cppunit-exec-prefix=PFX  Exec prefix where CppUnit is installed (optional)],
            cppunit_config_exec_prefix="$withval", cppunit_config_exec_prefix="")

  if test x$cppunit_config_exec_prefix != x ; then
     cppunit_config_args="$cppunit_config_args --exec-prefix=$cppunit_config_exec_prefix"
     if test x${CPPUNIT_CONFIG+set} != xset ; then
        CPPUNIT_CONFIG=$cppunit_config_exec_prefix/bin/cppunit-config
     fi
  fi
  if test x$cppunit_config_prefix != x ; then
     cppunit_config_args="$cppunit_config_args --prefix=$cppunit_config_prefix"
     if test x${CPPUNIT_CONFIG+set} != xset ; then
        CPPUNIT_CONFIG=$cppunit_config_prefix/bin/cppunit-config
     fi
  fi

  AC_PATH_PROG(CPPUNIT_CONFIG, cppunit-config, no)
  cppunit_version_min=$1

  AC_MSG_CHECKING(for Cppunit - version >= $cppunit_version_min)
  no_cppunit=""
  if test "$CPPUNIT_CONFIG" = "no" ; then
    no_cppunit=yes
  else
    CPPUNIT_CFLAGS=`$CPPUNIT_CONFIG --cflags`
    CPPUNIT_LIBS=`$CPPUNIT_CONFIG --libs`
    cppunit_version=`$CPPUNIT_CONFIG --version`

    cppunit_major_version=`echo $cppunit_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    cppunit_minor_version=`echo $cppunit_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    cppunit_micro_version=`echo $cppunit_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

    cppunit_major_min=`echo $cppunit_version_min | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    cppunit_minor_min=`echo $cppunit_version_min | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    cppunit_micro_min=`echo $cppunit_version_min | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

    cppunit_version_proper=`expr \
        $cppunit_major_version \> $cppunit_major_min \| \
        $cppunit_major_version \= $cppunit_major_min \& \
        $cppunit_minor_version \> $cppunit_minor_min \| \
        $cppunit_major_version \= $cppunit_major_min \& \
        $cppunit_minor_version \= $cppunit_minor_min \& \
        $cppunit_micro_version \>= $cppunit_micro_min `

    if test "$cppunit_version_proper" = "1" ; then
      AC_MSG_RESULT([$cppunit_major_version.$cppunit_minor_version.$cppunit_micro_version])
    else
      AC_MSG_RESULT(no)
      no_cppunit=yes
    fi
  fi

  if test "x$no_cppunit" = x ; then
     ifelse([$2], , :, [$2])     
  else
     CPPUNIT_CFLAGS=""
     CPPUNIT_LIBS=""
     ifelse([$3], , :, [$3])
  fi

  AC_SUBST(CPPUNIT_CFLAGS)
  AC_SUBST(CPPUNIT_LIBS)
])



