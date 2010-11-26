cmake_minimum_required (VERSION 2.6)
project (Regina)
set (PACKAGE_NAME regina)
# Look here for Find___.cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Modules needed for IOS check and function existence
include( CheckCXXSourceCompiles)
include( CheckFunctionExists)

# Version
set (PACKAGE_VERSION 4.6.1)

# Extract major/minor version
# Note: The PACKAGE_VERSION_MAJOR "output" is discarded, since it matches the
# whole string
string(REGEX MATCH "^([0-9]+).([0-9]+)" PACKAGE_VERSION_MAJOR "${PACKAGE_VERSION}")
set (PACKAGE_VERSION_MAJOR ${CMAKE_MATCH_1} )
set (PACKAGE_VERSION_MINOR ${CMAKE_MATCH_2} )

# Bug report email
set (PACKAGE_BUGREPORT "regina-user@lists.sourceforge.net")

# Useful directories

set (PREFIX /usr)
set (DATADIR ${PREFIX}/share)
set (PKGDATADIR ${DATADIR}/regina)
set (LIBDIR ${PREFIX}/lib)
set (PKGLIBDIR ${LIBDIR}/regina)


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

# Not a package per se, but finds right header file
# and right namespace
find_package(HashSet)

# Optionals
#find_package(Boost COMPONENTS python)
#find_package(PythonLibs)
#find_package(PythonInterp)
#find_package(KDE4)
#find_package(Qt4)
#find_package(MPI)
#find_package(JPEG)



# Configure file
configure_file (
  "${PROJECT_SOURCE_DIR}/engine/regina-config.h.in"
  "${PROJECT_BINARY_DIR}/engine/regina-config.h"
)
set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wno-deprecated)
add_subdirectory(engine)
