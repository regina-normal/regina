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

# Modules needed for IOS check and function existence
INCLUDE( CheckCXXSourceCompiles )
INCLUDE( CheckFunctionExists )
# Version
SET (PACKAGE_VERSION 4.90)
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
SET (LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
SET (PKGLIBDIR ${LIBDIR}/${PACKAGE_NAME})
SET (INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include/${PACKAGE_NAME})
SET (MANDIR ${DATADIR}/man)

SET (ENGINE_INCLUDES "${PROJECT_SOURCE_DIR}/engine" )
SET (ENGINE_LIBRARY regina-engine)

# Use rpath since KDE4 apps like to install into non-standard directories.
if (APPLE)
  set(CMAKE_INSTALL_NAME_DIR ${LIBDIR})
else (APPLE)
  set(CMAKE_INSTALL_RPATH ${LIBDIR} )
  set(CMAKE_SKIP_BUILD_RPATH FALSE)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
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
  "GMP"
  "Essential: large integer arithmetic support"
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
IF(DISABLE_KDEUI)
  MESSAGE(WARNING "User passed -DDISABLE_KDEUI=1: KDE user interface disabled.")
ELSE(DISABLE_KDEUI)
  set(KDE4_REQUIRED TRUE)
  FIND_PACKAGE(KDE4)

  IF(KDE4_FOUND)
    FIND_PACKAGE(Qt4)
    MACRO_LOG_FEATURE(QT4_FOUND
      "Qt4"
      "Required for Regina's graphical user interface"
      "http://qt.nokia.com/"
      TRUE)

    # Some versions of the KDE4 macros disable exception handling.
    # We need exceptions in regina, so put them back.
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}")

    set(SHARED_MIME_INFO_MINIMUM_VERSION "0.30")
    find_package(SharedMimeInfo)
    MACRO_LOG_FEATURE(SHARED_MIME_INFO_FOUND
      "SharedMimeInfo"
      "Required for Regina's graphical user interface"
      "http://freedesktop.org/wiki/Software/shared-mime-info"
      TRUE "0.30")

    # This must come after SharedMimeInfo is included, since the kdeui/
    # makefiles use SharedMimeInfo macros.
    ADD_SUBDIRECTORY(kdeui)
    MESSAGE(STATUS "KDE user interface enabled")

    # Check that the install prefix is sane (i.e., the same as KDE's).
    get_filename_component(REGINA_KDEBIN "${KDE4_KDECONFIG_EXECUTABLE}" PATH)
    get_filename_component(REGINA_KDEPREFIX "${REGINA_KDEBIN}" PATH)
    IF (NOT CMAKE_INSTALL_PREFIX STREQUAL REGINA_KDEPREFIX)
      MESSAGE("")
      MESSAGE("KDE seems to use the installed prefix: ${REGINA_KDEPREFIX}")
      MESSAGE("Your installation prefix is: ${CMAKE_INSTALL_PREFIX}\n")
      MESSAGE("Regina needs to be installed with the same prefix as KDE.")
      MESSAGE("Try passing: -DCMAKE_INSTALL_PREFIX=${REGINA_KDEPREFIX}")
      MESSAGE("If you really want a different prefix, pass: -DIGNORE_PREFIX=1"\n)
      IF (IGNORE_PREFIX)
        MESSAGE(WARNING
          "You passed IGNORE_PREFIX=1: continuing anyway as requested.")
      ELSE (IGNORE_PREFIX)
        MESSAGE(FATAL_ERROR
          "Stopping the build so you can resolve the prefix issue (see above).")
      ENDIF (IGNORE_PREFIX)
    ENDIF (NOT CMAKE_INSTALL_PREFIX STREQUAL REGINA_KDEPREFIX)
  ENDIF(KDE4_FOUND)
ENDIF(DISABLE_KDEUI)
MACRO_LOG_FEATURE(KDE4_FOUND
  "KDE4"
  "Required for Regina's graphical user interface"
  "http://www.kde.org/"
  KDE4_REQUIRED ""
  "To disable the graphical user interface, run: cmake -DDISABLE_KDEUI=1")

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
ADD_SUBDIRECTORY(scripts)

# Configure file
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/engine/regina-config.h.in"
  "${PROJECT_BINARY_DIR}/engine/regina-config.h"
)

MACRO_DISPLAY_FEATURE_LOG()

