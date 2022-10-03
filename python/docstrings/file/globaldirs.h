/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace regina::python::doc {


// Docstring regina::python::doc::GlobalDirs
static const char *GlobalDirs =
R"doc(Provides global routines that return directories in which various
components of Regina are installed on the system.

By default, these routines return directories that were configured by
*cmake* when Regina was compiled, which will only be useful with a
fixed filesystem installation of Regina (e.g., a typical Linux
install).

If Regina may be running from a different place in the filesystem
(e.g., if you are running an app bundle on macOS or if you are running
directly from the source tree), then you *must* call either setDirs()
or deduceDirs() when your application starts. Otherwise the
directories that GlobalDirs returns might be incorrect, and might not
even exist.)doc";

namespace GlobalDirs_ {

// Docstring regina::python::doc::GlobalDirs_::census
static const char *census =
R"doc(Returns the directory containing the large machine-encoded census data
files. These data files are not human-browsable: instead they are
built for fast performance and small size. Users can access them by
calling Census::lookup().

On a typical GNU/Linux system, this directory might (for example) be
``/usr/local/share/regina/data/census``.

.. warning::
    If Regina is not installed in the exact location configured at
    compile time (e.g., if you are running a macOS app bundle or you
    are running directly out of the source tre), you *must* call
    either setDirs() or deduceDirs() before calling this routine.

Returns:
    Regina's calculation engine data directory.)doc";

// Docstring regina::python::doc::GlobalDirs_::data
static const char *data =
R"doc(Returns the directory containing miscellaneous data files for internal
use Regina's calculation engine.

This is computed automatically as the ``data/`` subdirectory of
home().

On a typical GNU/Linux system, this directory might (for example) be
``/usr/local/share/regina/data`` .

.. warning::
    If Regina is not installed in the exact location configured at
    compile time (e.g., if you are running a macOS app bundle or you
    are running directly out of the source tre), you *must* call
    either setDirs() or deduceDirs() before calling this routine.

Returns:
    Regina's calculation engine data directory.)doc";

// Docstring regina::python::doc::GlobalDirs_::deduceDirs
static const char *deduceDirs =
R"doc(Ask Regina to deduce where its supporting files are installed.

You should call either setDirs() or deduceDirs() at runtime if Regina
is not installed in the location that was configured by *cmake* at
build time (e.g., if you are running a macOS app bundle, or if you are
running directly out of the source tree).

This routine sets all of the relevant paths automatically, based on
the type of build and the location of the given executable (which is
typically the program currently being run). It can identify the
correct paths for fixed XDG installations, relocatable macOS bundles,
Windows installations, and also running directly from within the
source tree.

If running from within the source tree, the executable must be inside
the *build* tree (any depth of subdirectory within the build tree is
fine). Moreover, it is assumed that the build tree is either an
immediate subdirectory of the source root, or else is the source root
itself. So, for example, an executable path of
``regina-X.Y/qtui/src/regina-gui`` or
``regina-X.Y/build/qtui/src/regina-gui`` is fine, but
``regina-X.Y/tmp/build/qtui/src/regina-gui`` is not.

This routine respects the following environment variables, and these
will take precedence over any automatic deductions:

- REGINA_HOME, if present and non-empty, will determine Regina's
primary home directory as returned by homeDir(). It will also fix the
census data directory as returned by censusDir() to be
``$REGINA_HOME/data/census``.

- REGINA_PYLIBDIR, if present and non-empty, will determine the
directory containing Regina's python module as returned by
pythonModule().

If you have an unusual setup where Regina cannot deduce the paths
correctly, you can always call setDirs() to set the paths explicitly
yourself.

Parameter ``executable``:
    the path to an executable, which would typically be the executable
    currently being run.)doc";

// Docstring regina::python::doc::GlobalDirs_::engineDocs
static const char *engineDocs =
R"doc(Returns the directory in which API documentation for Regina's
calculation engine is installed.

This is computed automatically:

- in most cases it will be the ``engine-docs/`` subdirectory of
home();

- in the special case where deduceDirs() was called and we are running
from the build tree, it will be the location in the build tree where
the API docs are built.

On a typical GNU/Linux system, this directory might (for example) be
``/usr/local/share/regina/engine-docs`` .

.. warning::
    If Regina is not installed in the exact location configured at
    compile time (e.g., if you are running a macOS app bundle or you
    are running directly out of the source tre), you *must* call
    either setDirs() or deduceDirs() before calling this routine.

Returns:
    Regina's calculation engine documentation directory.)doc";

// Docstring regina::python::doc::GlobalDirs_::examples
static const char *examples =
R"doc(Returns the directory in which example data files (including the
smaller but human-browsable census data files) are installed.

This is computed automatically as the ``examples/`` subdirectory of
home().

On a typical GNU/Linux system, this directory might (for example) be
``/usr/local/share/regina/examples`` .

.. warning::
    If Regina is not installed in the exact location configured at
    compile time (e.g., if you are running a macOS app bundle or you
    are running directly out of the source tre), you *must* call
    either setDirs() or deduceDirs() before calling this routine.

Returns:
    Regina's example and census data directory.)doc";

// Docstring regina::python::doc::GlobalDirs_::home
static const char *home =
R"doc(Returns Regina's primary home directory on the system. This directory
should contains subdirectories *data*, *icons*/, *examples*/ and so
on.

On a typical GNU/Linux system, this directory might (for example) be
``/usr/local/share/regina`` .

.. warning::
    If Regina is not installed in the exact location configured at
    compile time (e.g., if you are running a macOS app bundle or you
    are running directly out of the source tre), you *must* call
    either setDirs() or deduceDirs() before calling this routine.

Returns:
    Regina's primary home directory.)doc";

// Docstring regina::python::doc::GlobalDirs_::pythonModule
static const char *pythonModule =
R"doc(Returns the directory in which Regina's python module is installed, or
the empty string if the module is installed in python's standard site-
packages directory.

.. warning::
    If Regina is not installed in the exact location configured at
    compile time (e.g., if you are running a macOS app bundle or you
    are running directly out of the source tre), you *must* call
    either setDirs() or deduceDirs() before calling this routine.

Returns:
    Regina's python module directory.)doc";

// Docstring regina::python::doc::GlobalDirs_::setDirs
static const char *setDirs =
R"doc(Tells Regina explicitly where its supporting files are installed.

You should call either setDirs() or deduceDirs() at runtime if Regina
is not installed in the location that was configured by *cmake* at
build time (e.g., if you are running a macOS app bundle, or if you are
running directly out of the source tree).

In most settings, it is better to call deduceDirs(), since this avoids
you (the programmer) having to manage through the many different
possible platforms and runtime environments.

Empty strings are treated as follows:

- If *homeDir* or *censusDir* is an empty string, then the
corresponding directory will not be changed. Instead it will left at
its previous value from the last call to setDirs() or deduceDirs(), or
at the build-time configured default if neither setDirs() nor
deduceDirs() has been called before.

- If *pythonDir* is an empty string then this has an explicit meaning,
namely that the python module has been installed in python's standard
site-packages directory.

Parameter ``homeDir``:
    Regina's primary home directory; this will be returned by
    homeDir().

Parameter ``pythonDir``:
    the directory containing Regina's python module, or the empty
    string if the module has been installed in python's standard site-
    packages directory; this will be returned by pythonModule().

Parameter ``censusDir``:
    The directory containing the large machine-encoded census data
    files; this will be returned by census().)doc";

}

} // namespace regina::python::doc

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

