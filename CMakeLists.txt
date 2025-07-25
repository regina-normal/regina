CMAKE_MINIMUM_REQUIRED (VERSION 3.12)
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

# Regina's own cmake scripts:
INCLUDE( ReginaMacros )

# Version
SET (PACKAGE_VERSION 7.4)
SET (SNAPPY_VERSION 3.0.3)
SET (PACKAGE_STRING "${PACKAGE_PRETTY_NAME} ${PACKAGE_VERSION}")

# Extract major/minor version
# Note: The PACKAGE_VERSION_MAJOR "output" is discarded, since it matches the
# whole string
STRING(REGEX MATCH "^([0-9]+).([0-9]+)" PACKAGE_VERSION_MAJOR "${PACKAGE_VERSION}")
SET (PACKAGE_VERSION_MAJOR ${CMAKE_MATCH_1} )
SET (PACKAGE_VERSION_MINOR ${CMAKE_MATCH_2} )

IF(BUILD_INFO)
  SET (PACKAGE_BUILD_STRING "${PACKAGE_STRING} (${BUILD_INFO})")
ELSE(BUILD_INFO)
  SET (BUILD_INFO "" CACHE STRING
    "Optional additional information about this specific build.  Examples might be: \"Official macOS build\", or \"Debian package build\"."
  FORCE)
  SET (PACKAGE_BUILD_STRING "${PACKAGE_STRING}")
ENDIF(BUILD_INFO)
REGINA_ESCAPE_C (BUILD_INFO)
REGINA_ESCAPE_C (PACKAGE_BUILD_STRING)
MESSAGE (STATUS "Detailed version: ${PACKAGE_BUILD_STRING}")

# Make a release build by default.
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build.  Options are: Debug Release RelWithDebInfo MinSizeRel."
  FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

SET(HIGHDIM "OFF" CACHE BOOL INTERNAL)
IF(HIGHDIM)
  MESSAGE (STATUS "Dimensions 9-15: enabled")
  SET (REGINA_HIGHDIM TRUE)
ELSE(HIGHDIM)
  MESSAGE (STATUS "Dimensions 9-15: disabled for performance - to enable these, pass -DHIGHDIM=1")
  SET (REGINA_HIGHDIM FALSE)
ENDIF(HIGHDIM)

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
# HPC = filesystem installation suitable for HPC systems where there is
#       no need for a GUI or HTML documentation, and where you do not have root.
# Windows = self-contained movable install directory for MS Windows.
if (NOT REGINA_INSTALL_TYPE)
  if (WIN32)
    SET (REGINA_DEFAULT_INSTALL_TYPE Windows)
  else ()
    SET (REGINA_DEFAULT_INSTALL_TYPE XDG)
  endif ()
  SET (REGINA_INSTALL_TYPE ${REGINA_DEFAULT_INSTALL_TYPE} CACHE STRING
    "Choose how Regina installs itself.  Options are: XDG HPC Windows."
    FORCE)
ENDIF (NOT REGINA_INSTALL_TYPE)
MESSAGE (STATUS "Regina install type: ${REGINA_INSTALL_TYPE}")

# Sanity check the installation type, and set the boolean install type flags
# that will appear in regina-config.h.
# Note: even though Bundle is not supported in CMake, we must keep the flag
# here because the Xcode build uses it (via the corresponding #define).
SET(REGINA_INSTALL_XDG FALSE)
SET(REGINA_INSTALL_HPC FALSE)
SET(REGINA_INSTALL_BUNDLE FALSE)
SET(REGINA_INSTALL_WINDOWS FALSE)
IF (${REGINA_INSTALL_TYPE} STREQUAL XDG)
  # No sanity checking needed here (yet).
  SET(REGINA_INSTALL_XDG TRUE)
ELSEIF (${REGINA_INSTALL_TYPE} STREQUAL HPC)
  # No sanity checking needed here either.
  SET(REGINA_INSTALL_HPC TRUE)
