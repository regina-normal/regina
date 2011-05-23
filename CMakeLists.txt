CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (regina)
SET (PACKAGE_NAME regina)
SET (PACKAGE_PRETTY_NAME Regina)
# Look here for Find___.cmake modules
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Modules needed for IOS check and function existence
INCLUDE( CheckCXXSourceCompiles )
INCLUDE( CheckFunctionExists )
# Version
SET (PACKAGE_VERSION 4.6.1)
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
  

# Dependencies
FIND_PACKAGE(ZLIB REQUIRED)
INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})
FIND_PACKAGE(LibXml2 REQUIRED)
INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR})
FIND_PACKAGE(GMP REQUIRED)
INCLUDE_DIRECTORIES(${GMP_INCLUDE_DIR})
FIND_PACKAGE(ICONV REQUIRED)
INCLUDE_DIRECTORIES(${ICONV_INCLUDE_DIR})

FIND_PACKAGE(Threads REQUIRED)
IF(NOT CMAKE_USE_PTHREADS_INIT)
  MESSAGE(FATAL "Regina requires pthread support")
ENDIF(NOT CMAKE_USE_PTHREADS_INIT)

# Not a package per se, but finds right header file
# and right namespace
FIND_PACKAGE(HashSet)

# Optionals
FIND_PACKAGE(MPI)
FIND_PACKAGE(Doxygen)

# Test suite
ENABLE_TESTING() # This must appear before any calls to ADD_SUBDIRECTORY().

# Configure file
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/engine/regina-config.h.in"
  "${PROJECT_BINARY_DIR}/engine/regina-config.h"
)
# TODO: Get rid of this, eventually.
SET (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wno-deprecated)

# Core directories
ADD_SUBDIRECTORY(engine)
ADD_SUBDIRECTORY(utils)

# Python support
IF(NOT DISABLE_PYTHON)
  FIND_PACKAGE(Boost COMPONENTS python)
  FIND_PACKAGE(PythonLibs)
  FIND_PACKAGE(PythonInterp)
ENDIF(NOT DISABLE_PYTHON)

IF(Boost_FOUND AND PYTHONINTERP_FOUND AND PYTHONLIBS_FOUND)
  # We can support python.
  ADD_SUBDIRECTORY(pylib)  
  ADD_SUBDIRECTORY(python)  
  MESSAGE(STATUS "Python bindings enabled")
ELSE(Boost_FOUND AND PYTHONINTERP_FOUND AND PYTHONLIBS_FOUND)
  # We cannot support python.
  IF(ENABLE_PYTHON)
    MESSAGE(FATAL
	  "Python bindings unavailable; try again without ENABLE_PYTHON")
  ELSE(ENABLE_PYTHON)
    MESSAGE(WARNING "Python bindings disabled")
  ENDIF(ENABLE_PYTHON)
ENDIF(Boost_FOUND AND PYTHONINTERP_FOUND AND PYTHONLIBS_FOUND)

# TODO: Make this option show up in cmake -i.
IF(DISABLE_KDEUI)
  MESSAGE(WARNING "KDE user interface disabled")
ELSE(DISABLE_KDEUI)
  # TODO: KDE flags do bad things.
  # On debian lenny: fails to build (-fno-exceptions)
  # On MacOSX: test suite crashes
  FIND_PACKAGE(JPEG REQUIRED) # TODO: Why is this here?
  FIND_PACKAGE(KDE4 REQUIRED)
  FIND_PACKAGE(Qt4 REQUIRED)
  ADD_SUBDIRECTORY(kdeui)
  MESSAGE(STATUS "KDE user interface enabled")
ENDIF(DISABLE_KDEUI)

# Test suite, continued
FIND_PATH(CPPUNIT_INCLUDE_DIR cppunit/Test.h)
FIND_LIBRARY(CPPUNIT_LIBRARY NAMES cppunit)
IF (CPPUNIT_INCLUDE_DIR AND CPPUNIT_LIBRARY)
  MESSAGE(STATUS "Found CppUnit: ${CPPUNIT_LIBRARY}")
  ADD_SUBDIRECTORY(testsuite)
ELSE (CPPUNIT_INCLUDE_DIR AND CPPUNIT_LIBRARY)
  MESSAGE(WARNING "Could not find CppUnit: test suite disabled")
ENDIF (CPPUNIT_INCLUDE_DIR AND CPPUNIT_LIBRARY)

# Miscellaneous subdirectories
ADD_SUBDIRECTORY(docs)
ADD_SUBDIRECTORY(examples)
ADD_SUBDIRECTORY(icons)
ADD_SUBDIRECTORY(scripts)
