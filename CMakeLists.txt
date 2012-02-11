CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (regina)

# Let the user override the package name.
IF(NOT PACKAGE_NAME)
  SET (PACKAGE_NAME regina)
ENDIF(NOT PACKAGE_NAME)

SET (PACKAGE_PRETTY_NAME Regina)
# Look here for Find___.cmake modules
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Give packagers a way of insisting that every optional component is found.
# Set PACKAGING_MODE=1 to make every component compulsory.
# Set PACKAGING_MODE=1 and PACKAGING_NO_MPI=1 to make every component
# compulsory except for the MPI utilities.
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

# Options that were used with the (now-redundant) KDE4Defaults:
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)
set(CMAKE_COLOR_MAKEFILE ON)

# Modules needed for IOS check and function existence
INCLUDE( CheckCXXSourceCompiles )
INCLUDE( CheckFunctionExists )
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

# Useful directories

SET (DATADIR ${CMAKE_INSTALL_PREFIX}/share)
SET (PKGDATADIR ${DATADIR}/${PACKAGE_NAME})
SET (LIBDIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
SET (PKGLIBDIR ${LIBDIR}/${PACKAGE_NAME})
SET (INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include/${PACKAGE_NAME})
SET (MANDIR ${DATADIR}/man)

SET (ENGINE_INCLUDES "${PROJECT_SOURCE_DIR}/engine" )
SET (ENGINE_LIBRARY regina-engine)

if (APPLE)
  # In the old KDE builds we used rpath because KDE4 apps like to
  # install into non-standard directories.
  #
  # In the new Qt-only builds we remove rpath because app bundles should
  # be relocatable.  The commented line below is the old KDE-era rpath command:
  # set(CMAKE_INSTALL_NAME_DIR ${LIBDIR})
  #
  # TODO: Restore this if it's a no-bundle build (e.g., a fink install).
else (APPLE)
  # Don't use rpath under linux.
  # set(CMAKE_INSTALL_RPATH ${LIBDIR} )
  # set(CMAKE_SKIP_BUILD_RPATH FALSE)
  # set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  # set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif (APPLE)

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

# Test suite
ENABLE_TESTING() # This must appear before any calls to ADD_SUBDIRECTORY().

# TODO: Get rid of this, eventually.
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")

# TODO: Make this optional.
# Switch on compiler optimisations by default.
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
SET (CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -O3")

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

# TODO: Make this option show up in cmake -i.
IF(DISABLE_GUI)
  MESSAGE(WARNING "User passed -DDISABLE_GUI=1: graphical user interface disabled.")
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

  if (APPLE)
    # Include utilities for building MacOSX bundles.
    include(BundleUtilities)
  endif (APPLE)

  # This must come after SharedMimeInfo is included, since the kdeui/
  # makefiles use SharedMimeInfo macros.
  ADD_SUBDIRECTORY(kdeui)
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

