CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
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
SET (PACKAGE_VERSION 4.91)
SET (PACKAGE_STRING "${PACKAGE_PRETTY_NAME} ${PACKAGE_VERSION}")

# Extract major/minor version
# Note: The PACKAGE_VERSION_MAJOR "output" is discarded, since it matches the
# whole string
STRING(REGEX MATCH "^([0-9]+).([0-9]+)" PACKAGE_VERSION_MAJOR "${PACKAGE_VERSION}")
SET (PACKAGE_VERSION_MAJOR ${CMAKE_MATCH_1} )
SET (PACKAGE_VERSION_MINOR ${CMAKE_MATCH_2} )

# Bug report email
SET (PACKAGE_BUGREPORT "regina-user@lists.sourceforge.net")

# Make a release build by default.
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build.  Options are: Debug Release RelWithDebInfo MinSizeRel."
  FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

# Distinguish between the different Regina installation types:
# FullInstall = standard filesystem installation in a fixed location.
# Bundle = self-contained MacOSX app bundle that users can drag and drop.
if (NOT REGINA_INSTALL_TYPE)
  SET (REGINA_INSTALL_TYPE FullInstall CACHE STRING
    "Choose how Regina installs itself.  Options are: FullInstall Bundle."
	FORCE)
ENDIF (NOT REGINA_INSTALL_TYPE)

# Sanity check the installation type:
IF (${REGINA_INSTALL_TYPE} STREQUAL FullInstall)
  # No sanity checking needed here (yet).