ELSEIF (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  MESSAGE(FATAL_ERROR
    "REGINA_INSTALL_TYPE=Bundle is no longer supported. These CMake build scripts will give a unix-style installation in a fixed location; for a standalone macOS app bundle please use Xcode instead.")
ELSEIF (${REGINA_INSTALL_TYPE} STREQUAL Windows)
  IF (NOT WIN32)
    MESSAGE(FATAL_ERROR
      "REGINA_INSTALL_TYPE=Windows is only supported on MS Windows.")
  ENDIF (NOT WIN32)
  SET(REGINA_INSTALL_WINDOWS TRUE)
ELSE()
  MESSAGE(FATAL_ERROR
    "REGINA_INSTALL_TYPE must be XDG, HPC or Windows.")
ENDIF()

IF(NOT (WIN32 OR APPLE))
  CHECK_LSB_RELEASE()
ENDIF(NOT (WIN32 OR APPLE))

# Are we installing development files?
SET (REGINA_INSTALL_DEV ON)

# Give packagers a way of insisting that every optional component is found.
# Set PACKAGING_MODE=1 to make every component compulsory.
IF(NOT PACKAGING_MODE)
  SET (PACKAGING_MODE OFF CACHE STRING
    "Makes all optional components mandatory, so that cmake will fail if any optional component is not found."
    FORCE)
ENDIF(NOT PACKAGING_MODE)
IF(PACKAGING_MODE)
  SET(REGINA_MANDATORY TRUE)
ELSE(PACKAGING_MODE)
  SET(REGINA_MANDATORY FALSE)
ENDIF(PACKAGING_MODE)

# Options that indicate how python is shipped.
IF(NOT PYTHON_CORE_IN_ZIP)
  SET (PYTHON_CORE_IN_ZIP OFF CACHE STRING
    "Indicates that Regina ships its own Python, with the core Python libraries in pythonXY.zip."
    FORCE)
ENDIF(NOT PYTHON_CORE_IN_ZIP)

# The iOS build of Regina links the python module directly into the GUI,
# and there is a flag in regina-config.h to indicate this.
# Such a configuration is not, however, supported by regina's cmake build
# system, and so we do not allow cmake users to enable it.
SET (PYTHON_STATIC_LINK OFF)

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

# Make everything use C++20 (or later).
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED YES)
SET(CMAKE_CXX_EXTENSIONS OFF)

# Always include . in the header search path:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Installation directories:
# The GNUInstallDirs module provides (amongst other things):
# CMAKE_INSTALL_FULL_{BINDIR,LIBEXECDIR,LIBDIR,INCLUDEDIR,DATADIR,MANDIR}.
INCLUDE(GNUInstallDirs)

# Here we define some other installation directories that we will need:
SET (PKGLIBEXECDIR "${CMAKE_INSTALL_FULL_LIBEXECDIR}/${PACKAGE_NAME}")
SET (PKGINCLUDEDIR "${CMAKE_INSTALL_FULL_INCLUDEDIR}/${PACKAGE_NAME}")
SET (PKGDATADIR "${CMAKE_INSTALL_FULL_DATADIR}/${PACKAGE_NAME}")
SET (PKGHTMLDIR "${PKGDATADIR}/docs")
if (${REGINA_INSTALL_TYPE} STREQUAL XDG)
  UNSET (PYLIBDIR)
else ()
  SET (PYLIBDIR "${CMAKE_INSTALL_FULL_LIBDIR}/${PACKAGE_NAME}/python")
endif ()
SET (XDG_ICON_DIR "${CMAKE_INSTALL_FULL_DATADIR}/icons")
SET (XDG_APPS_INSTALL_DIR "${CMAKE_INSTALL_FULL_DATADIR}/applications")
SET (XDG_MIME_INSTALL_DIR "${CMAKE_INSTALL_FULL_DATADIR}/mime/packages")
SET (XDG_METAINFO_DIR "${CMAKE_INSTALL_FULL_DATADIR}/metainfo")

