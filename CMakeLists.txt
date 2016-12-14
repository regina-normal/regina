CMAKE_MINIMUM_REQUIRED (VERSION 2.8.11)
PROJECT (regina)

# Let the user override the package name.
IF(NOT PACKAGE_NAME)
  SET (PACKAGE_NAME regina CACHE STRING
    "The package name used for installation directories.  On some platforms this is 'regina-normal', not 'regina', to avoid conflicts with other software with the same name."
    FORCE)
ENDIF(NOT PACKAGE_NAME)

SET (PACKAGE_PRETTY_NAME Regina)
# Look here for Find___.cmake modules
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Version
SET (PACKAGE_VERSION 5.1)
SET (SNAPPY_VERSION 2.4)
SET (PACKAGE_STRING "${PACKAGE_PRETTY_NAME} ${PACKAGE_VERSION}")

# Extract major/minor version
# Note: The PACKAGE_VERSION_MAJOR "output" is discarded, since it matches the
# whole string
STRING(REGEX MATCH "^([0-9]+).([0-9]+)" PACKAGE_VERSION_MAJOR "${PACKAGE_VERSION}")
SET (PACKAGE_VERSION_MAJOR ${CMAKE_MATCH_1} )
SET (PACKAGE_VERSION_MINOR ${CMAKE_MATCH_2} )

# Make a release build by default.
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build.  Options are: Debug Release RelWithDebInfo MinSizeRel."
  FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

# Every year I swear I will drop windows support, and this year is no different.
# The windows build (as of November 2015) crashes in the test suite with -O3.
# It looks as though this might be a compiler problem: the ipa-cp-clone
# optimisation has been known to cause crashes under mingw in other projects
# in the past also.
IF (WIN32)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-ipa-cp-clone")
ENDIF (WIN32)

# Distinguish between the different Regina installation types:
# XDG = standard freedesktop.org filesystem installation in a fixed location.
# Bundle = self-contained app bundle for MacOS that users can drag and drop.
# Windows = self-contained movable install directory for MS Windows.
if (NOT REGINA_INSTALL_TYPE)
  if (APPLE)
    SET (REGINA_DEFAULT_INSTALL_TYPE Bundle)
  elseif (WIN32)
    SET (REGINA_DEFAULT_INSTALL_TYPE Windows)
  else ()
    SET (REGINA_DEFAULT_INSTALL_TYPE XDG)
  endif ()
  SET (REGINA_INSTALL_TYPE ${REGINA_DEFAULT_INSTALL_TYPE} CACHE STRING
    "Choose how Regina installs itself.  Options are: XDG Bundle Windows."
    FORCE)
ENDIF (NOT REGINA_INSTALL_TYPE)
MESSAGE (STATUS "Regina install type: ${REGINA_INSTALL_TYPE}")

# Sanity check the installation type:
IF (${REGINA_INSTALL_TYPE} STREQUAL XDG)
  # No sanity checking needed here (yet).
