/* Define if both int128_t and uint128_t types are available.
   This macro is for internal use only; end users should use the macro
   INT128_AVAILABLE and the typedefs from IntOfSize<16>, all of which
   are found in utilities/intutils.h. */
/* #undef INTERNAL_INT128_T_FOUND */

/* Define if both __int128_t and __uint128_t types are available.
   This macro is for internal use only; end users should use the macro
   INT128_AVAILABLE and the typedefs from IntOfSize<16>, all of which
   are found in utilities/intutils.h. */
#define INTERNAL___INT128_T_FOUND

/* Define if we should use boost's compile-time 128-bit integer detection,
   instead of explicitly testing for 128-bit integer types in cmake. */
/* #undef USE_BOOST_INT128 */

/* Define if 64-bit integer literals are available with no suffix */
#define NUMERIC_64_FOUND

/* Define if 64-bit integer literals are available using the LL suffix */
#define NUMERIC_64_LL_FOUND

/* Define if Boost.Python is available. */
#define BOOST_PYTHON_FOUND

/* Define if the Graphviz libraries are available. */
#define LIBGVC_FOUND

/* Define if langinfo.h and nl_langinfo() are available. */
#define LANGINFO_FOUND

/* Define if we are replacing Tokyo Cabinet with the older QDBM. */
/* #undef QDBM_AS_TOKYOCABINET */

/* Define as const if the declaration of iconv() needs const, or empty if not. */
#define ICONV_CONST 

/* Define to Regina's primary home directory on the system.
   This can always be changed at runtime vi GlobalDirs::setDirs(). */
#define REGINA_DATADIR "/usr/local/share/regina"

/* Define to the directory on the system in which Regina's python module is
   installed, or the empty string if the module is installed in a standard
   python location (i.e., it can be found automatically on python's sys.path).
   This can always be changed at runtime vi GlobalDirs::setDirs(). */
#define REGINA_PYLIBDIR ""

/* Define to the filename extension that we use for census databases. */
#define REGINA_DB_EXT "tdb"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "Regina 5.1"

/* Define to the version of this package. */
#define PACKAGE_VERSION "5.1"

/* Major version number of the package. */
#define PACKAGE_VERSION_MAJOR 5

/* Minor version number of the package. */
#define PACKAGE_VERSION_MINOR 0

/* Define to the version of SnapPy that is bundled with Regina. */
#define SNAPPY_VERSION "2.4"