# Make sure nobody is still using the old LIB_SUFFIX (which is now ignored):
IF (LIB_SUFFIX)
  UNSET(LIB_SUFFIX CACHE)
  MESSAGE(FATAL_ERROR "Regina no longer uses LIB_SUFFIX.  This should now be deduced automatically, but if you need to explicitly set the library installation directory then use CMAKE_INSTALL_LIBDIR instead.  For example, you might pass -DCMAKE_INSTALL_LIBDIR=lib64.")
ENDIF (LIB_SUFFIX)

# Useful build directories
SET (ENGINE_INCLUDES "${PROJECT_SOURCE_DIR}/engine" )
SET (ENGINE_LIBRARY regina-engine)

# Paths that we need to escape for @...@ substitution into scripts
REGINA_ESCAPE_BASH (CMAKE_SOURCE_DIR)

REGINA_ESCAPE_BASH (CMAKE_INSTALL_PREFIX)
REGINA_ESCAPE_PERL (CMAKE_INSTALL_PREFIX)
REGINA_ESCAPE_PERL (CMAKE_INSTALL_FULL_BINDIR)
REGINA_ESCAPE_BASH (CMAKE_INSTALL_FULL_LIBDIR)
REGINA_ESCAPE_PERL (CMAKE_INSTALL_FULL_LIBDIR)
REGINA_ESCAPE_PERL (PKGDATADIR)
REGINA_ESCAPE_BASH (PKGINCLUDEDIR)
REGINA_ESCAPE_PERL (PKGLIBEXECDIR)
REGINA_ESCAPE_PERL (PYLIBDIR)

REGINA_ESCAPE_BASH (PACKAGE_BUILD_STRING)

# RPATH support.
IF (NOT DISABLE_RPATH)
  SET(DISABLE_RPATH OFF CACHE BOOL
    "Do not build with rpath.  This option may break installations in non-standard directories, since executables may be unable to find Regina's libraries.  For installations in standard areas such as /usr however, this option is highly recommended." FORCE)
ENDIF (NOT DISABLE_RPATH)

IF (NOT DISABLE_RPATH)
  if (APPLE)
    set(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_FULL_LIBDIR}")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  else (APPLE)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  endif (APPLE)
ENDIF (NOT DISABLE_RPATH)

# i18n checks.
CHECK_CXX_SOURCE_COMPILES("
  #include \"langinfo.h\"
  int main() { nl_langinfo(CODESET); return 0; }"
  LANGINFO_FOUND )
if(NOT LANGINFO_FOUND)
  MESSAGE(WARNING "langinfo not found: UTF-8 will be used always, and the locale-specific encoding will be ignored.")
endif(NOT LANGINFO_FOUND)

CHECK_CXX_SOURCE_COMPILES("
  #include <stdint.h>
  int128_t x;
  uint128_t y;
  int main() { return 0; }"
  INTERNAL_INT128_T_FOUND )

CHECK_CXX_SOURCE_COMPILES("
  #include <stdint.h>
  __int128 x;
  __uint128 y;
  int main() { return 0; }"
  INTERNAL___INT128_FOUND )

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

# Test for the C++20 function std::lexicographical_compare_three_way, which
# is missing from some older versions of clang's libc++.
# It's not clear to me whether there is a C++20 feature test that could replace
# this; however, the only setting I've seen where this function is missing
# (debian buster with libc++-13) is also missing std::string::operator <=>,
# which is detected via __cpp_lib_three_way_comparison.  Anyway, let's just
# keep the explicit test for now.
CHECK_CXX_SOURCE_COMPILES("
  #include <algorithm>
  int x[3] = { 1, 2, 3 }, y[3] = { 1, 2, 3 };
  int main() {
    std::lexicographical_compare_three_way(x, x + 3, y, y + 3); return 0;
  }"
  LEXCMP_FOUND)

# Dependencies used by all components of Regina

# Note that pkg-config is optional, but it does help us find some libraries
# that we could perhaps not have found otherwise.
# In particular, most build environments will need it to locate Graphviz.
FIND_PACKAGE(PkgConfig QUIET)

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

