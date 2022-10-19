/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by ../gendoc.sh.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::buildInfo
static const char *buildInfo =
R"doc(Returns any additional information about this specific build of
Regina. For instance, an official download from the Regina website
might return "Official macOS build", and a package from the official
Debian servers might return "Debian package build".

This information is supplied at build time by passing the option
``-DBUILD_INFO=...`` to CMake.

If no such information was supplied at build time, then this string
will be empty.

Returns:
    any additional information supplied at build time.)doc";

// Docstring regina::python::doc::hasInt128
static const char *hasInt128 =
R"doc(Does this particular build of Regina support native 128-bit
arithmetic?

On those platforms that do support native 128-bit arithmetic, some
expensive algorithms (in particular for normal surface enumeration)
are optimised to run significantly faster in some cases.

Returns:
    ``True`` if and only if this build supports native 128-bit
    arithmetic.)doc";

// Docstring regina::python::doc::politeThreads
static const char *politeThreads =
R"doc(A suggestion for user interfaces as to how many threads should be used
for parallel computations.

The current implementation returns approximately half the number of
cores available on the machine.

Returns:
    the suggested number of threads to use.)doc";

// Docstring regina::python::doc::testEngine
static const char *testEngine =
R"doc(Tests to see if an interface can successfully communicate with the
underlying C++ calculation engine.

This routine simply uses the engine to return the same value that is
passed to it; it can be used to test whether communications between
the interface and the C++ engine are working properly.

Parameter ``value``:
    any integer; this same integer will be returned.

Returns:
    the same integer that was passed as *value*.)doc";

// Docstring regina::python::doc::versionMajor
static const char *versionMajor =
R"doc(Returns the major version number of this calculation engine. For
instance, version 2.3.1 would have major version 2.

Returns:
    the major version number of this calculation engine.)doc";

// Docstring regina::python::doc::versionMinor
static const char *versionMinor =
R"doc(Returns the minor version number of this calculation engine. For
instance, version 2.3.1 would have minor version 3.

Returns:
    the minor version number of this calculation engine.)doc";

// Docstring regina::python::doc::versionSnapPea
static const char *versionSnapPea =
R"doc(An alias for versionSnapPy(), which returns the version of SnapPy
whose underlying SnapPea kernel is built into Regina.

See versionSnapPy() for details.

Returns:
    the version of SnapPy whose underlying SnapPea kernel is built
    into Regina.)doc";

// Docstring regina::python::doc::versionSnapPy
static const char *versionSnapPy =
R"doc(Returns the version of SnapPy whose underlying SnapPea kernel is built
into Regina.

The SnapPea kernel is now maintained by the larger *SnapPy* project,
and Regina sources its version of the SnapPea kernel from one of the
recent SnapPy releases. See http://snappy.computop.org/ for details.

In Regina, you can access the SnapPea kernel through the
SnapPeaTriangulation class.

Returns:
    the version of SnapPy whose underlying SnapPea kernel is built
    into Regina.)doc";

// Docstring regina::python::doc::versionString
static const char *versionString =
R"doc(Returns the full version number of this calculation engine. For
instance, version 2.3.1 would have full version ``"2.3.1"``.

Returns:
    the version of this calculation engine.)doc";

// Docstring regina::python::doc::versionUsesUTF8
static const char *versionUsesUTF8 =
R"doc(Did the given version of Regina consistently use UTF-8 in its data
files?

In Regina versions 4.4 and below, no particular attention was paid to
character encodings. As a result, the GUI typically stored data in
LATIN1 (the default at the time for the Qt libraries).

As of Regina 4.5, all strings are now stored in UTF-8 where possible.

This routine allows programs to determine which regime a particular
version of Regina belongs to. This can be useful when working with
Regina data files on a low-level basis.

Any whitespace in the version string will confuse the result, and the
return value will be undefined.

As a special case, an empty string is treated as belonging to the
UTF-8 regime.

Parameter ``version``:
    a version string from some release of Regina, such as "4.2". This
    must not contain any whitespace padding.

Returns:
    ``True`` if the given version uses UTF-8 consistently, or
    ``False`` if the given version is an older version that did not
    pay attention to character encodings.)doc";

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