ELSEIF (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
  IF (NOT APPLE)
    MESSAGE(FATAL_ERROR "REGINA_INSTALL_TYPE=Bundle is only supported on MacOSX.")
  ENDIF (NOT APPLE)
ELSE()
  MESSAGE(FATAL_ERROR "REGINA_INSTALL_TYPE must be one of: FullInstall Bundle.")
ENDIF()

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
  SET (RESOURCEDIR ${BUNDLEDIR}/Contents/Resources)
  SET (BINDIR ${BUNDLEDIR}/Contents/MacOS)
  SET (LIBDIR ${BINDIR})
  SET (DATADIR ${RESOURCEDIR})
  SET (INCLUDEDIR ${RESOURCEDIR}/include)
  SET (PKGLIBDIR ${LIBDIR})
  SET (PKGDATADIR ${DATADIR})
  SET (MANDIR ${DATADIR}/man)
  SET (HTMLDIR ${PKGDATADIR}/docs)
else ()
  # Full install.
  SET (APPDIR ${CMAKE_INSTALL_PREFIX}/bin)
  SET (BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
  SET (LIBDIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
  SET (DATADIR ${CMAKE_INSTALL_PREFIX}/share)
  SET (INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include/${PACKAGE_NAME})
  SET (PKGLIBDIR ${LIBDIR}/${PACKAGE_NAME})
  SET (PKGDATADIR ${DATADIR}/${PACKAGE_NAME})
  SET (MANDIR ${DATADIR}/man)
  SET (HTMLDIR ${PKGDATADIR}/docs)
endif ()

# Useful build directories
SET (ENGINE_INCLUDES "${PROJECT_SOURCE_DIR}/engine" )
SET (ENGINE_LIBRARY regina-engine)

if (APPLE AND ${REGINA_INSTALL_TYPE} STREQUAL FullInstall)
  # Use rpath because apps may be installed into non-standard directories.
  set(CMAKE_INSTALL_NAME_DIR ${LIBDIR})
else ()
  # Don't use rpath under linux.
  # set(CMAKE_INSTALL_RPATH ${LIBDIR} )
  # set(CMAKE_SKIP_BUILD_RPATH FALSE)
  # set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  # set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif ()

# 64 bit integer checks
CHECK_CXX_SOURCE_COMPILES("
  long long x;
  signed long long y;
  unsigned long long z;
  int main() { return 0; }"
  LONG_LONG_FOUND )

IF(LONG_LONG_FOUND)
  CHECK_CXX_SOURCE_COMPILES("
    unsigned long long x = 0xFFFFFFFFFFFFFFFF;
    int main() { return 0; }"
    NUMERIC_64_FOUND)
  CHECK_CXX_SOURCE_COMPILES("
    unsigned long long x = 0xFFFFFFFFFFFFFFFFLL;
    int main() { return 0; }"
    NUMERIC_64_LL_FOUND)
ENDIF(LONG_LONG_FOUND)
  

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

FIND_PACKAGE(Threads REQUIRED)
IF(NOT CMAKE_USE_PTHREADS_INIT)
  MESSAGE(FATAL_ERROR "Regina requires pthread support.")
ENDIF(NOT CMAKE_USE_PTHREADS_INIT)

# Dependencies used by only some components of Regina
FIND_PACKAGE(POPT)
MACRO_LOG_FEATURE(POPT_FOUND
  "Popt"
  "Essential: command-line option processing"
  "http://rpm5.org/files/popt/"
  TRUE)

# Not a package per se, but finds right header file
# and right namespace
FIND_PACKAGE(HashSet)

# Optionals
FIND_PACKAGE(MPI)
MACRO_LOG_FEATURE(MPI_FOUND
  "MPI"
  "Build command-line tools for high-performance computing"
  "http://www.open-mpi.org/"
  REGINA_MANDATORY_MPI)

FIND_PACKAGE(Doxygen)
MACRO_LOG_FEATURE(DOXYGEN_FOUND
  "Doxygen"
  "Generate C++/Python API docs"
  "http://www.doxygen.org/"
  REGINA_MANDATORY)

FIND_PROGRAM(XSLTPROC_EXECUTABLE NAMES xsltproc DOC "XSLT processor")
IF (XSLTPROC_EXECUTABLE)
  SET (XSLTPROC_FOUND TRUE)
ENDIF (XSLTPROC_EXECUTABLE)
MACRO_LOG_FEATURE(XSLTPROC_FOUND
  "xsltproc"
  "Generate the user handbook"
  "http://xmlsoft.org/XSLT/"
  REGINA_MANDATORY)

# Test suite
ENABLE_TESTING() # This must appear before any calls to ADD_SUBDIRECTORY().

# TODO: Get rid of this, eventually.
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")

# Core directories
ADD_SUBDIRECTORY(engine)
ADD_SUBDIRECTORY(utils)

# Python support
IF(NOT DISABLE_PYTHON)
  FIND_PACKAGE(PythonInterp)
  FIND_PACKAGE(PythonLibrary)
  FIND_PACKAGE(Boost COMPONENTS python)
ENDIF(NOT DISABLE_PYTHON)

IF(Boost_FOUND AND PYTHONINTERP_FOUND AND PYTHONLIBRARY_FOUND)
  # We can support python.
  ADD_SUBDIRECTORY(pylib)  
  ADD_SUBDIRECTORY(python)  
  SET(BOOST_PYTHON_FOUND TRUE)
  MESSAGE(STATUS "Python bindings enabled")
ELSE(Boost_FOUND AND PYTHONINTERP_FOUND AND PYTHONLIBRARY_FOUND)
  # We cannot support python.
  IF(ENABLE_PYTHON)
    MESSAGE(FATAL_ERROR
      "Python bindings unavailable; try again without ENABLE_PYTHON.")
  ELSE(ENABLE_PYTHON)
    MESSAGE(WARNING "Could not find Boost.Python: Python bindings disabled.")
  ENDIF(ENABLE_PYTHON)
ENDIF(Boost_FOUND AND PYTHONINTERP_FOUND AND PYTHONLIBRARY_FOUND)
MACRO_LOG_FEATURE(BOOST_PYTHON_FOUND
  "Boost.Python"
  "Build Python bindings for Regina"
  "http://www.boost.org/"
  REGINA_MANDATORY)

IF(NOT DISABLE_GUI)
  SET (DISABLE_GUI OFF CACHE STRING
    "Disables the graphical user interface.  The calculation engine, Python bindings and other command-line utilities will still be built.  This option is suitable for use on high-performance clusters."
	FORCE)
ENDIF(NOT DISABLE_GUI)
IF(DISABLE_GUI)
  if (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
    MESSAGE(FATAL_ERROR "You cannot set DISABLE_GUI=1 with REGINA_INSTALL_TYPE=Bundle.")
  endif (${REGINA_INSTALL_TYPE} STREQUAL Bundle)

  MESSAGE(WARNING "User set DISABLE_GUI=1: graphical user interface disabled.")
ELSE(DISABLE_GUI)
  set(GUI_REQUIRED TRUE)
  FIND_PACKAGE(Qt4 COMPONENTS QtGui QtCore)

  set(SHARED_MIME_INFO_MINIMUM_VERSION "0.30")
  find_package(SharedMimeInfo)
  MACRO_LOG_FEATURE(SHARED_MIME_INFO_FOUND
    "SharedMimeInfo"
    "Required for Regina's graphical user interface"
    "http://freedesktop.org/wiki/Software/shared-mime-info"
    TRUE "0.30")

  if (${REGINA_INSTALL_TYPE} STREQUAL Bundle)
    include(BundleUtilities)
  endif (${REGINA_INSTALL_TYPE} STREQUAL Bundle)

  # This must come after SharedMimeInfo is included, since the qtui/
  # makefiles use SharedMimeInfo macros.
  ADD_SUBDIRECTORY(qtui)
  MESSAGE(STATUS "Graphical user interface enabled")
ENDIF(DISABLE_GUI)
MACRO_LOG_FEATURE(QT4_FOUND
  "Qt4"
  "Required for Regina's graphical user interface"
  "http://qt.nokia.com/"
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
ADD_SUBDIRECTORY(docs)
ADD_SUBDIRECTORY(examples)
ADD_SUBDIRECTORY(icons)

# Configure file
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/engine/regina-config.h.in"
  "${PROJECT_BINARY_DIR}/engine/regina-config.h"
)

MACRO_DISPLAY_FEATURE_LOG()

# CPack configuration to allow the developers to build a source tarball:
set(CPACK_PACKAGE_VERSION_MAJOR ${PACKAGE_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PACKAGE_VERSION_MINOR})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "Regina: Software for 3-manifold topology and normal surface theory")
set(CPACK_PACKAGE_VENDOR "The Regina development team")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.txt)
set(CPACK_GENERATOR TGZ)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "regina-${PACKAGE_VERSION}")
set(CPACK_SOURCE_GENERATOR TGZ)
# The following regexes match anywhere:
set(CPACK_SOURCE_IGNORE_FILES
  "~$"
  "^${PROJECT_BINARY_DIR}/"
  "^${PROJECT_SOURCE_DIR}/engine/snappea/kernel/unused/"
  "^${PROJECT_SOURCE_DIR}/icons/povray/"
  "^${PROJECT_SOURCE_DIR}/icons/src/"
  "^${PROJECT_SOURCE_DIR}/packaging/"
  "^${PROJECT_SOURCE_DIR}/utils/local/"
  "^${PROJECT_SOURCE_DIR}/utils/snappea/"
  "^${PROJECT_SOURCE_DIR}/www/"
  "^${PROJECT_SOURCE_DIR}.*/\\\\.svn/"
  "\\\\.DS_Store$"
)
include(CPack)