IF (NOT DISABLE_UTILS)
  CHECK_GETOPT()
  MACRO_LOG_FEATURE(GETOPT_FOUND
    "GNU getopt functions"
    "Required for Regina's command-line utilities"
    "Shipped with GNU libc"
    TRUE ""
    "To disable the command-line utilities, run: cmake -DDISABLE_UTILS=1")
ENDIF (NOT DISABLE_UTILS)

IF (NOT DISABLE_PYTHON)
  MESSAGE (STATUS "Searching for a Python installation")

  # Some basic sanity testing on the user's python settings.
  IF (PYTHON_EXECUTABLE)
    UNSET(PYTHON_EXECUTABLE CACHE)
    MESSAGE(FATAL_ERROR "Regina now uses a different method for finding Python. The old PYTHON_EXECUTABLE setting is no longer recognised. Please set Python_EXECUTABLE instead.")
  ENDIF (PYTHON_EXECUTABLE)
  IF (Python_EXECUTABLE)
    IF(${CMAKE_VERSION} VERSION_LESS "3.16.0")
      UNSET(Python_EXECUTABLE CACHE)
      MESSAGE(FATAL_ERROR "The Python_EXECUTABLE setting only works with CMake 3.16 and above. For now please either upgrade your CMake, or else remove (or temporarily hide) any conflicting Python installations so that the automated tools can find the correct Python without assistance.")
    ENDIF(${CMAKE_VERSION} VERSION_LESS "3.16.0")
  ENDIF (Python_EXECUTABLE)

  # Note: FindPython prioritises Python 3 (which is what we want also).
  FIND_PACKAGE(Python COMPONENTS Interpreter Development REQUIRED)

  IF (Python_FOUND)
    IF (${Python_VERSION_MAJOR} EQUAL 2)
      MESSAGE(FATAL_ERROR "Regina no longer supports Python 2.")
    ELSEIF (${Python_VERSION_MAJOR} EQUAL 3)
      IF (${Python_VERSION_MINOR} LESS 7)
        MESSAGE(FATAL_ERROR "Regina needs Python >= 3.7.")
      ELSEIF (${Python_VERSION_MINOR} LESS 12)
        # Use pybind11 2.x.y with our own hacks for subinterpreters.
        MESSAGE (STATUS "Using pybind11 2.x.y")
        SET (REGINA_PYBIND11_VERSION 2)
      ELSE ()
        # Use pybind11 3.x.y which has native support for subinterpreters,
        # but whose subinterpreter support requires python 3.12 or newer.
        MESSAGE (STATUS "Using pybind11 3.x.y")
        SET (REGINA_PYBIND11_VERSION 3)
      ENDIF ()
    ELSE ()
      MESSAGE(FATAL_ERROR "Unknown Python major version: ${Python_VERSION_MAJOR}")
    ENDIF ()
  ENDIF (Python_FOUND)

  # We need to properly escape some python locations for use in various scripts.
  REGINA_ESCAPE_PERL (Python_EXECUTABLE)
  REGINA_ESCAPE_BASH (Python_EXECUTABLE)
ENDIF (NOT DISABLE_PYTHON)

FIND_PACKAGE(KVStore)
MACRO_LOG_FEATURE(KVStore_FOUND
  "${KVStore_DISPLAYNAME}"
  "Essential: lightweight database library for census data"
  "${KVStore_DISPLAYURL}"
  TRUE)

# Optionals
FIND_PROGRAM(UNZIP_EXECUTABLE NAMES unzip DOC "Command-line unzip tool")
IF (UNZIP_EXECUTABLE)
  SET (UNZIP_FOUND TRUE)
ENDIF (UNZIP_EXECUTABLE)

IF (REGINA_ENGINE_DOCS)
  get_filename_component(REGINA_ENGINE_DOCS_FILE "${REGINA_ENGINE_DOCS}" ABSOLUTE)

  IF (NOT EXISTS ${REGINA_ENGINE_DOCS_FILE})
    MESSAGE (FATAL_ERROR "The argument to REGINA_ENGINE_DOCS should be a zip file containing pre-built API docs.  The file you gave (${REGINA_ENGINE_DOCS}) does not exist.")
  ENDIF (NOT EXISTS ${REGINA_ENGINE_DOCS_FILE})

  MESSAGE (STATUS "Using pre-built API docs from ${REGINA_ENGINE_DOCS_FILE}")

  SET (REGINA_BUILD_ENGINE_DOCS ${UNZIP_FOUND})
  MACRO_LOG_FEATURE(UNZIP_FOUND
    "unzip"
    "Extract the API docs"
    "http://www.info-zip.org/pub/infozip/"
    ${REGINA_MANDATORY})
