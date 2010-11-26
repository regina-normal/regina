# TODO Insert GPL licence bits

include (CheckFunctionExists)
include (CheckCXXSourceCompiles)
FIND_PATH(ICONV_INCLUDE_DIR NAMES iconv.h)

FIND_LIBRARY(ICONV_LIBRARY NAMES iconv)

IF(NOT ICONV_LIBRARY)
  CHECK_FUNCTION_EXISTS (iconv ICONV_FOUND)
ENDIF(NOT ICONV_LIBRARY)


IF(ICONV_INCLUDE_DIR AND ICONV_LIBRARY)
  SET(ICONV_FOUND TRUE)
ENDIF(ICONV_INCLUDE_DIR AND ICONV_LIBRARY)

IF(ICONV_FOUND)
  IF (NOT ICONV_FIND_QUIETLY)
    MESSAGE(STATUS "Found iconv")
  ENDIF (NOT ICONV_FIND_QUIETLY)
  
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
