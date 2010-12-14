cmake_minimum_required (VERSION 2.6)
project (regina)
set (PACKAGE_NAME regina)
set (PACKAGE_PRETTY_NAME Regina)
# Look here for Find___.cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Modules needed for IOS check and function existence
include( CheckCXXSourceCompiles )
include( CheckFunctionExists )
include( ExtraDist )
# Version
set (PACKAGE_VERSION 4.6.1)
set (PACKAGE_STRING "${PACKAGE_PRETTY_NAME} ${PACKAGE_STRING}")
# Extract major/minor version
# Note: The PACKAGE_VERSION_MAJOR "output" is discarded, since it matches the
# whole string
string(REGEX MATCH "^([0-9]+).([0-9]+)" PACKAGE_VERSION_MAJOR "${PACKAGE_VERSION}")
set (PACKAGE_VERSION_MAJOR ${CMAKE_MATCH_1} )
set (PACKAGE_VERSION_MINOR ${CMAKE_MATCH_2} )

# Bug report email
set (PACKAGE_BUGREPORT "regina-user@lists.sourceforge.net")

# Useful directories

set (DATADIR ${CMAKE_INSTALL_PREFIX}/share)
set (PKGDATADIR ${DATADIR}/${PACKAGE_NAME})
set (LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
set (PKGLIBDIR ${LIBDIR}/${PACKAGE_NAME})
set (INCLUDEDIR include/${PACKAGE_NAME})

# Determine if std::ios::nocreate is valid
check_cxx_source_compiles("
#include <ios>
int main(){static const int input = (std::ios::nocreate);
return 0;}
"
IOS_NOCREATE )

# Set appropriate flag
if (IOS_NOCREATE)
  set ( __USE_IOS_NOCREATE 1 )
else (IOS_NOCREATE)
  set ( __USE_IOS_NOCREATE 0 )
endif (IOS_NOCREATE)

# 64 bit integer checks
check_cxx_source_compiles("long long x; signed long long y ; unsigned long long z;"
  LONG_LONG_FOUND )

if(LONGLONG_FOUND)
  check_cxx_source_compiles("0xFFFFFFFFFFFFFFFF;" NUMERIC_64_FOUND)
  check_cxx_source_compiles("0xFFFFFFFFFFFFFFFFLL;" NUMERIC_64_LL_FOUND)
endif(LONGLONG_FOUND)
  

# Dependencies
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIR})
find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})
find_package(GMP REQUIRED)
include_directories(${GMP_INCLUDE_DIR})
find_package(ICONV REQUIRED)

find_package(Threads REQUIRED)
if(NOT CMAKE_USE_PTHREADS_INIT)
  message(FATAL "Regina requires pthread support")
endif(NOT CMAKE_USE_PTHREADS_INIT)

# Not a package per se, but finds right header file
# and right namespace
find_package(HashSet)

# Optionals
find_package(MPI)

if(ENABLE_PYTHON)
  find_package(Boost COMPONENTS python REQUIRED)
  find_package(PythonLibs REQUIRED)
  find_package(PythonInterp REQUIRED)
else(ENABLE_PYTHON)
  if(DISABLE_PYTHON)
  else(DISABLE_PYTHON)
    find_package(Boost COMPONENTS python)
    find_package(PythonLibs)
    find_package(PythonInterp)
  endif(DISABLE_PYTHON)
endif(ENABLE_PYTHON)

#find_package(JPEG)
#find_package(KDE4)
#find_package(Qt4)


set(ENGINE_INCLUDES "${PROJECT_SOURCE_DIR}/engine" )
set(ENGINE_LIBRARY regina-engine)
set(ENGINE_LIBRARY_STATIC regina-engine_static)

# Configure file
configure_file (
  "${PROJECT_SOURCE_DIR}/engine/regina-config.h.in"
  "${PROJECT_BINARY_DIR}/engine/regina-config.h"
)
set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wno-deprecated)
add_subdirectory(engine)
add_subdirectory(utils)

if(Boost_FOUND AND PYTHONINTERP_FOUND AND PYTHONLIBS_FOUND)
  add_subdirectory(pylib)  
  add_subdirectory(python)  
  message(STATUS "Python bindings enabled")
endif(Boost_FOUND AND PYTHONINTERP_FOUND AND PYTHONLIBS_FOUND)