ELSEIF (${REGINA_INSTALL_TYPE} STREQUAL HPC)
  SET (REGINA_BUILD_ENGINE_DOCS FALSE)
  MESSAGE (STATUS "Disabling API docs for HPC build.")
ELSE (${REGINA_INSTALL_TYPE} STREQUAL HPC)
  SET (REGINA_ENGINE_DOCS "" CACHE STRING
    "Extract API docs from the given pre-built zip file instead of building them manually."
    FORCE)

  FIND_PACKAGE(Doxygen)
  SET (DOXYGEN_MIN_VERSION 1.9.2)
  IF (DOXYGEN_FOUND)
    IF (DOXYGEN_VERSION VERSION_LESS DOXYGEN_MIN_VERSION)
      MESSAGE (STATUS "Doxygen version ${DOXYGEN_VERSION} is too old; Regina requires Doxygen >= ${DOXYGEN_MIN_VERSION}.")
      SET (DOXYGEN_FOUND FALSE)
    ELSEIF (DOXYGEN_VERSION VERSION_LESS 1.9.8)
      SET (DOXYGEN_TOPICS "<a href='modules.html'>Modules</a>")
    ELSE ()
      SET (DOXYGEN_TOPICS "<a href='topics.html'>Topics</a>")
    ENDIF ()
  ENDIF (DOXYGEN_FOUND)

  SET (REGINA_BUILD_ENGINE_DOCS ${DOXYGEN_FOUND})
  MACRO_LOG_FEATURE(DOXYGEN_FOUND
    "Doxygen"
    "Generate C++/Python API docs"
    "http://www.doxygen.org/"
    ${REGINA_MANDATORY})
ENDIF (REGINA_ENGINE_DOCS)

IF (REGINA_DOCS)
  get_filename_component(REGINA_DOCS_FILE "${REGINA_DOCS}" ABSOLUTE)

  IF (NOT EXISTS ${REGINA_DOCS_FILE})
    MESSAGE (FATAL_ERROR "The argument to REGINA_DOCS should be a zip file containing pre-built handbooks.  The file you gave (${REGINA_DOCS}) does not exist.")
  ENDIF (NOT EXISTS ${REGINA_DOCS_FILE})

  MESSAGE (STATUS "Using pre-built handbooks from ${REGINA_DOCS_FILE}")

  SET (REGINA_BUILD_HANDBOOK ${UNZIP_FOUND})
  MACRO_LOG_FEATURE(UNZIP_FOUND
    "unzip"
    "Extract the user handbook"
    "http://www.info-zip.org/pub/infozip/"
    ${REGINA_MANDATORY})
ELSEIF (${REGINA_INSTALL_TYPE} STREQUAL HPC)
  SET (REGINA_BUILD_HANDBOOK FALSE)
  MESSAGE (STATUS "Disabling handbooks for HPC build.")
ELSE (${REGINA_INSTALL_TYPE} STREQUAL HPC)
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
    ${REGINA_MANDATORY})
ENDIF (REGINA_DOCS)

# Setup that must appear before any calls to ADD_SUBDIRECTORY():
ENABLE_TESTING()

if (WIN32 AND NOT CYGWIN)
  SET (REGINA_PYTHON_EXTENSION "pyd")
  SET (REGINA_PYTHON_EXTENSION_NONSTANDARD TRUE)
else ()
  SET (REGINA_PYTHON_EXTENSION "so")
  SET (REGINA_PYTHON_EXTENSION_NONSTANDARD FALSE)
endif ()

