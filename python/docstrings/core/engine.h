/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *__doc_regina_hasInt128 =
R"doc(Does this particular build of Regina support native 128-bit
arithmetic?

On those platforms that do support native 128-bit arithmetic, some
expensive algorithms (in particular for normal surface enumeration)
are optimised to run significantly faster in some cases.

Returns:
    ``True`` if and only if this build supports native 128-bit
    arithmetic.)doc";

static const char *__doc_regina_politeThreads =
R"doc(A suggestion for user interfaces as to how many threads should be used
for parallel computations.

The current implementation returns approximately half the number of
cores available on the machine.

Returns:
    the suggested number of threads to use.)doc";

static const char *__doc_regina_testEngine =
R"doc(Tests to see if an interface can successfully communicate with the
underlying C++ calculation engine.

This routine simply uses the engine to return the same value that is
passed to it; it can be used to test whether communications between
the interface and the C++ engine are working properly.

Parameter ``value``:
    any integer; this same integer will be returned.

Returns:
    the same integer that was passed as *value*.)doc";

static const char *__doc_regina_versionMajor =
R"doc(Returns the major version number of this calculation engine. For
instance, version 2.3.1 would have major version 2.

Returns:
    the major version number of this calculation engine.)doc";

static const char *__doc_regina_versionMinor =
R"doc(Returns the minor version number of this calculation engine. For
instance, version 2.3.1 would have minor version 3.

Returns:
    the minor version number of this calculation engine.)doc";

static const char *__doc_regina_versionSnapPea =
R"doc(An alias for versionSnapPy(), which returns the version of SnapPy
whose underlying SnapPea kernel is built into Regina.

See versionSnapPy() for details.

Returns:
    the version of SnapPy whose underlying SnapPea kernel is built
    into Regina.)doc";

static const char *__doc_regina_versionSnapPy =
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

static const char *__doc_regina_versionString =
R"doc(Returns the full version number of this calculation engine. For
instance, version 2.3.1 would have full version ``"2.3.1"``.

Returns:
    the version of this calculation engine.)doc";

static const char *__doc_regina_versionUsesUTF8 =
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

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

