# Copyright (c) William Pettersson, 2011 
# Licensed under the GNU General Public License, version 2 or later
#
# As an exception, when this program is distributed through (i) the
# App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or
# (iii) Google Play by Google Inc., then that store may impose any
# digital rights management, device limits and/or redistribution
# restrictions that are required by its terms of service.

include (CheckFunctionExists)
include (CheckCXXSourceCompiles)

IF (APPLE)
  # Both /usr/include/iconv.h and /sw/include/iconv.h are highly likely
  # to turn up on the compile-time search path if we have fink installed.
  #
  # It is very difficult to push /usr/include to the front of the search path
  # when compiling, which raises the risk of compiling with /sw/include/iconv.h
  # but linking against /usr/lib/libiconv.dylib (which is incompatible).
  #
  # Our solution: make sure we use the iconv in /sw if it exists.
  FIND_PATH(ICONV_INCLUDE_DIR NAMES iconv.h PATHS /sw/include NO_DEFAULT_PATH)
  IF (NOT ICONV_INCLUDE_DIR)
    FIND_PATH(ICONV_INCLUDE_DIR NAMES iconv.h)
  ENDIF (NOT ICONV_INCLUDE_DIR)

  FIND_LIBRARY(ICONV_LIBRARY NAMES iconv libiconv PATHS /sw/lib NO_DEFAULT_PATH)
  IF (NOT ICONV_LIBRARY)
    FIND_LIBRARY(ICONV_LIBRARY NAMES iconv libiconv)
  ENDIF (NOT ICONV_LIBRARY)
ELSE (APPLE)
  FIND_PATH(ICONV_INCLUDE_DIR NAMES iconv.h)
  FIND_LIBRARY(ICONV_LIBRARY NAMES iconv libiconv)
ENDIF (APPLE)

IF(NOT ICONV_LIBRARY)
  CHECK_FUNCTION_EXISTS (iconv ICONV_FOUND)
ENDIF(NOT ICONV_LIBRARY)

IF(ICONV_INCLUDE_DIR AND ICONV_LIBRARY)
  SET(ICONV_FOUND TRUE)
ENDIF(ICONV_INCLUDE_DIR AND ICONV_LIBRARY)

IF(ICONV_FOUND)
  IF (NOT ICONV_FIND_QUIETLY)
    MESSAGE(STATUS "Found iconv: ${ICONV_INCLUDE_DIR}")
  ENDIF (NOT ICONV_FIND_QUIETLY)

  IF (NOT ICONV_LIBRARY)
    SET (ICONV_LIBRARY "")
  ENDIF (NOT ICONV_LIBRARY)
  
  check_cxx_source_compiles("
  #include <iconv.h>  
  extern \"C\"
  size_t iconv(iconv_t cd, const char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft);
  int main(){return 0;}
  " ICONV_CONST)
  if(ICONV_CONST)
    message(STATUS "Setting const")
    set(ICONV_CONST "const")
    mark_as_advanced(ICONV_CONST)
  endif(ICONV_CONST)

ELSE (ICONV_FOUND)
  IF (ICONV_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find iconv")
  ENDIF (ICONV_FIND_REQUIRED)
ENDIF(ICONV_FOUND)
