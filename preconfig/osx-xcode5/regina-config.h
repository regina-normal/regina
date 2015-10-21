/* Define if both int128_t and uint128_t types are available. */
/* #undef INT128_T_FOUND */

/* Define if both __int128_t and __uint128_t types are available. */
#define __INT128_T_FOUND

/* Define if native 128-bit arithmetic is available through either
   of the types defined above. */
#define INT128_AVAILABLE

/* Define if 64-bit integer literals are available with no suffix */
#define NUMERIC_64_FOUND

/* Define if 64-bit integer literals are available using the LL suffix */
#define NUMERIC_64_LL_FOUND

/* Define if Boost.Python is available. */
#define BOOST_PYTHON_FOUND

/* Define if langinfo.h and nl_langinfo() are available. */
#define LANGINFO_FOUND

/* Define if we are replacing Tokyo Cabinet with the older QDBM. */
/* #undef QDBM_AS_TOKYOCABINET */

/* Define if we are excluding Normaliz from the build. */
/* #undef EXCLUDE_NORMALIZ */

/* Define if we are excluding SnapPea / SnapPy from the build. */
/* #undef EXCLUDE_SNAPPEA */

/* Define as const if the declaration of iconv() needs const, or empty if not. */
#define ICONV_CONST 

/* Define to Regina's primary home directory on the system.
   This can always be changed at runtime vi NGlobalDirs::setDirs(). */
#define REGINA_DATADIR "/usr/local/share/regina"

/* Define to the directory on the system in which Regina's python module is
   installed.  can always be changed at runtime vi NGlobalDirs::setDirs(). */
#define REGINA_PYLIBDIR "/usr/local/lib/regina/python"

/* Define to the filename extension that we use for census databases. */
#define REGINA_DB_EXT "tdb"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "regina-user@lists.sourceforge.net"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "Regina 5.0"

/* Define to the version of this package. */
#define PACKAGE_VERSION "5.0"

/* Major version number of the package. */
#define PACKAGE_VERSION_MAJOR 5

/* Minor version number of the package. */
#define PACKAGE_VERSION_MINOR 0

