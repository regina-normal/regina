/* Define if both int128_t and uint128_t types are available.
   This macro is for internal use only; end users should use the macro
   INT128_AVAILABLE and the type aliases from IntOfSize<16>, all of which
   are found in utilities/intutils.h. */
/* #undef INTERNAL_INT128_T_FOUND */

/* Define if both __int128 and __uint128 types are available.
   This macro is for internal use only; end users should use the macro
   INT128_AVAILABLE and the type aliases from IntOfSize<16>, all of which
   are found in utilities/intutils.h. */
/* #undef INTERNAL___INT128_FOUND */

/* Define if both __int128_t and __uint128_t types are available.
   This macro is for internal use only; end users should use the macro
   INT128_AVAILABLE and the type aliases from IntOfSize<16>, all of which
   are found in utilities/intutils.h. */
#define INTERNAL___INT128_T_FOUND

/* Define if 64-bit integer literals are available with no suffix */
#define NUMERIC_64_FOUND

/* Define if 64-bit integer literals are available using the LL suffix */
#define NUMERIC_64_LL_FOUND

/* Define if we are supporting triangulations of dimensions 9-15. */
/* #undef REGINA_HIGHDIM */

/* Define if we are building the Python bindings. */
#define BUILD_PYTHON_BINDINGS

/* Define if the Graphviz libraries are available. */
#define LIBGVC_FOUND

/* Define if langinfo.h and nl_langinfo() are available. */
#define LANGINFO_FOUND

/* Define if we implement key-value stores using Tokyo Cabinet.
   Exactly one of these REGINA_KVSTORE_* macros should be defined. */
#define REGINA_KVSTORE_TOKYOCABINET

/* Define if we implement key-value stores using QDBM.
   Exactly one of these REGINA_KVSTORE_* macros should be defined. */
/* #undef REGINA_KVSTORE_QDBM */

/* Define if we implement key-value stores using LMDB.
   Exactly one of these REGINA_KVSTORE_* macros should be defined. */
/* #undef REGINA_KVSTORE_LMDB */

/* Define if we are building a standard freedesktop.org installation in
   a fixed location on the filesystem.
   Exactly one of REGINA_INSTALL_{XDG,HPC,BUNDLE,WINDOWS} should be defined. */
/* #undef REGINA_INSTALL_XDG */

/* Define if we are building a local installation in a fixed location suitable
   for an HPC system (typically in a user's home area, and with no GUI).
   Exactly one of REGINA_INSTALL_{XDG,HPC,BUNDLE,WINDOWS} should be defined. */
/* #undef REGINA_INSTALL_HPC */

/* Define if we are building a self-contained macOS app bundle that users can
   drag and drop.
   Exactly one of REGINA_INSTALL_{XDG,HPC,BUNDLE,WINDOWS} should be defined. */
#define REGINA_INSTALL_BUNDLE

/* Define if we are building a self-contained movable install directory
   for MS Windows.
   Exactly one of REGINA_INSTALL_{XDG,HPC,BUNDLE,WINDOWS} should be defined. */
/* #undef REGINA_INSTALL_WINDOWS */

/* Define if Regina ships its own python, with the core python libraries
   in pythonXY.zip. */
/* #undef PYTHON_CORE_IN_ZIP */

/* Define if Regina's python module is statically linked into the user
   interface. */
/* #undef PYTHON_STATIC_LINK */

/* Define as const if the declaration of iconv() needs const, or empty if not. */
#define ICONV_CONST 

/* Define to Regina's primary home directory on the system.
   This can always be changed at runtime via the GlobalDirs class. */
#define REGINA_DATADIR "/usr/local/share/regina"

/* Define to the directory on the system in which Regina's python module is
   installed, or the empty string if the module is installed in a standard
   python location (i.e., it can be found automatically on python's sys.path).
   This can always be changed at runtime via the GlobalDirs class. */
#define REGINA_PYLIBDIR ""

/* Define to the filename extension that we use for census databases. */
#define REGINA_DB_EXT "tdb"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "Regina 7.0"

/* Define to the version of this package. */
#define PACKAGE_VERSION "7.0"

/* Major version number of the package. */
#define PACKAGE_VERSION_MAJOR 7

/* Minor version number of the package. */
#define PACKAGE_VERSION_MINOR 0

/* Define to the version of SnapPy that is bundled with Regina. */
#define SNAPPY_VERSION "2.8"