ELSEIF (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  IF (NOT APPLE)
    MESSAGE(FATAL_ERROR
      "REGINA_INSTALL_TYPE=Bundle is only supported on MacOSX.")
  ENDIF (NOT APPLE)
ELSEIF (${REGINA_INSTALL_TYPE} STREQUAL Windows)
  IF (NOT WIN32)
    MESSAGE(FATAL_ERROR
      "REGINA_INSTALL_TYPE=Windows is only supported on MS Windows.")
  ENDIF (NOT WIN32)
ELSE()
  MESSAGE(FATAL_ERROR
    "REGINA_INSTALL_TYPE must be one of: XDG Bundle Windows.")
ENDIF()

# Are we installing development files?
IF (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  SET (REGINA_INSTALL_DEV OFF)
ELSE (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  SET (REGINA_INSTALL_DEV ON)
ENDIF (${REGINA_INSTALL_TYPE} STREQUAL Bundle)

# Give packagers a way of insisting that every optional component is found.
# Set PACKAGING_MODE=1 to make every component compulsory.
# Set PACKAGING_MODE=1 and PACKAGING_NO_MPI=1 to make every component
# compulsory except for the MPI utilities.
IF(NOT PACKAGING_MODE)
  SET (PACKAGING_MODE OFF CACHE STRING
    "Makes all optional components mandatory, so that cmake will fail if any optional component is not found."
    FORCE)
ENDIF(NOT PACKAGING_MODE)
IF(NOT PACKAGING_NO_MPI)
  SET (PACKAGING_NO_MPI OFF CACHE STRING
    "Leaves MPI as an optional component, even if PACKAGING_MODE is ON."
    FORCE)
ENDIF(NOT PACKAGING_NO_MPI)
IF(PACKAGING_MODE)
  SET(REGINA_MANDATORY TRUE)
  IF(PACKAGING_NO_MPI)
    SET(REGINA_MANDATORY_MPI FALSE)
  ELSE(PACKAGING_NO_MPI)
    SET(REGINA_MANDATORY_MPI TRUE)
  ENDIF(PACKAGING_NO_MPI)
ELSE(PACKAGING_MODE)
  SET(REGINA_MANDATORY FALSE)
  SET(REGINA_MANDATORY_MPI FALSE)
ENDIF(PACKAGING_MODE)

# For pretty logging of optional features at the end of the cmake run:
INCLUDE(MacroLogFeature)

# Modules needed for IOS check and function existence
INCLUDE( CheckCXXSourceCompiles )
INCLUDE( CheckFunctionExists )

# We need to determine threading flags *before* testing for C++11 std::thread.
FIND_PACKAGE(Threads REQUIRED)
IF(NOT Threads_FOUND)
  MESSAGE(FATAL_ERROR "Regina requires threading support.")
ENDIF(NOT Threads_FOUND)
IF(CMAKE_COMPILER_IS_GNUCXX)
  # The FindThreads module picks up -lpthread, not -pthread.
  # This is unfortunate, because std::thread must be built with -pthread
  # under g++; otherwise it builds but throws an exception at runtime.
  SET(CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS}")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

# C++11 requirements
INCLUDE(CheckCXX11Features)
SET(HAVE_CXX11_FEATURES TRUE)
NEED_CXX11_FEATURE("auto")
NEED_CXX11_FEATURE("constexpr")
NEED_CXX11_FEATURE("long_long")
NEED_CXX11_FEATURE("nullptr")
NEED_CXX11_FEATURE("rvalue_references")
NEED_CXX11_FEATURE("static_assert")
NEED_CXX11_FEATURE("stdexceptionptr")
NEED_CXX11_FEATURE("stdthread")
NEED_CXX11_FEATURE("variadic_templates")
IF(NOT HAVE_CXX11_FEATURES)
  MESSAGE(FATAL_ERROR "Regina requires C++11 support to compile.")
ENDIF()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_SWITCH}")

# Regina's own cmake scripts:
INCLUDE( ReginaMacros )

# Always include . in the header search path:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Installation directories
if (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  # MacOSX app bundle.
  SET (APPDIR ${CMAKE_INSTALL_PREFIX})
  SET (BUNDLEDIR ${APPDIR}/Regina.app)
  SET (BUNDLEDIR_ESCAPED "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/Regina.app")
  SET (PLUGINSDIR ${BUNDLEDIR}/Contents/Plugins)
  SET (PLUGINSDIR_ESCAPED "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/Regina.app/Contents/Plugins")
  SET (RESOURCEDIR ${BUNDLEDIR}/Contents/Resources)
  SET (BINDIR ${BUNDLEDIR}/Contents/MacOS)
  SET (LIBDIR ${BINDIR})
  SET (DATADIR ${RESOURCEDIR})
  SET (INCLUDEDIR ${RESOURCEDIR}/include)
  SET (PKGDATADIR ${DATADIR})
  SET (PYLIBDIR ${LIBDIR}/python)
elseif (${REGINA_INSTALL_TYPE} STREQUAL Windows)
  # MS Windows install.
  SET (APPDIR ${CMAKE_INSTALL_PREFIX}/bin)
  SET (BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
  SET (LIBDIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
  SET (DATADIR ${CMAKE_INSTALL_PREFIX}/share)
  SET (INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include/${PACKAGE_NAME})
  SET (PKGDATADIR ${DATADIR}/${PACKAGE_NAME})
  SET (PYLIBDIR ${LIBDIR}/${PACKAGE_NAME}/python)
else ()
  # Full install.
  SET (APPDIR ${CMAKE_INSTALL_PREFIX}/bin)
  SET (BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
  SET (LIBDIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
  SET (DATADIR ${CMAKE_INSTALL_PREFIX}/share)
  SET (INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include/${PACKAGE_NAME})
  SET (PKGDATADIR ${DATADIR}/${PACKAGE_NAME})
  UNSET (PYLIBDIR)
endif ()
SET (HTMLDIR ${PKGDATADIR}/docs)
SET (ICONDIR ${DATADIR}/icons)
SET (MANDIR ${DATADIR}/man)
SET (XDG_APPS_INSTALL_DIR ${DATADIR}/applications)
SET (XDG_MIME_INSTALL_DIR ${DATADIR}/mime/packages)

# Useful build directories
SET (ENGINE_INCLUDES "${PROJECT_SOURCE_DIR}/engine" )
SET (ENGINE_LIBRARY regina-engine)

# RPATH support.
IF (NOT DISABLE_RPATH)
  SET(DISABLE_RPATH OFF CACHE BOOL
    "Do not build with rpath.  This option may break installations in non-standard directories, since executables may be unable to find Regina's libraries.  For installations in standard areas such as /usr however, this option is highly recommended." FORCE)
ENDIF (NOT DISABLE_RPATH)

IF (NOT DISABLE_RPATH)
  if (APPLE)
    set(CMAKE_INSTALL_NAME_DIR ${LIBDIR})
  else (APPLE)
    set(CMAKE_INSTALL_RPATH ${LIBDIR} )
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  endif (APPLE)
ENDIF (NOT DISABLE_RPATH)

# Deprecation warnings.
IF(NOT STRICT_MODE)
  SET (STRICT_MODE OFF CACHE STRING
    "Compiles Regina with -DREGINA_STRICT, which (for supported compilers) issues warnings whenever deprecated code is used."
    FORCE)
ENDIF(NOT STRICT_MODE)
IF(STRICT_MODE)
  SET (CONFIG_STRICT_FLAGS "-DREGINA_STRICT")
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CONFIG_STRICT_FLAGS}")
ELSE(STRICT_MODE)
  SET (CONFIG_STRICT_FLAGS "")
ENDIF(STRICT_MODE)

# i18n checks.
CHECK_CXX_SOURCE_COMPILES("
  #include \"langinfo.h\"
  int main() { nl_langinfo(CODESET); return 0; }"
  LANGINFO_FOUND )
if(NOT LANGINFO_FOUND)
  MESSAGE(WARNING "langinfo not found: UTF-8 will be used always, and the locale-specific encoding will be ignored.")
endif(NOT LANGINFO_FOUND)

IF(NOT USE_BOOST_INT128)
  SET (USE_BOOST_INT128 OFF CACHE STRING
    "Use boost's compile-time 128-bit integer detection, instead of explicitly testing for 128-bit integers here in cmake."
    FORCE)
ENDIF(NOT USE_BOOST_INT128)

CHECK_CXX_SOURCE_COMPILES("
  #include <stdint.h>
  int128_t x;
  uint128_t y;
  int main() { return 0; }"
  INTERNAL_INT128_T_FOUND )

CHECK_CXX_SOURCE_COMPILES("
  #include <stdint.h>
  __int128_t x;
  __uint128_t y;
  int main() { return 0; }"
  INTERNAL___INT128_T_FOUND )

CHECK_CXX_SOURCE_COMPILES("
  unsigned long long x = 0xFFFFFFFFFFFFFFFF;
  int main() { return 0; }"
  NUMERIC_64_FOUND)
CHECK_CXX_SOURCE_COMPILES("
  unsigned long long x = 0xFFFFFFFFFFFFFFFFLL;
  int main() { return 0; }"
  NUMERIC_64_LL_FOUND)

# Dependencies used by all components of Regina
FIND_PACKAGE(ZLIB)
INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})
MACRO_LOG_FEATURE(ZLIB_FOUND
  "zlib"
  "Essential: compression support"
  "http://www.gzip.org/zlib/"
  TRUE)

FIND_PACKAGE(LibXml2)
INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR})
MACRO_LOG_FEATURE(LIBXML2_FOUND
  "libxml2"
  "Essential: XML support"
  "ftp.gnome.org"
  TRUE)

FIND_PACKAGE(GMP)
INCLUDE_DIRECTORIES(${GMP_INCLUDE_DIR})
MACRO_LOG_FEATURE(GMP_FOUND
  "GMP/C"
  "Essential: large integer arithmetic support for C"
  "http://gmplib.org/"
  TRUE)

FIND_PACKAGE(GMPXX)
INCLUDE_DIRECTORIES(${GMPXX_INCLUDE_DIR})
MACRO_LOG_FEATURE(GMPXX_FOUND
  "GMP/C++"
  "Essential: large integer arithmetic support for C++"
  "http://gmplib.org/"
  TRUE)

FIND_PACKAGE(ICONV)
INCLUDE_DIRECTORIES(${ICONV_INCLUDE_DIR})
MACRO_LOG_FEATURE(ICONV_FOUND
  "iconv"
  "Essential: internationalisation support"
  "http://www.gnu.org/s/libiconv/"
  TRUE)

FIND_PACKAGE(Jansson)
INCLUDE_DIRECTORIES(${JANSSON_INCLUDE_DIR})
MACRO_LOG_FEATURE(JANSSON_FOUND
  "Jansson"
  "Essential: JSON support"
  "http://www.digip.org/jansson/"
  TRUE)

FIND_PACKAGE(Boost 1.55 COMPONENTS iostreams python)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
MACRO_LOG_FEATURE(Boost_FOUND
  "Boost (>= 1.55)"
  "Essential: C++ components (iostreams, python)"
  "http://www.boost.org/"
  TRUE)

IF (Boost_FOUND)
  SET (OLD_INCLUDES ${CMAKE_REQUIRED_INCLUDES})
  SET (OLD_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES})
  SET (CMAKE_REQUIRED_INCLUDES ${Boost_INCLUDE_DIR})
  SET (CMAKE_REQUIRED_LIBRARIES ${Boost_IOSTREAMS_LIBRARY})
  # Ensure that boost.iostream was built against zlib.
  # If not, we will display the error message at the very end, after
  # MACRO_DISPLAY_FEATURE_LOG().
  CHECK_CXX_SOURCE_COMPILES("
    #include <boost/iostreams/filtering_streambuf.hpp>
    #include <boost/iostreams/filter/gzip.hpp>

    int main() {
        boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
        in.push(boost::iostreams::gzip_decompressor());
        return 0;
    }"
    BOOST_IOSTREAMS_ZLIB_FOUND)
  SET (CMAKE_REQUIRED_INCLUDES ${OLD_INCLUDES})
  SET (CMAKE_REQUIRED_LIBRARIES ${OLD_LIBRARIES})
ENDIF (Boost_FOUND)

FIND_PACKAGE(PkgConfig)
IF(NOT PKG_CONFIG_FOUND)
  MESSAGE(FATAL_ERROR "Regina requires pkgconfig.")
ENDIF(NOT PKG_CONFIG_FOUND)

IF(NOT QDBM)
  SET (QDBM OFF CACHE STRING
    "Makes Regina build against the older QDBM library, instead of the newer Tokyo Cabinet.  This should only be used on systems (e.g., MinGW) to which Tokyo Cabinet cannot be reliably ported."
    FORCE)
ENDIF(NOT QDBM)
IF (QDBM)
  PKG_CHECK_MODULES(TOKYOCABINET qdbm)
  MACRO_LOG_FEATURE(TOKYOCABINET_FOUND
    "QDBM"
    "Essential: lightweight database library for census data"
    "http://fallabs.com/qdbm/"
    TRUE)
  SET (QDBM_AS_TOKYOCABINET TRUE)
  SET (DB_EXT "qdb")
ELSE (QDBM)
  PKG_CHECK_MODULES(TOKYOCABINET tokyocabinet)
  MACRO_LOG_FEATURE(TOKYOCABINET_FOUND
    "Tokyo Cabinet"
    "Essential: lightweight database library for census data"
    "http://fallabs.com/tokyocabinet/"
    TRUE)
  SET (DB_EXT "tdb")
ENDIF (QDBM)

# Dependencies used by only some components of Regina
FIND_PACKAGE(POPT)
MACRO_LOG_FEATURE(POPT_FOUND
  "Popt"
  "Essential: command-line option processing"
  "http://rpm5.org/files/popt/"
  TRUE)

# Optionals
IF(NOT DISABLE_MPI)
  FIND_PACKAGE(MPI)
  MACRO_LOG_FEATURE(MPI_FOUND
    "MPI"
    "Build command-line tools for high-performance computing"
    "http://www.open-mpi.org/"
    REGINA_MANDATORY_MPI)
ENDIF(NOT DISABLE_MPI)

FIND_PACKAGE(Doxygen)
MACRO_LOG_FEATURE(DOXYGEN_FOUND
  "Doxygen"
  "Generate C++/Python API docs"
  "http://www.doxygen.org/"
  REGINA_MANDATORY)

IF (REGINA_DOCS)
  get_filename_component(REGINA_DOCS_FILE "${REGINA_DOCS}" ABSOLUTE)

  IF (NOT EXISTS ${REGINA_DOCS_FILE})
    MESSAGE (FATAL_ERROR "The argument to REGINA_DOCS should be a zip file containing pre-built handbooks.  The file you gave (${REGINA_DOCS}) does not exist.")
  ENDIF (NOT EXISTS ${REGINA_DOCS_FILE})

  MESSAGE (STATUS "Using pre-built handbooks from ${REGINA_DOCS_FILE}")

  FIND_PROGRAM(UNZIP_EXECUTABLE NAMES unzip DOC "Command-line unzip tool")
  IF (UNZIP_EXECUTABLE)
    SET (UNZIP_FOUND TRUE)
  ENDIF (UNZIP_EXECUTABLE)
  SET (REGINA_BUILD_HANDBOOK ${UNZIP_FOUND})
  MACRO_LOG_FEATURE(UNZIP_FOUND
    "unzip"
    "Extract the user handbook"
    "http://www.info-zip.org/pub/infozip/"
    REGINA_MANDATORY)
ELSE (REGINA_DOCS)
  SET (REGINA_DOCS "" CACHE STRING
    "Extract handbooks from the given pre-built zip file instead of building them manually."
    FORCE)

  FIND_PROGRAM(XSLTPROC_EXECUTABLE NAMES xsltproc DOC "XSLT processor")
  IF (XSLTPROC_EXECUTABLE)
    SET (XSLTPROC_FOUND TRUE)
  ENDIF (XSLTPROC_EXECUTABLE)
  SET (REGINA_BUILD_HANDBOOK ${XSLTPROC_FOUND})
  MACRO_LOG_FEATURE(XSLTPROC_FOUND
    "xsltproc"
    "Generate the user handbook"
    "http://xmlsoft.org/XSLT/"
    REGINA_MANDATORY)
ENDIF (REGINA_DOCS)

# Test suite
ENABLE_TESTING() # This must appear before any calls to ADD_SUBDIRECTORY().

# Core directories
ADD_SUBDIRECTORY(engine)
ADD_SUBDIRECTORY(utils)

# Python support
if (WIN32 AND NOT CYGWIN)
  SET (REGINA_PYTHON_EXTENSION "pyd")
  SET (REGINA_PYTHON_EXTENSION_NONSTANDARD TRUE)
else ()
  SET (REGINA_PYTHON_EXTENSION "so")
  SET (REGINA_PYTHON_EXTENSION_NONSTANDARD FALSE)
endif ()

IF(NOT DISABLE_PYTHON)
  FIND_PACKAGE(PythonInterp)
  FIND_PACKAGE(PythonLibs)
ENDIF(NOT DISABLE_PYTHON)

IF(Boost_FOUND AND PYTHONINTERP_FOUND AND PYTHONLIBS_FOUND)
  # We can support python.

  # Find the site-packages location in which the XDG build needs to put
  # the python module.
  EXECUTE_PROCESS(COMMAND
    "${PYTHON_EXECUTABLE}" -c "import distutils.sysconfig; print distutils.sysconfig.get_python_lib(True)"
    OUTPUT_VARIABLE SITE_PACKAGES
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE DISTUTILS_RESULT)
  IF ((NOT DISTUTILS_RESULT) AND IS_DIRECTORY "${SITE_PACKAGES}")
    MESSAGE(STATUS "Python site-packages directory: ${SITE_PACKAGES}")
  ELSE ()
    MESSAGE(FATAL_ERROR "Could not query the python site-packages directory.  Please either set the correct python interpreter (-DPYTHON_EXECUTABLE=...), or else disable python bindings (-DDISABLE_PYTHON).")
  ENDIF ()

  ADD_SUBDIRECTORY(python)
  SET(BOOST_PYTHON_FOUND TRUE)

  MESSAGE(STATUS "Python bindings enabled")
ELSEIF(Boost_FOUND)
  # Boost itself is mandatory - if it was not found then we have
  # bigger problems.
  MESSAGE(WARNING "Could not find Python interpreter and/or development files: Python bindings disabled.")
ENDIF()
MACRO_LOG_FEATURE(PYTHONINTERP_FOUND
  "Python interpreter"
  "Build Python bindings for Regina"
  "http://www.python.org/"
  REGINA_MANDATORY)
MACRO_LOG_FEATURE(PYTHONLIBS_FOUND
  "Python development files"
  "Build Python bindings for Regina"
  "http://www.python.org/"
  REGINA_MANDATORY)

# The Qt GUI
IF(NOT DISABLE_GUI)
  SET (DISABLE_GUI OFF CACHE STRING
    "Disables the graphical user interface.  The calculation engine, Python bindings and other command-line utilities will still be built.  This option is suitable for use on high-performance clusters."
    FORCE)
ENDIF(NOT DISABLE_GUI)
IF(DISABLE_GUI)
  set(GUI_REQUIRED FALSE)
  if (NOT ${REGINA_INSTALL_TYPE} STREQUAL XDG)
    MESSAGE(FATAL_ERROR
      "You can only set DISABLE_GUI=1 with REGINA_INSTALL_TYPE=XDG.")
  endif (NOT ${REGINA_INSTALL_TYPE} STREQUAL XDG)

  MESSAGE(WARNING "User set DISABLE_GUI=1: graphical user interface disabled.")
ELSE(DISABLE_GUI)
  set(GUI_REQUIRED TRUE)
  set(CMAKE_AUTOMOC ON)
  FIND_PACKAGE(Qt5Widgets)
  FIND_PACKAGE(Qt5Svg)

  IF (APPLE AND GRAPHVIZ_FRAMEWORK_PATH)
    # The GRAPHVIZ_FRAMEWORK_PATH option is a hack that allows you to link
    # with a single Graphviz framework, instead of the full suite of
    # Graphviz libraries and plugins.
    #
    # This framework should be called graphviz.framework, and should provide
    # all the necessary Graphviz components in just *one* library.  The
    # GRAPHVIZ_FRAMEWORK_PATH setting should be the directory *containing*
    # this framework (not the full path to the framework itself).
    #
    # It is expected that nobody except the Regina developer(s) will use
    # this option.
    #
    FIND_LIBRARY(GRAPHVIZ_FRAMEWORK graphviz PATHS ${GRAPHVIZ_FRAMEWORK_PATH})
    IF (GRAPHVIZ_FRAMEWORK)
      SET(GRAPHVIZ_TYPE Framework)
      SET(LIBGVC_FOUND TRUE)
    ELSE (GRAPHVIZ_FRAMEWORK)
      MESSAGE(WARNING "Could not find Graphviz framework, even though GRAPHVIZ_FRAMEWORK was set.")
      SET(LIBGVC_FOUND FALSE)
    ENDIF (GRAPHVIZ_FRAMEWORK)
  ELSE (APPLE AND GRAPHVIZ_FRAMEWORK_PATH)
    PKG_CHECK_MODULES(LIBGVC libgvc)
    IF(LIBGVC_FOUND)
      SET(GRAPHVIZ_TYPE Library)

      IF(WIN32)
        # FindPkgConfig has a nasty habit of converting paths like
        # "Program Files" into lists like "Program;Files".
        #
        # Here we convert these paths back; moreover, we convert them to
        # the ugly 8.3 filename format since windres.exe cannot (yet) cope
        # with paths containing spaces.  Sigh.
        #
        # Notes:
        # - This code assumes the Graphviz path does not contain semicolons.
        # - Although FindPkgConfig sets many LIBGVC_... variables, we only fix
        #   those that we use in our build system.
        #
        string(REPLACE ";" " " _TMP_STR "${LIBGVC_INCLUDEDIR}")
        execute_process(
          COMMAND cmd /c for %A in ("${_TMP_STR}") do @echo %~sA
          RESULT_VARIABLE _conv_res
          OUTPUT_VARIABLE _TMP_STR)
        string(REGEX REPLACE "\n" "" _TMP_STR "${_TMP_STR}")
        set(LIBGVC_INCLUDEDIR "${_TMP_STR}" CACHE INTERNAL "")

        string(REPLACE ";" " " _TMP_STR "${LIBGVC_LIBDIR}")
        execute_process(
          COMMAND cmd /c for %A in ("${_TMP_STR}") do @echo %~sA
          RESULT_VARIABLE _conv_res
          OUTPUT_VARIABLE _TMP_STR)
        string(REGEX REPLACE "\n" "" _TMP_STR "${_TMP_STR}")
        set(LIBGVC_LIBDIR "${_TMP_STR}" CACHE INTERNAL "")
      ENDIF(WIN32)
    ENDIF(LIBGVC_FOUND)
  ENDIF(APPLE AND GRAPHVIZ_FRAMEWORK_PATH)
  MACRO_LOG_FEATURE(LIBGVC_FOUND
    "GraphViz libraries"
    "Visualising graphs"
    "http://graphviz.org/"
    REGINA_MANDATORY)

  if (${REGINA_INSTALL_TYPE} STREQUAL XDG)
    set(SHARED_MIME_INFO_MINIMUM_VERSION "0.30")
    find_package(SharedMimeInfo)
    MACRO_LOG_FEATURE(SHARED_MIME_INFO_FOUND
      "SharedMimeInfo"
      "Required for Regina's graphical user interface"
      "http://freedesktop.org/wiki/Software/shared-mime-info"
      TRUE "0.30")
  endif (${REGINA_INSTALL_TYPE} STREQUAL XDG)

  if (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
    include(BundleUtilities)
  endif (${REGINA_INSTALL_TYPE} STREQUAL Bundle)

  # This must come after SharedMimeInfo is included, since the qtui/
  # makefiles use SharedMimeInfo macros.
  if (Qt5Widgets_FOUND AND Qt5Svg_FOUND)
    ADD_SUBDIRECTORY(qtui)
    SET(QT5_FOUND TRUE)
  else (Qt5Widgets_FOUND AND Qt5Svg_FOUND)
    SET(QT5_FOUND FALSE)
  endif (Qt5Widgets_FOUND AND Qt5Svg_FOUND)
  MESSAGE(STATUS "Graphical user interface enabled")
ENDIF(DISABLE_GUI)
MACRO_LOG_FEATURE(QT5_FOUND
  "Qt5"
  "Required for Regina's graphical user interface"
  "http://qt-project.org/"
  GUI_REQUIRED ""
  "To disable the graphical user interface, run: cmake -DDISABLE_GUI=1")

# Test suite, continued
FIND_PATH(CPPUNIT_INCLUDE_DIR cppunit/Test.h)
FIND_LIBRARY(CPPUNIT_LIBRARY NAMES cppunit)
IF (CPPUNIT_INCLUDE_DIR AND CPPUNIT_LIBRARY)
  SET(CPPUNIT_FOUND TRUE)
  MESSAGE(STATUS "Found CppUnit: ${CPPUNIT_LIBRARY}")
  ADD_SUBDIRECTORY(testsuite)
ELSE (CPPUNIT_INCLUDE_DIR AND CPPUNIT_LIBRARY)
  MESSAGE(WARNING "Could not find CppUnit: test suite disabled.")
ENDIF (CPPUNIT_INCLUDE_DIR AND CPPUNIT_LIBRARY)
MACRO_LOG_FEATURE(CPPUNIT_FOUND
  "CppUnit"
  "Build the full test suite for Regina"
  "http://sourceforge.net/projects/cppunit/"
  REGINA_MANDATORY)

# Miscellaneous subdirectories
ADD_SUBDIRECTORY(timing)
ADD_SUBDIRECTORY(docs)
ADD_SUBDIRECTORY(examples)

# Configure file
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/engine/regina-config.h.in"
  "${PROJECT_BINARY_DIR}/engine/regina-config.h"
)

MACRO_DISPLAY_FEATURE_LOG()

IF (Boost_FOUND AND NOT BOOST_IOSTREAMS_ZLIB_FOUND)
  MESSAGE(FATAL_ERROR
"Your boost.iostreams library appears to have been built without zlib/gzip support.  Regina needs this functionality.
If you built boost yourself, make sure that you built it with the libz development files installed.  You should see the line '- zlib : yes' appear in boost's configuration checks, at the very beginning of the boost build.")
ENDIF (Boost_FOUND AND NOT BOOST_IOSTREAMS_ZLIB_FOUND)

# CPack configuration to allow the developers to build a source tarball:
set(CPACK_PACKAGE_VERSION_MAJOR ${PACKAGE_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PACKAGE_VERSION_MINOR})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "Regina: Software for low-dimensional topology")
set(CPACK_PACKAGE_VENDOR "The Regina development team")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(CPACK_GENERATOR TGZ)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "regina-${PACKAGE_VERSION}")
set(CPACK_SOURCE_GENERATOR TGZ)
# The following regexes match anywhere:
set(CPACK_SOURCE_IGNORE_FILES
  "~$"
  "^${PROJECT_BINARY_DIR}/"
  "^${PROJECT_SOURCE_DIR}/engine/snappea/kernel/unused/"
  "^${PROJECT_SOURCE_DIR}/packaging/"
  "^${PROJECT_SOURCE_DIR}/utils/local/"
  "^${PROJECT_SOURCE_DIR}/utils/snappea/"
  "^${PROJECT_SOURCE_DIR}/www/"
  "^${PROJECT_SOURCE_DIR}/\\\\.git/"
  "^${PROJECT_SOURCE_DIR}.*/\\\\.svn/"
  "\\\\.DS_Store$"
)
include(CPack)