# Core directories
ADD_SUBDIRECTORY(admin)
ADD_SUBDIRECTORY(engine)

# Command-line utilities
IF (GETOPT_FOUND)
  ADD_SUBDIRECTORY(utils)
  MESSAGE(STATUS "Command-line utilities enabled.")
ELSE (GETOPT_FOUND)
  MESSAGE(STATUS "Command-line utilities disabled.")
ENDIF (GETOPT_FOUND)

# Python support

# This option must be 0 or 1 (it needs to be readable as a bool in perl).
# It is always 0 for CMake builds.
# (The option is present because some Xcode builds need sandboxing.)
SET (PYTHON_SANDBOXED 0)

IF(Python_FOUND)
  # We can support python.

  # Find the site-packages location in which the XDG build needs to put
  # the python module.
  MESSAGE(STATUS "Python site-packages directory: ${Python_SITELIB}")

  ADD_SUBDIRECTORY(python)
  SET(BUILD_PYTHON_BINDINGS TRUE)

  MESSAGE(STATUS "Python bindings enabled")
ELSEIF(Python_FOUND)
  MESSAGE(WARNING "Could not find Python interpreter and/or development files: Python bindings disabled.")
ENDIF()
MACRO_LOG_FEATURE(Python_FOUND
  "Python development files"
  "Build Python bindings for Regina"
  "http://www.python.org/"
  ${REGINA_MANDATORY})

# The Qt GUI
IF(NOT DISABLE_GUI)
  SET (DISABLE_GUI OFF CACHE STRING
    "Disables the graphical user interface.  The calculation engine, Python bindings and other command-line utilities will still be built."
    FORCE)
ENDIF(NOT DISABLE_GUI)
IF(${REGINA_INSTALL_TYPE} STREQUAL HPC)
  MESSAGE(STATUS "Disabling Regina's GUI for HPC build.")
ELSEIF(DISABLE_GUI)
  if (NOT ${REGINA_INSTALL_TYPE} STREQUAL XDG)
    MESSAGE(FATAL_ERROR
      "You can only set DISABLE_GUI=1 with REGINA_INSTALL_TYPE=XDG.")
  endif (NOT ${REGINA_INSTALL_TYPE} STREQUAL XDG)

  MESSAGE(WARNING "User set DISABLE_GUI=1: graphical user interface disabled.")
