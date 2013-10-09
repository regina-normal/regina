/* Define if both signed and unsigned long long types are available. */
#define LONG_LONG_FOUND

/* Define if both __int128_t and __uint128_t types are available. */
/* #undef INT128_FOUND */

/* Define if 64-bit integer literals are available with no suffix */
#define NUMERIC_64_FOUND

/* Define if 64-bit integer literals are available using the LL suffix */
#define NUMERIC_64_LL_FOUND

/* Define if Boost.Python is available. */
#define BOOST_PYTHON_FOUND

/* Define if langinfo.h and nl_langinfo() are available. */
#define LANGINFO_FOUND

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

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "regina-user@lists.sourceforge.net"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "Regina 4.95"

/* Define to the version of this package. */
#define PACKAGE_VERSION "4.95"

/* Major version number of the package. */
#define PACKAGE_VERSION_MAJOR 4

/* Minor version number of the package. */
#define PACKAGE_VERSION_MINOR 95