ELSE(DISABLE_GUI)
  set(CMAKE_AUTOMOC ON)

  FIND_PACKAGE(Qt6Widgets QUIET)
  FIND_PACKAGE(Qt6SvgWidgets QUIET)
  if (Qt6Widgets_FOUND AND Qt6SvgWidgets_FOUND)
    set(QT_FOUND TRUE)
    set(REGINA_QT 6)
    MESSAGE(STATUS "Found Qt6: ${Qt6_DIR}")
  else (Qt6Widgets_FOUND AND Qt6SvgWidgets_FOUND)
    MESSAGE(STATUS "Qt6 not found: will look for Qt5 instead")
    FIND_PACKAGE(Qt5Widgets QUIET)
    FIND_PACKAGE(Qt5Svg QUIET)
    if (Qt5Widgets_FOUND AND Qt5Svg_FOUND)
      set(QT_FOUND TRUE)
      set(REGINA_QT 5)
      MESSAGE(STATUS "Found Qt5: ${Qt5Core_DIR}")
    else (Qt5Widgets_FOUND AND Qt5Svg_FOUND)
      set(QT_FOUND FALSE)
      unset(REGINA_QT)
      MESSAGE(STATUS "Qt5 not found")
    endif (Qt5Widgets_FOUND AND Qt5Svg_FOUND)
  endif (Qt6Widgets_FOUND AND Qt6SvgWidgets_FOUND)

  IF (APPLE AND GRAPHVIZ_FRAMEWORK_PATH)
    # The GRAPHVIZ_FRAMEWORK_PATH option is a hack that allows you to link
    # with a single Graphviz framework, instead of the full suite of
    # Graphviz libraries and plugins.
    #
    # This framework should be called either graphviz.framework or
    # graphviz.xcframework, and should provide all the necessary Graphviz
    # components in just *one* library.  The GRAPHVIZ_FRAMEWORK_PATH setting
    # should be the directory *containing* this framework (not the full path
    # to the framework itself).
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
        # - We do not fix LIBGVC_INCLUDE_DIRS, since this is legitimately a
        #   semicolon-separated list.
        #
        string(REPLACE ";" " " _TMP_STR "${LIBGVC_LIBDIR}")
        execute_process(
          COMMAND cmd /c for %A in ("${_TMP_STR}") do @echo %~sA
          RESULT_VARIABLE _conv_res
          OUTPUT_VARIABLE _TMP_STR)
        string(REGEX REPLACE "\n" "" _TMP_STR "${_TMP_STR}")
        set(LIBGVC_LIBDIR "${_TMP_STR}" CACHE INTERNAL "")
      ENDIF(WIN32)
    ELSE(LIBGVC_FOUND)
      IF(NOT PKG_CONFIG_FOUND)
        MESSAGE(WARNING "Could not locate the Graphviz libraries, because pkg-config is not installed.")
      ENDIF(NOT PKG_CONFIG_FOUND)

      SET(GRAPHVIZ_TYPE None)
    ENDIF(LIBGVC_FOUND)
  ENDIF(APPLE AND GRAPHVIZ_FRAMEWORK_PATH)
  MACRO_LOG_FEATURE(LIBGVC_FOUND
    "GraphViz libraries"
    "Visualising graphs"
    "http://graphviz.org/"
    ${REGINA_MANDATORY})

  if (${REGINA_INSTALL_TYPE} STREQUAL XDG AND NOT APPLE)
    set(SHARED_MIME_INFO_MINIMUM_VERSION "0.30")
    find_package(SharedMimeInfo)
    MACRO_LOG_FEATURE(SHARED_MIME_INFO_FOUND
      "SharedMimeInfo"
      "Required for Regina's graphical user interface"
      "http://freedesktop.org/wiki/Software/shared-mime-info"
      TRUE "0.30")
  endif (${REGINA_INSTALL_TYPE} STREQUAL XDG AND NOT APPLE)

  # This must come after SharedMimeInfo is included, since the qtui/
  # makefiles use SharedMimeInfo macros.
  if (QT_FOUND)
    ADD_SUBDIRECTORY(qtui)
  endif (QT_FOUND)

  MACRO_LOG_FEATURE(QT_FOUND
    "Qt6 or Qt5"
    "Required for Regina's graphical user interface"
    "https://www.qt.io/"
    TRUE ""
    "To disable the graphical user interface, run: cmake -DDISABLE_GUI=1")
ENDIF()

# Miscellaneous subdirectories
ADD_SUBDIRECTORY(timing)
ADD_SUBDIRECTORY(docs)
ADD_SUBDIRECTORY(examples)

# Configure file
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/engine/regina-config.h.in"
  "${PROJECT_BINARY_DIR}/engine/regina-config.h"
  ESCAPE_QUOTES
)

MACRO_DISPLAY_FEATURE_LOG()

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
  "^${PROJECT_SOURCE_DIR}/Regina.xcodeproj/"
  "^${PROJECT_SOURCE_DIR}/admin/RELEASE.txt"
  "^${PROJECT_SOURCE_DIR}/admin/distcheck"
  "^${PROJECT_SOURCE_DIR}/engine/snappea/kernel/unused/"
  "^${PROJECT_SOURCE_DIR}/ios/"
  "^${PROJECT_SOURCE_DIR}/packaging/"
  "^${PROJECT_SOURCE_DIR}/python/docstrings/clang-support/"
  "^${PROJECT_SOURCE_DIR}/utils/local/"
  "^${PROJECT_SOURCE_DIR}/utils/snappea/"
  "^${PROJECT_SOURCE_DIR}/www/"
  "^${PROJECT_SOURCE_DIR}/\\\\.git/"
  "^${PROJECT_SOURCE_DIR}.*/\\\\.svn/"
  "\\\\.DS_Store$"
)
include(CPack)

